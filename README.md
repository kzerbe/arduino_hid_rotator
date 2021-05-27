
# Arduino HID Rotator

ieser Artikel zeigt, wie man mit einem preiswerten Microcontroller zu den meisten Endgeräten (PC, Tablet, Smartphone, Spielkonsole) kompatible Eingabegeräte (HIDs) selbst bauen kann, ohne dass dort Treiber installiert werden müssen. Ich möchte hier alles Wissen vermitteln (oder zumindest verlinken), welches man zum Aufbau und Programmierung solcher HIDs - die Maus, Tastatur oder Gamepad ersetzen können - braucht.  Schwerpunkt  ist dabei Automatisierung mittels "Makros", welche komplexe Eingabefolgen - die man aus einem Menü auswählen kann - an ein Endgerät sendet. Mehrere solcher HIDs kann man parallel zu den bereits vorhandenen (wie Tastatur und Maus) betreiben, indem man sie einfach an einen freien USB-Port steckt.

## Computer, Mobilgeräte und ihre "Peripherie"

Das wie die bekannte Bastelcomputer Serie Raspberry PI für Ausbildungszwecke geschaffene [Arduino Projekt](arduino.cc) wurde nicht  nur für Bildungseinrichtungen und Tüftler ("Maker"), sondern allen aufgegriffen, die schnell und einfach etwas steuern, regeln oder ihr Heim digitalisieren" wollen (smart home). Dabei sind die Raspberrys eher wie "richtige" Computer mit einem Betriebssystem (meist einer Linux-Distribution) ausgestattet, während die "Arduinos" sehr wenig Computerleistung aber viele Anschlußmöglichken (Peripherie) für Sensoren, Schalter, LEDs und Aktoren (etwa Motoren) bieten. Das Leistungsspektrum reicht hier bis zu 3D- Druckern, Mäh & Saug-Robotern und Wetterstationen.

Der gravierende Unterschied kommt daher, daß in den Raspberrys ein "System on a Chip" (SoC) vergleichbar  mit jenem von aktuellen Smartphones werkelt (also ein 64-Bit Prozessor um die 1 GHz Takt und oft mehr als einem Gigabyte Arbeitsspeicher), während die für Arduino-Projekte eingesetzen 8-Bit Microcontroller nur wenige Kilobyte Arbeitsspeicher haben und mit maximal 16 MHz getaktet sind. In vielen Projekten findet man deshalb beides und von solchen Microcontrollern braucht jeder "richtige Computer" (auch Smartphones) etliche in seiner Peripherie: Tastatur, Maus, Drucker, Festplatte, Kartenleser, Bluetooth/WLAN-Funk, Ladeschaltung, Lüfterregelung et cetera pp.

Früher hatten Computer viele unterschiedliche serielle und parallele Schnittstellen mit unterschiedlichen Steckern und Leistungsdaten. Inzwischen gibt es fast nur noch den [Universal Serial Bus (USB)](https://de.wikipedia.org/wiki/Universal_Serial_Bus) für alles. Ohne einen MicroController kann man inzwischen aber gar nichts mehr anschließen, selbst USB-Ladegeräte und Powerbanks müssen sich mit dem Mobilgerät auf eine Ladespannung einigen, denn mit der traditionellen USB-Betriebsspannung (5 Volt) dauert das Laden gefühlt ewig.

Die Vielzahl der unterschiedlichen USB-Geräte erfordert individuelle Protokolle, Geräteklassen und Betriebssystem-Treiber auf den "Host"-Rechnern, an welche man Peripheriegeräte anschließen will. Eine solche USB Geräteklasse ist [Human Interface Device (HID)](https://de.wikipedia.org/wiki/Human_Interface_Device) für Eingabegeräte wie Tastaturen, Mäuse, Gamepads und sonstige "Controller". Fast alle modernen Hosts mit USB-Anschlüssen unterstützen HID-Protokolle mit Standard-Treibern und man kann so mehrere HID-Geräte an einen Host anschließen.

Die meisten als "Arduino" bezeichneten Microcontroller-Boards sind über USB-Kabel anschließbar, aber die älteren verwenden Microcontroller, die nur über einen [Universal Asynchronous Receiver Transmitter (UART)](https://de.wikipedia.org/wiki/Universal_Asynchronous_Receiver_Transmitter) mit dem Host kommunizieren können. Den hierzu benötigten [RS-232](https://de.wikipedia.org/wiki/RS-232) seriellen Anschluß haben die meisten Computer nicht mehr und stattdessen ist auf älteren Ardiono Boards ein [USB Serial Port Adapter](https://openkb.zerbe.cloud/kb/usb-to-serial-adapter) aufgelötet.  Solche USB-Adapter basieren auch Chips unterschiedlicher Hersteller und brauchen passende USB-Treiber, die häufig für bestimmte Betriebssystem-Versionen nicht verfügbar, fehlerhaft, unsicher oder schwierig zu installieren sind. So gelingt die Programmierung mancher billigen, angeblich "Arduino-kompatiblen" Boards an manchem Rechner oft gar nicht.

## Arduino Boards mit USB HID Schnittstelle

Was liegt also näher, als Microcontroller zu verwenden, die USB selbst komplett unterstützen, um z.B. die generische HID Geräteklasse direkt verwenden zu können. Arduino-Boards sind meist mit einem der vielen [Microchip AVR](https://de.wikipedia.org/wiki/Microchip_AVR) Controller bestückt, ursprünglich dem ATmega 168 bzw. 328, der aber USB nicht unterstützt. Der neuere [ATmega 32u4](https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ATMega32U4.pdf) hat aber recht ähnliiche Leistungsdaten und besitzt selbst ein USB-Interface. Die Arduino-Software hat dafür auch eine HID Programmbibliothek. Dieser Controller wurde ursprünglich auf den Boards [Arduino Leonardo](https://store.arduino.cc/arduino-leonardo-with-headers) und [Arduino Micro](https://store.arduino.cc/arduino-micro) verbaut, aber unzählige Nachbauten überwiegend asiatischer Provinienz haben manchen Vorteil - vor allem aber einen günstigeren Preis. Bei den billigsten kommt es da aber auch zu signifikanten Qualitätsmängeln durch Einsparungen und schlechte Verarbeitung.

Das oben verlinkte circa 450 Seiten starke Handbuch des winzigen Atmega 32u4 ist sicher für Elektronikbastler etwas heftig, zeigt aber allein vom Umfang, das trotz nur 32 KB Flash-Speicher, 2.5 KB Arbeitsspeicher bei gerade mal 16 MHz Takt recht viel geht. Das liegt vor allem daran, dass Arduino-Boards mit einem vorinstalliertem [Bootloader](http://arduino-basics.com/arduino-mobil/standalone/bootloader/) ausgeliefert werden, der viele kritische Einstellungen bereits vornimmt und die Programmierung über die USB-Schnittstelle und den Start (Bootvorgang) selbst erstellter Anwendungen veranlasst. Die meisten Boards haben für letzteres noch einen Reset-Taster, bei manchen ist ein weiterer Taster zum Progammieren notwendig. Bei den Arduinos mit 32u4 sorgt der Bootloader auch schon dafür, dass das Board dem Host auch schon z.B. als "Arduino Leonardo" mit den Geräteklassen HID sowie CDC(ACM) sichtbar ist uns somit neben den genannten HID Eingabegeräten auch noch Kommunikationsgeräte (wie Modems und Arduinos Programmierschnittstelle) unterstützt - ohne das dafür weitere Host-Treiber benötigt würden.

Zur Anwendungsprogrammierung kann kostenlos die [Arduino IDE](https://www.arduino.cc/en/software) herunter geladen werden. Sie enthält auch die [GNU Compiler Collection](https://de.wikipedia.org/wiki/GNU_Compiler_Collection) mit der Anwendungen in den Sprachen C bzw. C++ entwickelt werden können, was durch ein umfangreiches Rahmenwerk und viele fertige Bibliotheken sehr einfach ist. Als IDE ist von Quelltext-Editierung, Übersetzung bis Hochladen auf das Board alles dabei, zu den Bibliotheken gehören auch viele Beispiele, sodaß der Einstieg sehr einfach bleibt, auch wenn ganz andere Microcontroller- Boards gebraucht werden, denn mittels "Boardverwalter" können auch Boards unterstützt werden, die ganz andere Microcontroller - auch 32/64- bittige wie etwa ESP8266, ESP32 oder Arm Cortex verwenden. Mit der Arduino IDE können auch viele Arduino Boards als Programmiergerät für noch gänzlich unprogrammierte Microcontroller verwendet werden, also einen Arduino Boodloader aufkopieren, sodaß zur Produktion Arduino basierter Peripheriegeräte noch nicht einmal ein Arduino-Board nötig ist.

Der Quelltexteditor ist leider etwas schwach, es können aber externe Editoren - etwa VisualStudio Code verwendet werden - diese beliebte Microsoft-IDE integriert sich sehr gut mit dem Arduino Workflow.

## Anschlussfreudige Microcontroller Boards
![file](https://openkb.zerbe.cloud/uploads/inline_files/image-1622123592959.png)
Wer Microcontroller-Board Anschlußpläne wie das obige noch nie gesehen hat, fühlt sich vielleicht davon erschlagen. Das ist jedenfalls der Anschlußplan für den in diesem Projekt verwendeten [Arduino Pro Micro](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro), der vom Hersteller Sparkfun (siehe Link) und vielen - meist asiatischen - "Nachbauern"  vertrieben wird.

Was gleich auffällt, ist die Mehrfachbelegung der Anschlüsse. Was vielleicht jemand - der sich schon mal mit Mikroprozessoren beschäftigt hat - vermisst, sind Address- und Datenbusse. Bei Mikroprozessoren und den meisten SoCs wie sie auch in PCs zu finden sind, liegt zumindest der Arbeitsspeicher außerhalb des Prozessors, was zu vielen Anschlußleitungen und komplizierten, mehrlagigen Mainboards führt. Mikrocontroller brauchen für ihre Aufgabe meist nur wenig Speicher und der ist fest eingebaut und nicht erweiterbar. Sie vermeiden weiterhin viele Anschlüsse durch möglichst flexibel programmierbare Mehrfachbelegung von diesen. 

So sind die grün markierten Anschlüsse A0 .. A9 Analog-Eingänge, die angelegte Spannungen messen, die manche Sensoren (etwa für Temperatur oder Druck) liefern. Die roten mit [Pulsweitenmodulation (PWM](https://de.wikipedia.org/wiki/Pulsdauermodulation) können als Pseudo-Analogausgänge genutzt werden, um etwa die Helligkeit von LEDs oder den Drehwinkel von Servomotoren zu steuern. Die gelben als "Serial" markierten RX- und TX- Leitungen gehören zu einem der oben erwähnten UARTs, das auch zur Programmierung des Boards und Kommunikation mit dem Host genutzt wird.  [Serial Peripheral Interface (SPI )](https://de.wikipedia.org/wiki/Serial_Peripheral_Interface) (lila) und [I2C ](https://de.wikipedia.org/wiki/I%C2%B2C) (grau) sind populäre serielle "Busse".  In diesem Projekt wird z.b. ein OLED-Display per I2C angeschlossen und SPI-Varianten werden etwa zur Ansteuerung von Grafik-Displays und Flash-Speichern wie SD Karten und dem Flash-Speicher in Microcontrollern genutzt. So kann man dieses Projekt auch um einen SD-Kartenleser erweitern, um viele und längere Makros auf einer Speicherkarte abzulegen. Die blauen Nummern der Anschlüsse geben die Pin-Nummern eines Vorlage-Arduinos (in diesem Fall Leonardo) an, die in der Software zur Addressierung der Anschlüsse zu verwenden sind; All diese "blauen" Pins können bei Bedarf auch entweder als digitaler Eingang oderAusgang genutzt werden, wobei Ausgänge durchaus mit einigen Milliampere Last (z.B. eine LED mit Vorwiderstand, aber kein Relais, Motor o.ä,) belastbar sind. 

Mit mehr Last als insgesamt 40 mA sollte man den das Board aber nicht quälen, zumal da auch noch ein Spannungsregler verbaut ist, sodass man an den RAW-Anschluß bis zu 12 V anlegen kann. Die Spannung VCC am 32u4 kann je nach Boardversion aber Arduino-typisch 5V oder den heute weitgehend üblichen 3.3V betragen. Die Einhaltung der richtigen Betriebsspannung (3.3 oder 5 Volt) in der gesamten Schaltung ist aber wichtig, weil eine höhere Spannung die Chips zerstört. Eine Reset-Taste hat dies Board nicht; dieser kann aber an die RST Leitung gegen GND geschaltet angeschlossen werden. Mehr zum Pro Micro in dem im ersten Absatz verlinktem Datenblatt.

## Sonstige Bauteile
Für das Projekt werden sonst noch benötigt:

### OLED Grafikdisplay mit mindestens 128 x 32 Pixel und I2C Schnittstelle
Das Display wird benötigt um das aktuell auszuführende Makro anzuzeigen. Mit einem 8 x 8 Pixel Zeichensatz können so 2 Zeilen je 16 Zeichen angezeigt werden. Mehr Auflösung schadet natürlich  nicht, wird aber auch nicht gebraucht. Geeignete Display besitzen selbst einen SSD1306 Controller und werden über die beiden I2C Leitungen mit dem Arduino Pro Micro verbunden. Die sehr universelle Arduino Library [U8x8lib](https://github.com/olikraus/u8g2/wiki/u8x8reference) von Oli Kraus kann sehr viele Displays unterstützen. Mit dem Arduino Bibliotheksverwalter bitte das Paket **U8g2** suchen und installieren.


### Drehwinkelgeber mit integriertem Taster (KY-40)

![file](https://openkb.zerbe.cloud/uploads/inline_files/image-1622129966593.jpg)

Ein Drehwinkelgeber enthält zwei in Drehrichtung angebrachte Schalter (A-C und B-C). Je nach Drehrichtung schaltet erst die A-C Verbindung oder die B-C Verbindung. **GND ** und **+**  (VCC 5V) liefern die Stromversorgung (für lediglich 2 Pullup-Widerstände), DT und CLK werden an Digitaleingänge des Arduinos angeschlossen, damit per Software Drehrichtung - und - Winkel bestimmt werden können. SW liefert an einen weiteren Digitaleingang den Zustand des Tasters, der durch Drücken des Drehknops betätigt wird.

Der Drehwinkel dient zur Auswahl des Makros, dessen Name dann auf dem Display angezeigt wird. Wird der Knopf gedrückt, wird diese Makro-Sequenz dem Host eingegeben.

Für diese "rotary encoder" gibt es diverse Arduino-Libraries, die aber wegen starker Schalter-Prellungen (bouncing) nur schlecht funktionieren. Zum Projekt gehört deshalb eine stabile auf dem GitHub Projekt [Rotary von Ben Buxton](https://github.com/buxtronix/arduino/tree/master/libraries/Rotary) aufbauende Implementierung per endlichem Automaten statt der sonst verbreiteten, sehr unzuverlässigen Delay-Lösungen.

### 2 Schalter, ein Taster und eine zweifarbige LED
Zwei Kippschalter werden mit zwei weiteren Digitaleingängen verbunden. Der erste schaltet die Makros bei Bedarf frei. Zumindest während der Programmierung ist es lästig, wenn beim Drücken des Drehschalters Makros unerwünschte Eingaben machen. Der zweite Schalter kann automatische Makro-Wiederholungen ("Rotationen") einschalten. Ein kleiner optionaler Taster kann mit dem RST-Pin verbunden werden, wenn man einen Hardware-Reset wünscht. Zwei Leds - oder eine zweifarbige - (inklusive Vorwiderstand!) an Digitalausgängen oder den Schaltern können (optional) anzeigen, ob gerade Makros ausgeführt werden und ob die automatische Wiederholung aktiviert ist.
 
### [optional] SD Kartenmodul mit SPI Schnittstelle

Von der Projektsoftware noch nicht berücksichtigt, aber leicht erweiterbar ist ein an die SPI- Anschlüsse des Boards anzuschließender SD Kartenleser. Wenn nicht gerade Romane wie "Krieg und Frieden" über die Tastaturschnittstelle "reingehackt" werden sollen, sehe ich in einer Tastatur-Makro-Anwendung keinen Bedarf für so viel Speicher. Allerdings passt in die 32 KB Flash des ATmega 32u4 auch nicht wirklich viel rein und um das zu ändern, muß der ganze Controller neu programmiert werden - und das geht auch nur ~10.000 mal. Die Arduino Libraries SPI und SD sowie ein Dateiparser für die Makros kosten aber auch einiges an Speicher und die nur 2.5 KB Arbeitspeicher könnten dafür auch nicht reichen, sodaß ein "größeres" ATmega- Modell statt dem 32u4 anzuraten wäre. Für eine Anwendung als MIDI-Controller (Sequencer) zur Ansteuerung z.B. eines Musikinstruments (Synthesizer), was per CDC(ACM) ja auch möglich ist, ist eine SD-Karte aber hochinteressant, sodaß ich das mit einem späteren Projekt sicher ausprobieren werde.

## Schaltung  "HID Rotator"
Da nicht viele Verbindungen herzustellen sind bzw. alle Teile nur mit Pins des Arduino verbunden werden, erscheinen mir Verbindungstabellen übersichtlicher als ein Schaltplan oder gar ein hübsches, aber meist verwirrendes Fritzing Bildchen.

| Arduino Pro Micro | Oled Display |
|-----|-----|
| GND | GND |
| VCC | VCC |
| Pin 2 | SDA |
| Pin 3 | SCL |

| Arduino Pro Micro | Drehwinkelgeber |
|-----|-----|
| GND | GND |
| VCC | + |
| Pin 5 | DT |
| Pin 6 | CLK |
| Pin 7 | SW |

| Arduino Pro Micro | Schalter Makro Aktivierung | LED |
|-----|-----|---|
| GND | Mittlerer Kontakt | Kathode LED |
| Pin  4 | Schließkontakt | Anode LED |

| Arduino Pro Micro | Schalter Autorepeat | LED |
|-----|-----|----|
| GND | Mittlerer Kontakt | Kathode LED |
| Pin  8 | Schließkontakt | Anode LED |

An die Schließkontakte kann die Anode bzw. der Vorwiderstand einer LED angeschlossen werden. Die LED leuchtet, wenn der Schalter geschlossen wird.

| Arduino Pro Micro | Reset Taster |
|-----|-----|
| GND | Kontakt |
| RST | Schließkontakt |

