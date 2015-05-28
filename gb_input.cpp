//
//  gb_input.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#include "gb_input.h"
#include <iostream>


gb_input::gb_input() {
    pressedA = 0;
    pressedB = 0;
    pressedStart = 0;
    pressedSelect = 0;
    pressedUp = 0;
    pressedDown = 0;
    pressedRight = 0;
    pressedLeft = 0;
}

int gb_input::update() {
    int ret1 = 1*pressedA+2*pressedB+4*pressedDown+8*pressedLeft+16*pressedRight+32*pressedSelect+64*pressedStart+128*pressedUp;
    
   
    
    pressedA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    pressedB = sf::Keyboard::isKeyPressed(sf::Keyboard::B);
    pressedStart = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);
    pressedSelect = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    
    pressedUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    pressedDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    pressedLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    pressedRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    int ret2 =1*pressedA+2*pressedB+4*pressedDown+8*pressedLeft+16*pressedRight+32*pressedSelect+64*pressedStart+128*pressedUp;

    return ret1-ret2;
}