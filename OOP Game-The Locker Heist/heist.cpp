// =====================================================================
//  File      : heist.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the IMPLEMENTATION file. It contains the bodies
//              of all functions that were declared in heist.h
// =====================================================================

#include "heist.h"


// =====================================================================
//  HELPER FUNCTIONS (for enums)
// =====================================================================

// Converts a WordRow value into text, e.g. WORD_1 -> "Word 1"
string wordRowToText(WordRow row)
{
    if (row == WORD_1)
    {
        return "Word 1";
    }
    return "Word 2";
}

// Converts a GameState value into text so it can be printed
string gameStateToText(GameState state)
{
    switch (state)
    {
    case STATE_MENU:
        return "Menu";
    case STATE_PLAYING:
        return "Playing";
    case STATE_WON:
        return "Won";
    case STATE_LOST:
        return "Lost";
    case STATE_EXIT:
        return "Exit";
    default:
        return "Unknown";
    }
}

// Converts a MenuChoice value into the text shown on the Main Menu
string menuChoiceToText(MenuChoice choice)
{
    switch (choice)
    {
    case MENU_NEW_GAME:
        return "New Game";
    case MENU_LEADERBOARD:
        return "View Leader/Score Board";
    case MENU_ABOUT:
        return "About";
    case MENU_RULES:
        return "Rules";
    case MENU_EXIT:
        return "Exit";
    default:
        return "Unknown";
    }
}


// =====================================================================
//  MODULE 7 : EXCEPTION HANDLING
// =====================================================================

// ---------------- Base class : GameException ----------------

// Constructor: saves the error message
GameException::GameException(string errorMessage)
{
    message = errorMessage;
}

// Returns only the error details
string GameException::getMessage() const
{
    return message;
}

// Base class name. Derived classes override this function.
string GameException::getErrorType() const
{
    return "Game Error";
}

// Joins the error type and the message, e.g. "[Rack Full] The rack ..."
// getErrorType() is virtual, so the derived class version is called here.
string GameException::getFullMessage() const
{
    return "[" + getErrorType() + "] " + message;
}

// Destructor: nothing to free, but it is virtual for safe deletion
GameException::~GameException()
{
}


// ---------------- Derived exception classes ----------------
// Each constructor builds its own message and passes it to the
// base class constructor (GameException) using the initializer list.

InvalidInputException::InvalidInputException(string details)
    : GameException("Invalid input: " + details)
{
}

string InvalidInputException::getErrorType() const
{
    return "Invalid Input";
}


InvalidPositionException::InvalidPositionException(int position, int maxPosition)
    : GameException("Position " + to_string(position) +
        " is not valid. Please choose between 1 and " +
        to_string(maxPosition) + ".")
{
}

string InvalidPositionException::getErrorType() const
{
    return "Invalid Position";
}


SlotFilledException::SlotFilledException(int position)
    : GameException("Slot " + to_string(position) +
        " on the board is already filled.")
{
}

string SlotFilledException::getErrorType() const
{
    return "Slot Filled";
}


WordIncompleteException::WordIncompleteException(WordRow row)
    : GameException(wordRowToText(row) +
        " is not complete yet. Collect all its letters first.")
{
}

string WordIncompleteException::getErrorType() const
{
    return "Word Incomplete";
}


WordCompleteException::WordCompleteException(WordRow row)
    : GameException(wordRowToText(row) +
        " is already complete. No more letters can be placed.")
{
}

string WordCompleteException::getErrorType() const
{
    return "Word Complete";
}


InvalidWordException::InvalidWordException(string typedWord)
    : GameException("\"" + typedWord + "\" is not the correct word.")
{
}

string InvalidWordException::getErrorType() const
{
    return "Wrong Word";
}


RackFullException::RackFullException()
    : GameException("The tile rack is full. No more letters can be added.")
{
}

string RackFullException::getErrorType() const
{
    return "Rack Full";
}


RackEmptyException::RackEmptyException()
    : GameException("The tile rack is empty. There is no letter to remove.")
{
}

string RackEmptyException::getErrorType() const
{
    return "Rack Empty";
}


AlreadyVisitedException::AlreadyVisitedException(string placeOrPerson)
    : GameException("The clue from " + placeOrPerson +
        " has already been collected.")
{
}

string AlreadyVisitedException::getErrorType() const
{
    return "Already Visited";
}


InterviewDeniedException::InterviewDeniedException(string personName, int requiredCredibility)
    : GameException(personName + " refuses to talk. You need at least " +
        to_string(requiredCredibility) + " credibility.")
{
}

string InterviewDeniedException::getErrorType() const
{
    return "Interview Denied";
}


FileNotFoundException::FileNotFoundException(string fileName)
    : GameException("The file \"" + fileName + "\" was not found.")
{
}

string FileNotFoundException::getErrorType() const
{
    return "File Not Found";
}


FileCorruptedException::FileCorruptedException(string fileName, string details)
    : GameException("The file \"" + fileName + "\" is corrupted: " + details)
{
}

string FileCorruptedException::getErrorType() const
{
    return "File Corrupted";
}