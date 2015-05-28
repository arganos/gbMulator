//
//  gb_interrupt.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#include "gb_interrupt.h"

int gb_interrupt_state::loadMEM(gb_mem_state *_gb_mem) {
    
    gb_mem = _gb_mem;
    return 1;
}

void gb_interrupt_state::execute() {
    unsigned char int_f = gb_mem->rb(IO_REG_IF);
    
    if (int_f&0x1)
        set_vblank();
    else
        clear_vblank();
    
    if (int_f&0x2)
        set_lcdc();
    else
        clear_lcdc();
    
    if (int_f&0x4)
        set_timer();
    else
        clear_timer();
    
    if (int_f&0x8)
        set_serial();
    else
        clear_serial();
    
    if (int_f&0x10)
        set_htl();
    else
        clear_htl();
    
    
    gb_mem->wb(IO_REG_IF, int_f);
}
