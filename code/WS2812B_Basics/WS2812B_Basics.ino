//Reference: https://xantorohara.github.io/led-matrix-editor/
//           https://www.had2know.org/technology/hsv-rgb-conversion-formula-calculator.html

#include "WS2812B.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define ascii_index 95
#define buffer_size inputLen*8

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
// set No.2 https://xantorohara.github.io/led-matrix-editor
uint8_t new_ascii[855] = {
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //
  0x21, 0x06, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00,   //! 00180018183c3c18
  0x22, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //" 00000000001b1b1b
  0x23, 0x36, 0x36, 0x7f, 0x36, 0x7f, 0x36, 0x36, 0x00,   //#
  0x24, 0x0c, 0x3e, 0x03, 0x1e, 0x30, 0x1f, 0x0c, 0x00,   //$
  0x25, 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,   //%
  0x26, 0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,   //& 006e333b6e1c361c
  0x27, 0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,   //' 0000000000180c0c
  0x28, 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,   //( 00180c0606060c18
  0x29, 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,   //) 00060c1818180c06
  0x2a, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,   //* 0000663cff3c6600
  0x2b, 0x00, 0x0c, 0x0c, 0x3f, 0x0c, 0x0c, 0x00, 0x00,   //+
  0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x06,   //,
  0x2d, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00,   //-
  0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x00,   //.
  0x2f, 0x06, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00,   //>
  0x40, 0x13, 0x33, 0x33, 0x3e, 0x30, 0x18, 0x0e, 0x00,   //0
  0x31, 0x0c, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x00,   //1
  0x32, 0x1e, 0x33, 0x30, 0x1c, 0x06, 0x33, 0x3f, 0x00,   //2
  0x33, 0x1e, 0x33, 0x30, 0x1c, 0x30, 0x33, 0x1e, 0x00,   //3
  0x34, 0x38, 0x3c, 0x36, 0x33, 0x7f, 0x30, 0x78, 0x00,   //4
  0x35, 0x3f, 0x03, 0x1f, 0x30, 0x30, 0x33, 0x1e, 0x00,   //5
  0x36, 0x1c, 0x06, 0x03, 0x1f, 0x33, 0x33, 0x1e, 0x00,   //6
  0x37, 0x3f, 0x33, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x00,   //7
  0x38, 0x1e, 0x33, 0x33, 0x1e, 0x33, 0x33, 0x13, 0x00,   //8
  0x39, 0x1e, 0x33, 0x33, 0x3e, 0x30, 0x18, 0x0e, 0x00,   //9
  0x3a, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00,   //: 000c0c00000c0c00
  0x3b, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,   //; 060c0c00000c0c00
  0x3c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,   //< 00180c0603060c18
  0x3d, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x3f, 0x00, 0x00,   //=
  0x3e, 0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,   //> 00060c1830180c06
  0x3f, 0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00,   //? 000c000c1830331e
  0x40, 0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,   //@ 001e037b7b7b633e
  0x41, 0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,   //A 0033333f33331e0c
  0x42, 0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,   //B 003f66663e66663f
  0x43, 0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,   //C
  0x44, 0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00,   //D
  0x45, 0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00,   //E
  0x46, 0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,   //F
  0x47, 0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00,   //G
  0x48, 0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00,   //H
  0x49, 0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,   //I
  0x4a, 0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00,   //J
  0x4b, 0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00,   //K
  0x4c, 0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,   //L
  0x4d, 0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,   //M
  0x4e, 0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00,   //N
  0x4f, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,   //O
  0x50, 0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,   //P
  0x51, 0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00,   //Q
  0x52, 0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,   //R
  0x53, 0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00,   //S
  0x54, 0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,   //T
  0x55, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00,   //U
  0x56, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,   //V
  0x57, 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00,   //W
  0x58, 0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00,   //X
  0x59, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00,   //Y
  0x5a, 0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00,   //Z
  0x5b, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,   //[  001e06060606061e
  0x5c, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00,   //\< 
  0x5d, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,   //]  001e18181818181e
  0x5e, 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,   //^  0000000063361c08
  0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,   //_  0f0f000000000000
  0x60, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,   //`  0000000000060703
  0x61, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00,   //a 
  0x62, 0xe0, 0x60, 0x60, 0x7c, 0x66, 0x66, 0xdc, 0x00,   //b
  0x63, 0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78, 0x00,   //c
  0x64, 0x1c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00,   //d
  0x65, 0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78, 0x00,   //e
  0x66, 0x38, 0x6c, 0x60, 0xf0, 0x60, 0x60, 0xf0, 0x00,   //f
  0x67, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,   //g
  0x68, 0xe0, 0x60, 0x6c, 0x76, 0x66, 0x66, 0xe6, 0x00,   //h
  0x69, 0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00,   //i
  0x6a, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78,   //j
  0x6b, 0xe0, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0xe6, 0x00,   //k
  0x6c, 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,   //l
  0x6d, 0x00, 0x00, 0xcc, 0xfe, 0xfe, 0xd6, 0xc6, 0x00,   //m
  0x6e, 0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0x00,   //n
  0x6f, 0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78, 0x00,   //o
  0x70, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x7c, 0x60, 0xf0,   //p
  0x71, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0x7c, 0x0c, 0x1e,   //q
  0x72, 0x00, 0x00, 0xdc, 0x76, 0x66, 0x60, 0xf0, 0x00,   //r
  0x73, 0x00, 0x00, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x00,   //s
  0x74, 0x10, 0x30, 0x7c, 0x30, 0x30, 0x34, 0x18, 0x00,   //t
  0x75, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00,   //u
  0x76, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,   //v
  0x77, 0x00, 0x00, 0xc6, 0xd6, 0xfe, 0xfe, 0x6c, 0x00,   //w
  0x78, 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00,   //x
  0x79, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xf8,   //y
  0x7a, 0x00, 0x00, 0xfc, 0x98, 0x30, 0x64, 0xfc, 0x00,   //z
  0x7b, 0x1c, 0x30, 0x30, 0xe0, 0x30, 0x30, 0x1c, 0x00,   //{
  0x7c, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,   //|
  0x7d, 0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00,   //}
  0x7e, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //~  0000000000003b6e
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

const uint64_t IMAGES[] = {   // Numbers from 0~9
  // 0x00 3f 0c 0c 0c 0c 0e 0c,  //1
  // 0x 00 3f 33 06 1c 30 33 1e,  //2
  // 0x 00 1e 33 30 1c 30 33 1e,  //3
  // 0x 00 78 30 7f 33 36 3c 38,
  // 0x 00 1e 33 30 30 1f 03 3f,
  // 0x 00 1e 33 33 1f 03 06 1c,
  // 0x 00 0c 0c 0c 18 30 33 3f,
  // 0x 00 1e 33 33 1e 33 33 1e,
  // 0x 00 0e 18 30 3e 33 33 1e,
  // 0x 00 3e676f7b73633e
};

const uint64_t IMAGES_LETTER[] = {
  // Letters
  0x0033333f33331e0c,//A
  0x003f66663e66663f,//B
  0x003c66030303663c,//C
  0x001f36666666361f,//D
  0x007f46161e16467f,//E
  0x000f06161e16467f,
  0x007c66730303663c,
  0x003333333f333333,
  0x001e0c0c0c0c0c1e,
  0x001e333330303078,
  0x006766361e366667,
  0x007f66460606060f,
  0x0063636b7f7f7763,
  0x006363737b6f6763,
  0x001c36636363361c,
  0x000f06063e66663f,
  0x00381e3b3333331e,
  0x006766363e66663f,
  0x001e33380e07331e,
  0x001e0c0c0c0c2d3f,
  0x003f333333333333,
  0x000c1e3333333333,
  0x0063777f6b636363,
  0x0063361c1c366363,
  0x001e0c0c1e333333,
  0x007f664c1831637f,
  0x0000000000000000,
  0x006e333e301e0000,
  0x003b66663e060607,
  0x001e3303331e0000,
  0x006e33333e303038,
  0x001e033f331e0000,
  0x000f06060f06361c,
  0x1f303e33336e0000,
  0x006766666e360607,
  0x001e0c0c0c0e000c,
  0x1e33333030300030,
  0x0067361e36660607,
  0x001e0c0c0c0c0c0e,
  0x00636b7f7f330000,
  0x00333333331f0000,
  0x001e3333331e0000,
  0x0f063e66663b0000,
  0x78303e33336e0000,
  0x000f06666e3b0000,
  0x001f301e033e0000,
  0x00182c0c0c3e0c08,
  0x006e333333330000,
  0x000c1e3333330000,
  0x00367f7f6b630000,
  0x0063361c36630000,
  0x1f303e3333330000,
  0x003f260c193f0000
};

const uint64_t IMAGES_SIGNS[] = {
  0x00000c0c3f0c0c00,
  0x000000003f000000,
  0x0000663cff3c6600,
  0x000103060c183060,
  0x0063660c18336300,
  0x00003f00003f0000,
  0x0000000000003b6e,
  0x0000000063361c08,
  0x00180c0603060c18,
  0x00060c1830180c06,
  0x00180c0606060c18,
  0x00060c1818180c06,
  0x001e06060606061e,
  0x001e18181818181e,
  0x00380c0c070c0c38,
  0x00070c0c380c0c07,
  0x000c0c0000000000,
  0x000c0c00000c0c00,
  0x060c0c00000c0c00,
  0x060c0c0000000000,
  0x00180018183c3c18,
  0x000c000c1830331e,
  0x001e037b7b7b633e,
  0x006e333b6e1c361c,
  0x000c1f301e033e0c,
  0x0036367f367f3636,
  0x00406030180c0603,
  0x0000000000180c0c,
  0x0000000000030606
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

const int IMAGES_LEN = sizeof(IMAGES) / 8;
const int IMAGES_LETTER_LEN = sizeof(IMAGES_LETTER) / 8;
const int IMAGES_SIGNS_LEN = sizeof(IMAGES_SIGNS) / 8;
const int IMAGES_OTHER_LEN = sizeof(IMAGES_OTHER) / 8;
const int IMAGES_Test_LEN = sizeof(IMAGES_Test) / 8;

//const uint16_t PixelMap1[] =
//{
//  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
//  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
//  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
//  96, 97, 98, 99, 100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
//  128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
//  160,  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,  172,  173,  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  186,  187,  188,  189,  190,  191,
//  192,  193,  194,  195,  196,  197,  198,  199,  200,  201,  202,  203,  204,  205,  206,  207,  208,  209,  210,  211,  212,  213,  214,  215,  216,  217,  218,  219,  220,  221,  222,  223,
//  224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,  247,  248,  249,  250,  251,  252,  253,  254,  255,
//};

WS2812B led(SPI_MOSI_PIN, NUM_OF_LEDS);

const uint16_t Zpix[] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

// To add 
void setup() {
  Serial.begin(115200);
  Serial.println("WS2812B test");
  led.begin();
  
  // displayChar(IMAGES[0], 0, 39, 174, 232);
  
  // Serial.println("================================");
  
  // put your setup code here, to run once:
  char inputString[] = "192.168";
  int inputLen = strlen(inputString);
  char buffer[buffer_size];

  int text1_len = getImageLength(*IMAGES_Test);
  for (uint8_t j = 0; j < IMAGES_Test_LEN; j++) {
    for (int i = 32; i > 0 - 32 - text1_len; i = i - 1) {
    //  scrollingImage(i, 39, 174, 232);
     scrollingImage(IMAGES_Test[j], i, 39, 174, 232);
     led.show();
     delay(15); // delay between each character
     led.clear();
     delayMicroseconds(1);
    }
  }

  convString2Byte(inputString, inputLen, buffer);
  setBytesPosition(inputString, inputLen, buffer, 0, 39, 174, 232);
  led.show();
  delay(1000);



  // for (int i = 0; i < inputLen; i++)
  // {
  //   Serial.print("letter "); Serial.print(inputString[i]); Serial.println("");
  //   for (int j = 0; j < 8; j++) {
  //     // get current row message (1 byte), reading order from left to right
  //     char current_byte = buffer[(i * 8) + j];
  //     Serial.print("0x "); printf("%02x", current_byte); Serial.println("");
  //     //      printf("0x%.2x ", buffer[(i * 8) + j]);
  //     for (int y = 0; y < 8; y++) {
  //        if (bitRead(current_byte, y) == 1) {
  //           // mark position on hardware
  //           // mapLEDXY(y + shift, i, RED, GREEN, BLUE)
  //        }
  //     }
  //     if (j % 8 == 7) {
  //       Serial.println("");
  //     }
  //   }
  // }
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

void loop() {
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

void convString2Byte(char *str_in, int strlen, char *array_out)
{
  for (int i = 0; i < strlen; i++)
  {
    for (int k = 0; k < ascii_index; k++)
    {
      if (str_in[i] == new_ascii[k * 9])
      {
        for (int j = 0; j < 8; j++)
        {
          array_out[(i * 8) + j] = new_ascii[((k * 9) + 1) + j];
        }
      }
    }
  }
}

// --------------------------------------------
// get current row message (1 byte), reading 
// order from left to right
// --------------------------------------------
void setBytesPosition(char *inputString, int inputLen, char *buffer, int shift, byte RED, byte  GREEN, byte BLUE) 
{
  unsigned int maxIndex = 0;
  unsigned int line_width[8];
  unsigned int char_width = line_width[maxIndex];
  
  for (int i = 0; i < inputLen; i++)
  {
    Serial.print("letter "); Serial.print(inputString[i]); Serial.println("");
    for (int x = 0; x < 8; x++)                         // x axis | vertical
    {
      char current_byte = buffer[(i * 8) + x];          // GET ROW VALUE
      Serial.print("0x "); printf("%02x", current_byte); Serial.println("");
      //      printf("0x%.2x ", buffer[(i * 8) + x]);
      for (int y = 0; y < 8; y++)                       // y axis | horizontal
      {
         if (bitRead(current_byte, y) == 1)             // FIND THE NON-ZERO BIT
         {
            // mark position on hardware
            Serial.print("Y,X : ("); Serial.print(y); Serial.print(","); Serial.print(x); Serial.println(")");
            line_width[i] = y;
            mapLEDXY(y + shift, x, RED, GREEN, BLUE);
         }
         // Serial.println("------------");
      }
      if (x % 8 == 7) 
      {
        Serial.println("");
      }
      for (int i = 0; i < 8; i++) {
        maxIndex = 0;

        if (line_width[i] > 8){
            line_width[i] = 0;    
        }
        if (line_width[i] > char_width) {
            char_width = line_width[i];
            maxIndex = i;
        }
        if (char_width > 8){
            char_width = 0;    
        }
        // Serial.print("Last on bit: ");Serial.print(line_width[i]); Serial.println(" ");
      }
    }
    shift += (char_width + Spacer);
    Serial.print("current shifting bits: "); Serial.print(shift); Serial.println("");
  }
}

// --------------------------------------------
// display information in IMAGES_Test as a 
// scrolling long sentence
// --------------------------------------------
void scrollingImage(uint64_t image, int shift_pattern, byte RED, byte  GREEN, byte BLUE) { 
    unsigned int char_shift = 0;
    
    for(unsigned int i = 0; i < sizeof(image)/8; i++){
        if (i > 0){ // SHIFT FROM THE SECOND BIT
            // char_shift = char_shift + getCharShift(IMAGES_Test[i-1]) + 1;
            char_shift = char_shift + getCharShift(image) + 1;
        }
        Serial.println(char_shift);
        displayChar(IMAGES_Test[i], char_shift + shift_pattern, RED, GREEN, BLUE);
    }
}

// --------------------------------------------
// Return length of image
// --------------------------------------------
unsigned int getImageLength(uint64_t image) { 
    unsigned int char_shift = 0;

    for(unsigned int i = 0; i < sizeof(image)/8; i++){
        if (i > 0){ // SHIFT FROM THE SECOND BIT
            char_shift = char_shift + getCharShift(image);
        }
    }

    return char_shift;
}

// --------------------------------------------
// Get character shift bits
// --------------------------------------------
unsigned int getCharShift(uint64_t image){
  unsigned int maxIndex = 0;
  unsigned int line_width[8];
  unsigned int char_width = line_width[maxIndex];

  for (int i = 0; i < 8; i++) {                         // x axis | vertical
    byte row = (image >> i * 8) & 0xFF;                 // GET ROW VALUE
    for (int y = 0; y < 8; y++) {                       // y axis | horizontal
      if (bitRead(row, y) == 1) {                       // FIND THE NON-ZERO BIT
        // Serial.print("Y,X : ("); Serial.print(y); Serial.print(","); Serial.print(i); Serial.println(")");
        line_width[i] = y;
      }
    }
    // Serial.println("------------");
  }

  for (int i = 0; i < 8; i++) {
    maxIndex = 0;

    if (line_width[i] > 8){
        line_width[i] = 0;    
    }
    if (line_width[i] > char_width) {
        char_width = line_width[i];
        maxIndex = i;
    }
    if (char_width > 8){
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
void displayChar(uint64_t image, int shift, byte RED, byte  GREEN, byte BLUE) {
  for (int i = 0; i < 8; i++)                               // x axis | vertical
  {                                                         
    byte row = (image >> i * 8) & 0xFF;                     // GET ROW VALUE
    Serial.print("0x "); printf("%02x", row); Serial.println("");
    for (int y = 0; y < 8; y++)                             // y axis | horizontal
    {                                                       
      if (bitRead(row, y) == 1)                             // FIND THE NON-ZERO BIT
      {                           
        // Serial.print("Y,X : ("); Serial.print(y); Serial.print(","); Serial.print(i); Serial.println(")");
        mapLEDXY(y + shift, i, RED, GREEN, BLUE);          // TURN ON NON-ZERO BIT
      }
    }
    Serial.println("------------");
  }
  //   Serial.println("==============");
}

// --------------------------------------------
// map from (x, y) to LED location in strip 
// zig zag
// top left LED starting bit
// --------------------------------------------
void mapLEDXY(int y, int x, byte RED, byte  GREEN, byte BLUE) {
  int RGBlocation = 0;

  if (y % 2 == 0) { //even column
    RGBlocation = x + y * NUM_OF_ROWS;

  } else { //odd column
    RGBlocation = 7 - x + y * NUM_OF_ROWS;
  }

  led.setPixelColor(RGBlocation, RED, GREEN, BLUE);

}


// --------------------------------------- test functions below ---------------------------------------

void symbol1() {
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

void symbol2(int shift, byte RED, byte  GREEN, byte BLUE) {
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

void symbol3(int shift, byte RED, byte  GREEN, byte BLUE) {
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

void symbol4(int shift, byte RED, byte  GREEN, byte BLUE) {
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