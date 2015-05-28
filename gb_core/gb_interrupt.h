//
//  gb_interrupt.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#ifndef __Game_Boy_Emulator__gb_interrupt__
#define __Game_Boy_Emulator__gb_interrupt__

#include "gb_mem.h"

#define INT_START_VBLANK 0x0040
#define INT_START_LCDC 0x0048
#define INT_START_SERIAL 0x0058
#define INT_START_TIMER 0x0050
#define INT_START_HTL 0x0060

class gb_interrupt_state {
private:
    
    bool is_vblank;
    bool is_lcdc;
    bool is_timer;
    bool is_serial;
    bool is_htl;
    
    bool is_new;
    
    
    //Memory
    gb_mem_state *gb_mem;

public:
    
    
    int loadMEM(gb_mem_state *_gb_mem);
    
    //component level
    void set_vblank() { if (!gb_mem->bitSet(IO_REG_IE, IO_REG_IEF_VBLANK_BIT)) return; is_vblank = 1; is_new = 1;gb_mem->setBit(IO_REG_IF, IO_REG_IEF_VBLANK_BIT);};
    void set_lcdc()  { if (!gb_mem->bitSet(IO_REG_IE, IO_REG_IEF_LCDC_BIT)) return; is_lcdc = 1; is_new = 1;gb_mem->setBit(IO_REG_IF, IO_REG_IEF_LCDC_BIT);};
    void set_timer() { if (!gb_mem->bitSet(IO_REG_IE, IO_REG_IEF_TIMER_BIT)) return; is_timer = 1; is_new = 1; gb_mem->setBit(IO_REG_IF, IO_REG_IEF_TIMER_BIT);};
    void set_serial() { if (!gb_mem->bitSet(IO_REG_IE, IO_REG_IEF_SERIAL_BIT)) return; is_serial = 1; is_new = 1;gb_mem->setBit(IO_REG_IF, IO_REG_IEF_SERIAL_BIT);};
    void set_htl() { if (!gb_mem->bitSet(IO_REG_IE, IO_REG_IEF_HTL_BIT)) return; is_htl = 1; is_new = 1;gb_mem->setBit(IO_REG_IF, IO_REG_IEF_HTL_BIT);};
    
    //CPU level
    bool is_int() {return is_new;};
    
    bool get_vblank() {return is_vblank;};
    bool get_lcdc() {return is_lcdc;}
    bool get_timer() {return is_timer;}
    bool get_serial() {return is_serial;}
    bool get_htl() {return is_htl;};
    
    
    void clear() {is_new = is_vblank | is_lcdc | is_timer |is_serial | is_htl;};
    void clear_vblank() { is_vblank = 0; gb_mem->resetBit(IO_REG_IF, IO_REG_IEF_VBLANK_BIT); clear();};
    void clear_lcdc() { is_lcdc = 0;gb_mem->resetBit(IO_REG_IF, IO_REG_IEF_LCDC_BIT); clear();};
    void clear_timer() { is_timer = 0;gb_mem->resetBit(IO_REG_IF, IO_REG_IEF_TIMER_BIT);clear();};
    void clear_serial() { is_serial = 0; gb_mem->resetBit(IO_REG_IF, IO_REG_IEF_SERIAL_BIT);clear();};
    void clear_htl() { is_htl = 0;gb_mem->resetBit(IO_REG_IF, IO_REG_IEF_HTL_BIT);clear();};
    
    void execute();
    void reset();
};

#endif /* defined(__Game_Boy_Emulator__gb_interrupt__) */
