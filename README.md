# Benismeter
Arduino Benis Anzeige

<h5>Benötigte Hardware:</h5>
1x Arduiono UNO<br>
1x Ethernet Shield<br>
1x LCD 1602<br>
einige Jumper Wires<br>
(Optional) 1x Potiontiometer ab 5k Ohm<br>
(Optional) 1x Breadboard<br>

<h5>Verbindung der Ports:</h5>

<h6>Spannungsversorgung des LCDs:</h6>

LCD VDD an 5V<br>
LCD VSS an Gnd<br>

<h6>Datenpins des LCDs:</h6>

LCD E an Pin 2<br>
LCD RS an Pin 3<br>
LCD D4 an Pin 5<br>
LCD D5 an Pin 6<br>
LCD D6 an Pin 8<br>
LCD D7 an Pin 9<br>
LCD RW an Gnd (RW ist für Lesen und Schreiben auf dem LCD, da wir nur schreiben kommt der Pin auf Gnd)<br>

<h6>Beleuchtung des LCDs:</h6>

LCD A an 3,3V<br>
LCD K an Gnd<br>

<h6>Kontrast des LCDs:</h6>

LCD V0 an den mittleren Pin des Potentiometers<br>
Und an den äußeren Pins 5V und Gnd<br>
(Falls man kein Poti zur Hand hat kann man dort auch direkt Gnd anschließen, dann hat das LCD allerdings einen sehr hohen Kontrast.)

<h5>Programmieren:</h5>

Arduino IDE(http://arduino.cc/en/Main/Software) starten und die pr0meter.ino anpassen:
const String username = "yannik12"; yannik12 durch den eigenen Benutzernamen ersetzen.

Arduino UNO über USB verbinden.
In Arduino unter Tools->Boards->Arduino UNO auswählen.
In Arduino unter Tools->Serieller Port->Den Port des Arduino UNO auswählen.
Mit STRG+Umschalt+U den Code kompilieren und auf den Arduion hochladen.
