
#include "msxgl.h"
#include "stdint.h"
#include "dos.h"
#include "stdio.h"
#include "string.h"
int putchar(int c){
  DOS_CharOutput(c);
  return 1;
}
#define DI() {__asm di __endasm;}
#define EI() {__asm ei __endasm;}

u8 	g_VBlank = 0;
void VBlankHook() {
  g_VBlank = 1;
}

void WaitVSync() {
  while(g_VBlank == 0) {}
  g_VBlank = 0;
}

typedef struct Sc3 {
  u8 w;
  u8 h;
  u8 data[1];
} Sc3;

u8 buf[64*48/2+2];
Sc3* sc3;
void sc3Init() {
  VDP_SetColor(0);
  VDP_ClearVRAM();
  VDP_SetMode(VDP_MODE_SCREEN3);
  VDP_RegWrite(4,1); VDP_RegWrite(2,0);
  for(u8 j=0;j<8;j++) {
    char buf[128];
    for(u8 k=0;k<128;k++) buf[k]=(k & 31)*6+j;
    DI();VDP_WriteVRAM_16K(buf,j<<7,128);EI();
  }
}
void sc3WriteVRAM(Sc3* sc3, u16 addr) {
  VDP_WriteVRAM_16K(sc3->data,addr,sc3->w*sc3->h);
}
void sc3PutPixel(Sc3* sc3, u8 x, u8 y, u8 col) {
  u8* dt = sc3->data;
  dt[(u16)sc3->h*x+y]=col;
}
void sc3DrawVLine(Sc3* sc3, u8 x, u8 y, u8 h, u8 col) {
  u8* dt = sc3->data;
  u8 h1 = sc3->h;
  u8* p = &dt[h1*x+y];
  do {
    *p++ =col;
  } while(--h);
}
void sc3FillRect(Sc3* sc3, u8 x, u8 y, u8 w, u8 h, u8 col) {
  u8* dt = sc3->data;
  i8 h1 = sc3->h;
  do {
    u8 h2=h;
    u8* p = &dt[h1*x+y];
    do {
      *p++ =col;
    } while(--h2);
    x++;
  } while(--w);
}
uint32_t rnd = 43243214321;
inline uint16_t rand(){
	rnd = rnd * 259 + 43431431;
	return ((uint16_t*)&rnd)[1];
}

void run() {
  for(u8 y=0,k=0;y<48;y++) {
    for(u8 x=0;x<32;x++,k=(k+1)&15) {
      sc3PutPixel(sc3, x, y, 0x11*k);
      //WaitVSync();
    }
    DI();sc3WriteVRAM(sc3,0x800);EI();
  }
  for(u8 x=0,k=0;x<32;x++) {
    for(u8 y=0;y<48;y+=12,k=(k+1)&15) {
      sc3DrawVLine(sc3, x, y, 12, 0x11*k);
      DI();sc3WriteVRAM(sc3,0x800);EI();
    }
  }
  for(u8 x=0,k=0;x<32;x+=4) {
    for(u8 y=0;y<48;y+=8,k=(k+1)&15) {
      sc3FillRect(sc3, x, y, 4, 8, 0x11*k);
      DI();sc3WriteVRAM(sc3,0x800);EI();
    }
  }
  for(u8 i=0,k=0;i<255;i++,k=(k+1)&15) {
    u16 v=rand();
    u8 x = v&31;
    u8 y = (v>>8) % 48;
    u16 v2=rand();
    u8 x2 = (v2&31);
    u8 y2 = ((v2>>8) % 48);
    if(x2<x) {u8 t=x;x=x2;x2=t;}
    if(y2<y) {u8 t=y;y=y2;y2=t;}
    u8 w = x2-x+1;
    u8 h = y2-y+1;
    sc3FillRect(sc3, x, y, w, h, 0x11*k);
    DI();sc3WriteVRAM(sc3,0x800);EI();
  }
}
int main() {
  sc3 = (Sc3*) buf;
  for(int i=0;i<64*48/2;i++)sc3->data[i]=0;
  sc3->w = 32; sc3->h = 48;
  Bios_SetHookCallback(H_TIMI, VBlankHook);
  sc3Init();
  while(1) {
    run();
  }
  return 0;
}
