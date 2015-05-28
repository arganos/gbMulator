//
//  gb_gpu.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#ifndef __Game_Boy_Emulator__gb_gpu__
#define __Game_Boy_Emulator__gb_gpu__

#include <QObject>

#include "gb_lcd.h"

#include "gb_mem.h"
#include "gb_interrupt.h"


#define IO_REG_LCDC 0xFF40
#define IO_REG_STAT 0xFF41

#define IO_REG_SCY  0xFF42
#define IO_REG_SCX  0xFF43

#define IO_REG_LY  0xFF44
#define IO_REG_LYC  0xFF45

#define IO_REG_DMA  0xFF46

#define IO_REG_BGP   0xFF47
#define IO_REG_OBP0  0xFF48
#define IO_REG_OBP1  0xFF49

#define IO_REG_WX  0xFF4B
#define IO_REG_WY  0xFF4A

class gb_gpu_state
{

public:

    gb_gpu_state();

private:
    
    short gpu_clock;
    
    //I/O Registers
    
    unsigned char lcdc,
                  stat,
                  scx, scy,
                  ly, lyc,
                  bgp, obp0, obp1,
                  wx, wy;
    
    bool operation;
    bool wndTileMapDisplaySelect;
    bool wndDisplay;
    bool bgWndTileDataSelect;
    bool bgTileMapDisplaySelect;
    bool spriteSize;
    bool spriteDisplay;
    bool bgWndDisplay;
    
    int mode;
    int modeDuration[4];
    int scanLineDuration;
    
    unsigned char curScanLine;
    
    
    //LCD
    gb_lcd_state *gb_lcd;
    
    //Memory
    gb_mem_state *gb_mem;
    //Interrupts
    gb_interrupt_state *gb_int;


public:

    bool shouldDisplay;
    
    int loadLCD(gb_lcd_state *_gb_lcd);
    int loadMEM(gb_mem_state *_gb_mem);
    int loadINT(gb_interrupt_state *_gb_int);
    
    void execute(int cycles);
};


#endif /* defined(__Game_Boy_Emulator__gb_gpu__) */
