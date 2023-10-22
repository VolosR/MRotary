#include <Arduino_GFX_Library.h>
#include <RotaryEncoder.h>
#include <ESP32Time.h>
#include <Wire.h>
#include "touch.h"
#include "bigFont.h"
#include "midleFont.h"
#include "smallFont.h"
#include "valueFont.h"

double rad=0.01745;

float x[360]; //outer point
float y[360];
float px[360]; //ineer point
float py[360];
float lx[360]; //long line 
float ly[360];
float shx[360]; //short line 
float shy[360];
float tx[360]; //text
float ty[360];

int PPgraph[24]={0};

int angle=0;
int value=0;
int chosenFont;
int chosenColor;
int r=118;
int sx=-2;
int sy=120;
int inc=18;
int a=0;
int prev=0;
String secs="00";
int second1=0;
int second2=0;
bool onOff=0;
String OO[2]={"OFF","ON"};
int deb=0;


#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define I2C_SDA_PIN 17
#define I2C_SCL_PIN 18
#define TOUCH_RST -1 // 38
#define TOUCH_IRQ -1 // 0

ESP32Time rtc(0); 

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    1 /* CS */, 46 /* SCK */, 0 /* SDA */,
    2 /* DE */, 42 /* VSYNC */, 3 /* HSYNC */, 45 /* PCLK */,
    11 /* R0 */, 15 /* R1 */, 12 /* R2 */, 16 /* R3 */, 21 /* R4 */,
    39 /* G0/P22 */, 7 /* G1/P23 */, 47 /* G2/P24 */, 8 /* G3/P25 */, 48 /* G4/P26 */, 9 /* G5 */,
    4 /* B0 */, 41 /* B1 */, 5 /* B2 */, 40 /* B3 */, 6 /* B4 */
);

// Uncomment for 2.1" round display
Arduino_ST7701_RGBPanel *gfx = new Arduino_ST7701_RGBPanel(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */,
    false /* IPS */, 480 /* width */, 480 /* height */,
    st7701_type5_init_operations, sizeof(st7701_type5_init_operations),
    true /* BGR */,
    10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);


#define PWM_CHANNEL 1
#define PWM_FREQ 5000//Hz
#define pwm_resolution_bits 10
#define IO_PWM_PIN 38

int n=0;
int xt = 0, yt = 0;

#define PIN_IN1 13
#define PIN_IN2 10
#define BUTTON 14

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

unsigned short grays[13];
#define red 0xD041
#define blue 0x0AD0
#define yellow 0x9381
#define bck TFT_BLACK
char dd[7]={'m','t','w','t','f','s','s'};

void setup() {
  pinMode(IO_PWM_PIN, OUTPUT); 
  pinMode(BUTTON, INPUT_PULLUP); 
  ledcSetup(PWM_CHANNEL, PWM_FREQ, pwm_resolution_bits);  
  ledcAttachPin(IO_PWM_PIN, PWM_CHANNEL); 
  ledcWrite(PWM_CHANNEL, 840); 

  rtc.setTime(0,47,13,10,23,2023,0); 

  sprite.createSprite(400,240);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  gfx->begin();
  gfx->fillScreen(BLACK);
 

     int co=220;
     for(int i=0;i<13;i++)
     {
     grays[i]=tft.color565(co, co, co);
     co=co-20;
     }

       for(int i=0;i<360;i++)
  {
       x[i]=(r*cos(rad*i))+sx;
       y[i]=(r*sin(rad*i))+sy;
       px[i]=((r-5)*cos(rad*i))+sx;
       py[i]=((r-5)*sin(rad*i))+sy;

       lx[i]=((r-24)*cos(rad*i))+sx;
       ly[i]=((r-24)*sin(rad*i))+sy;

       shx[i]=((r-12)*cos(rad*i))+sx;
       shy[i]=((r-12)*sin(rad*i))+sy;

      tx[i]=((r+28)*cos(rad*i))+sx;
      ty[i]=((r+28)*sin(rad*i))+sy;
  }
  draw();
}

void draw()
{

    
  
  sprite.fillSprite(grays[11]);
  sprite.drawWedgeLine(54,120,80,120,2,2,TFT_RED);
    for(int j=0;j<24;j++)
    for(int i=0;i<PPgraph[j];i++)
    sprite.fillRect(188+(j*6),90-(i*4),4,3,grays[5]);
  
  sprite.fillRect(180,136,100,3,grays[7]);
  sprite.fillRect(186,130,3,34,grays[7]);


  sprite.setTextDatum(4);
  sprite.loadFont(smallFont);
  sprite.drawString("%",60,108);
  sprite.unloadFont();

   sprite.loadFont(midleFont);

   for(int i=0;i<120;i++)
 {
   a=angle+(i*3);
   if(a>359)
   a=(angle+(i*3))-360;
   

   sprite.drawPixel(x[a],y[a],grays[6]);
   sprite.setTextColor(grays[2],grays[8]);

   if(i%3==0)
   sprite.drawWedgeLine(x[a],y[a],x[a]-6,y[a],1,1,grays[5],bck);

   if(i%6==0)
   sprite.drawWedgeLine(x[a],y[a],x[a]-18,y[a],2,2,grays[4],bck);
   if(i%12==0){
   sprite.drawWedgeLine(x[a],y[a],x[a]-30,y[a],2,2,grays[3],bck);
   sprite.drawString(String((i/6)*5),tx[a],ty[a],2);
   }

}
    


  sprite.setTextDatum(4);
  sprite.setTextColor(grays[2],grays[8]);
  
  for(int i=0;i<7;i++)
  {
    sprite.fillSmoothRoundRect(186+(i*30),2,26,26,3,grays[8],bck);
    sprite.drawString(String(dd[i]),186+((i+1)*30)-17,17);
  }
  sprite.unloadFont();
  sprite.drawWedgeLine(199+(rtc.getDayofWeek()*30),35,199+(rtc.getDayofWeek()*30),40,1,3,grays[3],bck);
  
  
  sprite.setTextDatum(0);
  sprite.setTextColor(grays[0],bck);
  sprite.loadFont(bigFont);
  sprite.drawString(rtc.getTime().substring(0,5),196,150);
  sprite.unloadFont();

   sprite.setTextDatum(0);
  sprite.setTextColor(grays[1],bck);
  sprite.loadFont(midleFont);
  sprite.drawString("October 23",196,104);  ////////////////////////date hard coded
  sprite.setTextDatum(4);
  sprite.fillRect(0,145,50,30,grays[9]);
  sprite.setTextColor(grays[3],grays[9]);
  sprite.drawString(OO[onOff],25,162); 
   
  sprite.unloadFont();


  sprite.setTextDatum(4);
  sprite.setTextColor(grays[1],bck);
  sprite.loadFont(valueFont);
  sprite.drawString(String((int)(value/10.00)),24,124);
  sprite.setTextColor(grays[3],bck);
   sprite.drawString(secs,362,78);   /// /////////////////////////////////seconds
  sprite.unloadFont();

  sprite.setTextColor(grays[7],bck);
 // sprite.drawString(String(x),150,50,4);
 // sprite.drawString(String(y),250,50,4);
  gfx->draw16bitBeRGBBitmap(40,120,(uint16_t*)sprite.getPointer(),400,240);

}


void readEncoder()
 {

  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    
    if(newPos>pos)
    angle=angle+inc;
    if(newPos<pos)
    angle=angle-inc;
    
    pos = newPos;
  } 
 if(angle<0)
    angle=359;

  if(angle>=360)
    angle=0;
}



void loop() {
   readEncoder();
  

   for(int i=0;i<24;i++)
      PPgraph[i]=random(1,12);

  if(digitalRead(BUTTON)==0)
  {
  if(deb==0) {deb=1; onOff=!onOff; draw();}
  }else deb=0;
 
  if (read_touch(&xt, &yt) == 1) {if(yt<240)angle=angle-3; else angle=angle+3;}
  
 second1=rtc.getSecond();

 if(second1<10) secs="0"+String(second1); else secs=String(second1);

 if(second1!=second2)
 {second2=second1; draw();}

  value=map(angle,0,359,1000,0);
  if(angle!=prev){
  draw();
  prev=angle;
  } 
 
}
