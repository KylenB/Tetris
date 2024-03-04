// file: tetris.h
// class: COP 3003 Programming II
// by: Kylen Bregula
// desc: Header file for the Main Application for a tetris game
// --------------------------------------------------

#ifndef TETRIS3_TETRIS_H
#define TETRIS3_TETRIS_H

#include <SFML/Graphics.hpp>

const int FPS = 30; // how many update frames per second
const int FRAME_RATE_MS = (1.f / float(FPS) * 1000.f); // per milisecond

const int FRAMES_NEW_SHAPE = 45; // default rate to show next shape
const int FRAMES_AUTO_MOVE = 30; // default rate that a shape auto moves downward

// The Number of rows and columns that make up the grid in the tetris game board
const int GAME_ROWS = 21;
const int GAME_COLUMNS = 10;

// Size of a square block (width * height) of each block in
// the tetris game board grid and individual blocks of a tetromino shape
const int BLOCK_SIZE = 30;

// Calculate required window size to accommodate game rows * columns with
// one block size of margin around the game board
const int WIN_WIDTH = GAME_COLUMNS * BLOCK_SIZE + (2 * BLOCK_SIZE); // screen width pixels
const int WIN_HEIGHT = GAME_ROWS * BLOCK_SIZE + (2 * BLOCK_SIZE); // screen height pixels

const int GRID_TOP = BLOCK_SIZE;
const int GRID_LEFT = BLOCK_SIZE;

const int START_CELL_COLUMN = 3; // column 4, array index 3
const int START_CELL_ROW = 20; // top row 21, array index 20

// Background color for the tetris board
const sf::Color BACKGROUND_COLOR = sf::Color::Black;

// Grid color for the tetris boad
const sf::Color GRID_COLOR = sf::Color(0xD3, 0xD3, 0xD3, 50);

struct KeyPressedState { // maintain state of each input key across frames
    bool prior; // state of key in prior frame: Pressed = true
    bool current; // state of key for this frame : Pressed = true
};

#endif //TETRIS3_TETRIS_H
