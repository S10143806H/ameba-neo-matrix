uint8_t new_ascii[855] = {
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //
  0x21, 0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x00,   //>!
  0x22, 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //"
  0x23, 0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00,   //#
  0x24, 0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00,   //$
  0x25, 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,   //%
  0x26, 0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,   //&
  0x27, 0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,   //'
  0x28, 0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,   //(
  0x29, 0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,   //)
  0x2a, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,   //*
  0x2b, 0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00,   //+
  0x2c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60,   //,
  0x2d, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00,   //-
  0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,   //.
  0x2f, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00,   //>/
  0x30, 0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00,   //0
  0x31, 0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00,   //1
  0x32, 0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00,   //2
  0x33, 0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00,   //3
  0x34, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,   //4
  0x35, 0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00,   //5
  0x36, 0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00,   //6
  0x37, 0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,   //7
  0x38, 0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00,   //8
  0x39, 0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00,   //9
  0x3a, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00,   //:
  0x3b, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,   //;
  0x3c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,   //<
  0x3d, 0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00,   //=
  0x3e, 0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,   //>
  0x3f, 0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00,   //?
  0x40, 0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,   //@
  0x41, 0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,   //A
  0x42, 0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,   //B
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
  0x5b, 0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00,   //[
  0x5c, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00,   //\<
  0x5d, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00,   //]
  0x5e, 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00,   //^
  0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,   //_
  0x60, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,   //`
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
  0x7e, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   //~
};

///////////////////////////////////////////////////////////////////
/*convert the string to 8x8 dot matrix 8-bytes packets*/
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define ascii_index 95
#define buffer_size str1_len*8

int i, j, k;

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  char str1[] = "Hi, how are you?";
  int str1_len = strlen(str1);
  char buffer[buffer_size];

  string_read_to_8x8_bytes_out(str1, str1_len, buffer);

  for (i = 0; i < str1_len; i++)
  {
    Serial.print("letter "); Serial.print(str1[i]); Serial.println("");
    for (j = 0; j < 8; j++) {
      Serial.print("0x "); Serial.print(buffer[(i * 8) + j],HEX); Serial.println("");
      //      printf("0x%.2x ", buffer[(i * 8) + j]);
      if (j % 8 == 7) {
        Serial.println("");
      }
    }
  }
}


void string_read_to_8x8_bytes_out(char *str_in, int strlen, char *array_out)
{
  for (i = 0; i < strlen; i++)
  {
    for (k = 0; k < ascii_index; k++)
    {
      if (str_in[i] == new_ascii[k * 9])
      {
        for (j = 0; j < 8; j++)
        {
          array_out[(i * 8) + j] = new_ascii[((k * 9) + 1) + j];
        }
      }
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:

}
