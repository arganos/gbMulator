/*
 *  gb_mem.cpp
 *  Game Boy Emulator
 *
 *  Created by Englebert Gilles on 5/1/15.
 *  Copyright 2015. All rights reserved.
 *
 */

#include "gb_mem.h"


gb_mem_state::gb_mem_state() {




    reset();
    
}



void gb_mem_state::setBit(unsigned short addr, int bit) {
    unsigned char val = rb(addr);
    
    val |= (unsigned char)(1<<bit);
    
    wb(addr, val);
}

void gb_mem_state::resetBit(unsigned short addr, int bit) {
    unsigned char val = rb(addr);
    
    val &= (unsigned char)(((1<<8)-1)-(1<<bit));
    
    wb(addr, val);
}

bool gb_mem_state::bitSet(unsigned short addr, int bit) {
    unsigned char val = rb(addr);
    
    return (val&(1<<bit));
}

void gb_mem_state::wb(unsigned short addr, unsigned char val) {

    
    if (cartridge.type == gb_cartridge::ROM_MBC1 || cartridge.type == gb_cartridge::ROM_MBC1_RAM_BATT) {
        if (addr < 0x2000) {
            //En/Disable RAM
            mbc1_ram_enable = (val == 0x0A);
#ifdef DEBUG_MEM
            cout << addr << " "<<hex << val+0l << endl;
            if (val != 0xA)
                cout << "MBC1: Disable RAM" << endl;
            else
                cout << "MBC1: Enable RAM" << endl;
#endif
        } else if (addr < 0x4000) {
            romBankNo = val%(1<<5);
            if (val == 0)
                val = 1;
#ifdef DEBUG_MEM
            cout << "MBC1: Switched romBank: " << dec<<romBankNo << endl;
#endif
            
            
        } else if (addr < 0x6000) {
            if (mbc1_memory_model == 1) {
                //RAM
                ramBankNo = val%4;
#ifdef DEBUG_MEM
                cout << "MBC1: Switched ramBank:  " << dec<<ramBankNo << endl;
#endif
                
            } else {
                //ROM
                romBankNo = ((val%4)<<5)+romBankNo%(1<<5);
#ifdef DEBUG_MEM
                cout << "MBC1: Switched romBank (2upper bits): " << dec<<romBankNo << endl;
#endif
            }
        } else if (addr < 0x8000) {
            mbc1_memory_model = val%2;
#ifdef DEBUG_MEM
            if (val == 0)
                cout << "MBC1: Switched MemModel to ROM" << endl;
            else
                cout << "MBC1: Switched MemModel to RAM" << endl;
#endif
        }
    }
    
    if (cartridge.type == gb_cartridge::ROM_MBC3_RAM_BATTERY) {
        if (addr < 0x2000) {
            //En/Disable RAM
            mbc3_ram_enable = (val == 0x0A);
            //cout << "addr = " <<hex << addr<< endl;
            //cout << val +0l<< " " << endl;
            if (val != 0x0A)
                ;//cout << "MBC3: Disable RAM" << endl;
            else
                ;//cout << "MBC3: Enable RAM" << endl;
        } else if (addr < 0x4000) {
            romBankNo = val%(1<<7);
            if (val == 0)
                val = 1;
            #ifdef DEBUG_MEM
            cout << "MBC3: Switched romBank: " << dec<<romBankNo << endl;
            #endif
        } else if (addr < 0x6000) {
            if (val < 4) {
                //RAM
                ramBankNo = val%4;
               //cout << "MBC3: Switched ramBank:  " << dec<<ramBankNo << endl;
            } else {
                //RTC
            }

        } else if (addr < 0x8000) {
            //RTC
        }
    }
    
    switch (addr&0xF000) {
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
            
            //ROM Bank 0
            
            
            
            break;
        
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            //Switchable ROM Bank

            break;
            
        case 0x8000:
        case 0x9000:
            //Video RAM
            
            
            vram[addr-0x8000] = val;
            break;
        
        case 0xA000:
        case 0xB000:
            //Switchable RAM Bank
            
            if ((signed short)ramBankNo < 0) {
                error("General: Ram bank no < 0!", 0);
            }
            else {
                //cout << ramBankNo << endl;
                 cartridge.ram_bank[ramBankNo].data[addr-0xA000] = val;
            }
            break;
        
        case 0xC000:
        case 0xD000:
            
            //Internal RAM
            iram[addr-0xC000] = val;
            break;
            
        case 0xE000:
            //Internal RAM Echo
            
            iram[addr-0xE000] = val;
            break;
            
        case 0xF000:
            if (addr < 0xFE00) {
                //Internal RAM Echo
                
                
                iram[addr-0xE000] = val;
            
            } else if (addr < 0xFEA0) {
                //Sprite Attribution Memory
                
                oam[addr-0xFE00] = val;
    
            } else if (addr < 0xFF00) {
                //Unusable
            
            } else if (addr < 0xFF4C) {
                //I/O Ports
                if (addr == IO_REG_DMA) {
                   // cout<< "dma" << endl;
                   // cout<< "dma base = "<<hex <<  val+0l << endl;
                    for (int spr_num = 0; spr_num< 40; spr_num += 1) {
                        oam[4*spr_num] = rb(val*0x100+4*spr_num);
                        oam[4*spr_num+1] = rb(val*0x100+4*spr_num+1);
                        oam[4*spr_num+2] = rb(val*0x100+4*spr_num+2);
                        //if (oam[4*spr_num+2] != 0)
                        //cout << oam[4*spr_num+2] +0l << endl;
                        oam[4*spr_num+3] = rb(val*0x100+4*spr_num+3);
                    }
                }
                //if (addr == IO_REG_DIV) {
                   // cout << "write"<<endl;
                   // io_ports[addr-0xFF00]  = 0;
                //} else
                    io_ports[addr-0xFF00] = val;
            
            } else if (addr < 0xFF80) {
                //Unusable
                
            } else if (addr < 0xFFFF) {
                //High RAM
                //if (!inBios)
                //    cout << hex<< "write val to hram[0x"<<addr<< "] = "<< val+0u << endl;
                hram[addr-0xFF80] = val;
                
            } else {
                //Interrupt Enable Register
                
                ier = val;
            }
            
            break;
    }
}


unsigned char gb_mem_state::rb(unsigned short addr) {
    
    switch (addr&0xF000) {
        case 0x0000:
            if (inBios && (addr < 0x100))
                return bios[addr];
            if (addr == 0x100) {
                inBios = 0;
            }
            
            
        case 0x1000:
        case 0x2000:
        case 0x3000:
            
            //ROM Bank 0
            
            return cartridge.rom_bank[0].data[addr];
            
            break;
            
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            //Switchable ROM Bank
            if ((signed short)romBankNo < 0) {
                error("General: Rom bank no < 0", 0);
                //cout << romBankNo << endl;
            }
            else {
                //If ROM_ONLY, romBankNo defaults to 1
                return cartridge.rom_bank[romBankNo].data[addr-0x4000];
            }
            break;
            
        case 0x8000:
        case 0x9000:
            //Video RAM
            
            
            return vram[addr-0x8000];
            break;
            
        case 0xA000:
        case 0xB000:
            //Switchable RAM Bank
            //cout <<ramBankNo<<endl;
            if ((signed short)ramBankNo < 0) {
                error("General: Ram bank no < 0!", 0);
            }
            else {
                //cout << ramBankNo << endl;
                return cartridge.ram_bank[ramBankNo].data[addr-0xA000];
            }
            break;
            
        case 0xC000:
        case 0xD000:
            //Internal RAM
            
            return iram[addr-0xC000];
            break;
            
        case 0xE000:
            //Internal RAM Echo
            
            
            return iram[addr-0xE000];
            break;
            
        case 0xF000:
            if (addr < 0xFE00) {
                //Internal RAM Echo
                
                
                return iram[addr-0xE000];
                
            } else if (addr < 0xFEA0) {
                //Sprite Attribution Memory
                
                return oam[addr-0xFE00];
                
            } else if (addr < 0xFF00) {
                //Unusable
                
            } else if (addr < 0xFF4C) {
                //I/O Ports
                //cout << hex << addr-0xFF00 << endl;
                return io_ports[addr-0xFF00];
                
            } else if (addr < 0xFF80) {
                //Unusable
                
            } else if (addr < 0xFFFF) {
                //High RAM
                //cout << "read"<<endl;
                return hram[addr-0xFF80];
                
            } else {
                //Interrupt Enable Register
                
                return ier;
            }
            
            break;
    }
    return -1;
}

void gb_mem_state::ww(unsigned short addr, unsigned short val) {
    cout << "NOOO" << endl;
   // mem[addr] = val%(1<<8);
   // mem[addr+1] = val/(1<<8);
}
short gb_mem_state::rw(unsigned short addr) {
   // return (((short)mem[addr+1])<<8)+mem[addr];
    return -1;

}


int gb_mem_state::loadBIOS(string filename) {
    ifstream biosfile;
    
    biosfile.open(filename.c_str(), ifstream::binary | ifstream::in);
    
    if (!biosfile.is_open())
        return 0;
    
    biosfile.read((char*)bios, GB_BIOS_SIZE);

    bool ok = biosfile.is_open();
    
    biosfile.close();

    return ok;
    
}

int gb_mem_state::loadROM(string filename) {
    bool ok = cartridge.loadCartridge(filename);
    
    
    
    return !ok;
    
}


void gb_mem_state::reset() {
    inBios = 1;

    ramBankNo = 0;
    romBankNo = 1;

    ioUpdate = 0;

    for (int i = 0; i < 0x2000; i++)
        vram[i] = 0;
    for (int i = 0; i < 0x2000; i++)
        iram[i] = 0;
    for (int i = 0; i < 0xA0; i++)
        oam[i] = 0;
    for (int i = 0; i < 0x4C; i++)
        io_ports[i] = 0;
    for (int i = 0; i < 0x7F; i++)
        hram[i] = 0;

    ier = 0;
}
