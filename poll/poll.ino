
#include <string.h>
#include <Keyboard.h>   // HID Tastatur
#include <U8x8lib.h>    // diverse LCD/OLED Displays  mit 8x8 Pixel Zeichensatz

#include "Rotary.h"     // rotary encoder KY-040 (Drehschalter) Unterstützung

// Tastatur Makro 
typedef struct {
  char* label;          // Name für Auswahl
  char* data;           // einzugebende Sequenz
  char* delays;         // Pausen nach jedem Zeichen
} Macro;

#define ENC_CLK 5       // Pin für Drehschalter Taktimpuls
#define ENC_DT  6       // Pin für Drehschalter Daten
#define ENC_SW  7       // Pin für Drehschalter Taste

#define DEBOUNCE_MILLIS 100 // Entprelldauer Drehschalter-Taste
#define LINE_LENGTH 16      // Zeichen/Displayzeile

#define MACRO_COUNT 4   // Anzahl der Makros

// Daten für Makros
Macro macros[MACRO_COUNT] = {
  { 
    label: "fee@foo.com", 
    data: "geheim\n",
    delays: "\1\100\1\100\1\1"
  },
  { 
    label: "Stamplar DD", 
    data: "12345\n",
    delays: "\1\1\1\1\1\1"
  },
  { 
    label: "Magplar DD", 
    data: "54321\n",
    delays: "\1\1\1\1\1\1"
  },
  { 
    label: "Harri Hirsch", 
    data: "Harri Hirsch\n",
    delays: "\100\100\100\100\100"
  }
};

// Drehschalter Instanz
Rotary rotary = Rotary(ENC_CLK, ENC_DT);

// Display Instanz (für 128*64 Pixel OLED Display mit SSD1306 controller am I2C Bus)
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   

volatile int counter = 0;     // aktuelle Drehschalter Position (Schalter)
bool hidEnabled = false;      // Makro-Ausgabe an HID eingeschaltet ?
bool playing = false;         // wird gerade ein Makro abgespielt ?
bool autoplay = false;        // Makro wiederholen ?
char* playPosition = nullptr; // nächstes Zeichen für aktuelles Makro
char* delayPosition = nullptr;// nächste Pause für aktuelles Makro

// Arduino Einrichtung 
void setup() {
  pinMode(ENC_SW, INPUT_PULLUP);  // Pin für Drehschalter Taste als Eingang
  
  // Display initialisieren und Kopfzeile invertiert ausgeben
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);  
  u8x8.setCursor(0,1);
  u8x8.inverse();
  u8x8.print("Makro auswählen");
  u8x8.noInverse();
}

// ausgewähltes Makro anzeigen
void updateLabel(int pos) {
  char* label = macros[pos].label;
  u8x8.setCursor(0,2);
  u8x8.print(label); 
  // Restzeile löschen
  int blanks = LINE_LENGTH - strlen(label);
  for (int n=0; n < blanks; ++n) {
    u8x8.print('.'); 
  } 
}

// Makro wiedergeben
void play() {
  // Makro auswählen
  Macro* mac = &macros[counter];
  
  // setze auf erstes Zeichen und dessen Dauer
  if (playing && !playPosition) {
    playPosition = mac->data;
    delayPosition = mac->delays;
  }

  // Ende des Makros erreicht
  if (!playPosition) {
    playing = false;
    return;
  }

  if (!delayPosition) {
    delayPosition = mac->delays;
  }
  
  // Zeichen als Tastendruck an Host schicken und Pause
  Keyboard.write(*playPosition++);
  delay((int)(*delayPosition++) * 10);

  if (!*playPosition) { // Makro Ende ?
    if (autoplay) { // wiederholen
      char* playPosition = mac->data;
      delayPosition = mac->delays;
    } else { // Nakrowiedergabe beenden
      playPosition = nullptr;
      delayPosition = nullptr;
      playing = false;
    }
  }
}

// Makro Wiedergabe auslösen
void startPlay(bool pressed) {
  // Makro-Wiedergabe eingeschaltet und Drehschalter gedrückt ?
  if (hidEnabled && pressed) {
    playing = true;
  }
}

// Arduino Hauptschleife
void loop() {
  // behandle Drehschalter-Taste
  rotary.click(ENC_SW, DEBOUNCE_MILLIS, startPlay);
  // behandle Drehschalter-Drehung
  rotate();
  // aktivierte Makros wiedergeben
  play();
}

// Drehschalter Drehung behandeln
void rotate() {
  // frage aktuellen Drehschalter Zustand ab
  unsigned char result = rotary.process();
  if (result == DIR_CW) { // Linksdrehung efolgt
    if (--counter < 0) counter = MACRO_COUNT -1;
    updateLabel(counter); // anzeigen
  } else if (result == DIR_CCW) { // Rechtsdrehung erfolgt
    if (++counter >= MACRO_COUNT) counter = 0;
    updateLabel(counter); // anzeigen
  }
}
