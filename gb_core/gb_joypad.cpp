//
//  gb_joypad.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#include "gb_joypad.h"


gb_joypad_state::gb_joypad_state() {
    
}

int gb_joypad_state::loadMEM(gb_mem_state *_gb_mem) {
    gb_mem = _gb_mem;
    return 1;
}

int gb_joypad_state::loadINT(gb_interrupt_state *_gb_int) {
    gb_int = _gb_int;
    return 1;
}

void gb_joypad_state::execute() {
    if (input.update()) {
        //Fire interrupt
        gb_int->set_htl();
        
    }
    
    p1 = gb_mem->rb(IO_REG_P1);
    //cout << p1+0l << endl;
    if (((p1 & 0x30) != 0x30) && ((p1 & 0x30) != 0x00)) {
        if (!(p1 & 0x10)) {
            //P14 selected
            p1 &= 0xF0;
            p1 += 1*(1-input.pressedRight)  +
                  2*(1-input.pressedLeft)   +
                  4*(1-input.pressedUp)     +
                  8*(1-input.pressedDown)   ;
        } else if (!(p1 & 0x20)) {
            //P15 selected
            p1 &= 0xF0;
            p1 += 1*(1-input.pressedA)       +
                  2*(1-input.pressedB)       +
                  4*(1-input.pressedSelect)  +
                  8*(1-input.pressedStart)   ;
        }
    }
    
    //cout <<hex<< p1+0u << endl;
    
    gb_mem->wb(IO_REG_P1, p1);
}
