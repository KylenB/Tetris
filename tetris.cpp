// file: tetris.cpp
// class: COP 3003 Programming II
// by: Kylen Bregula 
// desc: Main Application for a tetris game 
// --------------------------------------------------

#include <SFML/Graphics.hpp>
#include "Tetromino.h"
#include "Tetris.h"
#include "TetrisBoard.h"
#include <iostream>

// function declarations (proto-types)
// --------------------------------------------------
bool processEvents(sf::RenderWindow & window, KeyPressedState input[]);
bool update(KeyPressedState input[], TetrisBoard & board);
void render(sf::RenderWindow & window, TetrisBoard & board);

// function definitions
// --------------------------------------------------
int main(){
    // create the game window width x height with title
    sf::RenderWindow window {sf::VideoMode{WIN_WIDTH,WIN_HEIGHT},"Tetris"};

    // game board grid for the Tetris game
    TetrisBoard gameboard;

    // Keyboard state handling
    KeyPressedState keyStates[sf::Keyboard::KeyCount] = {0};

    // update frame timing
    // --------------------------------------------------
    sf::Clock frameTimer; // frame rate timer
    int lag{0}; // cumulative lag time each frame

    bool gameover = false;
    // main game loop
    // --------------------------------------------------
    while(!gameover){
        lag += frameTimer.restart().asMilliseconds();

        gameover = processEvents(window, keyStates);

        // wait until we get to a frame boundary tp update
        while (lag >= FRAME_RATE_MS){

            gameover = update(keyStates, gameboard);

            lag -= FRAME_RATE_MS; // reduce lag by 1 frame
        }

        render(window, gameboard);

    } // end main game loop

    // clean up the main window
    window.close();

    return 0; //return success on exit
} // end main

/**
 * process window and keyboard events 
 * @param window = reference to the main window
 * @return true = window closing
*/

/**
 * process window and keyboard events 
 * @param window - reference to the main window
 * @param input - prior and current state of each keyboard key
 * @return true = window closing
*/
bool processEvents(sf::RenderWindow & window, KeyPressedState input[]){
    bool closing = false;

    sf::Event event;

    while (window.pollEvent(event)){

        // close requested event
        if (event.type == sf::Event::Closed){
            closing = true; // return closing true
        } else if (event.type == sf::Event::KeyReleased){
            // if key's prior state is off
            if (!input[event.key.code].prior){
                // set current and prior state on
                // will be turned off in update
                input[event.key.code].current = true; // detected this loop
                input[event.key.code].prior = true; // ignore next loop
            }
        }
    }

    return closing;
} // end process events

/**
 * update state of game objects each frame
 * @param input - keyboard input
 * @param board - Tetris game board
 * @return true if game should end
*/
bool update(KeyPressedState input[], TetrisBoard & board){
    bool endgame = false;

    // update objects the gameboard
    endgame = board.update(input);

    return endgame;
} // end update

/**
process key input for update frames
@param input - current ke states
@param key - check state of particular key
@return true if key is in a pressed state
*/
bool isKeyPressed(KeyPressedState input[], sf::Keyboard::Key key){
    bool pressed = false;
    // check if key was pressed to rotate the shape
    if (input[key].prior){
        // this is a current key press for this frame
        if (input[key].current){
            pressed = true;
            // turn off the current flag to indicate we processedit
            input[key].current = false;
        }
        else {  // we were skipping a frame
            //turn off the prior flag, so it can be picked again
            input[key].prior = false;
        }
    }
    return pressed;
}

/**
 * @param window - reference to the main window
 * @param board - main gameboard
*/
void render(sf::RenderWindow & window, TetrisBoard & board){

    window.clear(BACKGROUND_COLOR);

    board.render(window);

    window.display();

} // end render