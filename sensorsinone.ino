#include <dht.h> 
dht DHT;

#include <SFE_BMP180.h>
#include <Wire.h>
#include <NewPing.h>
#include <MQ2.h>


SFE_BMP180 pressure;




#define DHT11_PIN 2
#define ALTITUDE 90.0 // Altitude of Robojax Headquarter (Ajax, Ontario, Canada)
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 200






NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


//for mq2


int temp; //variable to hold temperature sensor value
long tm,t,d; //variables to record time in seconds
int Analog_Input = A4;
int lpg, co, smoke;
MQ2 mq2(Analog_Input);





void setup() {


//for infrared


pinMode(A0,INPUT);
pinMode(A1,OUTPUT);
pinMode(A2,OUTPUT);
pinMode(11,OUTPUT);
digitalWrite(A2,HIGH);
digitalWrite(A1,LOW);


Serial.begin(9600);



Serial.println("Started Analysis"); 

//for bmp



 if (pressure.begin())
   Serial.println("BMP180 init success");
//  else
//  {
//    // Oops, something went wrong, this is usually a connection problem,
//    // see the comments at the top of this sketch for the proper connections.
//
//    Serial.println("BMP180 init fail");
//    while(1); // Pause forever.
//  }


//for mq2




//  Serial.println("Date & Time,Temperature *C");
 pinMode(1,INPUT);//temperature sensor connected to analog 1
 analogReference(DEFAULT);
  mq2.begin();








}

void loop() { 


 Serial.println("---------for dht-----------");
// READ DATA

int chk = DHT. read11(DHT11_PIN);

Serial.println(" Humidity " );

Serial.println(DHT.humidity, 1);

Serial.println(" Temparature ");

Serial.println(DHT.temperature, 1);






//for bmp

char status;
  double T,P,p0,a;

 Serial.println("---------for bmp-----------");
  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 90 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement");
      }
      else Serial.println("error starting pressure measurement");
    }
    else Serial.println("error retrieving temperature measurement");
  }
  else Serial.println("error starting temperature measurement");



//for infrared

 Serial.println("---------for infrared-----------");


 Serial.println(analogRead(A0));
  delay(200);
  if(analogRead(A0)<500)
  digitalWrite(11,HIGH);
  else
  digitalWrite(11,LOW);




//for sonar

 Serial.println("---------for sonar-----------");


 
unsigned int uS = sonar.ping();

pinMode(ECHO_PIN,OUTPUT);

digitalWrite(ECHO_PIN,LOW);

pinMode(ECHO_PIN,INPUT);

Serial.print("Ping: ");

Serial.print(uS / US_ROUNDTRIP_CM);

Serial.println("cm");



//for mq2

temp = analogRead(1); //analog reading temperature sensor values



 Serial.println("---------for mq2-----------");

  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();


  Serial.flush();

 
  






delay(2000); }
