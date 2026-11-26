// =====================================================================
//  File      : heist.h
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the HEADER file. It only DECLARES things
//              (enums, structs, classes and function prototypes).
//              All function bodies are written in heist.cpp.
//              In this way the interface is kept separate from
//              the implementation (3-file structure).
// =====================================================================

#ifndef HEIST_H     // Header guard: stops this file from being
#define HEIST_H     // included more than once in the same program

#include <iostream>
#include <string>

using namespace std;


// =====================================================================
//  MODULE 1 : ENUMERATIONS
// ---------------------------------------------------------------------
//  An enum gives meaningful NAMES to a fixed set of integer values.
//  Instead of remembering that 0 means "Word 1" and 1 means "Word 2",
//  we simply write WORD_1 and WORD_2. This makes the code easy to read
//  and protects us from using wrong numbers by mistake.
// =====================================================================

// WordRow tells us on which row of the Scrabble board a letter belongs.
// Row 1 holds the 7 letters from locations, Row 2 holds the 8 letters
// from characters.
// NOTE: WordRow is written first because the Clue and Location structs
// below use it.
enum WordRow
{
    WORD_1,     // letters collected from the 7 locations
    WORD_2      // letters collected from the 8 people
};

// GameState stores the current state of the player in the game.
// GameEngine will use it to decide what to do next.
// We added the prefix "STATE_" because MenuChoice also has an "Exit"
// value, and two normal enums cannot have the same name in C++.
enum GameState
{
    STATE_MENU,      // player is on the main menu
    STATE_PLAYING,   // a game is running
    STATE_WON,       // player caught the culprit
    STATE_LOST,      // credibility became zero or turns finished
    STATE_EXIT       // player wants to close the program
};

// MenuChoice is used for the Main Menu options.
// We start it from 1 so that the number the user types (1 to 5)
// matches the enum value directly.
enum MenuChoice
{
    MENU_NEW_GAME = 1,   // [1] New Game
    MENU_LEADERBOARD,    // [2] View Leader/Score Board
    MENU_ABOUT,          // [3] About
    MENU_RULES,          // [4] Rules
    MENU_EXIT            // [5] Exit
};


// =====================================================================
//  MODULE 1 : STRUCTURES
// ---------------------------------------------------------------------
//  A struct groups related data under ONE name. Instead of keeping a
//  letter, a position and a row in three separate variables, we keep
//  them together in one struct. Structs improve code reusability,
//  because the same struct can be used again and again in many classes
//  (Cottage, Scrabble, TileRack, Characters...).
//  Every member has a default value so a new struct is never filled
//  with garbage values.
// =====================================================================

// Tile is one box (slot) of the Scrabble board or of a TileRack.
struct Tile
{
    char tileLetter = ' ';   // letter placed in this box (' ' = no letter)
    bool filled = false;     // true when a letter has been placed here
};

// Clue is a "carrier". It carries one letter and its position
// from a Location or a Character to the Scrabble board.
struct Clue
{
    char clueLetter = ' ';      // the letter given as a clue
    int cluePosition = 0;       // board position of the letter (starts from 1)
    WordRow clueRow = WORD_1;   // which word (row) this letter belongs to
};

// Location is one place of the cottage (Garden, Kitchen, Bedroom_1 ...).
// Every location gives one letter for Word 1.
struct Location
{
    string locName = "";           // name of the place, e.g. "Study_Room"
    char letterClueLoc = ' ';      // letter hidden at this place
    int boardPosLoc = 0;           // position of that letter in Word 1
    WordRow locRow = WORD_1;       // always WORD_1 for locations
    bool locSearchFlag = false;    // true after the detective searches it
};


// =====================================================================
//  HELPER FUNCTIONS (for enums)
//  An enum is stored as a number, so these functions convert an enum
//  value into readable text that we can show on the screen.
// =====================================================================
string wordRowToText(WordRow row);
string gameStateToText(GameState state);
string menuChoiceToText(MenuChoice choice);

#endif // HEIST_H