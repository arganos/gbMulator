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

   // pxMap = new PixelMap();
    pxMap.create(sf::Vector2u(PIXEL_SIZE, PIXEL_SIZE), SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int i = 0; i < 4; i++)
        colo[i] = sf::Color(255-83*i, 255-83*i, 255-83*i, 255);
}


void gb_lcd_state::drawPixel(int x, int y, int col) {
    
    
    if (x < 0 || x >= SCREEN_WIDTH)
        return;
    if (y < 0 || y >= SCREEN_HEIGHT)
        return;
    
    pxMap.setPixel(x, y, col);

}


