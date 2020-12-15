#include <SoftwareSerial.h>

/* Pins 8, 9, 10 and 11 of Arduino are connected to L298N Motor Driver Input pins i.e. 
IN1, IN2, In3 and IN4 respectively*/

#define LED1 8
#define LED2 9
#define LED3 10
#define LED4 11
#define DEBUG true
//#define RXPIN 2
//#define TXPIN 3

SoftwareSerial esp8266Serial(2,3); //Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.


void setup()
  {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    Serial.begin(9600);
    
    esp8266Serial.begin(115200); //Baud rate for communicating with ESP8266. Your's might be different.
    esp8266Data("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
    esp8266Data("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation
  
    /*Change the following command as per your requirement i.e. enter the 
    SSID and Password of your WiFi Network in the command.*/
    
    esp8266Data("AT+CWJAP=\"<Add SSID Name>\",\"<Add SSID Password>\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.
                                   
  //  while(!esp8266Serial.find("OK")) 
    //{
     // }
      
    esp8266Data("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 
  
    /* The following statement is to assign Static IP Address to ESP8266. 
    The syntax is AT+CIPSTA=<ip>,<gateway>,<netmask>. 
    This will assign a Static IP Address of 192.168.1.254 (in my case)
    to the ESP8266 Module. Change this value as per your requirements i.e. this IP address 
    shouldn't conflict with any other device. 
    Also, the second and third parameters are Gateway and Net Mask values. 
    You can get these values from ipconfig command in command prompt*/
  // In windows, type - ipconfig, In Linux, type - ifconfig 
    esp8266Data("AT+CIPSTA=\"192.168.43.10\",\"192.168.43.1\",\"255.255.255.0\"\r\n", 5000, DEBUG); // Assign Static IP Address
    esp8266Data("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 

    esp8266Data("AT+CIPMUX=1\r\n", 5000, DEBUG);
    esp8266Data("AT+CIPSERVER=1,8010\r\n", 5000, DEBUG);
  }

void loop()
  {
    if (esp8266Serial.available())
      {
        if (esp8266Serial.find("+IPD,"))
          {
            String msg;
            esp8266Serial.find("?");
            msg = esp8266Serial.readStringUntil(' ');
            String command1 = msg.substring(0, 5);
            String command2 = msg.substring(6,9);
                        
           if (DEBUG) 
              {
                Serial.println(command1);//Must print "robot"
                Serial.println(command2);//Must print "FWD" or "REV" or "LFT" or "RGT" or "STP"
              }
            delay(100);          

              if (command2 == "FWD") 
                {
                  LED1_ON();
                 }
                 if (command2 == "REV") 
                {
                  LED2_ON();
                 }
                 if (command2 == "RGT") 
                {
                  LED3_ON();
                 }
                 if (command2 == "LFT") 
                {
                  LED4_ON();
                 }
                 if (command2 == "STP") 
                {
                  stopALL();
                 }
                 
          }
      }
  }

void LED1_ON()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void LED2_ON()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void LED3_ON()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
}

void LED4_ON()
{
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
}

void stopALL()
{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}
   
String esp8266Data(String command, const int timeout, boolean debug)
  {
    String response = "";
    esp8266Serial.print(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (esp8266Serial.available())
          {
            char c = esp8266Serial.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
