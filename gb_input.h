//
//  gb_input.h
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 14/05/15.
//
//

#ifndef __Game_Boy_Emulator__gb_input__
#define __Game_Boy_Emulator__gb_input__

//#include <Keyboard.hpp>
#include <SFML/Graphics.hpp>

class gb_input {
public:
    bool pressedA,
         pressedB,
         pressedStart,
         pressedSelect,
         pressedUp,
         pressedDown,
         pressedRight,
         pressedLeft;
    
    
     gb_input();
    
    int update();
};


#endif /* defined(__Game_Boy_Emulator__gb_input__) */
