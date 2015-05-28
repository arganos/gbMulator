/*
 *  gb_mem.h
 *  Game Boy Emulator
 *
 *  Created by Englebert Gilles on 5/1/15.
 *  Copyright 2015. All rights reserved.
 *
 */

#ifndef GB_MEM_H
#define GB_MEM_H

#define GB_MEM_SIZE 0x10000
#define GB_BANK_SIZE 0x4000
#define GB_BIOS_SIZE 256

#define IO_REG_P1 0xFF00

#define IO_REG_SB 0xFF01
#define IO_REG_SC 0xFF02

#define IO_REG_DIV  0xFF04
#define IO_REG_TIMA 0xFF05
#define IO_REG_TMA  0xFF06
#define IO_REG_TAC  0xFF07


#define IO_REG_IE 0xFFFF
#define IO_REG_IF 0xFF0F

#define IO_REG_IEF_HTL_BIT 4
#define IO_REG_IEF_LCDC_BIT 1
#define IO_REG_IEF_SERIAL_BIT 3
#define IO_REG_IEF_TIMER_BIT 2
#define IO_REG_IEF_VBLANK_BIT 0

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

#include <fstream>
#include <iostream>

#include "gb_cartridge.h"

//#define DEBUG_MEM 0

using namespace std;


static int histo[0x100];

class gb_mem_state {
    unsigned char bios[GB_BIOS_SIZE];
    
    unsigned char vram[0x2000],  //Video RAM
                  iram[0x2000],  //Internal RAM
                  oam [0xA0],    //Sprite Attribution Memory
                  io_ports[0x4C],//I/O Ports
                  hram[0x7F],    //High RAM
                  ier;           //Interrupt Enable Register
    
    bool inBios;
    bool ioUpdate;
    
    gb_cartridge cartridge;
    unsigned short romBankNo;
    unsigned short ramBankNo;
    
    //MBC1
    bool mbc1_ram_enable;
    bool mbc1_memory_model;
    
    //MBC3
    bool mbc3_ram_enable;
    bool mbc3_rtc_enable;
    
    
    
public:
    
    gb_mem_state();
    
    void setBit(unsigned short addr, int bit);
    void resetBit(unsigned short addr, int bit);
    
    bool bitSet(unsigned short addr, int bit);
    
    void wb(unsigned short addr, unsigned char val);
    unsigned char rb(unsigned short addr);
    
    void ww(unsigned short addr, unsigned short val);
    short rw(unsigned short addr);
	
    int loadBIOS(string filename);
    int loadROM(string filename);
};

#endif