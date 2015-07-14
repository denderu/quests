

/*********УПРАВЛЯЮЩИЙ ПИН MAX485*************/
#define PIN_RS        3

/************ВХОД  И ВЫХОД ГЕРКОНА**************/
#define REED_INPUT    5
#define REED_OUTPUT   6

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

/************ПИНЫ ДВИГАТЕЛЯ***************/
#define MOTOR_UP             A3
#define MOTOR_DOWN           A4

/************КОНЦЕВИК ДВЕРИ***************/
#define SENSOR_CLOSE         11
#define SENSOR_OPEN          12


enum {NONE, IS_START1, IS_STOP1, IS_START2, IS_STOP2};

enum {COME_CLOSE, COME_OPEN, COME_STOP, NOTHING};

enum {CLOSE, OPEN, UNDEFINED};

enum {STOP,UP,DOWN};

/*************КОЛ-ВО ЗАМЕРОВ СОПРОТИВЛЕНИЯ РАСТВОРА ДЛЯ ВЫВЕДЕНИЕ СРЕДНЕГО ЗНАЧЕНИЯ****************/
const int accuracy_meas = 10;

/*************ОПОРНОЕ ЗНАЧЕНИЕ ДЛЯ СРАВНЕНИЯ СОПРОТИВЛЕНИЯ РАСТВОРА******************/
const int min_reference_DNA = 100;
const int max_reference_DNA = 150;

//bool rs_flag = false;
int count_bytes = 0;                                //количество правильных принятых байт  
int count_f, count_a, count_b, count_e = 0;         //количество необходимых байт

/*********************РАЗРЕШЕНИЕ ПРОВЕРКИ КРУЖКИ И ДНК**********************/
bool allow_DNA = false;                               //флаг разрешения проверки геркона
bool allow_CHIP = false;
bool is_Win = false;

/*********************ТЕКУЩАЯ ОТПРАВКА*************************/
String current_Tx = "";

/*********************СОСТОЯНИЕ ДВЕРИ**************************/
bool door_state;



/*********************КОМАНДА ДВИГАТЕЛЮ***********************/
int motor_cmd;

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
  //digitalWrite(RELAY_LED,HIGH);
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
     digitalWrite(MOTOR_UP,HIGH); 
     digitalWrite(MOTOR_DOWN,LOW);
     delay(100);
  
}

void door_down() {
  
     move_motor = UP;
     digitalWrite(MOTOR_UP,LOW);
     digitalWrite(MOTOR_DOWN,HIGH);
     delay(100); 
  
}

/**********************ДВИГАТЕЛЬ СТОП*********************/
void door_stop() {
  
     move_motor = STOP;
     digitalWrite(MOTOR_UP,LOW);
     digitalWrite(MOTOR_DOWN,LOW);
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
     
     for (i=0;i<length_meas-1;i++) {
      
          meas[i] = analogRead(ANALOG_SENSOR); 
       
     }
     
     for (i=0;i<length_meas-1;i++) {
       
          sum_meas+=meas[i];
       
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
                              break;
               case NONE: break;
            
            
          }

  }
  
  if (is_Win) {
    
         magnet_off();
    
  }
  

}


void clear_bytes() {

  count_bytes = 0;
  count_f = 0;
  count_a = 0;
  count_e = 0;
  count_b = 0;

}

