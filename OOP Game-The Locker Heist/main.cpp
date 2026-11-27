// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 4 - Part B) it is used to TEST
//              the Scrabble class (jagged array, placing letters, checks).
// =====================================================================

#include "heist.h"

// ---------------------------------------------------------------------
//  TEST HELPER : makes a Clue in one line
// ---------------------------------------------------------------------
Clue makeClue(char letter, int position, WordRow row)
{
    Clue newClue;
    newClue.clueLetter = letter;
    newClue.cluePosition = position;
    newClue.clueRow = row;
    return newClue;
}

// ---------------------------------------------------------------------
//  TEST HELPER : tries to place a clue and prints the result.
//  If an exception is thrown, its message is shown (no crash).
// ---------------------------------------------------------------------
void tryToPlace(Scrabble& scrabble, Clue clue)
{
    cout << "Placing '" << clue.clueLetter << "' at position " << clue.cluePosition
        << " of " << wordRowToText(clue.clueRow) << " ... ";
    try
    {
        scrabble.placeLetter(clue);
        cout << "Placed." << endl;
    }
    catch (GameException& error)
    {
        cout << endl << "   " << error.getFullMessage() << endl;
    }
}


int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 4 (PART B) TEST    " << endl;
    cout << "==================================================" << endl;

    // Target words of the GUARD profile from clue.txt
    Scrabble scrabble;
    scrabble.setTargetWords("GERMANS", "SHEPHERD");

    // ------------------------------------------------------------
    // TEST 1 : Empty board (jagged array: 7 slots and 8 slots)
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Empty board ---" << endl;
    scrabble.displayBoard();

    // ------------------------------------------------------------
    // TEST 2 : Placing some letters in any order.
    // Each letter goes to its own position.
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : Placing letters ---" << endl;
    tryToPlace(scrabble, makeClue('R', 3, WORD_1));   // Study_Room
    tryToPlace(scrabble, makeClue('G', 1, WORD_1));   // Bedroom_1
    tryToPlace(scrabble, makeClue('S', 1, WORD_2));   // Mr.Leonardo
    tryToPlace(scrabble, makeClue('D', 8, WORD_2));   // Mr.Jad
    scrabble.displayBoard();

    // ------------------------------------------------------------
    // TEST 3 : Wrong moves -> exceptions
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Wrong moves ---" << endl;
    tryToPlace(scrabble, makeClue('X', 9, WORD_1));   // row 1 has only 7 slots
    tryToPlace(scrabble, makeClue('X', 0, WORD_2));   // positions start from 1
    tryToPlace(scrabble, makeClue('R', 3, WORD_1));   // slot 3 already filled

    // ------------------------------------------------------------
    // TEST 4 : Revealing a placed letter again
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Reveal letters ---" << endl;
    cout << "Letter at position 3 of Word 1: " << scrabble.revealLetter(WORD_1, 3) << endl;
    cout << "Placed so far in Word 1: " << scrabble.getPlacedWord(WORD_1)
        << "  (" << scrabble.getFilledCount(WORD_1) << " of 7)" << endl;

    // ------------------------------------------------------------
    // TEST 5 : Typing a word before its row is complete
    // ------------------------------------------------------------
    cout << "\n--- TEST 5 : Word not complete yet ---" << endl;
    try
    {
        scrabble.isWordCorrect(WORD_1, "GERMANS");
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 6 : Completing Word 1, then trying to add one more letter
    // ------------------------------------------------------------
    cout << "\n--- TEST 6 : Completing Word 1 ---" << endl;
    tryToPlace(scrabble, makeClue('E', 2, WORD_1));
    tryToPlace(scrabble, makeClue('M', 4, WORD_1));
    tryToPlace(scrabble, makeClue('A', 5, WORD_1));
    tryToPlace(scrabble, makeClue('N', 6, WORD_1));
    tryToPlace(scrabble, makeClue('S', 7, WORD_1));
    scrabble.displayBoard();
    tryToPlace(scrabble, makeClue('Z', 2, WORD_1));   // row is full now

    // ------------------------------------------------------------
    // TEST 7 : DEEP COPY (copy constructor and assignment operator)
    // ------------------------------------------------------------
    cout << "\n--- TEST 7 : Deep copy ---" << endl;
    Scrabble copiedBoard = scrabble;                  // copy constructor
    copiedBoard.placeLetter(makeClue('H', 2, WORD_2));

    Scrabble assignedBoard;
    assignedBoard = scrabble;                         // assignment operator
    assignedBoard.placeLetter(makeClue('E', 3, WORD_2));

    cout << "COPY     Word 2: " << copiedBoard.getPlacedWord(WORD_2) << endl;
    cout << "ASSIGNED Word 2: " << assignedBoard.getPlacedWord(WORD_2) << endl;
    cout << "ORIGINAL Word 2: " << scrabble.getPlacedWord(WORD_2)
        << "  (original is safe)" << endl;

    // ------------------------------------------------------------
    // TEST 8 : Your turn! Word 1 is complete, type it.
    // Small letters are also accepted. Try a wrong word first.
    // ------------------------------------------------------------
    cout << "\n--- TEST 8 : Form Word 1 yourself ---" << endl;
    scrabble.displayWord(WORD_1);

    bool solved = false;
    while (solved == false)
    {
        string typedWord;
        cout << "Type Word 1: ";
        cin >> typedWord;

        if (scrabble.isWordCorrect(WORD_1, typedWord))
        {
            cout << "Correct! Word 1 is " << scrabble.getTargetWord(WORD_1) << "." << endl;
            solved = true;
        }
        else
        {
            InvalidWordException wrongWord(typedWord);
            cout << wrongWord.getFullMessage() << " (-10 credibility in the real game)" << endl;
        }
    }

    // All boards are destroyed here; their destructors free the heap memory
    cout << "\nModule 4 (Part B) test finished successfully." << endl;
    return 0;
}
