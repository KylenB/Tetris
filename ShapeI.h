// file: ShapeI.cpp
// class: COP 3003 Programming II
// by: Kylen Bregula
// desc: Header file for one out of 7 shapes
// --------------------------------------------------

#ifndef TETRIS3_SHAPEI_H
#define TETRIS3_SHAPEI_H

#include "Tetris.h"
#include "Tetromino.h"
#include <SFML/Graphics.hpp>

class ShapeI : public Tetromino {
private:

public:
    explicit ShapeI(sf::Vector2f position = {0.f, 0,})
        : Tetromino(4, 4) { // init rows and columns to 3x3
        // set the shape type
        _shapeType = SHAPE_I;

        // define the matrix values
        int blocks [16] = {0,1,0,0,
                           0,1,0,0,
                           0,1,0,0,
                           0,1,0,0 };


        // set the internal matrix
        setMatrix(blocks);

        // set the screen size, position, and color
        _size = sf::Vector2f(_rows * BLOCK_SIZE, _columns * BLOCK_SIZE);
        _position = position;
        _fillColor = sf::Color(LIGHT_BLUE);

    } // default constructor

}; // class shape

#endif //TETRIS3_SHAPEI_H
