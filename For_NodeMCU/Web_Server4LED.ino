 // Load Wi-Fi library
#include<ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Add SSID name";
const char* password = "<Add SSID password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String LED1_State = "off";
String LED2_State = "off";
String LED3_State = "off";
String LED4_State = "off";

// Assign output variables to GPIO pins
const int LED1 = 12;
const int LED2 = 13;
const int LED3 = 14;
const int LED4 = 15;

// Current time
unsigned long currentTime = millis();

// Previous time
unsigned long previousTime = 0;

// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
 
// Set your static ip address
// In Windows, type = ipconfig, In Linux, type = ifconfig - to know about your private IP address and Gateway
IPAddress local_ip (192,168,43,11);

// Set your Gateway ip address
IPAddress gateway(192,168,43,1);

// Set subnet mask
IPAddress subnet(255,255,255,0);

void setup()
{
  Serial.begin(115200);
  // Initialize the output variables as outputs

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // Set outputs to LOW
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  // Configures static IP address

  if ( !WiFi.config(local_ip, gateway, subnet))
  {
    Serial.println("STA failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // print local ip address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}

void loop()
{

  WiFiClient client = server.available();  // Listen for incoming clients
  
  if (client)     // If a new client connects,
  {
    Serial.println("New Client.");    // print a message out in the serial port
    String currentLine = "";    // make a string to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
  
    while (client.connected() && currentTime - previousTime <= timeoutTime) // loop while the client's connected
    {
      currentTime = millis();
      
      if ( client.available() )   // If there are bytes to read from the client,
      {
        char c = client.read();   // read a byte, then
        Serial.write(c);    // print it out the serial monitor
        header += c;

        if ( c == '\n' )  // If the byte is a newline character
        {
        // If the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
          
            if ( currentLine.length() == 0)
            {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line: 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();


          // turns the GPIOs 'ON' and 'OFF'
          if (header.indexOf("GET /12/on") >= 0)
          {
          Serial.println("LED1 on");
          LED1_State = "on";
          digitalWrite(LED1 , HIGH);
          }
          else if ( header.indexOf("GET /12/off") >= 0)
          {
          Serial.println("LED1 off");
          LED1_State = "off";
          digitalWrite(LED1 , LOW);
          }

          else if ( header.indexOf("GET /13/on") >= 0)
          {
          Serial.println("LED2 on");
          LED2_State = "on";
          digitalWrite(LED2 , HIGH);
          }

          else if ( header.indexOf("GET /13/off") >= 0 )
          {
          Serial.println("LED2 off");
          LED2_State = "off";
          digitalWrite(LED2, LOW);
          }
        
          else if ( header.indexOf("GET /14/on") >= 0 )
          {
          Serial.println("LED3 on");
          LED3_State = "on";
          digitalWrite(LED3, HIGH);
          }

          else if ( header.indexOf("GET /14/off") >= 0 )
          {
          Serial.println("LED3 off");
          LED3_State = "off";
          digitalWrite(LED3, LOW);
          }

          else if ( header.indexOf("GET /15/on") >= 0 )
          {
          Serial.println("LED4 on");
          LED4_State = "on";
          digitalWrite(LED4, HIGH);
          }

          else if ( header.indexOf("GET /15/off") >= 0 )
          {
          Serial.println("LED4 off");
          LED4_State = "off";
          digitalWrite(LED4, LOW);
          }


        // Display the HTML web page
        client.println("<!DOCTYPE html><html>");
        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
        client.println("<link rel = \"icon\" href =\"data:,\">");
        // CSS to style the ON/OFF buttons
        // Feel free to change the background-color and font-size attributes to fit your preferences
        
        client.println("<style>html { font-family:Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
        client.println(".button {background-color: orange; border: none; color: white; padding: 16px 40px;");
        client.println("text-decoration:none; font-size:30px; margin:2px; cursor: pointer;}");
        client.println(".button2 {background-color: #77878A; }");
        client.println(".button3 {background-color: grey; }");
        client.println(".button4 {background-color: green; }");
        client.println(".button5 {background-color: blue; }</style></head>");

        // Web page heading
        client.println("<body><h1> WiFi Controlled LED</h1>");
        
        // Display current state, and ON/OFF buttons for LED1
        client.println("<p> LED1_State " + LED1_State + "</p>");
        
        // If the LED1_State is off, it displays the ON button
        if (LED1_State == "off") 
        {
        client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
        }
        else
        {
        client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
        }

        // Display current state, and ON/OFF buttons for LED2
        client.println("<p>LED2_State " + LED2_State + "</p>");

        // If the LED2_State is off, it displays the ON button
        if (LED2_State == "off") 
        {
        client.println("<p><a href=\"/13/on\"><button class=\"button button3\">ON</button></a></p>");
        }
        else
        {
        client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
        }

        // Display current state, and ON/OFF buttons for LED3
        client.println("<p>LED3_State " + LED3_State + "</p>");

        // If the LED3_State is off, it displays the ON button
        if (LED3_State == "off") 
        {
        client.println("<p><a href=\"/14/on\"><button class=\"button button4\">ON</button></a></p>");
        }
        else
        {
        client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
        }
        // Display current state, and ON/OFF buttons for LED4
        client.println("<p>LED4_State " + LED4_State + "</p>");
        
        // If the LED4_State is off, it displays the ON button
        if (LED4_State == "off")
        {
        client.println("<p><a href=\"/15/on\"><button class=\"button button5\">ON</button></a></p>");
        }
        else
        {
        client.println("<p><a href=\"/15/off\"><button class=\"button button2\">OFF</button></a></p>");
        }

        client.println("</body></html>");
    
        // The HTTP response ends with another blank line
        client.println();

        // Break out of the while loop
        break;

      }
      else
      {
          // If you got a newline, then clear currentLine
      currentLine = "";
      }
    }
    else if ( c != '\r')    // If you got anything else but a carriage return character, 
    {
    currentLine += c; // add it to the end of the currentLine
    }
  }

}

// Clear the header variable

header = "";

// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");

}

}
