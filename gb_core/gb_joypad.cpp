//
//  gb_joypad.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#include "gb_joypad.h"


gb_joypad_state::gb_joypad_state() {
    reset();
}

int gb_joypad_state::loadMEM(gb_mem_state *_gb_mem) {
    gb_mem = _gb_mem;
    return 1;
}

int gb_joypad_state::loadINT(gb_interrupt_state *_gb_int) {
    gb_int = _gb_int;
    return 1;
}

void gb_joypad_state::keyDown(char key) {
    gb_int->set_htl();
cout << "JPD:"<<key << endl;
    switch (key) {
        case 'A':
            pressedA = 1;
            break;
    case 'B':
        pressedB = 1;
        break;
    case 'E': //SELECT
        pressedSelect = 1;
        break;
    case 'T': //START
        pressedStart = 1;
        break;
    case 'U':
        pressedUp = 1;
        break;
    case 'D':
        pressedDown = 1;
        break;
    case 'R':
        pressedRight = 1;
        break;
    case 'L':
        pressedLeft = 1;
        break;
    default:
        break;
    }
}

void gb_joypad_state::keyUp(char key) {
    gb_int->set_htl();

    switch (key) {
        case 'A':
            pressedA = 0;
            break;
    case 'B':
        pressedB = 0;
        break;
    case 'E': //SELECT
        pressedSelect = 0;
        break;
    case 'T': //START
        pressedStart = 0;
        break;
    case 'U':
        pressedUp = 0;
        break;
    case 'D':
        pressedDown = 0;
        break;
    case 'R':
        pressedRight = 0;
        break;
    case 'L':
        pressedLeft = 0;
        break;
    default:
        break;
    }
}

void gb_joypad_state::execute() {

    //cout << pressedStart << endl;
    p1 = gb_mem->rb(IO_REG_P1);
    //cout << p1+0l << endl;
    if (((p1 & 0x30) != 0x30) && ((p1 & 0x30) != 0x00)) {
        if (!(p1 & 0x10)) {
            //P14 selected
            p1 &= 0xF0;
            p1 += 1*(1-pressedRight)  +
                  2*(1-pressedLeft)   +
                  4*(1-pressedUp)     +
                  8*(1-pressedDown)   ;
        } else if (!(p1 & 0x20)) {
            //P15 selected
            p1 &= 0xF0;
            p1 += 1*(1-pressedA)       +
                  2*(1-pressedB)       +
                  4*(1-pressedSelect)  +
                  8*(1-pressedStart)   ;
        }
    }
    
    //cout <<hex<< p1+0u << endl;
    
    gb_mem->wb(IO_REG_P1, p1);
}

void gb_joypad_state::reset() {
    pressedA = 0;
    pressedB = 0;
    pressedDown = 0;
    pressedLeft = 0;
    pressedRight = 0;
    pressedUp = 0;
    pressedSelect = 0;
    pressedStart = 0;
}
