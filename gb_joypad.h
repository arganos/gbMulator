//
//  gb_joypad.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#ifndef __Game_Boy_Emulator__gb_joypad__
#define __Game_Boy_Emulator__gb_joypad__

#include "gb_input.h"
#include "gb_mem.h"

#include "gb_interrupt.h"

class gb_joypad_state {
private:
    gb_input input;
    
    //Memory
    gb_mem_state *gb_mem;
    //Interrupts
    gb_interrupt_state *gb_int;
    
    unsigned char p1;
    
public:
    gb_joypad_state();
    
    int loadMEM(gb_mem_state &_gb_mem);
    int loadINT(gb_interrupt_state &_gb_int);
    
    void execute();
};


#endif /* defined(__Game_Boy_Emulator__gb_joypad__) */
