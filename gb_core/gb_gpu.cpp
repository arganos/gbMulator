//
// gb_gpu.cpp
// Game Boy Emulator
//
// Created by Gilles Englebert on 05/05/15.
// Copyright 2015. All rights reserved.
//
//
#include "gb_gpu.h"
#include <iostream>


gb_gpu_state::gb_gpu_state() {
    gpu_clock = 0;
    mode = 2;
    curScanLine = 0;
    
    modeDuration[0] = 204;
    modeDuration[1] = 4560;
    modeDuration[2] = 80;
    modeDuration[3] = 172;
    
    scanLineDuration = 456;
}

void gb_gpu_state::execute(int cycles) {
    
    shouldDisplay = 0;

    lcdc = gb_mem->rb(IO_REG_LCDC);
    stat = gb_mem->rb(IO_REG_STAT);
    scx = gb_mem->rb(IO_REG_SCX);
    scy = gb_mem->rb(IO_REG_SCY);
    ly  = gb_mem->rb(IO_REG_LY);
    lyc = gb_mem->rb(IO_REG_LYC);
    bgp  = gb_mem->rb(IO_REG_BGP);
    obp0 = gb_mem->rb(IO_REG_OBP0);
    obp1 = gb_mem->rb(IO_REG_OBP1);
    wx = gb_mem->rb(IO_REG_WX);
    wy = gb_mem->rb(IO_REG_WY);
    
    operation = lcdc&(1<<7);
    wndTileMapDisplaySelect = lcdc&(1<<6);
    wndDisplay = lcdc&(1<<5);
    bgWndTileDataSelect = lcdc&(1<<4);
    bgTileMapDisplaySelect = lcdc&(1<<3);
    spriteSize = lcdc&(1<<2);
    spriteDisplay = lcdc&(1<<1);
    bgWndDisplay = lcdc&(1<<0);
    
    
    if (!operation)
        return;
    
    gpu_clock += cycles;
    
    
    if (stat&(1<<6)) {
        if (lyc == ly)
            gb_int->set_lcdc();
        } else  if (stat&(1<<5)) {
            if (mode == 2)
                gb_int->set_lcdc();
        } else  if (stat&(1<<4)) {
            if (mode == 1)
                gb_int->set_lcdc();
        } else if (stat&(1<<3)) {
            if (mode == 0)
                gb_int->set_lcdc();
    }
    
    if (mode == 1)
        curScanLine= 144 + gpu_clock/scanLineDuration;
    
    if (gpu_clock >= modeDuration[mode]) {
        gpu_clock -= modeDuration[mode];
        if (mode == 2) {
            mode = 3;
        } else if (mode == 3) {
            mode = 0;
        } else if (mode == 0) {
            if (curScanLine == 143) {
                mode = 1;
                gb_int->set_vblank();
                } else {
                mode = 2;
                curScanLine++;
            }
        } else if (mode == 1) {
            mode = 2;
            curScanLine = 0;
            
            int bm[160][144] ;
            for (int x = 0; x < 160; x++)
                for (int y = 0; y < 144; y++)
                    bm[x][y] = 0;
                    
            //Draw Background:
            if (bgWndDisplay) {
                // cout << "BackGround" << endl;
                unsigned short mapAddr, tileAddr;
                if (bgTileMapDisplaySelect == 0) {
                    //0x9800-0x9bff
                    mapAddr = 0x9800;
                    } else {
                    mapAddr = 0x9C00; //a
                }
                if (bgWndTileDataSelect == 0) {
                    //8800-97ff signed
                    tileAddr = 0x8800;
                    } else {
                    //8000-8fff unsigned
                    tileAddr = 0x8000; //a
                }
                
                bool pixeldrawn[160][144];
                for (int x = 0; x < 160; x++)
                    for (int y = 0; y < 144; y++)
                        pixeldrawn[x][y] = 0;
                //Draw 32x32 tiles
                for (int x = 0; x < 32; x++)
                for (int y = 0; y < 32; y++) {
                    unsigned char descr = gb_mem->rb(32*y+x+mapAddr);
                    unsigned short st_addr = tileAddr;
                    if (tileAddr == 0x8000)
                    st_addr += 16*((unsigned short)descr);
                    else {
                        unsigned char ndescr = (descr+128);
                        st_addr += 16*((unsigned short)ndescr);
                    }
                    for (int j = 0; 2*j < 16; j++) {
                        unsigned char a = gb_mem->rb(st_addr+2*j),
                        b = gb_mem->rb(st_addr+2*j+1) ;
                        for (int bt = 0; bt < 8; bt++) {
                            unsigned char dotdata =(!!(a&(1<<bt)) + 2*(!!(b&(1<<bt))));
                            unsigned char color = (bgp>>(2*dotdata))%4;
                            //adjust coordinates, due to location of data in memory
                            int X =((8*x+(7-bt)-scx+256))%256;
                            int Y =((8*y+j-scy+256))%256;
                            if (X < 0 || X >= 160)
                            continue;
                            if (Y < 0 || Y >= 144)
                            continue;
                            bm[X][Y] = color;
                            gb_lcd->drawPixel(X, Y, color);
                        }
                    }
                }
            }
            //Draw Window:
            if (bgWndDisplay && wndDisplay) {
                unsigned short mapAddr, tileAddr;
                // cout << "Window" << endl;
                if (wndTileMapDisplaySelect == 0) {
                    //0x9800-0x9bff
                    mapAddr = 0x9800;
                    } else {
                    mapAddr = 0x9C00; //a
                }
                if (bgWndTileDataSelect == 0) {
                    //8800-97ff signed
                    tileAddr = 0x8800;
                    } else {
                    //8000-8fff unsigned
                    tileAddr = 0x8000; //a
                }
                //((unsigned short)descr) cout <<
                //cout << bgWndDisplay << " " << bgTileMapDisplaySelect << endl;
                //Draw 32x32 tiles
                for (int x = 0; x < 32; x++)
                for (int y = 0; y < 32; y++) {
                    unsigned char descr = gb_mem->rb(32*y+x+mapAddr);
                    unsigned short st_addr = tileAddr;
                    if (tileAddr == 0x8000)
                    st_addr += 16*((unsigned short)descr);
                    else {
                        // if (descr != 0)
                        //cout <<hex << "descr = "<< descr+0l << endl;
                        unsigned char ndescr = (descr+128);
                        //cout << hex << "newd = " << ndescr +0l<< endl;
                        st_addr += 16*((unsigned short)ndescr);
                    }
                    // cout << tileAddr << endl;
                    for (int j = 0; 2*j < 16; j++) {
                        unsigned char a = gb_mem->rb(st_addr+2*j),
                        b = gb_mem->rb(st_addr+2*j+1) ;
                        // cout << a+0l << " " << b+0l << endl;
                        for (int bt = 0; bt < 8; bt++) {
                            unsigned char dotdata =(!!(a&(1<<bt)) + 2*(!!(b&(1<<bt))));
                            unsigned char color = (bgp>>(2*dotdata))%4;
                            // cout << (((unsigned short)dotdata)&0xff)<< endl;
                            //adjust coordinates, due to location of data in memory
                            int X =8*x+(7-bt)+wx-7;
                            int Y =8*y+j+wy;
                            if (X < 0 || X >= 160)
                            continue;
                            if (Y < 0 || Y >= 144)
                            continue;
                            bm[X][Y] = color;
                            gb_lcd->drawPixel(X, Y, color);
                        }
                    }
                }
            }
            if (spriteDisplay) {
                unsigned short spt_addr = 0x8000;
                unsigned short sat_start = 0xFE00, sat_end = 0xFE9F;
                unsigned char x, y, num, flags;
                if (spriteSize == 0) {
                    for (unsigned short cur_addr = sat_start; cur_addr <= sat_end; cur_addr += 4) {
                        y = gb_mem->rb(cur_addr);
                        x = gb_mem->rb(cur_addr+1);
                        num  = gb_mem->rb(cur_addr+2);
                        flags = gb_mem->rb(cur_addr+3);
                        x -= 8;
                        y -= 16;
                        if (!((x < 160) && (y < 144)))
                        continue;
                        //cout << "8x8" << endl;
                        for (int j = 0; 2*j < 16; j++) {
                            unsigned char a = gb_mem->rb(spt_addr+16*num+2*j),
                            b = gb_mem->rb(spt_addr+16*num+2*j+1) ;
                            //cout << a+0l << " " << b+0l << endl;
                            for (int bt = 0; bt < 8; bt++) {
                                unsigned char dotdata =(!!(a&(1<<bt)) + 2*(!!(b&(1<<bt))));
                                unsigned char color = (((flags&0x10)?obp1:obp0)>>(2*dotdata))%4;
                                
                                int X, Y;
                               
                                if (flags&(1<<5))
                                    X = x+bt;
                                else
                                    X =x+7-bt;
                                
                                if (flags&(1<<6))
                                    Y =y+7-j;
                                else
                                    Y =y+j;
                                if (X < 0 || X >= 160)
                                continue;
                                if (Y < 0 || Y >= 144)
                                continue;
                                if (flags & (1<<7)) {
                                    if (bm[X][Y] == 0) {
                                        bm[X][Y] = color;
                                        gb_lcd->drawPixel(X, Y, color);
                                    }
                                    } else if (dotdata != 0){
                                    bm[X][Y] = color;
                                    gb_lcd->drawPixel(X, Y, color);
                                }
                            }
                            //cout << endl;
                        }
                    }
                    } else {
                    for (unsigned short cur_addr = sat_start; cur_addr <= sat_end; cur_addr += 4) {
                        y = gb_mem->rb(cur_addr);
                        x = gb_mem->rb(cur_addr+1);
                        num  = gb_mem->rb(cur_addr+2);
                        flags = gb_mem->rb(cur_addr+3);
                        x -= 8;
                        y -= 16;
                        //cout << "8x16" << endl;
                        for (int j = 0; 2*j < 32; j++) {
                            unsigned char a = gb_mem->rb(spt_addr+16*num+2*j),
                            b = gb_mem->rb(spt_addr+16*num+2*j+1) ;
                            //cout << a+0l << " " << b+0l << endl;
                            for (int bt = 0; bt < 8; bt++) {
                                unsigned char dotdata =(!!(a&(1<<bt)) + 2*(!!(b&(1<<bt))));
                                unsigned char color = (((flags&0x10)?obp1:obp0)>>(2*dotdata))%4;
                                int X, Y;
                                if (flags&(1<<5))
                                X = x+bt;
                                else
                                X =x+7-bt;
                                if (flags&(1<<6))
                                Y =y+7-j;
                                else
                                Y =y+j;
                                if (X < 0 || X >= 160)
                                continue;
                                if (Y < 0 || Y >= 144)
                                continue;
                                if (flags & (1<<7)) {
                                    if (bm[X][Y] == 0) {
                                        bm[X][Y] = color;
                                        gb_lcd->drawPixel(X, Y, color);
                                    }
                                    } else if (dotdata != 0){
                                    bm[X][Y] = color;
                                    gb_lcd->drawPixel(X, Y, color);
                                }
                            }
                        }
                    }
                }
            }
            //Tell the Worker that he should show the frame
           shouldDisplay = 1;
        }
    }
    gb_mem->wb(IO_REG_LY, (unsigned char)curScanLine);
    stat = ((stat>>3)<<3) + ((curScanLine==lyc)<<2)+ mode;
    gb_mem->wb(IO_REG_STAT, (unsigned char)stat);
}

int gb_gpu_state::loadLCD(gb_lcd_state *_gb_lcd) {
    gb_lcd = _gb_lcd;
    return 1;
}

int gb_gpu_state::loadMEM(gb_mem_state *_gb_mem) {
    gb_mem = _gb_mem;
    return 1;
}
int gb_gpu_state::loadINT(gb_interrupt_state *_gb_int) {
    gb_int = _gb_int;
    return 1;
}
