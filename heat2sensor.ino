#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Adafruit_Sensor.h>
#include "max6675.h" 

float Temp1,Temp2,Temp3;
void handle_OnConnect();
char* ssid="#########";   //add your ssid and pass
char* pass="#########";   //add your ssid and pass
ESP8266WebServer server;

//Temperature Sensor1
int thermo1D0 = 5; // so
int thermo1CS = 4; 
int thermo1CLK = 0; // sck
float temp1 = 0; 
MAX6675 thermocouple1(thermo1CLK, thermo1CS, thermo1D0); 


//Temperature Sensor2
int thermo2D0 = 2; // so
int thermo2CS = 14; 
int thermo2CLK = 12; // sck
float temp2 = 0; 
MAX6675 thermocouple2(thermo2CLK, thermo2CS, thermo2D0); 


void setup() 
{

  //Static IP address configuration
  IPAddress staticIP(192, 168, 1, 100);     //ESP static ip
  IPAddress gateway(192, 168, 1, 1);        //IP Address of your WiFi Router (Gateway)
  IPAddress subnet(255, 255, 255, 0);       //Subnet mask
  IPAddress dns(8, 8, 8, 8);                //DNS
  WiFi.disconnect();                        //Prevent connecting to wifi based on previous configuration
  
  WiFi.hostname();                          // DHCP Hostname
  WiFi.begin(ssid,pass);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("IP Address  -->");
  Serial.print(WiFi.localIP());
  server.on("/", handle_OnConnect);

  server.begin();

}

void loop() 
{

    server.handleClient();
}
    
void handle_OnConnect()
{
    Temp1 = thermocouple1.readCelsius(); 
    delay(100);
    Temp2 = thermocouple2.readCelsius();
    delay(100);
    server.send(200, "text/html", SendHTML(Temp1,Temp2));
}

String SendHTML(float temp1,float temp2)
{
  String ptr ="<!doctype html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<meta charset=\"utf-8\">\n";
  ptr +="<title>Laboratories Pvt.Ltd.</title>\n";
  ptr +="\t<link href='https://fonts.googleapis.com/css?family=Orbitron' rel='stylesheet' type='text/css'>\n";
  ptr +="<style type=\"text/css\">\n";
  ptr +="#main {\n";
  ptr +="    font-family: Cambria, \"Hoefler Text\", \"Liberation Serif\", Times, \"Times New Roman\", serif;\n";
  ptr +="    font-size: 40px;\n";
  ptr +="    font-style: oblique;\n";
  ptr +="    font-weight: bolder;\n";
  ptr +="    color: #F52F0B;\n";
  ptr +="    text-decoration: blink;\n";
  ptr +="    height: 500px;\n";
  ptr +="    width: auto;\n";
  ptr +="\tbackground-color: #CEECF5;\n";
  ptr +="}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="\n";
  ptr +="<body>\n";
  ptr +="<div id=\"main\">\n";
  ptr +="<p>OVEN NO.2 : \n";
  ptr +=(float)temp2;
  ptr +="C</p>\n";
  ptr +="<p>OVEN NO.5 : \n";
  ptr +=(float)temp1;
  ptr +="C</p>\n";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
