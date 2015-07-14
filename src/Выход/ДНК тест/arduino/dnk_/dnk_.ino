

/*********УПРАВЛЯЮЩИЙ ПИН MAX485*************/
#define PIN_RS        3

/************ВХОД  И ВЫХОД ГЕРКОНА**************/
#define REED_INPUT    A3
#define REED_OUTPUT   A4

/***********ВЫХОД С ДЕЛИТЕЛЯ ИЗМЕРИТЕЛЯ СОПРОТИВЛЕНИЯ РАСТВОРА************/
#define ANALOG_SENSOR    A0
#define HIGH_SENSOR      A1
#define LOW_SENSOR       A2

/***********ВЫХОДЫ НА РЕЛЕ******************/
#define RELAY_LED        7            //реле на светодиод
#define RELAY_MAGNET     8            //реле на магнит

/************ВХОД И ВЫХОД НА ТРАНЗИСТОР**********************/
#define CHIP_IN          9
#define CHIP_OUT         10

/*************ОТВЕТНЫЕ ПОСЫЛКИ******************/
#define WRONG_BULB          "f1"
#define WRONG_DNA           "f2"
#define CORRECT_DNA         "f3"
#define CHIP_ABSENT         "f4"
#define CHIP_OK             "f5"
#define FAIL                "f9"
#define MOTOR_FAIL          "f6"

/************ПИНЫ ДВИГАТЕЛЯ***************/
#define MOTOR_UP             5
#define MOTOR_DOWN           6

/************КОНЦЕВИК ДВЕРИ***************/
#define SENSOR_CLOSE         11
#define SENSOR_OPEN          12


enum {NONE, IS_START1, IS_STOP1, IS_START2, IS_STOP2, DROP_DOWN, DROP_UP};				//команды от дисплея

enum {COME_CLOSE, COME_OPEN, COME_STOP, NOTHING};					//команды двигателю

enum {CLOSE, OPEN, UNDEFINED};										//состояния концевика

enum {STOP,UP,DOWN};												//действия двигателя

/*************КОЛ-ВО ЗАМЕРОВ СОПРОТИВЛЕНИЯ РАСТВОРА ДЛЯ ВЫВЕДЕНИЕ СРЕДНЕГО ЗНАЧЕНИЯ****************/
const int accuracy_meas = 10;


/*************ЗАДЕРЖКА ДЛЯ ДВИГАТЕЛЯ***********************/
const int motor_delay = 2000;
unsigned long time_motor;

/*************ОПОРНОЕ ЗНАЧЕНИЕ ДЛЯ СРАВНЕНИЯ СОПРОТИВЛЕНИЯ РАСТВОРА******************/
const int min_reference_DNA = 700;
const int max_reference_DNA = 800;

/*************БАЙТЫ***********************/
int count_bytes = 0;                                //количество правильных принятых байт  
int count_f, count_a, count_b, count_e, count_t, count_r = 0;         //количество необходимых байт

/*********************РАЗРЕШЕНИЕ ПРОВЕРКИ КРУЖКИ И ДНК**********************/
bool allow_DNA = false;                               //флаг разрешения проверки геркона
bool allow_CHIP = false;
bool is_Win = false;

/*********************ТЕКУЩАЯ ОТПРАВКА*************************/
String current_Tx = "";

/********************ДЕЙСТВИЯ ДВИГАТЕЛЯ***********************/
int move_motor = STOP;

/*******************ПОЛОЖЕНИЕ ДАТЧИКА*************************/
int sens;

/***********************НАБОР ФУНКЦИЙ******************/
bool check_reed();
bool check_DNA();
bool check_chip();
void statement_Rx();
void statement_Tx();
void send_Tx(String msg);
void check_bytes(byte b);
int check_Rx_cmd();
void run_cmd();
void clear_bytes();

void door_up();
void door_down();
void door_stop();
int get_sensor_position();
void led_control(int c);


/*************************SETUP**************************/
void setup() {
  // put your setup code here, to run once:


  /**********ИНИЦИАЛИЗАЦИЯ UART*************/
  Serial.begin(9600);
  
  /**********UART НА ПРИЕМ**********/
  pinMode(PIN_RS, OUTPUT);
  digitalWrite(PIN_RS, LOW);

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  
  /********ГЕРКОН************/
  pinMode(REED_INPUT,OUTPUT);
  digitalWrite(REED_INPUT,LOW);
  pinMode(REED_OUTPUT,INPUT);
  digitalWrite(REED_OUTPUT,HIGH);
  
  /**********ДНК*************/
  pinMode(HIGH_SENSOR,OUTPUT);
  pinMode(LOW_SENSOR,OUTPUT);
  digitalWrite(HIGH_SENSOR,HIGH);
  digitalWrite(LOW_SENSOR,LOW);
  
  /********ЧИП**************/
  pinMode(CHIP_IN,OUTPUT);
  digitalWrite(CHIP_IN,LOW);
  pinMode(CHIP_OUT,INPUT);
  digitalWrite(CHIP_OUT,HIGH);
  
  /********ДВИГАТЕЛЬ***********/
  pinMode(MOTOR_UP,OUTPUT);
  digitalWrite(MOTOR_UP,LOW);
  pinMode(MOTOR_DOWN,OUTPUT);
  digitalWrite(MOTOR_DOWN,LOW);
  
  /*********КОНЦЕВИК***********/
  pinMode(SENSOR_CLOSE,INPUT);
  pinMode(SENSOR_OPEN,INPUT);
  digitalWrite(SENSOR_CLOSE,HIGH);
  digitalWrite(SENSOR_OPEN,HIGH);
  
  /*******РЕЛЕ***********/
  pinMode(RELAY_LED,OUTPUT);
  pinMode(RELAY_MAGNET,OUTPUT);
  digitalWrite(RELAY_MAGNET,HIGH);

  /**********ЧИСТИМ БАЙТЫ**********/
  clear_bytes();
  
  /**********ДАЕМ КОМАНДУ НА ОТКРЫТИЕ ДВЕРИ В НАЧАЛЕ ИГРЫ*******************/
  
  sens = get_sensor_position();
  
  if (sens != OPEN) {
    
          door_up();
          led_control(1);
          
  }
  
  
  
  
}

/***********************УПРАВЛЕНИЕ ПОДСВЕТКОЙ*************************/
void led_control(int c) {
  
  
  if (c == 1) {
    
        digitalWrite(RELAY_LED,LOW);
        
  }
  
  else if (c == 0) {
    
        digitalWrite(RELAY_LED,HIGH);
    
  }
  
}


/*******************ДВИГАТЕЛЬ ВВЕРХ************************/
void door_up() {
  
  
     move_motor = UP; 
     time_motor = millis();
     analogWrite(MOTOR_UP,200); 
     digitalWrite(MOTOR_DOWN,LOW);
     delay(100);
  
}

void door_down() {
  
     move_motor = DOWN;
     time_motor = millis();
     digitalWrite(MOTOR_UP,LOW);
     analogWrite(MOTOR_DOWN,70);
     delay(100); 
  
}

/**********************ДВИГАТЕЛЬ СТОП*********************/
void door_stop() {
  
     move_motor = STOP;
     digitalWrite(MOTOR_UP,HIGH);
     digitalWrite(MOTOR_DOWN,HIGH);
     delay(100); 
  
}

/*******************ПРОВЕРКА ГЕРКОНА***********************/
bool check_reed() {
  
     if (digitalRead(REED_OUTPUT) == 0) {
      
          delay(30);
          
          if (digitalRead(REED_OUTPUT) == 0) {
            
               return true; 
            
          }
     }
     
     return false;
  
}

/**************************ПРОВЕРКА ДНК******************/
bool check_DNA() {
  
     int i;
     int meas[accuracy_meas];
     int length_meas = sizeof(meas) / sizeof(int);
     int average_meas;
     int sum_meas = 0;
     
     
     
     for (i=0;i<accuracy_meas;i++) {
      
          meas[i] = analogRead(ANALOG_SENSOR); 
           
     }
     
     for (i=0;i<accuracy_meas;i++) {
       
          sum_meas = sum_meas + meas[i];
       
     }
     
     average_meas = sum_meas / accuracy_meas;
     
     
     if (average_meas >= min_reference_DNA && 
         average_meas <= max_reference_DNA) return true;
     else return false;
  
}

/************************ПРОВЕРКА ЧИПА**************************/
bool check_chip() {
  
    if (digitalRead(CHIP_OUT) == 0) {
      
       delay(30);
      
       if (digitalRead(CHIP_OUT) == 0) {
        
           return true;
         
       }
      
    }
    
    return false;
  
}

/***********************УСТАНОВКА В ПРИЕМ MAX485***********************/
void statement_Rx() {
  
  
    digitalWrite(PIN_RS, LOW);
    delay(500);


}

/*************************УСТАНОВКА В ПЕРЕДАЧУ MAX485*******************/
void statement_Tx() {

    
  
  digitalWrite(PIN_RS, HIGH);
  delay(500);
  


}

/********************НЕПОСРЕДСТВЕННАЯ ОТПРАВКА НА ДИСПЛЕЙ*****************************/
void send_Tx(String msg) {

  Serial.println(msg);        //отправляем
  delay(200);
  
}


/*****************ПРОВЕРКА ПРИНЯТОГО БАЙТА*************************/
void check_bytes(byte b) {
  
  
      switch(b) {
        
        case 70: count_bytes++;
                 count_f++;
                 break;          //если символ F, то общее кол-во байт +1 и байт F +1
        case 65: count_bytes++;
                 count_a++;
                 break;          //если символ A, то общее кол-во байт +1 и байт A +1
        case 66: count_bytes++;
                 count_b++;
                 break;          //если символ B, то общее кол-во байт +1 и байт B +1
        case 69: count_bytes++;
                 count_e++;
                 break;          //если символ E, то общее кол-во байт +1 и байт E +1
                 
        case 82: count_bytes++;
                 count_r++;
                 break;
        
        case 84: count_bytes++;
                 count_t++;
                 break;        
        
      }
  
}

/***********************ПРОВЕРКА КОМАНДЫ ОТ ДИСПЛЕЯ*************************/
int check_Rx_cmd() {
  
  if (count_bytes == 2 && 
      count_f == 2) {
      
          clear_bytes();                  //чистим кол-во байт
          return IS_START1;
   
   }
   
   else if (count_bytes == 2 && 
            count_f == 1 && 
            count_a == 1) {
      
      
          clear_bytes();             //чистим кол-во байт
          return IS_STOP1;
    }
    
    else if (count_bytes == 2 && 
             count_f == 1 && 
             count_e == 1) {
      
          clear_bytes();
          return IS_START2;
      
    }
    
    
     else if (count_bytes == 2 && 
              count_f == 1 && 
              count_b == 1) {
      
      
          clear_bytes();             //чистим кол-во байт
          return IS_STOP2;
    }  
    else if (count_bytes == 2 &&
             count_t == 1 &&
             count_f == 1) {
               
              clear_bytes();
             return DROP_UP; 
               
             }
             
    else if (count_bytes == 2 &&
             count_r == 1 &&
             count_f == 1) {
           
                 clear_bytes();
                 return DROP_DOWN;
           
             }    
  
     else { return NONE; }
  
}

/*******************ЗАПУСК ОТПРАВКИ***********************************/
void run_cmd() {
  
  
        statement_Tx();
        if (current_Tx == "") {current_Tx = FAIL;}
        send_Tx(current_Tx);
        statement_Rx();
    
     
}



/*********************ОКРЫТИЕ МАГНИТА***********************/
void magnet_off() {
  
     digitalWrite(RELAY_MAGNET,LOW);
    
}


/*********************ПОЛОЖЕНИЕ КОНЦЕВИКА*********************/
int get_sensor_position() {
  
     int _open = !digitalRead(SENSOR_OPEN);
     int _close = !digitalRead(SENSOR_CLOSE);
    
    if (_open && !_close) {
        
              return OPEN;
              
      }
      
      if (!_open && _close) {
        
              return CLOSE;
              
      }
      
      if (!_open && !_close) {
        
               return UNDEFINED;
        
      }
      
  
  
}





/******************************ГЛАВНЫЙ ЦИКЛ******************************/
void loop() {
  // put your main code here, to run repeatedly:

  if (move_motor != STOP) {
    
    
      unsigned long t;
      t = millis();
      
      if (t - time_motor >= motor_delay) {
           
             time_motor = 0;
             door_stop();
             door_up();
             allow_DNA = false;
             allow_CHIP = false; 
             current_Tx = MOTOR_FAIL;
             
        
      }
    
    
       switch(move_motor) {
        
            case UP:    sens = get_sensor_position();

                        if (sens == OPEN) {
                          
                               door_stop();
                               move_motor = STOP; 
                          
                        }
                        break;
                        
             case DOWN:    sens = get_sensor_position();

                           if (sens == CLOSE) {
                  
                                door_stop();
                                move_motor = STOP;
                  
                           }           
                           break;
        
       } 
    
  }
  
  if (allow_DNA) {
    
     if (check_reed() && 
         check_DNA()) 
         {
             current_Tx = CORRECT_DNA;
         } 
     else if (check_reed() && 
             !check_DNA()) 
             {
                 current_Tx = WRONG_DNA;
             }
     else if (!check_reed()) 
             {
                 current_Tx = WRONG_BULB;
             }
    
  }
  
  if (allow_CHIP) {
    
      if (check_chip()) 
          {
            current_Tx = CHIP_OK; 
            is_Win = true;
          }
      else 
          {
            current_Tx = CHIP_ABSENT; 
          } 
    
  }


  if (Serial.available() > 0) {

          
    
    
          delay(10);
          byte s = Serial.read();
          check_bytes(s);
          
          switch(check_Rx_cmd()) {
           
               case IS_START1: allow_DNA = true; 
                               door_down(); led_control(0);
                               break;
               case IS_STOP1: allow_DNA = false; 
                              run_cmd(); 
                              door_up(); led_control(1);
                              break;
               case IS_START2: allow_CHIP = true; 
                              door_down(); led_control(0); 
                              break;
               case IS_STOP2: allow_CHIP = false; 
                              run_cmd(); 
                              door_up(); led_control(1); 
                              if (is_Win) {magnet_off();}
                              break;
               case DROP_DOWN: door_down();
                                break;
               case DROP_UP: door_up();
                              break;               
               case NONE: break;
            
            
          }

  }
  

  

}


void clear_bytes() {

  count_bytes = 0;
  count_f = 0;
  count_a = 0;
  count_e = 0;
  count_b = 0;
  count_t = 0;
  count_r = 0;

}

