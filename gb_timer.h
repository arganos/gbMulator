//
//  gb_timer.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 24/05/15.
//
//

#ifndef __Game_Boy_Emulator__gb_timer__
#define __Game_Boy_Emulator__gb_timer__

#include "gb_interrupt.h"
#include "gb_mem.h"

class gb_timer_state {
private:
    
    //Memory
    gb_mem_state *gb_mem;
    //Interrupts
    gb_interrupt_state *gb_int;
    
    unsigned char div, tima, tma, tac;
    
     int clock, div_clock;
    unsigned int ticks;
    
public:
    gb_timer_state();
    
    int loadMEM(gb_mem_state &_gb_mem);
    int loadINT(gb_interrupt_state &_gb_int);
    
    void execute(int cycles);
};

#endif /* defined(__Game_Boy_Emulator__gb_timer__) */
