// Reference: https://xantorohara.github.io/led-matrix-editor/
//            https://www.had2know.org/technology/hsv-rgb-conversion-formula-calculator.html
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "WS2812B.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// #define DEBUG
#ifdef DEBUG                                      // Macros are usually in all capital letters.
#define DPRINT(...) Serial.print(__VA_ARGS__)     // DPRINT is a macro, debug print
#define DPRINTLN(...) Serial.println(__VA_ARGS__) // DPRINTLN is a macro, debug print with new line
#else
#define DPRINT(...)   // now defines a blank line
#define DPRINTLN(...) // now defines a blank line
#endif

#define ascii_index 95
#define buffer_size inputLen * 8

#if defined(BOARD_RTL8722DM)
#define SPI_MOSI_PIN 11
#elif defined(BOARD_RTL8722DM_MINI)
#define SPI_MOSI_PIN 9
#elif defined(BOARD_RTL8720DN_BW16)
#define SPI_MOSI_PIN PA12
#endif

#define NUM_OF_LEDS 8 * 32
#define NUM_OF_ROWS 8
#define NUM_OF_COLS 32
#define FontHeight 4
#define Spacer 1

// set No.2 https://xantorohara.github.io/led-matrix-editor
uint8_t ascii_5[99] = {
  0x30, 0x07, 0x05, 0x05, 0x05, 0x07, 0x00, 0x00, 0x00, // 0
  0x31, 0x02, 0x03, 0x02, 0x02, 0x07, 0x00, 0x00, 0x00, // 1
  0x32, 0x07, 0x04, 0x07, 0x01, 0x07, 0x00, 0x00, 0x00, // 2
  0x33, 0x07, 0x04, 0x07, 0x04, 0x07, 0x00, 0x00, 0x00, // 3
  0x34, 0x05, 0x05, 0x07, 0x04, 0x04, 0x00, 0x00, 0x00, // 4
  0x35, 0x07, 0x01, 0x07, 0x04, 0x07, 0x00, 0x00, 0x00, // 5
  0x36, 0x07, 0x01, 0x07, 0x05, 0x07, 0x00, 0x00, 0x00, // 6
  0x37, 0x07, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, // 7
  0x38, 0x07, 0x05, 0x07, 0x05, 0x07, 0x00, 0x00, 0x00, // 8
  0x39, 0x07, 0x05, 0x07, 0x04, 0x07, 0x00, 0x00, 0x00, // 9
  0x3a, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, // :
};

uint8_t new_ascii[855] = {
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //
  0x21, 0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x00, // !
  0x22, 0x1b, 0x1b, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, // "
  0x23, 0x36, 0x36, 0x7f, 0x36, 0x7f, 0x36, 0x36, 0x00, // #
  0x24, 0x0c, 0x3e, 0x03, 0x1e, 0x30, 0x1f, 0x0c, 0x00, // $
  0x25, 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00, // %
  0x26, 0x1c, 0x36, 0x1c, 0x6e, 0x3b, 0x33, 0x6e, 0x00, // &
  0x27, 0x0c, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, // '
  0x28, 0x18, 0x0c, 0x06, 0x06, 0x06, 0x0c, 0x18, 0x00, // (
  0x29, 0x06, 0x0c, 0x18, 0x18, 0x18, 0x0c, 0x06, 0x00, // )
  0x2a, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, // *
  0x2b, 0x00, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x00, 0x00, // +
  0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x06, // ,
  0x2d, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, // -
  0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00, // .
  0x2f, 0x06, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 0x00, // /
  0x30, 0x3e, 0x63, 0x73, 0x7b, 0x6f, 0x67, 0x3e, 0x00, // 0
  0x31, 0x0c, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x00, // 1
  0x32, 0x1e, 0x33, 0x30, 0x1c, 0x06, 0x33, 0x3f, 0x00, // 2
  0x33, 0x1e, 0x33, 0x30, 0x1c, 0x30, 0x33, 0x1e, 0x00, // 3
  0x34, 0x38, 0x3c, 0x36, 0x33, 0x7f, 0x30, 0x78, 0x00, // 4
  0x35, 0x3f, 0x03, 0x1f, 0x30, 0x30, 0x33, 0x1e, 0x00, // 5
  0x36, 0x1c, 0x06, 0x03, 0x1f, 0x33, 0x33, 0x1e, 0x00, // 6
  0x37, 0x3f, 0x33, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x00, // 7
  0x38, 0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x1e, 0x00, // 8
  0x39, 0x1e, 0x33, 0x33, 0x3e, 0x30, 0x18, 0x0e, 0x00, // 9
  0x3a, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x00, // :
  0x3b, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x0c, 0x06, // ;
  0x3c, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x00, // <
  0x3d, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x3f, 0x00, 0x00, // =
  0x3e, 0x06, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00, // >
  0x3f, 0x1e, 0x33, 0x30, 0x18, 0x0c, 0x00, 0x0c, 0x00, // ?
  0x40, 0x3e, 0x63, 0x7b, 0x7b, 0x7b, 0x03, 0x1e, 0x00, // @
  0x41, 0x0c, 0x1e, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x00, // A
  0x42, 0x3f, 0x66, 0x66, 0x3e, 0x66, 0x66, 0x3f, 0x00, // B
  0x43, 0x3c, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3c, 0x00, // C
  0x44, 0x1f, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1f, 0x00, // D
  0x45, 0x7f, 0x46, 0x16, 0x1e, 0x16, 0x46, 0x7f, 0x00, // E
  0x46, 0x7f, 0x46, 0x16, 0x1e, 0x16, 0x06, 0x0f, 0x00, // F
  0x47, 0x3c, 0x66, 0x03, 0x03, 0x73, 0x66, 0x3e, 0x00, // G
  0x48, 0x33, 0x33, 0x33, 0x3f, 0x33, 0x33, 0x33, 0x00, // H
  0x49, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, // I
  0x4a, 0x1e, 0x0c, 0x0c, 0x0c, 0x33, 0x33, 0x1e, 0x00, // J 001e333330303078
  0x4b, 0xe6, 0x66, 0x6c, 0x1e, 0x6c, 0x66, 0xe6, 0x00, // K
  0x4c, 0xf0, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7f, 0x00, // L
  0x4d, 0x63, 0xee, 0x7f, 0x7f, 0x63, 0x63, 0x63, 0x00, // M
  0x4e, 0x63, 0xe6, 0xf6, 0xde, 0x73, 0x63, 0x63, 0x00, // N
  0x4f, 0x38, 0x6c, 0x63, 0x63, 0x63, 0x6c, 0x38, 0x00, // O
  0x50, 0x3f, 0x66, 0x66, 0x3e, 0x06, 0x06, 0xf0, 0x00, // P
  0x51, 0x1e, 0x33, 0x33, 0x33, 0xdc, 0x1e, 0x1c, 0x00, // Q
  0x52, 0x3f, 0x66, 0x66, 0x3e, 0x6c, 0x66, 0xe6, 0x00, // R
  0x53, 0x1e, 0x33, 0x07, 0x70, 0x1c, 0x33, 0x1e, 0x00, // S
  0x54, 0x3f, 0xb4, 0x0c, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, // T
  0x55, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f, 0x00, // U
  0x56, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x00, // V 000c1e3333333333
  0x57, 0x63, 0x63, 0x63, 0x6b, 0x7f, 0x77, 0x63, 0x00, // W
  0x58, 0x63, 0x63, 0x6c, 0x38, 0x38, 0x6c, 0x63, 0x00, // X
  0x59, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x0c, 0x1e, 0x00, // Y
  0x5a, 0x7f, 0x63, 0x8c, 0x18, 0x32, 0x66, 0x7f, 0x00, // Z 007f664c1831637f
  0x5b, 0x1e, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1e, 0x00, // [
  0x5c, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x40, 0x00, // \ /
  0x5d, 0x1e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1e, 0x00, // ]
  0x5e, 0x08, 0x1c, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, // ^
  0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, // _
  0x60, 0x03, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, // `
  0x61, 0x00, 0x00, 0x1e, 0x30, 0x3e, 0x33, 0x6e, 0x00, // a
  0x62, 0x07, 0x06, 0x06, 0x3e, 0x66, 0x66, 0x3b, 0x00, // b
  0x63, 0x00, 0x00, 0x1e, 0x33, 0x03, 0x33, 0x1e, 0x00, // c 001e3303331e0000
  0x64, 0x1c, 0x0c, 0x0c, 0x3e, 0x33, 0x33, 0x6e, 0x00, // d 006e33333e303038
  0x65, 0x00, 0x00, 0x1e, 0x33, 0x3f, 0x03, 0x1e, 0x00, // e
  0x66, 0x38, 0x6c, 0x06, 0xf0, 0x06, 0x06, 0xf0, 0x00, // f 000f06060f06361c
  0x67, 0x00, 0x00, 0x6e, 0x33, 0x33, 0x3e, 0x0c, 0xf8, // g 1f303e33336e0000
  0x68, 0x07, 0x06, 0x6c, 0x6e, 0x66, 0x66, 0xe6, 0x00, // h 006766666e360607
  0x69, 0x0c, 0x00, 0x0e, 0x0c, 0x0c, 0x0c, 0x1e, 0x00, // i
  0x6a, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x33, 0x33, 0x1e, // j 1e33333030300030
  0x6b, 0x07, 0x06, 0x66, 0x6c, 0x1e, 0x6c, 0xe6, 0x00, // k 0067361e36660607
  0x6c, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x0f, 0x00, // l
  0x6d, 0x00, 0x00, 0x33, 0x7f, 0x7f, 0x63, 0x63, 0x00, // m
  0x6e, 0x00, 0x00, 0xf8, 0x33, 0x33, 0x33, 0x33, 0x00, // n 00333333331f0000
  0x6f, 0x00, 0x00, 0x1e, 0x33, 0x33, 0x33, 0x1e, 0x00, // o 001e3333331e0000
  0x70, 0x00, 0x00, 0x3b, 0x66, 0x66, 0x3e, 0x06, 0xf0, // p
  0x71, 0x00, 0x00, 0x6e, 0x33, 0x33, 0x3e, 0x0c, 0x1e, // q
  0x72, 0x00, 0x00, 0xdc, 0x6e, 0x66, 0x06, 0xf0, 0x00, // r
  0x73, 0x00, 0x00, 0x3e, 0x03, 0x1e, 0x0c, 0xf8, 0x00, // s
  0x74, 0x10, 0x0c, 0x3e, 0x0c, 0x0c, 0x34, 0x18, 0x00, // t
  0x75, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6e, 0x00, // u
  0x76, 0x00, 0x00, 0x33, 0x33, 0x33, 0x1e, 0x0c, 0x00, // v
  0x77, 0x00, 0x00, 0x63, 0x63, 0x7f, 0x7f, 0x6c, 0x00, // w
  0x78, 0x00, 0x00, 0x63, 0x6c, 0x38, 0x6c, 0x63, 0x00, // x
  0x79, 0x00, 0x00, 0x33, 0x33, 0x33, 0x3e, 0x0c, 0xf8, // y 1f303e3333330000
  0x7a, 0x00, 0x00, 0x3f, 0x19, 0x0c, 0x26, 0x3f, 0x00, // z
  0x7b, 0x38, 0x0c, 0x0c, 0x07, 0x0c, 0x0c, 0x38, 0x00, // {
  0x7c, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // |
  0x7d, 0x07, 0x0c, 0x0c, 0x38, 0x0c, 0x0c, 0x07, 0x00, // }
  0x7e, 0x6e, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ~
};

const uint64_t IMAGES_Test[] = {
  // Hello, Ameba!
  0x003333333f333333,
  0x001e033f331e0000,
  0x001e0c0c0c0c0c0e,
  0x001e0c0c0c0c0c0e,
  0x001e3333331e0000,
  0x060c0c0000000000,
  0x0033333f33331e0c,
  0x00636b7f7f330000,
  0x001e033f331e0000,
  0x003b66663e060607,
  0x006e333e301e0000,
  0x00180018183c3c18,
};

const uint64_t IMAGES_OTHER[] = {
  0x18187e03037e1818,
  0x003f67060f26361c,
  0x0c0c3f0c3f1e3333,
  0x1e331c36361cc67c,
  0x3c4299858599423c,
  0x0000cc663366cc00,
  0x3c42a59da59d423c,
  0x007e0018187e1818,
  0x000000001c36361c,
  0x000000001c080c08,
  0x0000003c0c18301c,
  0x0000001c3018301c,
  0x03063e6666660000,
  0x00d8d8d8dedbdbfe,
  0x00003366cc663300,
  0x03f3f6ecbd3363c3,
  0xf03366cc7b3363c3,
  0x80e6acdbb463c403,
  0x001e3303060c000c,
  0x00637f63361c0007,
  0x00637f63361c0070,
  0x00637f633e00361c,
  0x00637f633e003b6e,
  0x0063637f63361c63,
  0x00333f331e000c0c,
  0x007333337f33367c,
  0x1e30181e3303331e,
  0x003f061e063f0007,
  0x003f061e063f0038,
  0x003f061e063f120c,
  0x003f061e063f0036,
  0x001e0c0c0c1e0007,
  0x001e0c0c0c1e0038,
  0x001e0c0c1e00120c,
  0x001e0c0c0c1e0033,
  0x003f66666f6f663f,
  0x00333b3f3733003f,
  0x00183c663c18000e,
  0x00183c663c180070,
  0x00183c663c18663c,
  0x003e63633e003b6e,
  0x00183c66663c18c3,
  0x0000361c081c3600,
  0x001d366f7b73365c,
  0x003c66666666000e,
  0x003c666666660070,
  0x003c66666600663c,
  0x001e333333330033,
  0x0018183c66660070,
  0x0f063e66663e060f,
  0x03031f331f331e00,
  0x007e333e301e0007,
  0x007e333e301e0038,
  0x00fc667c603cc37e,
  0x007e333e301e3b6e,
  0x007e333e301e0033,
  0x007e333e301e0c0c,
  0x00fe33fe30fe0000,
  0x1c301e03031e0000,
  0x001e033f331e0007,
  0x001e033f331e0038,
  0x003c067e663cc37e,
  0x001e033f331e0033,
  0x001e0c0c0c0e0007,
  0x001e0c0c0c0e001c,
  0x003c1818181c633e,
  0x001e0c0c0c0e0033,
  0x001e333e301b0e1b,
  0x003333331f001f00,
  0x001e33331e000700,
  0x001e33331e003800,
  0x001e33331e00331e,
  0x001e33331e003b6e,
  0x001e33331e003300,
  0x063c6e7e763c6000,
  0x007e333333000700,
  0x007e333333003800,
  0x007e33333300331e,
  0x007e333333003300,
  0x1f303e3333003800,
  0x00063e663e060000,
  0x1f303e3333003300
};

const int IMAGES_OTHER_LEN = sizeof(IMAGES_OTHER) / 8;
const int IMAGES_Test_LEN = sizeof(IMAGES_Test) / 8;

char ssid[] = "QQZ";
char pass[] = "11223344";

WS2812B led(SPI_MOSI_PIN, NUM_OF_LEDS);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup()
{
  char inputString[] = "Wi-Fi";
  //  char inputHour[] = "12";
  //  char inputMin[] = "34";
  //  char inputSec[] = "56";

  int inputLen = strlen(inputString);
  char buffer[buffer_size];

  Serial.begin(115200);
  Serial.println("WS2812B test");

  led.begin();
  led.clear();
  // Display static input string in Matrix
  convString2Byte(inputString, inputLen, buffer, 7);
  setBytesPosition(inputString, inputLen, buffer, 0, 19, 124, 140);
  led.show();
  
  WiFi.begin(ssid, pass);
  led.clear();
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  timeClient.begin();


  //  // Display time in HH:MM:SS
  //  convString2Byte(inputHour, strlen(inputHour), buffer, 5);
  //  setBytesPosition(inputHour, strlen(inputHour), buffer, 3, 19, 124, 140);
  //
  //  convString2Byte(inputMin, strlen(inputMin), buffer, 5);
  //  setBytesPosition(inputMin, strlen(inputMin), buffer, 13, 19, 124, 140);
  //
  //  convString2Byte(inputSec, strlen(inputSec), buffer, 5);
  //  setBytesPosition(inputSec, strlen(inputSec), buffer, 23, 19, 124, 140);
  //
  //  led.show();
  //  delay(1000);
  //
  //  while (1)
  //  {
  //    convString2Byte(inputCol, strlen(inputCol), buffer, 5);
  //    setBytesPosition(inputCol, strlen(inputCol), buffer, 11, 19, 124, 140);
  //
  //    convString2Byte(inputCol, strlen(inputCol), buffer, 5);
  //    setBytesPosition(inputCol, strlen(inputCol), buffer, 21, 19, 124, 140);
  //
  //    led.show();
  //    delay(1000);
  //
  //    convString2Byte(inputCol, strlen(inputCol), buffer, 5);
  //    setBytesPosition(inputCol, strlen(inputCol), buffer, 11, 0, 0, 0);
  //
  //    convString2Byte(inputCol, strlen(inputCol), buffer, 5);
  //    setBytesPosition(inputCol, strlen(inputCol), buffer, 21, 0, 0, 0);
  //
  //    led.show();
  //    delay(1000);
  //  }

 

  // Display scrolling input string in Matrix
  // for (int i = 32; i > 0 - 32 - inputLen; i = i - 1) // shift from R to L
  // {
  //     convString2Byte(inputString, inputLen, buffer, 7);
  //     setBytesPosition(inputString, inputLen, buffer, i, 19, 124, 140);
  //     led.show();
  //     delay(10);
  //     led.clear();
  //     delayMicroseconds(1);
  // }

  // Set a specific LED with a certain color
  //  led.setPixelColor(0, 50, 0, 0);
  //  led.setPixelColor(1, 0, 50, 0);
  //  led.setPixelColor(2, 0, 0, 50);
  //  led.setPixelColor(8, 0, 0, 50);
  //  led.show();
  //  delay(1000);

  // Fill the entire LED strip with the same color
  //    led.fill(60, 0, 25, 0, 8);
  //    led.show();

  //  symbol1();
  //  led.show();
  //  delay(1000);
  //  symbol2(8, 0, 153, 76);
  //  led.show();
  //  delay(1000);
  //  symbol3(16, 255, 128, 0);
  //  led.show();
  //  delay(1000);
  //  symbol4(24, 255, 204, 204);
  //  led.show();
  //  delay(1000);
  //  led.fill(0, 0, 0, 0, 255);
  //  led.show();
  //
  //  for (int i = 0; i < 32; i = i + 2) { // heart shape shifting
  //    symbol3(i, 255, 153, 255);
  //    led.show();
  //    delay(100);
  //    led.fill(0, 0, 0, 0, 255);
  //    led.show();
  //  }
}

void loop()
{
  char inputString[] = "Ameba!";

  int inputLen = strlen(inputString);
  char buffer[buffer_size];

  timeClient.update();
  // +1H 3600
  // +8H 28800
  timeClient.setTimeOffset(28800);
  Serial.println();
  Serial.println(timeClient.getFormattedTime());

  String sHour(timeClient.getHours());
  if (timeClient.getHours() < 10) {
    sHour = "0" + String(timeClient.getHours());
  }
  String sMins(timeClient.getMinutes());
  if (timeClient.getMinutes() < 10) {
    sMins = "0" + String(timeClient.getMinutes());
  }
  String sSecs(timeClient.getSeconds());
  if (timeClient.getSeconds() < 10) {
    sSecs = "0" + String(timeClient.getSeconds());
  }
  char cHour[sHour.length()];
  char cMins[sMins.length()];
  char cSecs[sSecs.length()];
  char inputCol[2] = ":";

  for (int i = 0; i < sizeof(cHour); i++) {
    cHour[i] = sHour[i];
    DPRINT(cHour[i]);
  }

  Serial.println();
  for (int i = 0; i < sizeof(cMins); i++) {
    cMins[i] = sMins[i];
    DPRINT(cMins[i]);
  }
  Serial.println();
  for (int i = 0; i < sizeof(cSecs); i++) {
    cSecs[i] = sSecs[i];
    DPRINT(cSecs[i]);
  }
  Serial.println();

  // // Display time in HH:MM:SS
  convString2Byte(inputCol, 1, buffer, 5);
  setBytesPosition(inputCol, 1, buffer, 11, 19, 124, 140);
  convString2Byte(inputCol, 1, buffer, 5);
  setBytesPosition(inputCol, 1, buffer, 21, 19, 124, 140);
  // led.show();

  convString2Byte(cHour, 2, buffer, 5);
  setBytesPosition(cHour, 2, buffer, 3, 19, 124, 140);
  // //Serial.print("String lens: ");
  // //Serial.println(strlen(cMins));
  convString2Byte(cMins, 2, buffer, 5);
  setBytesPosition(cMins, 2, buffer, 13, 19, 124, 140);

  convString2Byte(cSecs, 2, buffer, 5);
  setBytesPosition(cSecs, 2, buffer, 23, 19, 124, 140);
  
  led.show();
  delay(1000);
  led.clear();

  // Serial.println("===============================");
  // Serial.println(i + 1);
  // // put your main code here, to run repeatedly:
  // for (int s = 32; s > 0; s--) {
  //   displayChar(IMAGES_Test[i], s, 39, 174, 232);
  //   led.show();
  //   delay(100);
  //   led.fill(0, 0, 0, 0, 255);
  //   led.show();
  // }
  //  displayChar(IMAGES[i], 0, 39, 174, 232);
  //  led.show();
  //  displayChar(IMAGES_LETTER[i], 8, 255, 153, 255);
  //  led.show();
  //  displayChar(IMAGES_SIGNS[i], 16, 255, 128, 0);
  //  led.show();
  //  displayChar(IMAGES_OTHER[i], 24, 255, 204, 204);
  //  led.show();
  //  delay(1000);
  //  led.fill(0, 0, 0, 0, 255);
  // led.show();
  //  if (++i >= IMAGES_OTHER_LEN) {
  //    i = 0;
  //    //    while (1);
  //  }
}

// --------------------------------------------
// convert input string to ASCII
// --------------------------------------------
void convString2Byte(char *str_in, int strlen, char *array_out, int font)
{
  for (int i = 0; i < strlen; i++)
  {
    for (int k = 0; k < ascii_index; k++)
    {
      if (font == 5)
      {
        if (str_in[i] == ascii_5[k * 9])
        {
          for (int j = 0; j < 8; j++)
          {
            array_out[(i * 8) + j] = ascii_5[((k * 9) + 1) + j];
          }
        }
      }
      else if (font == 7)
      {
        if (str_in[i] == new_ascii[k * 9])
        {
          for (int j = 0; j < 8; j++)
          {
            array_out[(i * 8) + j] = new_ascii[((k * 9) + 1) + j];
          }
        }
      }
      else
      {
        printf("[ERROR] Current font is not supported");
        break;
      }
    }
  }
}

// --------------------------------------------
// get current row message (1 byte), reading
// order from left to right
// --------------------------------------------
void setBytesPosition(char *inputString, int inputLen, char *buffer, int shift, byte RED, byte GREEN, byte BLUE)
{
  unsigned int maxIndex = 0;
  unsigned int line_width[8];
  unsigned int char_width = line_width[maxIndex];

  for (int i = 0; i < inputLen; i++)
  {
    DPRINTLN("======================");
    printf("Letter %c\r\n", inputString[i]);
    DPRINTLN("------------");

    for (int x = 0; x < 8; x++) // x axis | vertical
    {
      // get ASCII value of current row
      char current_byte = buffer[(i * 8) + x];
      printf("0x%02x, ", current_byte);
      // mark non-zero bits in each row
      for (int y = 0; y < 8; y++) // y axis | horizontal
      {
        if (bitRead(current_byte, y) == 1) // FIND THE NON-ZERO BIT
        {
          // mark position on hardware
          DPRINT("\rY,X : (");
          DPRINT(y);
          DPRINT(",");
          DPRINT(x);
          DPRINT(") | ");
          // store width of NON-ZERO bit
          line_width[i] = y;
          DPRINT("line_width[");
          DPRINT(i);
          DPRINT("] = ");
          DPRINTLN(line_width[i]);

          mapLEDXY(y + shift, x, RED, GREEN, BLUE); // mark position to turn on
        }
      }
    }
    // retrun charecter width
    printf("\r\n#%d's Char Width = %d\r\n", i, (line_width[i] + 1));

    if (inputString[i] != ':') // for time display
    {
      shift += (line_width[i] + 1 + Spacer);
    }
    else
    {
      shift += (line_width[i] + 1);
    }
    DPRINT("Next Char Starting: ");
    DPRINTLN(shift);
    printf("==================\r\n");
  }
}

// --------------------------------------------
// display information in IMAGES_Test as a
// scrolling long sentence
// --------------------------------------------
void scrollingImage(uint64_t image, int shift_pattern, byte RED, byte GREEN, byte BLUE)
{
  unsigned int char_shift = 0;

  for (unsigned int i = 0; i < sizeof(image) / 8; i++)
  {
    if (i > 0)
    { // SHIFT FROM THE SECOND BIT
      // char_shift = char_shift + getCharShift(IMAGES_Test[i-1]) + 1;
      char_shift = char_shift + getCharShift(image) + 1;
    }
    Serial.println(char_shift);
    displayChar(IMAGES_Test[i], char_shift + shift_pattern, RED, GREEN, BLUE);
  }
}

// --------------------------------------------
// Get character shift bits
// --------------------------------------------
unsigned int getCharShift(uint64_t image)
{
  unsigned int maxIndex = 0;
  unsigned int line_width[8];
  unsigned int char_width = line_width[maxIndex];

  for (int i = 0; i < 8; i++)
  { // x axis | vertical
    byte row = (image >> i * 8) & 0xFF; // GET ROW VALUE
    for (int y = 0; y < 8; y++)
    { // y axis | horizontal
      if (bitRead(row, y) == 1)
      { // FIND THE NON-ZERO BIT
        // Serial.print("Y,X : ("); Serial.print(y); Serial.print(","); Serial.print(i); Serial.println(")");
        line_width[i] = y;
      }
    }
    // Serial.println("------------");
  }

  for (int i = 0; i < 8; i++)
  {
    maxIndex = 0;

    if (line_width[i] > 8)
    {
      line_width[i] = 0;
    }
    if (line_width[i] > char_width)
    {
      char_width = line_width[i];
      maxIndex = i;
    }
    if (char_width > 8)
    {
      char_width = 0;
    }
    // Serial.print("Last on bit: ");Serial.print(line_width[i]); Serial.println(" ");
  }
  //   Serial.print("Char width: "); Serial.println(char_width + 1);
  //   Serial.println("==============");
  return (char_width + 1);
}

// --------------------------------------------
// Display 8x8 character
// --------------------------------------------
void displayChar(uint64_t image, int shift, byte RED, byte GREEN, byte BLUE)
{
  for (int i = 0; i < 8; i++) // x axis | vertical
  {
    byte row = (image >> i * 8) & 0xFF; // GET ROW VALUE
    Serial.print("0x ");
    printf("%02x\r", row);
    for (int y = 0; y < 8; y++) // y axis | horizontal
    {
      if (bitRead(row, y) == 1) // FIND THE NON-ZERO BIT
      {
        // Serial.print("Y,X : ("); Serial.print(y); Serial.print(","); Serial.print(i); Serial.println(")");
        mapLEDXY(y + shift, i, RED, GREEN, BLUE); // TURN ON NON-ZERO BIT
      }
    }
    Serial.println("------------");
  }
  led.show();
  Serial.println("==============");
}

// --------------------------------------------
// map from (x, y) to LED location in strip
// zig zag
// top left LED starting bit
// --------------------------------------------
void mapLEDXY(int y, int x, byte RED, byte GREEN, byte BLUE)
{
  int RGBlocation = 0;

  if (y % 2 == 0)
  { // even column
    RGBlocation = x + y * NUM_OF_ROWS;
  }
  else
  { // odd column
    RGBlocation = 7 - x + y * NUM_OF_ROWS;
  }

  led.setPixelColor(RGBlocation, RED, GREEN, BLUE);
}

// --------------------------------------- test functions below ---------------------------------------


// define patterns in hard code
void symbol1()
{
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

void symbol2(int shift, byte RED, byte GREEN, byte BLUE)
{
  // .:iL
  mapLEDXY(0 + shift, 6, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 6, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 6, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 6, RED, GREEN, BLUE);
}

void symbol3(int shift, byte RED, byte GREEN, byte BLUE)
{
  // (heart)
  mapLEDXY(1 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(1 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(1 + shift, 4, RED, GREEN, BLUE);

  mapLEDXY(2 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 5, RED, GREEN, BLUE);

  mapLEDXY(3 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 6, RED, GREEN, BLUE);

  mapLEDXY(4 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 6, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 7, RED, GREEN, BLUE);

  mapLEDXY(5 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 6, RED, GREEN, BLUE);

  mapLEDXY(6 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 5, RED, GREEN, BLUE);

  mapLEDXY(7 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 4, RED, GREEN, BLUE);
}

void symbol4(int shift, byte RED, byte GREEN, byte BLUE)
{
  // (R)
  mapLEDXY(0 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(0 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(0 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(0 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(1 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(1 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 0, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(2 + shift, 7, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 0, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(3 + shift, 7, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 0, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(4 + shift, 7, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 0, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 5, RED, GREEN, BLUE);
  mapLEDXY(5 + shift, 7, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 1, RED, GREEN, BLUE);
  mapLEDXY(6 + shift, 6, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 2, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 3, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 4, RED, GREEN, BLUE);
  mapLEDXY(7 + shift, 5, RED, GREEN, BLUE);
}
