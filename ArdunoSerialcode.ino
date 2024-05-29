#include <SoftwareSerial.h>
SoftwareSerial SMESerial (6, 7);
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>    

#include <Adafruit_SSD1306.h>
#define OLED_Address 0x3C // 0x3C device address of I2C OLED. Few other OLED has 0x3D
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64

const int PulseWire = 0;      
const int Ind = 13;           
int Threshold = 500;           

//Oled Code 
int a=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 560
#define LowerThreshold 530
// oled codes end

PulseSensorPlayground pulseSensor;  

void setup() {   
  Serial.begin(9600);
  SMESerial.begin(9600);
  Serial.println("Serial Begin");
//Oled codes
   oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
   oled.clearDisplay();
   oled.setTextSize(2);
//oled codes end 

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(Ind);       
  pulseSensor.setThreshold(Threshold);   
  delay(20);

   if (pulseSensor.begin()) { // If puslse sensor connect properly
    Serial.println("We created a pulseSensor Object !"); 
  }
}

void loop() {

  if(a>127)
{
oled.clearDisplay();
a=0;
lasta=a;
  delay(50);       
}

puslesensor();

  ThisTime=millis();
  int value=analogRead(0);
  oled.setTextColor(WHITE);
  int b= 60-(value/16);
  oled.writeLine(lasta,lastb,a,b,WHITE);
  lastb=b;
  lasta=a;
 
if(value>UpperThreshold)
{
  if(BeatComplete)
    {
      BPM=ThisTime-LastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
      tone(8,1000,250);
    }
  if(BPMTiming==false)
    {
  LastTime=millis();
  BPMTiming=true;
    }
}
if((value<LowerThreshold)&(BPMTiming))
BeatComplete=true;
 
oled.writeFillRect(0,50,128,16,BLACK);
oled.setCursor(0,50);
oled.print("BPM: ");
oled.print(BPM);
 
oled.display();
delay(100); 
a++;

delay(100); 
}

void puslesensor(){
 int myBPM = pulseSensor.getBeatsPerMinute();  
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
  
   Serial.println("♥  A HeartBeat Happened ! "); 
   Serial.println(String("BPM: ") + myBPM);                        
                     
   SMESerial.print('\r');
   SMESerial.print(myBPM);
   SMESerial.print('|');
   SMESerial.print('\n');
  
   Serial.print('\r');
   Serial.print(myBPM);
   Serial.print('|');
   Serial.print('\n');
}
}

  
