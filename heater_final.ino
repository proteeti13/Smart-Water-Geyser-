#include <DallasTemperature.h>

#include <OneWire.h>
int greenLedPin = 2;
int yellowLedPin = 3;
int redLedPin = 4;
int in1=11;
int switch_pin=12;
int count=0;
int flag=0;
int p=9000;

int temp_sensor = 5;

float temperature = 0;
int lowerLimit = 15;
int higherLimit = 70;

OneWire oneWirePin(temp_sensor);
DallasTemperature sensors(&oneWirePin);


void setup(void){
  Serial.begin(9600);
  
  //Setup the LEDS to act as outputs
  pinMode(redLedPin,OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(yellowLedPin,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(switch_pin,INPUT);
  
  sensors.begin();
}



void loop(){
  Serial.print("Requesting Temperatures from sensors: ");
  sensors.requestTemperatures(); 
  Serial.println("DONE");
  
  temperature = sensors.getTempCByIndex(0);

  
  
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  
  Serial.print("Temperature is ");
  Serial.print(temperature);

  //Setup the LEDS to act as outputs
  if(digitalRead(switch_pin)==HIGH)
  {
    digitalWrite(in1,HIGH);
    Serial.println(", Switch OFF");
    delay(5000);
    int count2=0;
    while(1)
    {
      if(digitalRead(switch_pin)==HIGH)
      {
        if(count2>=1800 && count2<p) p-=(count2/2);
        digitalWrite(in1,LOW);
        Serial.println(", Switch ON");
        Serial.println(p);
        break;
      }
      count2++;
      delay(5000);
    }
    count=0;
    flag=0;
  }
  
  if(temperature <= lowerLimit ){
    Serial.print(", Yellow LED is Activated");
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(in1,LOW);
    Serial.println(", Heater ON");
  }
  else if(temperature > lowerLimit && temperature < 45 ){
    Serial.print(", Green LED is Activated");
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(in1,LOW);
    Serial.println(", Heater ON");
  }

  else if(temperature >= higherLimit ){
    Serial.print(", Red LED is Activated");
    digitalWrite(redLedPin, HIGH);
    digitalWrite(in1,HIGH);
    Serial.println(", Heater OFF");
    flag=1;
  }

  if(count==p) 
  {
    digitalWrite(in1,HIGH);
    Serial.println(", Switch OFF");
    //flag=1;
    int count1=0;
    while(1)
    {
      delay(5000);
      count1++;
      if (digitalRead(switch_pin)==HIGH)
      {
        digitalWrite(in1,LOW);
        Serial.println(", Switch ON");
        count=0;
        flag=0;
        break;
      }
    } 
    Serial.println(count1);
    if(count1<=3600) p+=(count1/2);
    Serial.println(p);
  }
  
  delay(5000);
  count++;
  Serial.println(count);
}

