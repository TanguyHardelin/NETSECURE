#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

const char * touche = "123456789C0<";
bool demarage = false;
bool digicode_started=false;
String user_input = "";


void setup() {
  Serial.begin(9600);
  //Serial.println("ILI9341 Test!");

  tft.begin();

  /*
    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

  */
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  Serial.println("Touchscreen started");
}


void loop(void) {
  //Affiche le clavier au démarage:
  if (!demarage) {
    demarage = true;
    tft.fillScreen(ILI9341_WHITE);
  }

  //On lit les messages du raspberry:
  String message_from_serial=ReadSerial();
  action_serial(message_from_serial);
  
  // On regarde si quelqu'un appui
  if (ts.bufferEmpty()) {
    return;
  }
  if(!digicode_started){
    return ;
  }
  
  TS_Point p = ts.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  char c = get_touch_pressed(p.x, p.y);
  //On envoie le charactere au raspberry:
  Serial.println(c);

  user_input += c;
  if (c == 'C') {
    user_input = "";
  }
}
void action_serial(String message){
 //On regarde si on est pas en error
 if (message== "E"){
      error_affichage();
      user_input = "";
      //On remet tout blanc
      update_affichage();

      //On vide le buffer
      TS_Point p_tmp = ts.getPoint();
      p_tmp.x = map(p_tmp.x, TS_MINX, TS_MAXX, 0, tft.width());
      p_tmp.y = map(p_tmp.y, TS_MINY, TS_MAXY, 0, tft.height());
      while (get_touch_pressed(p_tmp.x, p_tmp.y) && !ts.bufferEmpty()) {
        p_tmp = ts.getPoint();
        p_tmp.x = map(p_tmp.x, TS_MINX, TS_MAXX, 0, tft.width());
        p_tmp.y = map(p_tmp.y, TS_MINY, TS_MAXY, 0, tft.height());
      }
  }
  if (message=="D"){
     draw_text("Lanestel");
     digicode_started=false;
  }
  if(message=="N"){
    update_affichage();
    digicode_started=true;
  }
}
void draw_text(String text){
  //On clear l'écran:
  tft.fillScreen(ILI9341_WHITE);

  //On affiche les touches:
  int w = tft.width(), h = tft.height();
  for (int i = 0; i < text.length(); i ++) {
      tft.drawChar(i*30+5, h/2-10,text[i], ILI9341_BLACK, ILI9341_WHITE, 5);
  }
  yield();
}

char get_touch_pressed(int x, int y) {
  //On affiche les touches:
  int w = tft.width(), h = tft.height();
  int largeur_touche = w / 3 - 2;
  int hauteur_touche = h / 4 - 2;
  int index_touche_x = 0, index_touche_y = 0;

  int i;
  for (i = 0; i < x; i += (largeur_touche + 2)) {
    index_touche_x++;
  }
  int j;
  for (j = 0; j < y; j += (hauteur_touche + 2)) {
    index_touche_y++;
  }


  tft.fillRect(i - largeur_touche - 2, j - hauteur_touche - 2, largeur_touche, hauteur_touche, ILI9341_BLUE);
  tft.drawChar(i - largeur_touche + largeur_touche / 4, j - hauteur_touche + hauteur_touche / 4, touche[(index_touche_x - 1) + (index_touche_y - 1) * 3], ILI9341_WHITE, ILI9341_BLUE, 5);
  int compteur = 0;

  //On vide le buffer
  while (ts.touched())
    TS_Point p = ts.getPoint();


  delay(20);
  tft.fillRect(i - largeur_touche - 2, j - hauteur_touche - 2, largeur_touche, hauteur_touche, ILI9341_WHITE);
  tft.drawChar(i - largeur_touche + largeur_touche / 4, j - hauteur_touche + hauteur_touche / 4, touche[(index_touche_x - 1) + (index_touche_y - 1) * 3], ILI9341_BLACK, ILI9341_WHITE, 5);
  return touche[(index_touche_x - 1) + (index_touche_y - 1) * 3];

}
void update_affichage() {
  //On clear l'écran:
  tft.fillScreen(ILI9341_BLACK);

  //On affiche les touches:
  int w = tft.width(), h = tft.height();
  int largeur_touche = w / 3 - 2;
  int hauteur_touche = h / 4 - 2;
  int index_touche_x = 0, index_touche_y = 0;
  for (int i = 0; i < w; i += (largeur_touche + 2)) {
    for (int j = 0; j < h; j += (hauteur_touche + 2)) {
      tft.fillRect(i, j, largeur_touche, hauteur_touche, ILI9341_WHITE);
      tft.drawChar(i + largeur_touche / 4, j + hauteur_touche / 4, touche[index_touche_x + index_touche_y * 3], ILI9341_BLACK, ILI9341_WHITE, 5);
      index_touche_y++;
    }
    index_touche_x++;
    index_touche_y = 0;
  }
  yield();

}

void error_affichage() {
  //On clear l'écran:
  tft.fillScreen(ILI9341_BLACK);

  //On affiche les touches:
  int w = tft.width(), h = tft.height();
  int largeur_touche = w / 3 - 2;
  int hauteur_touche = h / 4 - 2;
  int index_touche_x = 0, index_touche_y = 0;
  for (int i = 0; i < w; i += (largeur_touche + 2)) {
    for (int j = 0; j < h; j += (hauteur_touche + 2)) {
      tft.fillRect(i, j, largeur_touche, hauteur_touche, ILI9341_RED);
      tft.drawChar(i + largeur_touche / 4, j + hauteur_touche / 4, touche[index_touche_x + index_touche_y * 3], ILI9341_WHITE, ILI9341_RED, 5);
      index_touche_y++;
    }
    index_touche_x++;
    index_touche_y = 0;
  }
  yield();
  String t="";
  while (ReadSerial()!= "N") {    
    delay(1000);
  }
  //On vide le buffer:
   while (ts.touched()){
     TS_Point p = ts.getPoint();
     p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
     p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
     char c = get_touch_pressed(p.x, p.y);
   }
}

String ReadSerial(){
  String retour="";
  while(Serial.available()){
    char c=Serial.read();
    retour+=c;
  }
  return retour;
}


