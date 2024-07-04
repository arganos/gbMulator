//
//  gb_lcd.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#ifndef __Game_Boy_Emulator__gp_lcd__
#define __Game_Boy_Emulator__gp_lcd__

#include <SFML/Graphics.hpp>

#include <stdio.h>
#include "gb_util.h"
#include <iostream>
#include <vector>

static const int SCREEN_WIDTH = 160;
static const int SCREEN_HEIGHT = 144;

static const int PIXEL_SIZE = 4;

using namespace std;

class gb_lcd_state
{

public:
    //Window & Screen
    gb_lcd_state();
    void drawPixel(int x, int y, int col) ;

    vector<vector<int> > screen;
};

#endif /* defined(__Game_Boy_Emulator__gp_lcd__) */
