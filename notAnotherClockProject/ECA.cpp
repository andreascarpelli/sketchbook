#include "ECA.h"

#include <U8glib.h>

#define CHANGE_MS 5000

extern U8GLIB_ST7920_128X64 *lcd;
extern SFRGBLEDMatrix *ledMatrix;

void ECA::newAutomata(){
  byte x;
  byte y;
  char buff[10];
  // new rule
  rule=random(255);
  // show on LCD
  x=lcd->getWidth()/2-lcd->getStrWidth(itoa(rule, buff, 10))/2;
  y=lcd->getHeight()/2+lcd->getFontAscent()/2;
  lcd->firstPage();
  do {
    lcd->drawStr(x, y, itoa(rule, buff, 10));
  } 
  while( lcd->nextPage() );
  row=1;
  color=ledMatrix->spectrum(random(ledMatrix->spectrumLen()), ledMatrix->spectrumLen()-1);
  for(byte c=0;c<ledMatrix->width;c++)
    if(random(2))
      ledMatrix->paintPixel(color, c, 0);
    else
      ledMatrix->paintPixel(BLACK, c, 0);
  ledMatrix->show();
  lastLCDUpdate=millis();
}

byte ECA::getPattern(int x, int y){
  byte p;
  if(ledMatrix->getPixel(x-1, y-1)==color)
    p=4;
  if(ledMatrix->getPixel(x, y-1)==color)
    p=p|2;
  if(ledMatrix->getPixel(x+1, y-1)==color)
    p=p|1;
  return p;
}

ECA::ECA():
Mode(PSTR("ECA")){
  newAutomata();
}

void ECA::loop(){
  unsigned long time;
  time=millis();
  for(int c=0;c<ledMatrix->width;c++)
    if(rule&(1<<getPattern(c, row)))
      ledMatrix->paintPixel(color, c, row);
    else
      ledMatrix->paintPixel(BLACK, c, row);
  ledMatrix->show();
  if(++row>=ledMatrix->height){
    if(time-lastLCDUpdate>CHANGE_MS){
      newAutomata();
      lastLCDUpdate=time;
    }
  }
}

ECA::~ECA(){

}




















