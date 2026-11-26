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
//  MODULE 7 : EXCEPTION HANDLING
// ---------------------------------------------------------------------
//  An exception is an error that happens while the program is running
//  (wrong input, full rack, missing file ...). Instead of crashing, we
//  THROW an exception object and CATCH it somewhere else, show a
//  message and let the game continue safely.
//
//  GameException is the BASE class of all our game errors. Every
//  special error is a DERIVED class of it. Because of this, one
//  catch block  "catch (GameException& error)"  can catch ALL of them.
// =====================================================================

// Base class for every error of the game
class GameException
{
protected:
    string message;     // details of what went wrong

public:
    GameException(string errorMessage = "Something went wrong in the game.");

    // Returns the details of the error
    string getMessage() const;

    // Returns the name (type) of the error.
    // It is VIRTUAL, so every derived class gives its own name and the
    // correct version is chosen at runtime (polymorphism).
    virtual string getErrorType() const;

    // Returns type + message together, ready to print on screen
    string getFullMessage() const;

    // Virtual destructor so derived exception objects are destroyed properly
    virtual ~GameException();
};

// Thrown when the user types something wrong
// (e.g. menu choice 9, letters instead of a number, name with a space)
class InvalidInputException : public GameException
{
public:
    InvalidInputException(string details);
    string getErrorType() const override;
};

// Thrown when a position (or menu number) is outside the allowed range
class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(int position, int maxPosition);
    string getErrorType() const override;
};

// Thrown when a letter is placed on a board slot that is already filled
class SlotFilledException : public GameException
{
public:
    SlotFilledException(int position);
    string getErrorType() const override;
};

// Thrown when the detective tries to form a word but the row is not full yet
class WordIncompleteException : public GameException
{
public:
    WordIncompleteException(WordRow row);
    string getErrorType() const override;
};

// Thrown when a letter is added to a word (row) that is already complete
class WordCompleteException : public GameException
{
public:
    WordCompleteException(WordRow row);
    string getErrorType() const override;
};

// Thrown when the detective types a wrong word
class InvalidWordException : public GameException
{
public:
    InvalidWordException(string typedWord);
    string getErrorType() const override;
};

// Thrown when a letter is added to a TileRack that has no space left
class RackFullException : public GameException
{
public:
    RackFullException();
    string getErrorType() const override;
};

// Thrown when we try to take a letter out of an empty TileRack
class RackEmptyException : public GameException
{
public:
    RackEmptyException();
    string getErrorType() const override;
};

// Thrown when the same clue (letter) is collected a second time
class AlreadyVisitedException : public GameException
{
public:
    AlreadyVisitedException(string placeOrPerson);
    string getErrorType() const override;
};

// Thrown when the detective's credibility is too low for an interview
class InterviewDeniedException : public GameException
{
public:
    InterviewDeniedException(string personName, int requiredCredibility);
    string getErrorType() const override;
};

// Thrown when a .txt file cannot be opened
class FileNotFoundException : public GameException
{
public:
    FileNotFoundException(string fileName);
    string getErrorType() const override;
};

// Thrown when a .txt file is opened but its data is in a wrong format
class FileCorruptedException : public GameException
{
public:
    FileCorruptedException(string fileName, string details);
    string getErrorType() const override;
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