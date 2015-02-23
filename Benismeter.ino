/*
Verbindung der Ports:

Spannungsversorgung des LCDs:
LCD VDD an 5V
LCD VSS an Gnd

Datenpins des LCDs:
LCD E an Pin 2
LCD RS an Pin 3 
LCD D4 an Pin 5 
LCD D5 an Pin 6
LCD D6 an Pin 8
LCD D7 an Pin 9
LCD RW an Gnd //RW ist für Lesen und Schreiben auf dem LCD, da wir nur schreiben kommt der Pin auf Gnd

Beleuchtung des LCDs:
LCD A an 3,3V
LCD K an Gnd

Kontrast des LCDs:
LCD V0 an den mittleren Pin des Potentiometers
Und an den äußeren Pins 5V und Gnd
(Falls man kein Poti zur Hand hat kann man dort auch direkt Gnd anschließen, dann hat das LCD allerdings einen sehr hohen Kontrast.)
*/
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

//Hier dürft ihr noch rumspielen:
const String username = "yannik12";            //Pr0gramm Username
const unsigned int alle_x_sekunden_refrech=30; //Intervall in dem die Infos von Pr0gramm aktualisiert werden sollen
const unsigned int alle_x_sekunden_wechsel=3;  //Intervall in dem das Display zwischen Benis und Rang wechselt
const boolean zentriert=false;                 //Boolean der angibt ob der Username, Rang und Benis zentriert dargesetllt werden soll.

//Ab hier wird es gefährlich
// Pins für das LCD setze
LiquidCrystal lcd(2, 3, 5, 6, 8, 9);

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// initialize the library instance:
EthernetClient client;
char server[] = "pr0gramm.com"; 
//IPAddress server(10,225,156,53);//Adresse des Porxy Servers

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = alle_x_sekunden_refrech*1000;  // delay between updates, in milliseconds

unsigned long lastDrawTime = 0; 
const unsigned long DrawInterval = alle_x_sekunden_wechsel*1000;  // delay between updates, in milliseconds
boolean drawed = false;

String content; // für die Antwortvon pr0gramm
long benis;      //Speichert den Benis
byte Mark;       //Speichert den Rang

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  Serial.begin(9600);
  //Pr0gramm startup
  lcd.setCursor(0, 0);
  lcd.print(setmitte(">_  Pr0gramm"));
  lcd.setCursor(0, 1);
  lcd.print(setmitte("Benismeter"));
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection(Es wird DHCP genutzt um IP und Gateway zu bekommen):
  Ethernet.begin(mac);
  // print the Ethernet board/shield's IP address:
  Serial.println("My IP address: ");
  Serial.println(Ethernet.localIP());

}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
      char c = client.read();
      if (c == ',')//JSON Antwort bei den Kommas trennen
      {
        if (content.startsWith("\"score\":")) {//Benis auslesen
          benis = content.substring(8).toInt(); 
        }
        if (content.startsWith("\"mark\":")) {//Rang auslesen
          Mark = content.substring(7).toInt();
        }           
        //'0':'Schwuchtel','1':'Neuschwuchtel','2':'Altschwuchtel','3':'Admin','4':'Gesperrt','5':'Moderator','6':'Fliesentischbesitzer','7':'Lebende Legende','8':'pr0wichtler'
        //Serial.println(content);
        content = "";
      }else{
        content += String(c); 
      }
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    //Serial.println("disconnecting.");
    client.stop();
  }

  // Wenn man nicht verbunden ist und der Timeout(postingInterval) um ist
  if(!client.connected() && ((millis() - lastConnectionTime > postingInterval) || lastConnectionTime == 0)) {
    httpRequest();
  }
  if(!client.connected() && ((millis() - lastDrawTime > DrawInterval) || lastDrawTime == 0)) {
    draw();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request:
    client.println("GET /api/profile/info?name=" + username + "&self=true HTTP/1.1");
    client.println("Host: pr0gramm.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

void draw() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(setmitte(username));
  
  lcd.setCursor(0, 1);
  if(drawed){
    String temp;
    switch (Mark) {
      case 0:
        temp = "Schwuchtel";
        break;
      case 1:
        temp = "Neuschwuchtel";
        break;
      case 2:
        temp = "Altschwuchtel";
        break;
      case 3:
        temp = "Admin";
        break;
      case 4:
        temp = "Gesperrt";
        break;
      case 5:
        temp = "Moderator";
        break;
      case 6:
        temp = "Fliesentischbesitzer";
        break;
      case 7:
        temp = "Lebende Legende";
        break;
      case 8:
        temp = "pr0wichtler";
        break;
      default: 
        temp = "Unknown";
    }
    lcd.print(setmitte(temp));
  }else{
    lcd.print(setmitte("Benis: " + String(benis)));
  }
  drawed = !drawed;//Toggle
  lastDrawTime = millis();
}
String setmitte(String text)
{
  if(zentriert){
    int anzahlspace = ((16 - text.length())/2);
    String antwort = "";
    for (int i=1; i <= anzahlspace; i++){
      antwort += " ";
    } 
    return antwort + text;
  }else{
    return text;
  }
}
