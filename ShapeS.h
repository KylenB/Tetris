// file: ShapeS.cpp
// class: COP 3003 Programming II
// by: Kylen Bregula
// desc: Header file for one out of 7 shapes
// --------------------------------------------------

#ifndef TETRIS3_SHAPES_H
#define TETRIS3_SHAPES_H

#include "Tetris.h"
#include "Tetromino.h"
#include <SFML/Graphics.hpp>

class ShapeS : public Tetromino {
private:

public:
    explicit ShapeS(sf::Vector2f position = {0.f, 0,})
        : Tetromino(3, 3) { // init rows and columns to 3x3
        // set the shape type
        _shapeType = SHAPE_S;

        // define the matrix values
        int blocks [9] = {0,0,0,
                         0,1,1,
                         1,1,0};

        // set the internal matrix
        setMatrix(blocks);

        // set the screen size, position, and color
        _size = sf::Vector2f(_rows * BLOCK_SIZE, _columns * BLOCK_SIZE);
        _position = position;
        _fillColor = sf::Color(GREEN);
    } // default constructor

}; // class shape

#endif //TETRIS3_SHAPES_H
