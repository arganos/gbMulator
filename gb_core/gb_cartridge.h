//
//  gb_cartridge.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#ifndef __Game_Boy_Emulator__gb_cartridge__
#define __Game_Boy_Emulator__gb_cartridge__

#define GB_ROM_BANK_SIZE 0x4000
#define GB_RAM_BANK_SIZE 0x2000

#include <string>
#include "gb_util.h"

using namespace std;

//ROM Bank structure

class gb_rom_bank{

public:
    
    unsigned char data[GB_ROM_BANK_SIZE];

};

//RAM Bank structure

class gb_ram_bank {

public:

    unsigned char data[GB_RAM_BANK_SIZE];

};


//Cartridge Structure. Contains Header information and ROM/RAM Bank data

class gb_cartridge {

public:
    
    //MBC Version (0/1/3/5)
    enum gb_cartridge_type {
        ROM_ONLY = 0,
        
        ROM_MBC1 = 1,
        ROM_MBC1_RAM = 2,
        ROM_MBC1_RAM_BATT = 3,
        
        ROM_MBC2 = 5,
        ROM_MBC2_BATT = 6,
        
        ROM_RAM = 8,
        ROM_RAM_BATT = 9,
        
        ROM_MBC3_RAM_BATTERY = 13
    };
    
    //Bank Data
    gb_rom_bank rom_bank[128];
    gb_ram_bank ram_bank[16];
    
    //ROM Header information
    string title;
    
    bool cgb;
    bool sgb;
    
    unsigned short license_code;
    
    gb_cartridge_type type;
    
    unsigned int rom_size;
    unsigned int ram_size;
    
    bool loadCartridge(string filename);
};


#endif /* defined(__Game_Boy_Emulator__gb_cartridge__) */
