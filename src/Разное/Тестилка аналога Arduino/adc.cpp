void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode(A1,OUTPUT);
digitalWrite(A1,HIGH);
pinMode(A2,OUTPUT);
digitalWrite(A2,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println(analogRead(A0));
    
    delay(500);

}