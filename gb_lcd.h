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

static const int SCREEN_WIDTH = 160;
static const int SCREEN_HEIGHT = 144;

static const int PIXEL_SIZE = 4;

using namespace std;
class PixelMap : public sf::Drawable, public sf::Transformable
{
public:
    
    bool create(sf::Vector2u tileSize,  unsigned int width, unsigned int height)
    {
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);
        
        w = width;
        h = height;
        
        for (int i = 0; i < 4; i++)
            colo[i] = sf::Color(255-83*i, 255-83*i, 255-83*i, 255);
        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];
                
                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                
                // define its 4 texture coordinates
                quad[0].color = colo[0];
                quad[1].color = colo[0];
                quad[2].color = colo[0];
                quad[3].color = colo[0];
            }
        
        return true;
    }
    
    void setPixel(unsigned int x, unsigned int y, int color) {
         sf::Vertex* quad = &m_vertices[(x + y * w) * 4];
        
        
        quad[0].color = colo[color];
        quad[1].color = colo[color];
        quad[2].color = colo[color];
        quad[3].color = colo[color];
    
    }
    
    sf::VertexArray m_vertices;
private:
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
        
        // apply the tileset texture
        states.texture = &m_tileset;
        
        // you may also override states.shader or states.blendMode if you want
        
        // draw the vertex array
        target.draw(m_vertices, states);
    }
    
    int w, h;
    
    sf::Color colo[4];
    sf::Texture m_tileset;
};


class gb_lcd {
    
    
    

    //sf::Image img;
    //sf::Uint8 pixels[4 * (SCREEN_WIDTH*PIXEL_SIZE) * (SCREEN_HEIGHT*PIXEL_SIZE)];
    //sf::Texture texture;
    //sf::Sprite sprite;
    
    PixelMap pxMap;
    
    
    sf::Color colo[4];
public:
    
    
    //Window & Screen
    sf::RenderWindow* window;
    
    gb_lcd();
    ~gb_lcd();
    
    void drawPixel(int x, int y, int col);
    
    void show();
    
    void clear();
    
    
};

#endif /* defined(__Game_Boy_Emulator__gp_lcd__) */
