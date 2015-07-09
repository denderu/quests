HANDLE hOpen = -1;    			//хэндл уарт

#define LENGTH 20            	//длина строки приема

int pos = 0;            		//стадия загадки
int timer_count = 0;			//счетчик таймера 5 секундного
char time_str[10];				//строк для Label, для вставки оставшихся секунд

/*****************
0 - начало ЗАПУСК системы первый раз
1 - ДНК пройден, ЗАПУСК для проверки чипа
2 - Конец загадки
******************/

int iRxData=0;					//количество принятых байт

/*******************НАЖАТИЕ КНОПКИ ЗАПУСК**********************/
void TextPushButton6OnClick(tWidget *pWidget)
{
		//ЕСЛИ ПЕРВЫЙ ЗАПУСК
        if (pos == 0) {

				int ret_count;							  //обратный отсчет
				BOOL ret;                                 //отправка в порт
				char buf[2] = "FF";                       //посылка в уарт, запуск FF
				ret = uart_Send(hOpen,buf);               //отправляем
          
				ret_count = 5-timer_count;
				usprintf(time_str, "%d", ret_count);	  //каст для Label
           
				static char stt[28] = "Измерение...";
				LabelTextSet(&Label5, time_str);
				LabelTextSet(&Label4, stt);

				TimerEnabledSet(&Timer7,1);              //таймер отсчета 5 секунд
           
		}

		//ЕСЛИ ВТОРОЙ ЗАПУСК
		if (pos == 1) {

				int ret_count;
				BOOL ret;

				char buf[2] = "FE";                       //посылка в уарт, запуск с топливом FF

				ret = uart_Send(hOpen,buf);               //отправляем

				ret_count = 5-timer_count;
				usprintf(time_str, "%d", ret_count);


                static char stt[28] = "Измерение...";
				LabelTextSet(&Label5, time_str);
				LabelTextSet(&Label4, stt);

				TimerEnabledSet(&Timer7,1);              //таймер отсчета 5 секунд

		}
}

/*****************ТАЙМЕР 5 СЕКУНД НА ПРОВЕРКУ*****************/
void Timer7OnExecute(tWidget *pWidget)
{
			timer_count++;				

			//если прошло 5 с
			if (timer_count > 5) {

					BOOL ret;
					TimerEnabledSet(&Timer7,0);				//останавливаем таймер
					timer_count = 0;						//обнуляем счетчик
					char buf[2];							//строка для отправки

					//если первый запуск то отправляем AF
					if (pos == 0) {
						strcpy(buf, "AF");
					}
					//если второй запуск то отправляем BF
					if (pos == 1) {
						strcpy(buf, "BF");
					}

					ret = uart_BinSend(hOpen,buf,2);             //проверка окончена, выдавай результат
					TimerEnabledSet(&Timer8,1);					 //запускаем таймер для приема ответа


			}

			//через каждую секунду обновляем label и пищим
			int ret = 5-timer_count;
			usprintf(time_str, "%d", ret);
			LabelTextSet(&Label5, time_str);
			hmi_Beep();
}

/****************************ТАЙМЕР ДЛЯ ПРОВЕРКИ ПРИЕМА ОТВЕТА ОТ ARDUINO***********************/
void Timer8OnExecute(tWidget *pWidget)
{
			static char recv_str[LENGTH];							//строка приема
            int res=0,ret=0;
            
			if(hOpen < 0) return;
			
            if(uart_GetRxDataCount(hOpen)==0) return;
			
			//Если число принятых байт не 0
            if (iRxData != uart_GetRxDataCount(hOpen))
            {
               iRxData = uart_GetRxDataCount(hOpen);
               return;
            }
			
            iRxData = (iRxData<LENGTH)?iRxData:LENGTH;				
			
            res = uart_BinRecv(hOpen, recv_str,iRxData);
			
            recv_str[iRxData]=0;
			
            uart_Purge(hOpen, 0, 1);									//чистим буфер
			
            if (res)
            {
					hmi_Beep();

					LabelTextSet(&Label9, recv_str);					//вспомогательный label


					BOOL c1,c2 = false;
					int type = 0;


					for (int j = 0;j<iRxData;j++)
					{


							if (!c1)
							{

								if (recv_str[j] == 'f')
								{
									c1 = true;
									continue;
								}

							}

							if (c1)
							{

								if (recv_str[j] == '1') {c2 = true; type = 1; break;}
								if (recv_str[j] == '2') {c2 = true; type = 2;break;}
								if (recv_str[j] == '3') {c2 = true; type = 3; break;}
								if (recv_str[j] == '4') {c2 = true; type = 4; break;}
								if (recv_str[j] == '5') {c2 = true; type = 5; break;}

							}


					}

                  static char * st1;

                  if (c1 && c2 )
                  {

                      switch(type)
                      {

                        case 1:  st1 = "Простите, кружка не та";
                                 pos = 0;
                                 break;
                        case 2:  st1 = "Простите, ДНК не тот";
                                 pos = 0;
                                 break;
                        case 3:  st1 = "Ок, поддайте топлива";
                                 pos = 1;
                                 break;
                        case 4:  st1 = "Не хватает топлива";
                                 pos = 1;
                                 break;
                        case 5:  st1 = "Вы герои";
                                 pos = 2;
                                 break;



                     }  ///конец switch
                     LabelTextSet(&Label4, st1);
                  }  ///конец if
            }  ///конец if res
            TimerEnabledSet(&Timer8,0);
            iRxData = 0;
}


void Frame12OnCreate()
{

  if (hOpen < 0) {
      hOpen = uart_Open("COM1,9600,N,8,1");     //открытие уарт
   }

}


void Frame12OnDestroy()
{

   if (hOpen != -1) {
         uart_Close(hOpen);
         hOpen = -1;
    }
}

