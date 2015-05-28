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

gb_lcd::gb_lcd() {

   
   
    pxMap.create(sf::Vector2u(PIXEL_SIZE, PIXEL_SIZE), SCREEN_WIDTH, SCREEN_HEIGHT);
    // img.create(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE, sf::Color::Black);
   /* for (int x = 0; x < SCREEN_WIDTH; x++)
        for (int y = 0; y < SCREEN_HEIGHT; y++)
            for (int a = 0; a < PIXEL_SIZE; a++)
                for (int b = 0; b < PIXEL_SIZE; b++)
                    pixels[4 *( (PIXEL_SIZE * x + a) + (PIXEL_SIZE * y + b)) + 3] = 255;*/
    
    for (int i = 0; i < 4; i++)
        colo[i] = sf::Color(255-83*i, 255-83*i, 255-83*i, 255);
}


gb_lcd::~gb_lcd() {
    window->close();

}


void gb_lcd::drawPixel(int x, int y, int col) {
    
   /* rectangle.setFillColor(sf::Color(255-83*col, 255-83*col, 255-83*col, 255));
    rectangle.setOutlineColor(sf::Color(255-83*col,255- 83*col, 255-83*col, 255));
    
    rectangle.setPosition(x*PIXEL_SIZE, y*PIXEL_SIZE);
    
    window.draw(rectangle);*/
    
    
    if (x < 0 || x >= SCREEN_WIDTH)
        return;
    if (y < 0 || y >= SCREEN_HEIGHT)
        return;
    
    pxMap.setPixel(x, y, col);
    
   /* for (int a = 0; a < PIXEL_SIZE; a++)
        for (int b = 0; b < PIXEL_SIZE; b++)
            img.setPixel(PIXEL_SIZE*x+a ,PIXEL_SIZE*y +b, colo[col]);*/
    
    
}

void gb_lcd::show() {
    
    window->clear();
    window->draw(pxMap.m_vertices);
    window->display();
}

void gb_lcd::clear() {
}