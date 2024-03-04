// file: TetrisBoard.cpp
// class: COP 3003 Programming II
// by: Kylen Bregula
// desc: cpp file for creating the tetris board, this will be the play area for the tetris blocks
// --------------------------------------------------

#include <iostream>
#include "TetrisBoard.h"
#include "ShapeI.h"
#include "ShapeJ.h"
#include "ShapeL.h"
#include "ShapeO.h"
#include "ShapeS.h"
#include "ShapeT.h"
#include "ShapeZ.h"

// local function declarations
bool isKeyPressed(KeyPressedState input[], sf::Keyboard::Key key);

// default constructor
TetrisBoard::TetrisBoard() {

    // initialize frame counters
    _counters = {FRAMES_NEW_SHAPE, 0,
                 FRAMES_AUTO_MOVE, 0};

    // set up the game board grid
    // --------------------------------------------------

    // position and size of a grid block
    sf::Vector2f  position{GRID_LEFT, GRID_TOP}; // screen position
    sf::Vector2f size{BLOCK_SIZE, BLOCK_SIZE}; // screen size (pixels

    // create a temp block shape to update the grid cells
    sf::RectangleShape block{size};
    block.setFillColor(BACKGROUND_COLOR);
    block.setOutlineColor(GRID_COLOR);
    block.setOutlineThickness(-1.f);

    // populate the game board grid with block shapes
    for (int row = GAME_ROWS - 1; row >= 0; --row){

        for (int col = 0; col < GAME_COLUMNS; ++col){
            block.setPosition(position); // set screen position of block

            _cells[row][col] = {false, block}; // copy shape to grid

            position.x += size.x; // move block right 1 cell

        } // columns left to right

        position.x = GRID_LEFT; // rest block left to left side of grid
        position.y += size.y; // move block down to next row of grid

    } // rows from the top to bottom {0}

    // intialize random number generation
    _randGenerator = std::mt19937(_randDevice()); // see the generator engine
    _randDistribution = std::uniform_int_distribution<>(Tetromino::ShapeType::SHAPE_NONE + 1,
                                                        Tetromino::ShapeType::SHAPE_COUNT - 1);

    // intialize current and next shape properties

    _currentShape = nullptr;
    _nextShape = nullptr;
    _currentCell = sf::Vector2i{0,0};

    // get the next shape
    // --------------------------------------------------
    nextShape();

} // default constructor


/**
 * Destructor cleans up gameboard objects
 */
TetrisBoard::~TetrisBoard() {
    delete _currentShape;
    _currentShape = nullptr;

    delete _nextShape;
    _nextShape = nullptr;
}

/**
 * draw game objects on the window
 * @param window - amin game window
 */
void TetrisBoard::render(sf::RenderWindow &window) {
    // draw the grid
    for (int row = 0; row < GAME_ROWS; ++row){
        for (int col = 0; col < GAME_COLUMNS; ++col){
            window.draw(_cells[row][col].block);
        } // each column
    } // each row

    // draw current shape if we have one
    if (_currentShape)
        _currentShape->draw(window);

} // render

/**
* update shape objects on the tetris board
 * @param input - User keypress
 * return true if game should end
*/
bool TetrisBoard::update(KeyPressedState *input){
    bool endGame;

    if (_currentShape) {
        // Left or Right
        if(isKeyPressed(input, sf::Keyboard::Key::A)) {
            if (canMoveShape(Tetromino::Movement::MoveLeft)) {
                _currentShape->move(Tetromino::Movement::MoveLeft);
                _currentCell.x -= 1;
            }
        } else if (isKeyPressed(input, sf::Keyboard::Key::D)) {
            if (canMoveShape(Tetromino::Movement::MoveRight)) {
                _currentShape->move(Tetromino::Movement::MoveRight);
                _currentCell.x += 1;
            }
        }

        bool moveDown = false; // true if we are moving the shape down

        // Down
        if (isKeyPressed(input, sf::Keyboard::Key::S)) {
            moveDown = true;
        }

        // if user requested, or it is time to auto move shape down
        if (moveDown || _counters.autoMove >= _counters.autoMoveRate){
            // see if we can move it down first
            if (canMoveShape(Tetromino::Movement::MoveDown)) {
                // if yes then move it down
                _currentShape->move(Tetromino::Movement::MoveDown);
                _currentCell.y -= 1;
            }
            // reset auto move counter
            _counters.autoMove = 0;
        } // auto or user move down
        else {
            // otherwise increment frame counter
            _counters.autoMove++;
        }

        // always check to see if the shape could move down again
        // and lock it if it is on-top of another shape of the floor
        if(!canMoveShape(Tetromino::Movement::MoveDown)) {
            lockShape();
            delete _currentShape;
            _currentShape = nullptr;
        }

        // Rotate
        if(isKeyPressed(input, sf::Keyboard::Key::Space)) {
            if(canRotateShape()){
                _currentShape->rotate();
            }
        }
        //Debug
        if(isKeyPressed(input, sf::Keyboard::Key::Q)) {
            std::cout << _currentShape->toString();
        }
    } // current shape exists
    else { // no current shape
        // count frames until time to show next shape
        if (_counters.newShape < _counters.newShapeRate){
            _counters.newShape++; // increase frame counter
        } else { // show next shape
            _counters.newShape = 0; // reset counter
            _currentShape = _nextShape; // copy next to current
            nextShape(); // get next shape
        }
    }
    return endGame;
} // update





// Private methods
// --------------------------------------------------

/**
 * Determine if the current shape can move in any direction.
 * left, right, or down, either for user input or timed move down
 * @param direction - direction shape is trying to move
 * @return true if the shape an be moved 1 block in the direction given
 */
bool TetrisBoard::canMoveShape(Tetromino::Movement direction){
    bool canMove = true;

    // make a copy of the current cell
    sf::Vector2i tempCell = _currentCell;

    // move temp cell location
    switch(direction) {
        case Tetromino::MoveLeft:
            if (_currentCell.x < -1)
                canMove = false;
            else
                tempCell.x -= 1;
            break;

        case Tetromino::MoveRight:
            if (_currentCell.x + _currentShape->getColumns() > GAME_COLUMNS + 1) // need + 1 for bar (1) shape
                canMove = false;
            else
                tempCell.x += 1;
            break;

        case Tetromino::MoveDown:
            if (_currentCell.y < 0) {
                canMove = false;
            } else {
                tempCell.y -= 1;
            }
    }   // direction

    if(canMove){
        // declare temp shape so default constructor is called to
        Tetromino tempShape; // intialize empty shape before copy

        // make a copy of the current shape to temp shape
        tempShape = *_currentShape;

        canMove =!hasCollision(tempShape, tempCell);
    }

    return canMove;
} // canMoveShape

/**
 * Determine if a shape can rotate in its current position and not
 * collide with any existing shapes. the shape may have to perform
 * a wall-kick to put it in back inside the gameboard.
 * @return true if it can rotate
 */
 bool TetrisBoard::canRotateShape(){
     bool canRotate = true;

     // make a copy of the current cell
     sf::Vector2i  tempCell = _currentCell;

     // make sure location and shape are inside
     // gameboard grid
     wallKick(tempCell);

     // declare temp shape so default constructor is called to
     Tetromino tempShape; // initialize empty shape before copy

     // make a copy of the current shape to temp shape
     tempShape = *_currentShape;

     // rotate the temp shape
     tempShape.rotate();

     canRotate = !hasCollision(tempShape, tempCell);

     if (canRotate) { // copy any wall-kicks back to current cell location
         // see if the x moved between the temp and current during a wall-kick
         int diffx = std::abs(tempCell.x - _currentCell.x);
         if (diffx > 0) {
             // assume on left wall (x is -), so move right
             auto dir = Tetromino::Movement::MoveRight;
             if (_currentCell.x > 0) // if the x was + then on the right side
                 dir = Tetromino::Movement::MoveLeft;
             _currentShape->move(dir, diffx);
             _currentCell = tempCell;
         }
     }



         _currentCell = tempCell;

     return canRotate;
 } // canRotate

 /*
  * Move the location of a shape back inside the gameboard
  * @param locaiton - of the (current) shape to adjust
  */
 void TetrisBoard::wallKick(sf::Vector2i & location) {

     if (location.x < 0){
         location.x = 0;
     }
     else if (location.x + _currentShape->getColumns() >= GAME_COLUMNS) {
         int diff = location.x + _currentShape->getColumns() - GAME_COLUMNS;
         location.x -= diff;
     }

 } // wallKick

/**
 * see if any of the blocks in a shape overaly a block in the grid.
 * or a area outside the walls
 * @param shape - reference to the shape to check
 * @param location - location of the shape
 * @return true if there is a collision
 */
bool TetrisBoard::hasCollision(Tetromino& shape, sf::Vector2i  location){
    bool collision = false;

    // used to indicate if current block in grid is filled or outside walls
    bool filled{};

    // check if temp shape at new temp cell location has any collisions with filled cells
    for (int row = 0; row < shape.getRows() && !collision; ++row){

        for (int column = 0; column < shape.getColumns() && !collision; ++column){

            // determine if grid cell is filled
            if ( location.x + column < 0 ||                 // outside left
                 location.x + column >= GAME_COLUMNS ||     // outside right
                 location.y - row < 0) {                     // below bottom
                filled = true;
            } else {
                filled = _cells[location.y - row][location.x + column].filled;
            }

            // check for collision between temp shape and grid cell
            if (filled and shape.hasBlock(row, column)) {
                collision = true;
            }

        } // each column

    } // each row

    return collision;
} // has collision






/**
* Randomly selects the nest shape to show
*/
void TetrisBoard::nextShape() {
    // reset current shape call to top center
    _currentCell = sf::Vector2i(START_CELL_COLUMN, START_CELL_ROW);

    // calculate starting screen position
    float x = (_currentCell.x + 1) * BLOCK_SIZE;
    float y = (GAME_ROWS - _currentCell.y) * BLOCK_SIZE;
    sf::Vector2f  position(x,y);

    // generate random shape type
    auto nextType = static_cast<Tetromino::ShapeType>(_randDistribution(_randGenerator));

    // generate a shape for next shape
    switch(nextType){
        case Tetromino::ShapeType::SHAPE_I:
            _nextShape = new ShapeI(position);
            break;
        case Tetromino::ShapeType::SHAPE_J:
            _nextShape = new ShapeJ(position);
            break;
        case Tetromino::ShapeType::SHAPE_L:
            _nextShape = new ShapeL(position);
            break;
        case Tetromino::ShapeType::SHAPE_O:
            _nextShape = new ShapeO(position);
            break;
        case Tetromino::ShapeType::SHAPE_S:
            _nextShape = new ShapeS(position);
            break;
        case Tetromino::ShapeType::SHAPE_T:
            _nextShape = new ShapeT(position);
            break;
        case Tetromino::ShapeType::SHAPE_Z:
            _nextShape = new ShapeZ(position);
            break;
    }

} // next shape


/**
 * lock the current shape into the gameboard
 */
void TetrisBoard::lockShape() {

    for (int row = 0; row < _currentShape->getRows(); ++row){

        for ( int column = 0; column < _currentShape->getColumns(); ++column){

            if ( _currentCell.x + column >= 0 ||                    // inside left wall
                 _currentCell.x + column <= (GAME_COLUMNS - 1) ||   // inside right wall
                 _currentCell.y - row >= 0 ) {                       // inside bottom

                if (_currentShape->hasBlock(row, column)) {
                    _cells[_currentCell.y - row][_currentCell.x + column].block.setFillColor(_currentShape->getFillColor());
                    _cells[_currentCell.y - row][_currentCell.x + column].filled = true;
                }
            }

        }   // each column
    }   // each row

} // lock shape