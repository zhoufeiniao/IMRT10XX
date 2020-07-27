﻿#include "drv_oled.h"
#include"FreeRTOS.h"
#include"task.h"
/*********************************************************************************************************************
宏定义
*********************************************************************************************************************/
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF
#define X_WIDTH         128
#define Y_WIDTH         64



const uint8_t F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

const uint8_t logobmp2[8][128] =
{
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x3F,
0x1F,0x1F,0x0F,0x0F,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x83,0xFF,0xFF,},
         {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x03,0x03,0x7F,0x7F,0x7F,0x7F,0x7F,0x03,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x03,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF3,0xF3,0xF3,0x03,0x03,0xF3,0xF3,0xF3,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x80,0x80,
0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,},
          {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xC0,0xC0,0xFE,0xFE,0xFE,0xFE,0xFE,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x81,0x01,0x01,0x01,0x01,0x03,0x03,
0x03,0x07,0x07,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0x3F,0x0F,0x03,0x00,0x00,0xC0,0xC0,0x70,0x38,0x1C,0x0E,0x07,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,},
           {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x3F,0x1F,0x1F,0x0F,0x0F,0x0F,0x07,
0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x0F,0x0F,0x0F,0x0F,
0x0F,0x1F,0x1F,0x1F,0x3F,0x3F,0x7F,0x7F,0x7F,0xFF,0xFC,0xF0,0xE0,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x1F,0x7F,0xFF,0xFF,0x3F,
0x03,0x00,0x80,0xF0,0x70,0x1E,0x0F,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
0x80,0xC0,0xC0,0xE0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
            {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,0x1F,
0x1F,0x0F,0x07,0x83,0x83,0xC1,0xC0,0xE0,0xE0,0xF0,0xF0,0xF8,0xFC,0xFC,0xFC,0xFC,
0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7E,0x7E,0x3E,0x3C,0x3C,0x3C,0x1C,
0x18,0x10,0x10,0x00,0x00,0x08,0x08,0x00,0x00,0x04,0x01,0x01,0x03,0x03,0x83,0x83,
0x82,0xC2,0xC0,0xE4,0xE4,0xE4,0xE4,0x6C,0x78,0x78,0x30,0x30,0x30,0x61,0xF3,0xF0,
0xFC,0xFF,0x3F,0x31,0x30,0x30,0x18,0x18,0x1C,0x1C,0x1C,0x0E,0x0F,0x0F,0x0F,0x0F,
0x0F,0x0F,0x1F,0x1F,0x3F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
             {0xFF,0xFF,0xFF,0x3F,0x1F,0x0F,0x07,0x07,0x03,0x83,0x81,0x81,0x81,0x80,0x80,0x80,
0x80,0x80,0x80,0x80,0x80,0x80,0x01,0x01,0x01,0x01,0x01,0x00,0x02,0x02,0x02,0x03,
0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,
0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0xC0,0x40,0x60,0x20,
0x20,0x30,0x30,0x10,0x18,0x08,0x00,0x04,0x04,0x02,0x02,0x03,0x03,0x43,0x03,0x03,
0x03,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0xF0,0xFF,0x07,0x00,
0x00,0x03,0x06,0x1C,0x18,0xF0,0xF0,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x9F,0xFF,0xFF,0xFF,0xFF,},
              {0xFF,0xF7,0xF0,0xF8,0xFC,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFC,0xFC,0xFC,0xFC,0xF8,0xF8,0xF0,0xF0,0xF0,0xF0,0xF0,0xE0,0xE0,0xE0,
0xE2,0xE2,0xE2,0xE2,0xE0,0xE0,0xF0,0xF1,0xF1,0xF0,0xF0,0xF0,0xF0,0xF8,0xF8,0xF8,
0xF8,0xF8,0xF8,0xFC,0xFC,0xFC,0xFC,0xFE,0xFE,0xFE,0xFE,0xFE,0xE1,0xE0,0xE0,0xE0,
0xE0,0xE0,0xF0,0xF0,0xF0,0xF8,0xF8,0xFC,0xFE,0xFE,0xFC,0xFC,0xF8,0xF1,0xF0,0xC0,
0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,
0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
               {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
};




const uint8_t F8X16[] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//  0
	0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
	0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
	0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
	0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
	0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
	0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
	0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
	0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
	0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
	0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
	0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
	0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
	0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
	0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
	0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
	0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
	0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
	0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
	0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
	0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
	0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
	0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
	0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
	0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
	0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
	0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
	0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
	0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
	0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
	0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
	0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
	0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
	0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
	0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
	0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
	0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
	0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
	0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
	0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
	0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
	0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
	0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
	0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
	0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
	0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
	0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
	0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
	0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
	0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
	0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
	0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
	0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
	0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
	0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
	0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
	0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
	0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
	0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
	0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
	0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
	0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
	0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
	0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
	0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
	0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
	0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
	0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
	0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
	0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
	0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
	0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
	0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
	0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
	0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
	0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
	0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
	0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
	0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
	0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
	0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ 94

};



//内部使用用户无需调用
void OLED_WrDat(uint8_t data)
{
	OLED_DC_SET();
	OLED_WrByte(data);
}

//内部使用用户无需调用
void OLED_WrByte(uint8_t OneByte)
{
	OLED_CS_CLR();
	uint8_t i = 8;
	while (i--)
	{
		OLED_D0_CLR();
		if (OneByte & 0x80) { OLED_D1_SET(); }
		else { OLED_D1_CLR(); }
		OLED_D0_SET();
		OneByte <<= 1;
	}
	OLED_CS_SET();
}

//内部使用用户无需调用
void OLED_WrCmd(uint8_t cmd)
{
	OLED_DC_CLR();
	OLED_WrByte(cmd);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示坐标设置
//  @param      x			x轴坐标设置
//  @param      y			y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x01);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清屏函数
//  @param      bmp_data	填充颜色选着(0x00 or 0xff)
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Fill(uint8_t bmp_data)//填充颜色选择0x00 or 0xff
{
	uint8_t y, x;

	for (y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for (x = 0; x < X_WIDTH; x++)	OLED_WrDat(bmp_data);
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED初始化函数，注意必须在mcux tools将引脚初始化为输出
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Init(void)
{
	OLED_DC_SET();
	OLED_D0_SET();
	OLED_D1_SET();
	OLED_CS_SET();
	OLED_RES_CLR();
	vTaskDelay(10);
	OLED_RES_SET();
	vTaskDelay(10);//等待复位完成
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏
	OLED_Set_Pos(0, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED点亮一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_PutPixel(uint8_t x, uint8_t y)
{
	uint8_t data1;  //data1当前点的数据

	OLED_Set_Pos(x, y);
	data1 = 0x01 << (y % 8);
	OLED_WrCmd(0xb0 + (y >> 3));
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x00);
	OLED_WrDat(data1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED清除一个点
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_ClrPixel(uint8_t x, uint8_t y)
{
	uint8_t data1;  //data1当前点的数据

	OLED_Set_Pos(x, y);
	data1 = 0x00 << (y % 8);
	OLED_WrCmd(0xb0 + (y >> 3));
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x00);
	OLED_WrDat(data1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 126) { x = 0; y++; }
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)	OLED_WrDat(F6x8[c][i]);
		x += 6;
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示字符串(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      ch[]        字符串
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;

	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 120) { x = 0; y++; }

		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)	OLED_WrDat(F8X16[c * 16 + i]);

		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)	OLED_WrDat(F8X16[c * 16 + i + 8]);

		x += 8;
		j++;
	}
}


//内部使用用户无需调用
void OLED_HEXACSII(uint16_t hex, uint8_t* Print)
{
	uint8_t hexcheck;
	uint8_t TEMP;
	TEMP = 6;
	Print[TEMP] = '\0';
	while (TEMP)
	{
		TEMP--;
		hexcheck = hex % 10;
		hex /= 10;
		Print[TEMP] = hexcheck + 0x30;
	}

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示无符号数(8*16字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      num         无符号数
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num)
{
	uint8_t* ch1, ch[7];

	OLED_HEXACSII(num, ch);
	ch1 = &ch[1];
	while (*ch1 != '\0')
	{
		OLED_P8x16Str(x, y, ch1);	//显示数字
		x += 8;
		ch1++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示有符号数(6*8字体)
//  @param      x			x轴坐标设置
//  @param      y           y轴坐标设置
//  @param      num         有符号数
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_Print_Num1(uint8_t x, uint8_t y, int16_t num)
{
	uint8_t* ch1, ch[7];
	if (num < 0) { num = -num; OLED_P6x8Str(x, y, (uint8_t*)"-"); }
	else         OLED_P6x8Str(x, y, (uint8_t*)" ");
	x += 6;
	OLED_HEXACSII(num, ch);
	ch1 = &ch[1];
	while (*ch1 != '\0')
	{
		OLED_P6x8Str(x, y, ch1);	//显示数字
		x += 6;
		ch1++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      OLED显示图像
//  @param      high  		图像高度
//  @param      width 		图像宽度
//  @param      *p    		图像地址（数组）
//  @param      value 		二值化阀值
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void dis_bmp(uint16_t high, uint16_t width, uint8_t* p, uint8_t value)
{
	int16_t i, j;
	int16_t temp, temp1;
	uint8_t dat;


	temp1 = high % 8;
	if (temp1 == 0) temp = high / 8;
	else           temp = high / 8 + 1;


	for (i = 0; i < temp; i++)
	{
		OLED_Set_Pos(0, i);
		for (j = 0; j < width; j++)
		{
			dat = 0;
			if (i < (temp - 1) || !temp1 || temp1 >= 1)dat |= (*(p + i * 8 * width + j + width * 0) > value ? 1 : 0) << 0;
			if (i < (temp - 1) || !temp1 || temp1 >= 2)dat |= (*(p + i * 8 * width + j + width * 1) > value ? 1 : 0) << 1;
			if (i < (temp - 1) || !temp1 || temp1 >= 3)dat |= (*(p + i * 8 * width + j + width * 2) > value ? 1 : 0) << 2;
			if (i < (temp - 1) || !temp1 || temp1 >= 4)dat |= (*(p + i * 8 * width + j + width * 3) > value ? 1 : 0) << 3;
			if (i < (temp - 1) || !temp1 || temp1 >= 5)dat |= (*(p + i * 8 * width + j + width * 4) > value ? 1 : 0) << 4;
			if (i < (temp - 1) || !temp1 || temp1 >= 6)dat |= (*(p + i * 8 * width + j + width * 5) > value ? 1 : 0) << 5;
			if (i < (temp - 1) || !temp1 || temp1 >= 7)dat |= (*(p + i * 8 * width + j + width * 6) > value ? 1 : 0) << 6;
			if (i < (temp - 1) || !temp1 || temp1 >= 8)dat |= (*(p + i * 8 * width + j + width * 7) > value ? 1 : 0) << 7;

			OLED_WrDat(dat);
		}
	}
}

void image_size_half_forOLED(uint8_t* src, uint8_t* dst, const int row, const int col)
{

	int half_rowt, half_colt, half_col;
	half_col = col / 2;

	for (int rowt = 0; rowt < row; rowt++)
	{
		half_rowt = rowt / 2;
		for (int colt = 0; colt < col; colt++)
		{
			half_colt = colt / 2;
			if (rowt % 2 == 0 && colt % 2 == 0)
			{
				*(dst + (half_rowt) * (half_col)+half_colt) = *(src + rowt * col + colt);
			}
		}

	}

}

/**********************************************************************************************************************
*  @brief      OLED显示图像
*  @param      NULL
*  @return     void
*  @since      v1.0
*  Sample usage:			OLED_Logo();
**********************************************************************************************************************/
void OLED_Logo(void)
{
	uint8_t y, x;
	for (y = 1; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for (x = 0; x < 128; x++)
			OLED_WrDat(logobmp2[y][x]);
	}
}

/**********************************************************************************************************************
*  @brief      OLED清除字符位
*  @param      x轴坐标设置
*  @param      y轴坐标设置
*  @param      num 需要清除的字符个数
*  @return     void
*  @since      v1.0
*  Sample usage:			Str_Clr(70,0,7);
**********************************************************************************************************************/
void Str_Clr(uint32_t x, uint32_t y, uint32_t num)
{
	OLED_Set_Pos(x, y);
	for (uint8_t j = num; j > 0; j--)
		for (uint8_t i = 5; i > 0; i--)
			OLED_WrDat(F6x8[0][i]);
}



/**********************************************************************************************************************
*  @brief      OLED反色显示
*  @param      x			x轴坐标设置
*  @param      y           y轴坐标设置
*  @param      ch[]        字符串
*  @return     void
*  @since      v1.0
*  Sample usage:
**********************************************************************************************************************/
void OLED_P6x8Rst(uint8_t x, uint8_t y, uint8_t ch[])
{

	uint8_t c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 126) { x = 0; y++; }
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)	OLED_WrDat(~F6x8[c][i]);
		x += 6;
		j++;
	}
}


uint8_t gray[64*128];
#define max(a,b) (a>b?a:b)
/******************************************************************************
	  函数说明：显示图片，自动缩放至合适大小
	  入口数据：src   图片
	  返回值：  无
******************************************************************************/
void OLED_PrintPicture(img_t* src, uint8_t threshold)
{
	int scale = 1;
	int height, width;
	while (1)//查询缩放比例
	{
		height = src->height / scale;
		width = src->width / scale;
		if (height <= 64 && width <= 128)
		{
			break;
		}
		else {
			++scale;
		}
	}
	//刷
	if (src->format == PixelFormatGray)
	{
		uint8_t* p = src->pImg;
		for (uint32_t i = 0; i < height; i += 1)
		{
			for (uint32_t j = 0; j < width; j += 1)
			{
				gray[i * width + j] = p[i * scale * src->width + j * scale];
			}
		}
	}
	else if (src->format == PixelFormatRGB565)
	{
		uint16_t* p = src->pImg;
		uint16_t buf;
		for (uint32_t i = 0; i < height; i += 1)
		{
			for (uint32_t j = 0; j < width; j += 1)
			{
			    buf = p[i * scale * src->width + j * scale];
                //HSV的V通道的算法
				gray[i * width + j] = max(max(RGB565_R(buf),RGB565_G(buf)),RGB565_B(buf));
			}
		}
	}
	dis_bmp(height, width, gray, threshold);
}
