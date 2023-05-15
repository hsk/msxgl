#include "msxgl.h"
#include "stdint.h"
#include "dos.h"
#include "stdio.h"
void Open(const c8* filename,FCB* fcb) {
	Mem_Set(0, fcb, sizeof(FCB));
	Mem_Copy(filename, fcb->Name, 11);
	DOS_Open(fcb);
}
void Load(FCB *file,u8* addr,u16 size) {
	for (u16 i = 0; i < size; i += 128) {
		DOS_SetTransferAddr(addr);
		DOS_SequentialRead(file);
		addr += 128;
	}
}

void LoadVRAM(FCB *file,u16 addr,u16 size) {
	u8 buff[128];
	for (u16 i = 0; i < size; i += 128) {
		DOS_SetTransferAddr(buff);
		DOS_SequentialRead(file);
		VDP_WriteVRAM_16K(buff, addr, 128);
		addr += 128;
	}
}

int putchar(int c){
	DOS_CharOutput(c);
	return 1;
}
i16 g_VBlank;
u16 pals[64];
u8 paln=0;
void VBlankHook() {
	g_VBlank--;
	if(g_VBlank==0)VDP_SetPalette(&pals[paln*16+1]);
}

void WaitVBlankSet(u8 n) {
	g_VBlank = n;
}

void WaitVBlank(u8 n) {
	while(g_VBlank>=n) {}
}

void main() {
	VDP_SetColor(0);
	VDP_ClearVRAM();
	VDP_SetMode(VDP_MODE_SCREEN3);
	Bios_SetHookCallback(H_TIMI, VBlankHook);
	do {
		FCB fcb;
		Open("DAT     SR3",&fcb);
		Load(&fcb,(u8*)pals,128);
		VDP_SetPalette(&pals[1]);
		VDP_RegWrite(4,1); VDP_RegWrite(2,0);
		paln=0;
		VDP_RegWrite(4,1+paln);
		for(u8 j=0;j<8;j++) {
			u8 buf[128];
			for(u8 k=0;k<128;k++) buf[k]=(k & 31)+j*32;
			VDP_WriteVRAM_16K(buf,j*128,128);
		}
		WaitVBlankSet(30);
		for(int i=1;i<64;i++){
			paln=1-paln;
			LoadVRAM(&fcb, 0x800+paln*0x800, 0x600);
			WaitVBlank(15);
			VDP_RegWrite(4,1+paln);
			WaitVBlank(0);
			WaitVBlankSet(30);
		}
		DOS_Close(&fcb);
		WaitVBlank(15);
		VDP_FillVRAM_16K(0,0x800,0x1000);
		WaitVBlankSet(30);
		WaitVBlank(0);
	} while(1);
}
