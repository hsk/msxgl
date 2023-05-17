
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
u8 ship[] = {
  4,5,
  0x00,0x08,0x8F,0x08,0x00,
  0xFE,0xFE,0xFE,0xFE,0xFE,
  0x00,0xFE,0x00,0xFE,0x00,
  0x00,0x40,0xFE,0x40,0x00,
  4,5,
  0x00,0x00,0x08,0x00,0x00,
  0xFE,0xFE,0xFE,0xFE,0xFE,
  0x00,0xFE,0x00,0xFE,0x00,
  0x00,0x40,0xFE,0x40,0x00,

};
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
inline void sc3PutPixelNoClip(Sc3* sc3, u8 x, u8 y, u8 col) {
  u8* dt = sc3->data;
  dt[(u16)sc3->h*x+y]=col;
}
inline void sc3DrawVLineNoClip(Sc3* sc3, u8 x, u8 y, u8 h, u8 col) {
  u8* dt = sc3->data;
  u8 h1 = sc3->h;
  u8* p = &dt[h1*x+y];
  do {
    *p++ =col;
  } while(--h);
}
inline void sc3FillRectNoClip(Sc3* sc3, u8 x, u8 y, u8 w, u8 h, u8 col) {
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
void sc3DrawImageNoClip(Sc3* sc3, Sc3* im, u8 x, u8 y) {
  u8 w = im->w;
  u8 h = im->h; 
  u8* src = im->data;
  u8* dt = sc3->data;
  i8 h1 = sc3->h;
  do {
    u8 h2=h;
    u8* p = &dt[h1*x+y];
    do {
      *p++ = *src++;
    } while(--h2);
    x++;
  } while(--w);
}
void sc3Clear(Sc3* sc3,u8 col) {
  u16 len=sc3->w*sc3->h;
  u8* dt = sc3->data;
  do {
    *dt++ = col;
  } while(--len);
}
void sc3PutPixel(Sc3* sc3, i8 x, i8 y, u8 col) {
  if (x < 0 || sc3->w <= x || y < 0 || sc3->h <= y) return;
  sc3PutPixelNoClip(sc3,x,y,col);
}
void sc3DrawVLine(Sc3* sc3, i8 x, i8 y, i8 h, u8 col) {
  if (x < 0 || sc3->w <= x) return;
  h += y;
  if (h < y) { i8 t = y; h=y; h=t; }
  if (h <= 0 || sc3->h <= y) return;
  if (y < 0) y = 0;
  if (h > sc3->h) h = sc3->h;
  h -= y;
  sc3DrawVLineNoClip(sc3, x, y, h, col);
}

void sc3FillRect(Sc3* sc3, i8 x, i8 y, i8 w, i8 h, u8 col) {
  w += x;
  if (w < x) { i8 t = x; w=x; w=t; }
  if (w <= 0 || sc3->w <= x) return;
  if (x < 0) x = 0;
  if (w > sc3->w) w = sc3->w;
  w -= x;
  
  h += y;
  if (h < y) { i8 t = y; h=y; h=t; }
  if (h <= 0 || sc3->h <= y) return;
  if (y < 0) y = 0;
  if (h > sc3->h) h = sc3->h;
  h -= y;
  
  sc3FillRectNoClip(sc3, x, y, w, h, col);
}

uint32_t rnd = 43243214321;
inline uint16_t rand(){
	rnd = rnd * 259 + 43431431;
	return ((uint16_t*)&rnd)[1];
}
void run() {
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
}
void run2() {
  i8 x=0,y=0;
  i8 xs[16];
  for(int i=0;i<16;i++)xs[i]=rand()&31;
  for(;;){
    WaitVSync();
    DI();sc3WriteVRAM(sc3,0x800);EI();
    if(Keyboard_IsKeyPressed(KEY_UP))    y-=2;
    if(Keyboard_IsKeyPressed(KEY_DOWN))  y+=2;
    if(Keyboard_IsKeyPressed(KEY_LEFT))  x--;
    if(Keyboard_IsKeyPressed(KEY_RIGHT)) x++;
    if(Keyboard_IsKeyPressed(KEY_SPACE)) break;
    sc3Clear(sc3,0);
    sc3FillRect(sc3,x-2,y-4,4,8,0x22);
    for(u8 i=0;i<16;i++) {
      u8 t=xs[i]=(xs[i]-1)&31;
      sc3PutPixel(sc3,t,i*3,0x33);

    }
  }
  for(;;)if(!Keyboard_IsKeyPressed(KEY_SPACE)) break;
}
void run3() {
  i8 x=0,y=0;
  u8 xs[16],vs[16];
  for(int i=0;i<16;i++){
    xs[i]=rand()&255;
    vs[i]=(rand()&15)+2;
  }
  u8 s = 0;
  for(;;){
    WaitVSync();
    DI();sc3WriteVRAM(sc3,0x800);EI();
    if(Keyboard_IsKeyPressed(KEY_UP)&& y-2 >= 0)    y-=2;
    if(Keyboard_IsKeyPressed(KEY_DOWN)&& y+5+2 <= 48)  y+=2;
    if(Keyboard_IsKeyPressed(KEY_LEFT) && x-1 >= 0)  x--;
    if(Keyboard_IsKeyPressed(KEY_RIGHT)&& x+1+4<=32) x++;
    if(Keyboard_IsKeyPressed(KEY_SPACE)) break;
    sc3Clear(sc3,0);
    sc3DrawImageNoClip(sc3,(Sc3*)&ship[s],x,y);
    s = (2+4*5)-s;
    for(u8 i=0;i<16;i++) {
      u8 t=xs[i]=(xs[i]-vs[i]);
      sc3PutPixel(sc3,t>>3,i*3,0xe);

    }
  }
  for(;;)if(!Keyboard_IsKeyPressed(KEY_SPACE)) break;
}
int main() {
  sc3 = (Sc3*) buf;
  for(int i=0;i<64*48/2;i++)sc3->data[i]=0;
  sc3->w = 32; sc3->h = 48;
  Bios_SetHookCallback(H_TIMI, VBlankHook);
  sc3Init();
  while(1) {
    run2();
    run3();
    run();                               
  }
  return 0;
}
