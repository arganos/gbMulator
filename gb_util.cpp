//
//  gb_util.cpp
//  Game Boy Emulator
//
//  Created by Gilles Englebert on 05/05/15.
//  Copyright 2015. All rights reserved.
//
//

#include "gb_util.h"

#include <iostream>

void error(string message, unsigned short pc) {
    cout << hex<<pc<<":  Error: "<<message << endl;
    
}