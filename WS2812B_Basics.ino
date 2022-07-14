#include "WS2812B.h"

#if defined(BOARD_RTL8722DM)
#define SPI_MOSI_PIN        11
#elif defined(BOARD_RTL8722DM_MINI)
#define SPI_MOSI_PIN       9
#elif defined(BOARD_RTL8720DN_BW16)
#define SPI_MOSI_PIN       PA12
#endif

#define NUM_OF_LEDS  8*32
#define NUM_OF_ROWS  8
#define NUM_OF_COLS  32
#define FontHeight 4
#define Spacer 2
int HAHA = 0;

const uint16_t PixelMap[] =
{
  0,  15,  16,  31,  32,  47,  48,  63,  64,  79,  80,  95,  96,   111,   112,   127,   128,   143,   144,   159,   160,   175,   176,   191,   192,  207,  208,  223,  224,  239,  240,  255,
  1,  14,  17,  30,  33,  46,  49,  62,  65,  78,  81,  94,  97,   110,   113,   126,   129,   142,   145,   158,   161,   174,   177,   190,   193,  206,  209,  222,  225,  238,  241,  254,
  2,  13,  18,  29,  34,  45,  50,  61,  66,  77,  82,  93,  98,   109,   114,   125,   130,   141,   146,   157,   162,   173,   178,   189,   194,  205,  210,  221,  226,  237,  242,  253,
  3,  12,  19,  28,  35,  44,  51,  60,  67,  76,  83,  92,  99,   108,   115,   124,   131,   140,   147,   156,   163,   172,   179,   188,   195,  204,  211,  220,  227,  236,  243,  252,
  4,  11,  20,  27,  36,  43,  52,  59,  68,  75,  84,  91,  100,  107,   116,   123,   132,   139,   148,   155,   164,   171,   180,   187,   196,  203,  212,  219,  228,  235,  244,  251,
  5,  10,  21,  26,  37,  42,  53,  58,  69,  74,  85,  90,  101,  106,   117,   122,   133,   138,   149,   154,   165,   170,   181,   186,   197,  202,  213,  218,  229,  234,  245,  250,
  6,  9,   22,  25,  38,  41,  54,  57,  70,  73,  86,  89,  102,  105,   118,   121,   134,   137,   150,   153,   166,   169,   182,   185,   198,  201,  214,  217,  230,  233,  246,  249,
  7,  8,   23,  24,  39,  40,  55,  56,  71,  72,  87,  88,  103,  104,   119,   120,   135,   136,   151,   152,   167,   168,   183,   184,   199,  200,  215,  216,  231,  232,  247,  248,
};
const uint16_t PixelMap1[] =
{
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
  96, 97, 98, 99, 100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
  128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
  160,  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,  172,  173,  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  186,  187,  188,  189,  190,  191,
  192,  193,  194,  195,  196,  197,  198,  199,  200,  201,  202,  203,  204,  205,  206,  207,  208,  209,  210,  211,  212,  213,  214,  215,  216,  217,  218,  219,  220,  221,  222,  223,
  224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,  247,  248,  249,  250,  251,  252,  253,  254,  255,
};

WS2812B led(SPI_MOSI_PIN, NUM_OF_LEDS);

const uint16_t Zpix[] =
{
  0xFF, 0xFF,0xFF,0xFF,0xFF,0xFF,
  
};

const uint16_t Zpix1[] =
{
  0xFF, 0xFF,
  //  0x04, 0x00, 0xFF, 0xE0, 0x80, 0x20, 0xBF, 0xA0, 0x20, 0x80, 0x3F, 0x80, 0x20, 0x00, 0x3F, 0xC0, 0x20, 0x40, 0x20, 0x40, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 官
  //  0x04, 0x00, 0xFF, 0xE0, 0x84, 0x20, 0xBF, 0xA0, 0x04, 0x00, 0xFF, 0xE0, 0x4A, 0x40, 0x7F, 0xC0, 0x04, 0x00, 0x52, 0xA0, 0x9F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 憲
  //  0x0F, 0x00, 0xE2, 0x80, 0x44, 0x40, 0x4B, 0xA0, 0xE0, 0x00, 0x4E, 0xA0, 0x4A, 0xA0, 0x6F, 0x40, 0xCB, 0x40, 0x0E, 0xA0, 0x0A, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 瑜
};

void setup() {
  Serial.begin(115200);
  Serial.println("WS2812B test");
  led.begin();
  //Set a specific LED with a certain color
//  led.setPixelColor(0, 50, 0, 0);
//  led.setPixelColor(1, 0, 50, 0);
//  led.setPixelColor(2, 0, 0, 50);
//  led.setPixelColor(8, 0, 0, 50);
//  led.show();
//  delay(1000);
  //Fill the entire LED strip with the same color
  //    led.fill(60, 0, 25, 0, 8);
  //    led.show();
  symbol1();
  led.show();
  delay(1000);
  symbol2(8, 0, 153, 76);
  led.show();
  delay(1000);
  symbol3(16,255,128,0);
  led.show();
  delay(1000);
  symbol4(24,255,204,204);
  led.show();
  delay(1000);
  led.fill(0, 0, 0, 0, 255);
  led.show();
  
  for (int i=0;i<32;i=i+2){
    symbol3(i,255,153,255);
    led.show();
    delay(1000);
    led.fill(0, 0, 0, 0, 255);
    led.show();
    
  }
//  UnicodeChaDisplay(0, 0, 0, 39, 174, 232, HAHA % 2);
}

void loop() {
  //#define Spacer 2
  //  delay(500);
  //  UnicodeChaDisplay(0, 0, 0, 39, 174, 232, HAHA % 2);
  //  for (int i = 0; i < 33; i++)
  //  {
  //    UnicodeChaDisplay(i, 0, 0, 39, 174, 232, HAHA % 2);
  //    Serial.println("WS2812B UnicodeCha");
  //    delay(5000);
  //  }
}

void mapLEDXY(int y, int x, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * NUM_OF_ROWS;

  } else { //odd column
    RGBlocation = 7 - x + y * NUM_OF_ROWS;
  }

  led.setPixelColor(RGBlocation, RED, GREEN, BLUE);

}

void symbol1(){
   // <=
  mapLEDXY(0, 3, 39, 174, 232);
  mapLEDXY(1, 2, 39, 174, 232);
  mapLEDXY(1, 3, 39, 174, 232);
  mapLEDXY(1, 4, 39, 174, 232);
  mapLEDXY(2, 1, 39, 174, 232);
  mapLEDXY(2, 2, 39, 174, 232);
  mapLEDXY(2, 3, 39, 174, 232);
  mapLEDXY(2, 4, 39, 174, 232);
  mapLEDXY(2, 5, 39, 174, 232);
  mapLEDXY(3, 0, 39, 174, 232);
  mapLEDXY(3, 1, 39, 174, 232);
  mapLEDXY(3, 2, 39, 174, 232);
  mapLEDXY(3, 3, 39, 174, 232);
  mapLEDXY(3, 4, 39, 174, 232);
  mapLEDXY(3, 5, 39, 174, 232);
  mapLEDXY(3, 6, 39, 174, 232);
  mapLEDXY(4, 2, 39, 174, 232);
  mapLEDXY(4, 3, 39, 174, 232);
  mapLEDXY(4, 4, 39, 174, 232);
    mapLEDXY(5, 2, 39, 174, 232);
  mapLEDXY(5, 3, 39, 174, 232);
  mapLEDXY(5, 4, 39, 174, 232);
    mapLEDXY(6, 2, 39, 174, 232);
  mapLEDXY(6, 3, 39, 174, 232);
  mapLEDXY(6, 4, 39, 174, 232);
    mapLEDXY(7, 2, 39, 174, 232);
  mapLEDXY(7, 3, 39, 174, 232);
  mapLEDXY(7, 4, 39, 174, 232);
}

void symbol2(int shift, byte RED, byte  GREEN, byte BLUE){
  // .:iL
  mapLEDXY(0+shift, 6, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 6, RED, GREEN, BLUE); 
  mapLEDXY(4+shift, 3, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 4, RED, GREEN, BLUE); 
  mapLEDXY(4+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 6, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 1, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 2, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 3, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 4, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 6, RED, GREEN, BLUE);  
}

void symbol3(int shift, byte RED, byte  GREEN, byte BLUE){
  // (heart)
  mapLEDXY(1+shift, 2, RED, GREEN, BLUE);
  mapLEDXY(1+shift, 3, RED, GREEN, BLUE);
  mapLEDXY(1+shift, 4, RED, GREEN, BLUE);
  
  mapLEDXY(2+shift, 1, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(2+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 5, RED, GREEN, BLUE); 

  mapLEDXY(3+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(3+shift, 3, RED, GREEN, BLUE);  
  mapLEDXY(3+shift, 4, RED, GREEN, BLUE); 
  mapLEDXY(3+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(3+shift, 6, RED, GREEN, BLUE); 

  mapLEDXY(4+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(4+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 6, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 7, RED, GREEN, BLUE); 
  
  mapLEDXY(5+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(5+shift, 3, RED, GREEN, BLUE);
  mapLEDXY(5+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(5+shift, 5, RED, GREEN, BLUE); 
  mapLEDXY(5+shift, 6, RED, GREEN, BLUE);  
  
  mapLEDXY(6+shift, 1, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 5, RED, GREEN, BLUE); 
  
  mapLEDXY(7+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(7+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(7+shift, 4, RED, GREEN, BLUE);  

}

void symbol4(int shift, byte RED, byte  GREEN, byte BLUE){
  // (R)
  mapLEDXY(0+shift, 2, RED, GREEN, BLUE);
  mapLEDXY(0+shift, 3, RED, GREEN, BLUE);
  mapLEDXY(0+shift, 4, RED, GREEN, BLUE);
  mapLEDXY(0+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(1+shift, 1, RED, GREEN, BLUE);  
  mapLEDXY(1+shift, 1, RED, GREEN, BLUE); 
  mapLEDXY(2+shift, 0, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 2, RED, GREEN, BLUE); 
  mapLEDXY(2+shift, 3, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 4, RED, GREEN, BLUE); 
  mapLEDXY(2+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(2+shift, 7, RED, GREEN, BLUE); 
  mapLEDXY(3+shift, 0, RED, GREEN, BLUE);  
  mapLEDXY(3+shift, 2, RED, GREEN, BLUE); 
  mapLEDXY(3+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(3+shift, 7, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 0, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 2, RED, GREEN, BLUE); 
  mapLEDXY(4+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(4+shift, 7, RED, GREEN, BLUE);
  mapLEDXY(5+shift, 0, RED, GREEN, BLUE);  
  mapLEDXY(5+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(5+shift, 5, RED, GREEN, BLUE);  
  mapLEDXY(5+shift, 7, RED, GREEN, BLUE); 
  mapLEDXY(6+shift, 1, RED, GREEN, BLUE);  
  mapLEDXY(6+shift, 6, RED, GREEN, BLUE); 
  mapLEDXY(7+shift, 2, RED, GREEN, BLUE);  
  mapLEDXY(7+shift, 3, RED, GREEN, BLUE); 
  mapLEDXY(7+shift, 4, RED, GREEN, BLUE);  
  mapLEDXY(7+shift, 5, RED, GREEN, BLUE); 
}

void UnicodeChaDisplay(int WhichLetter, int CorX , int CorY, int Red, int Green, int Blue, boolean Bitshift) // 起點在左上方(0,0)
{
  switch (Bitshift)
  {
    case 0:
      for (byte x = 0; x < ((FontHeight - 1) * 2) - CorY * 2; ++x) // 12 Pixel HIGHT(11+11) & (假設CorY>0)修正超出顯示範圍的"列"(- CorY)
      {
        byte data = pgm_read_byte (&Zpix[x + WhichLetter * 32]); // fetch data from program memory
        Serial.print("0x");
        Serial.print(Zpix[x], HEX);
        Serial.print(", ");
        for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
        {
          int EvenNumber = PixelMap[(CorX + CorY * 44) + (7 - y) + 44 * (x / 2)]; // 偶數列
          int OddNumber = PixelMap[(CorX + CorY * 44) + 8 + (7 - y) + 44 * (x / 2)]; // 奇數列

          if (x % 2 == 0) // 偶數(HEX左半)
          {
            Serial.print(EvenNumber); Serial.print(", ");
            if ((data >> y) & 0x01)
            {
              Serial.print("1"); // turn on the LED at location (x,y)
              led.setPixelColor(EvenNumber, Red, Green, Blue);
            }
            else
            {
              Serial.print("0"); // turn off the LED at location (x,y)
              led.setPixelColor(EvenNumber, 0, 0, 0);
            }
          }
          else // 奇數(HEX右半)
          {
            Serial.print(OddNumber); Serial.print(", ");
            if ((data >> y) & 0x01)
            {
              Serial.print("1"); // turn on the LED at location (x,y)
              led.setPixelColor(OddNumber, Red, Green, Blue);
            }
            else
            {
              Serial.print("0"); // turn off the LED at location (x,y)
              led.setPixelColor(OddNumber, 0, 0, 0);
            }
          }
        }
        led.show();
        Serial.println("");
      }
      break;

    case 1:
      // 左半先做
      for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
      {
        for (int x = 0; x < (FontHeight - 1); ++x) // 偶數先列出
        {
          byte data = pgm_read_byte (&Zpix[2 * x + (32 * WhichLetter + 0)]); // fetch data from program memory
          Serial.print("0x");
          Serial.print(Zpix[2 * x + (32 * WhichLetter + 0)], HEX);
          Serial.print(", ");

          if ((data >> y) & 0x01) // 直的一行讀下來，判斷是1還是0
          {
            Serial.print("1, ");
            Serial.println(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y)]);
            led.setPixelColor(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y)] , Red, Green, Blue);
          }
          else
          {
            Serial.print("0, ");
            Serial.println(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y)]);
            led.setPixelColor(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y)] , 0, 0, 0);
          }
        }
        led.show();
        Serial.println("");
      }
      // 再做右半
      for (int y = 7; y >= 0; --y) // 由左至右讀取該byte的每個bit(MSB讀取方式)
      {
        for (int x = 0; x < (FontHeight - 1); ++x) // 偶數先列出
        {
          byte data = pgm_read_byte (&Zpix[(2 * x + 1) + (32 * WhichLetter + 0)]); // fetch data from program memory
          Serial.print("0x");
          Serial.print(Zpix[(2 * x + 1) + (32 * WhichLetter + 0)], HEX);
          Serial.print(", ");

          if ((data >> y) & 0x01) // 直的一行讀下來，判斷是1還是0
          {
            Serial.print("1, ");
            Serial.println(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y) + 8]);
            led.setPixelColor(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y) + 8] , Red, Green, Blue);
          }
          else
          {
            Serial.print("0, ");
            Serial.println(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y) + 8]);
            led.setPixelColor(PixelMap[(CorX + CorY * 44) + (44 * x + 0) + (7 - y) + 8], 0, 0, 0);
          }
        }
        led.show();
        Serial.println("");
      }
      break;
  }
}
