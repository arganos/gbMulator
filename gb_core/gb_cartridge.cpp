//
//  gb_cartridge.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#include "gb_cartridge.h"

#include <fstream>
#include <iostream>

//Loads the ROM data from a file. The data is supposed to follow the standard ROM Header format for Game Boy games.

bool gb_cartridge::loadCartridge(string filename) {
    ifstream cartr;
    
    cartr.open(filename.c_str(), ifstream::binary | ifstream::in);
    
    if (!cartr.is_open())
        return 0;
    
    cartr.read((char*)rom_bank[0].data, GB_ROM_BANK_SIZE);
    
    //Extract Game Title
    for (int i = 0x134; i <= 0x142; i++)
        if (rom_bank[0].data[i] != 0) {
            title +=rom_bank[0].data[i];
        }
    //Extract CGB Flag
    cgb = (rom_bank[0].data[0x143] == 0x80);
    
    //Extract SGB Flag
    cgb = (rom_bank[0].data[0x146] == 0x03);
    
    //Extract License code
    if (rom_bank[0].data[0x14B] == 0x33)
        license_code = 16*(rom_bank[0].data[0x144]-'0')+rom_bank[0].data[0x145]-'0';
    else
        license_code = rom_bank[0].data[0x14B];
        
    //Extract Cartridge Type
    if (rom_bank[0].data[0x147] == 0)
        type =  ROM_ONLY;
    if (rom_bank[0].data[0x147] == 1)
        type =  ROM_MBC1;
    if (rom_bank[0].data[0x147] == 3)
        type =  ROM_MBC1_RAM_BATT;
    if (rom_bank[0].data[0x147] == 0x13)
        type =  ROM_MBC3_RAM_BATTERY ;
    
    //Extract ROM size
    if (rom_bank[0].data[0x148] < 7)
        rom_size = 2*GB_ROM_BANK_SIZE*(1<<rom_bank[0].data[0x148]);
    else if (rom_bank[0].data[0x148] == 0x52)
        rom_size = 72*GB_ROM_BANK_SIZE;
    else if (rom_bank[0].data[0x148] == 0x53)
        rom_size = 80*GB_ROM_BANK_SIZE;
    else if (rom_bank[0].data[0x148] == 0x54)
        rom_size = 96*GB_ROM_BANK_SIZE;
    else {
        rom_size = 0;
        error("Unknown ROM size", 0);
    }
    
    //Extract RAM size
    switch (rom_bank[0].data[0x149]) {
        case 0:
            ram_size = 0;
            break;
        case 1:
            ram_size = GB_RAM_BANK_SIZE/4;
            break;
        case 2:
            ram_size = GB_RAM_BANK_SIZE;
            break;
        case 3:
            ram_size = GB_RAM_BANK_SIZE*4;
            break;
        case 4:
            ram_size = GB_RAM_BANK_SIZE*16;
            break;
    }
    
    //Read data for the remaining ROM banks
    
    for (unsigned int i = 1; i < rom_size/GB_ROM_BANK_SIZE; i++)
        cartr.read((char*)rom_bank[i].data, GB_ROM_BANK_SIZE);
    
    bool ok = !cartr;
    
    cartr.close();
    
    return ok;
    
}
