
#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h" 

char ssid[] = robotika;    //  jmeno wifi
char pass[] = opravdunemuzuprozradit;   // heslo
int keyIndex = 0;           
WiFiEspClient  client;

#ifndef HAVE_HWSerial3
//#include "SoftwareSerial.h"
//SoftwareSerial Serial3(6, 7); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 0;

void setup() {
  
  Serial.begin(115200); //Initialize serial
  while(!Serial){
    ; 
  }  
  
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);

  while (!Serial) {
    ; 
  }

  Serial.print("Searching for ESP8266..."); 
  
  WiFi.init(&Serial3);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
  Serial.println("found it!");
    
  ThingSpeak.begin(client);  











  
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // change the value
  number++;
  if(number > 99){
    number = 0;
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial3.begin(rates[i]);
    delay(100);
    Serial3.print("AT+UART_DEF=");
    Serial3.print(baudrate);
    Serial3.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial3.begin(baudrate);



















  
}
