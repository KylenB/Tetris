// file: Tetromino.cpp
// class: COP 3003 Programming II
// by: Kylen Bregula
// desc: Implementation of an abstract Tetromino Shape
// --------------------------------------------------
#include "Tetromino.h"
#include "Matrix.h"

Tetromino::Tetromino(){
    _shapeType = SHAPE_NONE;
    // all other properties are class objects with their own default constructors
}

Tetromino::Tetromino(int rows, int columns)
    : Matrix{rows, columns} {
    _shapeType = SHAPE_NONE;
    // all other properties are class objects with their own default constructors
}


Tetromino::Tetromino(int rows, int columns,int *blocks)
    : Matrix{rows, columns, blocks} {
    _shapeType = SHAPE_NONE;
    // all other properties are class objects with their own default constructors
}

/**
 * Move a Tetromino one block left, right or down. no collision
 * checking is done here, so this should only be called if it is
 * possible to move in the direction given.
 * @param direction - move shape in given direction
 * @param blocks = the number of blocks to move
 */
void Tetromino::move(Movement direction, int blocks) {
    switch(direction) {
        case MoveLeft:
            _position.x -= (_size.x / _columns) * blocks;
            break;
        case MoveRight:
            _position.x += (_size.x / _columns) * blocks;
            break;
        case MoveDown:
            _position.y += (_size.y / _rows) * blocks;
    }
}   // end move

/**
 * draw the Tetromino by going through the matrix
 * and creating a RectangleShape, Sizing, Positioning, and
 * filling its color, for any cells with a 1
 * @param window - target window to draw on
*/
void Tetromino::draw(sf::RenderWindow &window){
    sf::Vector2f blockSize {_size.x / _columns, _size.y / _rows};

    // start block position at top left of Tetromino
    sf::Vector2f blockPos = _position;
    for (int row = 0; row < _rows; ++row){
        blockPos.x = _position.x;

        for (int col = 0; col < _columns; ++col) {
            // see if cell in matrix has a 1 in it
            if(hasBlock(row, col)) {
                //  create temp block, position, and color it
                sf::RectangleShape block(blockSize);
                block.setPosition(blockPos);
                block.setFillColor(_fillColor);
                window.draw(block);
            }
            blockPos.x += blockSize.x;
        } // for each column
        blockPos.y += blockSize.y;
    } // for each row
} //    end draw

/**
 * Rotate this shape counter-clockwise 90 degress
*/
void Tetromino::rotate(){
    anticlockwise();
} // end rotate

/**
 * Print and Check what is the positioning in a text based promt
 * @return
 */
std::string Tetromino::toString(){
    std::stringstream ssShape;

    ssShape << "Tetromino:\n";
    ssShape << + "Size: (" << int(_size.x) << "," << int(_size.y) << ")\n";
    ssShape << + "Position: (" << int(_position.x) << "," << int(_position.y) << ")\n";
    ssShape << + "Color : (" << int(_fillColor.r) << "," << int(_fillColor.g) << "," << int(_fillColor.b) << ")\n";
    ssShape << "Matrix: \n";
    ssShape << Matrix::toString() << std::endl;

    return ssShape.str();
}



