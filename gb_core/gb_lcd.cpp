//
//  gb_lcd.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#include "gb_lcd.h"
#include <iostream>

gb_lcd_state::gb_lcd_state() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        screen.push_back(vector<int>());
        for (int y = 0; y < SCREEN_HEIGHT; y++)
            screen[x].push_back(0);
    }
}


void gb_lcd_state::drawPixel(int x, int y, int col) {
    
    
    if (x < 0 || x >= SCREEN_WIDTH)
        return;
    if (y < 0 || y >= SCREEN_HEIGHT)
        return;
    
    screen[x][y] = col;

}


