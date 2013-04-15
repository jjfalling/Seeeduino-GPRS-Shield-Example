//Serial Relay - Arduino will patch a 
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART 


//NOTE: this is a modified version that will turn on the modem if its off 
 

#include <SoftwareSerial.h>
 
SoftwareSerial GPRS(7, 8);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int bufferCount=0;     // bufferCounter for buffer array 

int foo=0;
boolean devReady=false;
int i=0;

void setup()
{
  GPRS.begin(19200);               // the GPRS baud rate   
  Serial.begin(19200);             // the Serial port of Arduino baud rate.


  //use the enable debug command to see if the modem is on
  GPRS.println("AT+CMEE=2"); 
  
  //it may take a while, so look through until something happens
  while(devReady == false){
    foo++;
     
    //see if we got a response
    if (GPRS.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
    {
      
    
      Serial.println("Device ready");
      devReady = true;
    
     
    }
    //otherwise see how many time we have run through this. if over 10000 times (enought to wait for a response), try to turn on the device
    else if (foo > 10000){
      Serial.println("Modem seems to be off. Attempting to turn on...");
      powerUpOrDown();
      foo = 0;
    }
         
  }

  Serial.write(buffer,bufferCount);            // if no data transmission ends, write buffer to hardware serial port
  clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
  bufferCount = 0;                       // set bufferCounter of while loop to zero

  //enable verbose mode
  GPRS.println("AT+CMEE=2"); 

}


 
void loop()
{
  if (GPRS.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
  {
    while(GPRS.available())          // reading data into char array 
    {
      buffer[bufferCount++]=GPRS.read();     // writing data into array
      if(bufferCount == 64)break;
  }
    Serial.write(buffer,bufferCount);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    bufferCount = 0;                       // set bufferCounter of while loop to zero
 
 
  }
  if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
    GPRS.write(Serial.read());       // write it to the GPRS shield
}




void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<bufferCount;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}




void powerUpOrDown()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}
