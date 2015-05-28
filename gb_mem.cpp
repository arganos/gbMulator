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
    inBios = 1;
    
    ramBankNo = 0;
    romBankNo = 1;
    
    
    
    ioUpdate = 0;
    
    for (int i = 0;  i < 0x100; i++)
    histo[i] = 0;
    
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
    
    
    bool ok = !biosfile;
    
    biosfile.close();
    
    return ok;
    
}

int gb_mem_state::loadROM(string filename) {
    bool ok = cartridge.loadCartridge(filename);
    
    
    
    return ok;
    
}

/*
 [0x00001D57] 0xF0 0xBA      LDH A,[$BA]
 [0x00001D59] 0xA7           AND A
 [0x00001D5A] 0xC8           RET Z
 [0x00001D5B] 0xF8 0x00      LD HL,SP+$00
 [0x00001D5D] 0x7C           LD A,H
 [0x00001D5E] 0xE0 0xBF      LDH [$BF],A
 [0x00001D60] 0x7D           LD A,L
 [0x00001D61] 0xE0 0xC0      LDH [$C0],A
 [0x00001D63] 0xF0 0xBB      LDH A,[$BB]
 [0x00001D65] 0xA7           AND A
 [0x00001D66] 0x28 0x14      JR Z,$14 ; 0x1D7C
 [0x00001D68] 0x3D           DEC A
 [0x00001D69] 0x28 0x1F      JR Z,$1F ; 0x1D8A
 [0x00001D6B] 0x21 0x90 0xC4 LD HL,$C490
 [0x00001D6E] 0xF9           LD SP,HL
 [0x00001D6F] 0xF0 0xBD      LDH A,[$BD]
 [0x00001D71] 0x67           LD H,A
 [0x00001D72] 0xF0 0xBC      LDH A,[$BC]
 [0x00001D74] 0x6F           LD L,A
 [0x00001D75] 0x11 0x80 0x01 LD DE,$0180
 [0x00001D78] 0x19           ADD HL,DE
 [0x00001D79] 0xAF           XOR A
 [0x00001D7A] 0x18 0x1E      JR $1E ; 0x1D9A
 [0x00001D7C] 0x21 0xA0 0xC3 LD HL,$C3A0
 [0x00001D7F] 0xF9           LD SP,HL
 [0x00001D80] 0xF0 0xBD      LDH A,[$BD]
 [0x00001D82] 0x67           LD H,A
 [0x00001D83] 0xF0 0xBC      LDH A,[$BC]
 [0x00001D85] 0x6F           LD L,A
 [0x00001D86] 0x3E 0x01      LD A,$01
 [0x00001D88] 0x18 0x10      JR $10 ; 0x1D9A
 [0x00001D8A] 0x21 0x18 0xC4 LD HL,$C418
 [0x00001D8D] 0xF9           LD SP,HL
 [0x00001D8E] 0xF0 0xBD      LDH A,[$BD]
 [0x00001D90] 0x67           LD H,A
 [0x00001D91] 0xF0 0xBC      LDH A,[$BC]
 [0x00001D93] 0x6F           LD L,A
 [0x00001D94] 0x11 0xC0 0x00 LD DE,$00C0
 [0x00001D97] 0x19           ADD HL,DE
 [0x00001D98] 0x3E 0x02      LD A,$02
 [0x00001D9A] 0xE0 0xBB      LDH [$BB],A
 [0x00001D9C] 0x06 0x06      LD B,$06
 [0x00001D9E] 0xD1           POP DE
 [0x00001D9F] 0x73           LD [HL],E
 [0x00001DA0] 0x2C           INC L
 [0x00001DA1] 0x72           LD [HL],D
 [0x00001DA2] 0x2C           INC L
 [0x00001DA3] 0xD1           POP DE
 [0x00001DA4] 0x73           LD [HL],E
 [0x00001DA5] 0x2C           INC L
 [0x00001DA6] 0x72           LD [HL],D
 [0x00001DA7] 0x2C           INC L
 [0x00001DA8] 0xD1           POP DE
 [0x00001DA9] 0x73           LD [HL],E
 [0x00001DAA] 0x2C           INC L
 [0x00001DAB] 0x72           LD [HL],D
 [0x00001DAC] 0x2C           INC L
 [0x00001DAD] 0xD1           POP DE
 [0x00001DAE] 0x73           LD [HL],E
 [0x00001DAF] 0x2C           INC L
 [0x00001DB0] 0x72           LD [HL],D
 [0x00001DB1] 0x2C           INC L
 [0x00001DB2] 0xD1           POP DE
 [0x00001DB3] 0x73           LD [HL],E
 [0x00001DB4] 0x2C           INC L
 [0x00001DB5] 0x72           LD [HL],D
 [0x00001DB6] 0x2C           INC L
 [0x00001DB7] 0xD1           POP DE
 [0x00001DB8] 0x73           LD [HL],E
 [0x00001DB9] 0x2C           INC L
 [0x00001DBA] 0x72           LD [HL],D
 [0x00001DBB] 0x2C           INC L
 [0x00001DBC] 0xD1           POP DE
 [0x00001DBD] 0x73           LD [HL],E
 [0x00001DBE] 0x2C           INC L
 [0x00001DBF] 0x72           LD [HL],D
 [0x00001DC0] 0x2C           INC L
 [0x00001DC1] 0xD1           POP DE
 [0x00001DC2] 0x73           LD [HL],E
 [0x00001DC3] 0x2C           INC L
 [0x00001DC4] 0x72           LD [HL],D
 [0x00001DC5] 0x2C           INC L
 [0x00001DC6] 0xD1           POP DE
 [0x00001DC7] 0x73           LD [HL],E
 [0x00001DC8] 0x2C           INC L
 [0x00001DC9] 0x72           LD [HL],D
 [0x00001DCA] 0x2C           INC L
 [0x00001DCB] 0xD1           POP DE
 [0x00001DCC] 0x73           LD [HL],E
 [0x00001DCD] 0x2C           INC L
 [0x00001DCE] 0x72           LD [HL],D
 [0x00001DCF] 0x3E 0x0D      LD A,$0D
 [0x00001DD1] 0x85           ADD A,L
 [0x00001DD2] 0x6F           LD L,A
 [0x00001DD3] 0x30 0x01      JR NC,$01 ; 0x1DD6
 [0x00001DD5] 0x24           INC H
 [0x00001DD6] 0x05           DEC B
 [0x00001DD7] 0x20 0xC5      JR NZ,$C5 ; 0x1D9E
 [0x00001DD9] 0xF0 0xBF      LDH A,[$BF]
 [0x00001DDB] 0x67           LD H,A
 [0x00001DDC] 0xF0 0xC0      LDH A,[$C0]
 [0x00001DDE] 0x6F           LD L,A
 [0x00001DDF] 0xF9           LD SP,HL
 [0x00001DE0] 0xC9           RET
 
 
 
 
 [0x00001DE1] 0xF0 0xC1      LDH A,[$C1]
 [0x00001DE3] 0xA7           AND A
 [0x00001DE4] 0xC8           RET Z
 [0x00001DE5] 0xF8 0x00      LD HL,SP+$00
 [0x00001DE7] 0x7C           LD A,H
 [0x00001DE8] 0xE0 0xBF      LDH [$BF],A
 [0x00001DEA] 0x7D           LD A,L
 [0x00001DEB] 0xE0 0xC0      LDH [$C0],A
 [0x00001DED] 0xF0 0xC1      LDH A,[$C1]
 [0x00001DEF] 0x6F           LD L,A
 [0x00001DF0] 0xF0 0xC2      LDH A,[$C2]
 [0x00001DF2] 0x67           LD H,A
 [0x00001DF3] 0xF9           LD SP,HL
 [0x00001DF4] 0xF0 0xC3      LDH A,[$C3]
 [0x00001DF6] 0x6F           LD L,A
 [0x00001DF7] 0xF0 0xC4      LDH A,[$C4]
 [0x00001DF9] 0x67           LD H,A
 [0x00001DFA] 0xF0 0xC5      LDH A,[$C5]
 [0x00001DFC] 0x47           LD B,A
 [0x00001DFD] 0xAF           XOR A
 [0x00001DFE] 0xE0 0xC1      LDH [$C1],A
 [0x00001E00] 0x18 0x9C      JR $9C ; 0x1D9E
 [0x00001E02] 0xF0 0xCB      LDH A,[$CB]
 [0x00001E04] 0xA7           AND A
 [0x00001E05] 0xC8           RET Z
 [0x00001E06] 0xF8 0x00      LD HL,SP+$00
 [0x00001E08] 0x7C           LD A,H
 [0x00001E09] 0xE0 0xBF      LDH [$BF],A
 [0x00001E0B] 0x7D           LD A,L
 [0x00001E0C] 0xE0 0xC0      LDH [$C0],A
 [0x00001E0E] 0xF0 0xCC      LDH A,[$CC]
 [0x00001E10] 0x6F           LD L,A
 [0x00001E11] 0xF0 0xCD      LDH A,[$CD]
 [0x00001E13] 0x67           LD H,A
 [0x00001E14] 0xF9           LD SP,HL
 [0x00001E15] 0xF0 0xCE      LDH A,[$CE]
 [0x00001E17] 0x6F           LD L,A
 [0x00001E18] 0xF0 0xCF      LDH A,[$CF]
 [0x00001E1A] 0x67           LD H,A
 [0x00001E1B] 0xF0 0xCB      LDH A,[$CB]
 [0x00001E1D] 0x47           LD B,A
 [0x00001E1E] 0xAF           XOR A
 [0x00001E1F] 0xE0 0xCB      LDH [$CB],A
 [0x00001E21] 0xD1           POP DE
 [0x00001E22] 0x73           LD [HL],E
 [0x00001E23] 0x2C           INC L
 [0x00001E24] 0x73           LD [HL],E
 [0x00001E25] 0x2C           INC L
 [0x00001E26] 0x72           LD [HL],D
 [0x00001E27] 0x2C           INC L
 [0x00001E28] 0x72           LD [HL],D
 [0x00001E29] 0x2C           INC L
 [0x00001E2A] 0xD1           POP DE
 [0x00001E2B] 0x73           LD [HL],E
 [0x00001E2C] 0x2C           INC L
 [0x00001E2D] 0x73           LD [HL],E
 [0x00001E2E] 0x2C           INC L
 [0x00001E2F] 0x72           LD [HL],D
 [0x00001E30] 0x2C           INC L
 [0x00001E31] 0x72           LD [HL],D
 [0x00001E32] 0x2C           INC L
 [0x00001E33] 0xD1           POP DE
 [0x00001E34] 0x73           LD [HL],E
 [0x00001E35] 0x2C           INC L
 [0x00001E36] 0x73           LD [HL],E
 [0x00001E37] 0x2C           INC L
 [0x00001E38] 0x72           LD [HL],D
 [0x00001E39] 0x2C           INC L
 [0x00001E3A] 0x72           LD [HL],D
 [0x00001E3B] 0x2C           INC L
 [0x00001E3C] 0xD1           POP DE
 [0x00001E3D] 0x73           LD [HL],E
 [0x00001E3E] 0x2C           INC L
 [0x00001E3F] 0x73           LD [HL],E
 [0x00001E40] 0x2C           INC L
 [0x00001E41] 0x72           LD [HL],D
 [0x00001E42] 0x2C           INC L
 [0x00001E43] 0x72           LD [HL],D
 [0x00001E44] 0x23           INC HL
 [0x00001E45] 0x05           DEC B
 [0x00001E46] 0x20 0xD9      JR NZ,$D9 ; 0x1E21
 [0x00001E48] 0x7D           LD A,L
 [0x00001E49] 0xE0 0xCE      LDH [$CE],A
 [0x00001E4B] 0x7C           LD A,H
 [0x00001E4C] 0xE0 0xCF      LDH [$CF],A
 [0x00001E4E] 0xF8 0x00      LD HL,SP+$00
 [0x00001E50] 0x7D           LD A,L
 [0x00001E51] 0xE0 0xCC      LDH [$CC],A
 [0x00001E53] 0x7C           LD A,H
 [0x00001E54] 0xE0 0xCD      LDH [$CD],A
 [0x00001E56] 0xF0 0xBF      LDH A,[$BF]
 [0x00001E58] 0x67           LD H,A
 [0x00001E59] 0xF0 0xC0      LDH A,[$C0]
 [0x00001E5B] 0x6F           LD L,A
 [0x00001E5C] 0xF9           LD SP,HL
 [0x00001E5D] 0xC9           RET
 [0x00001E5E] 0xF0 0xC6      LDH A,[$C6]
 [0x00001E60] 0xA7           AND A
 [0x00001E61] 0xC8           RET Z
 [0x00001E62] 0xF8 0x00      LD HL,SP+$00
 [0x00001E64] 0x7C           LD A,H
 [0x00001E65] 0xE0 0xBF      LDH [$BF],A
 [0x00001E67] 0x7D           LD A,L
 [0x00001E68] 0xE0 0xC0      LDH [$C0],A
 [0x00001E6A] 0xF0 0xC7      LDH A,[$C7]
 [0x00001E6C] 0x6F           LD L,A
 [0x00001E6D] 0xF0 0xC8      LDH A,[$C8]
 [0x00001E6F] 0x67           LD H,A
 [0x00001E70] 0xF9           LD SP,HL
 [0x00001E71] 0xF0 0xC9      LDH A,[$C9]
 [0x00001E73] 0x6F           LD L,A
 [0x00001E74] 0xF0 0xCA      LDH A,[$CA]
 [0x00001E76] 0x67           LD H,A
 [0x00001E77] 0xF0 0xC6      LDH A,[$C6]
 [0x00001E79] 0x47           LD B,A
 [0x00001E7A] 0xAF           XOR A
 [0x00001E7B] 0xE0 0xC6      LDH [$C6],A
 [0x00001E7D] 0xD1           POP DE
 [0x00001E7E] 0x73           LD [HL],E
 [0x00001E7F] 0x2C           INC L
 [0x00001E80] 0x72           LD [HL],D
 [0x00001E81] 0x2C           INC L
 [0x00001E82] 0xD1           POP DE
 [0x00001E83] 0x73           LD [HL],E
 [0x00001E84] 0x2C           INC L
 [0x00001E85] 0x72           LD [HL],D
 [0x00001E86] 0x2C           INC L
 [0x00001E87] 0xD1           POP DE
 [0x00001E88] 0x73           LD [HL],E
 [0x00001E89] 0x2C           INC L
 [0x00001E8A] 0x72           LD [HL],D
 [0x00001E8B] 0x2C           INC L
 [0x00001E8C] 0xD1           POP DE
 [0x00001E8D] 0x73           LD [HL],E
 [0x00001E8E] 0x2C           INC L
 [0x00001E8F] 0x72           LD [HL],D
 [0x00001E90] 0x2C           INC L
 [0x00001E91] 0xD1           POP DE
 [0x00001E92] 0x73           LD [HL],E
 [0x00001E93] 0x2C           INC L
 [0x00001E94] 0x72           LD [HL],D
 [0x00001E95] 0x2C           INC L
 [0x00001E96] 0xD1           POP DE
 [0x00001E97] 0x73           LD [HL],E
 [0x00001E98] 0x2C           INC L
 [0x00001E99] 0x72           LD [HL],D
 [0x00001E9A] 0x2C           INC L
 [0x00001E9B] 0xD1           POP DE
 [0x00001E9C] 0x73           LD [HL],E
 [0x00001E9D] 0x2C           INC L
 [0x00001E9E] 0x72           LD [HL],D
 [0x00001E9F] 0x2C           INC L
 [0x00001EA0] 0xD1           POP DE
 [0x00001EA1] 0x73           LD [HL],E
 [0x00001EA2] 0x2C           INC L
 [0x00001EA3] 0x72           LD [HL],D
 [0x00001EA4] 0x23           INC HL
 [0x00001EA5] 0x05           DEC B
 [0x00001EA6] 0x20 0xD5      JR NZ,$D5 ; 0x1E7D
 [0x00001EA8] 0x7D           LD A,L
 [0x00001EA9] 0xE0 0xC9      LDH [$C9],A
 [0x00001EAB] 0x7C           LD A,H
 [0x00001EAC] 0xE0 0xCA      LDH [$CA],A
 [0x00001EAE] 0xF8 0x00      LD HL,SP+$00
 [0x00001EB0] 0x7D           LD A,L
 [0x00001EB1] 0xE0 0xC7      LDH [$C7],A
 [0x00001EB3] 0x7C           LD A,H
 [0x00001EB4] 0xE0 0xC8      LDH [$C8],A
 [0x00001EB6] 0xF0 0xBF      LDH A,[$BF]
 [0x00001EB8] 0x67           LD H,A
 [0x00001EB9] 0xF0 0xC0      LDH A,[$C0]
 [0x00001EBB] 0x6F           LD L,A
 [0x00001EBC] 0xF9           LD SP,HL
 [0x00001EBD] 0xC9           RET
 [0x00001EBE] 0xF0 0xD7      LDH A,[$D7]
 [0x00001EC0] 0xA7           AND A
 [0x00001EC1] 0xC8           RET Z
 [0x00001EC2] 0xF0 0xD8      LDH A,[$D8]
 [0x00001EC4] 0x3C           INC A
 [0x00001EC5] 0xE0 0xD8      LDH [$D8],A
 [0x00001EC7] 0xFE 0x14      CP $14
 [0x00001EC9] 0xD8           RET C
 [0x00001ECA] 0xFE 0x15      CP $15
 [0x00001ECC] 0x28 0x28      JR Z,$28 ; 0x1EF6
 [0x00001ECE] 0x21 0x40 0x91 LD HL,$9140
 [0x00001ED1] 0x0E 0x10      LD C,$10
 [0x00001ED3] 0xFA 0x85 0xD0 LD A,[$D085]
 [0x00001ED6] 0x3C           INC A
 [0x00001ED7] 0xE6 0x07      AND $07
 [0x00001ED9] 0xEA 0x85 0xD0 LD [$D085],A
 [0x00001EDC] 0xE6 0x04      AND $04
 [0x00001EDE] 0x20 0x08      JR NZ,$08 ; 0x1EE8
 [0x00001EE0] 0x7E           LD A,[HL]
 [0x00001EE1] 0x0F           RRCA
 [0x00001EE2] 0x22           LD [HLI],A
 [0x00001EE3] 0x0D           DEC C
 [0x00001EE4] 0x20 0xFA      JR NZ,$FA ; 0x1EE0
 [0x00001EE6] 0x18 0x06      JR $06 ; 0x1EEE
 [0x00001EE8] 0x7E           LD A,[HL]
 [0x00001EE9] 0x07           RLCA
 [0x00001EEA] 0x22           LD [HLI],A
 [0x00001EEB] 0x0D           DEC C
 [0x00001EEC] 0x20 0xFA      JR NZ,$FA ; 0x1EE8
 [0x00001EEE] 0xF0 0xD7      LDH A,[$D7]
 [0x00001EF0] 0x0F           RRCA
 [0x00001EF1] 0xD0           RET NC
 [0x00001EF2] 0xAF           XOR A
 [0x00001EF3] 0xE0 0xD8      LDH [$D8],A
 [0x00001EF5] 0xC9           RET

 
 
 
 
 PUSH AF
 [0x00002025] 0xC5           PUSH BC
 [0x00002026] 0xD5           PUSH DE
 [0x00002027] 0xE5           PUSH HL
 [0x00002028] 0xF0 0xB8      LDH A,[$B8]
 [0x0000202A] 0xEA 0x22 0xD1 LD [$D122],A
 [0x0000202D] 0xF0 0xAE      LDH A,[$AE]
 [0x0000202F] 0xE0 0x43      LDH [$43],A
 [0x00002031] 0xF0 0xAF      LDH A,[$AF]
 [0x00002033] 0xE0 0x42      LDH [$42],A
 [0x00002035] 0xFA 0xA0 0xD0 LD A,[$D0A0]
 [0x00002038] 0xA7           AND A
 [0x00002039] 0x20 0x04      JR NZ,$04 ; 0x203F
 [0x0000203B] 0xF0 0xB0      LDH A,[$B0]
 [0x0000203D] 0xE0 0x4A      LDH [$4A],A
 [0x0000203F] 0xCD 0x57 0x1D CALL $1D57
 [0x00002042] 0xCD 0xE1 0x1D CALL $1DE1
 [0x00002045] 0xCD 0x01 0x1D CALL $1D01
 [0x00002048] 0xCD 0x5E 0x1E CALL $1E5E
 [0x0000204B] 0xCD 0x02 0x1E CALL $1E02
 [0x0000204E] 0xCD 0xBE 0x1E CALL $1EBE
 [0x00002051] 0xCD 0x80 0xFF CALL $FF80
 */

