//
//  gb_timer.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 24/05/15.
//
//

#include "gb_timer.h"

gb_timer_state::gb_timer_state() {
    tima = 0;
    tac = 0;
    tma = 0;
    
    clock = 0;
    div_clock = 0;
}

void gb_timer_state::execute(int cycles) {
    div  = gb_mem->rb(IO_REG_DIV);
    tima = gb_mem->rb(IO_REG_TIMA);
    tma  = gb_mem->rb(IO_REG_TMA);
    tac  = gb_mem->rb(IO_REG_TAC);
    
    //1048576 clock cycles per second
    div_clock += cycles;
    //cout << cycles << "cyc" << endl;
    //cout << div_clock +0l << "aaa"<<endl;
    if (div_clock >= 256) {
      //  cout << "foo " << endl;
        div_clock -= 256;
      //  cout << "div = " << div+0l << endl;
        div = (div+1)%256;
       // cout << div +0l<< endl;
    }
     gb_mem->wb(IO_REG_DIV, div);
    if (!(tac & (1<<2)))
        return;
    
    clock += cycles;
   
    
    /*
     00: 4. 096 KHz ( ~4. 194 KHz SGB)
     01: 262. 144 Khz ( ~268. 4 KHz SGB) 
     10: 65. 536 KHz ( ~67. 11 KHz SGB) 
     11: 16. 384 KHz ( ~16. 78 KHz SGB)
     */
    
    int mod = 1;
    if ((tac & 3) == 0) {
        mod = 1024;
    } else if ((tac & 3) == 1) {
        mod = 16;
    } else if ((tac & 3) == 2) {
        mod = 64;
    } else if ((tac & 3) == 3) {
        mod = 256;
    }
    
    //cout << "cycles = "<<dec << cycles << endl;
    while (clock >= mod) {
        tima = (tima+1)%256;
        clock-=mod;
        //cout << "TIMA = " << tima << endl;
        if (!tima) {
            tima = tma;
            gb_int->set_timer();
           // cout << "Fire" << endl;
        }
    }
   
    gb_mem->wb(IO_REG_TIMA, tima);
    gb_mem->wb(IO_REG_TMA, tma);
    gb_mem->wb(IO_REG_TAC, tac);
}

int gb_timer_state::loadMEM(gb_mem_state *_gb_mem) {
    
    gb_mem = _gb_mem;
    return 1;
}

int gb_timer_state::loadINT(gb_interrupt_state *_gb_int) {
    gb_int = _gb_int;
    return 1;
}

void gb_timer_state::reset() {
    tima = 0;
    tac = 0;
    tma = 0;

    clock = 0;
    div_clock = 0;
}
