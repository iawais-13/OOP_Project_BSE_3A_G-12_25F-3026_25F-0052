// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 1) it is used to TEST that
//              our enums and structs are working correctly.
// =====================================================================

#include "heist.h"

int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 1 TEST             " << endl;
    cout << "==================================================" << endl;

    // ---------------- Testing the enums ----------------

    // Printing all Main Menu options with the help of MenuChoice enum
    cout << "\n--- Main Menu (from MenuChoice enum) ---" << endl;
    for (int option = MENU_NEW_GAME; option <= MENU_EXIT; option++)
    {
        // (MenuChoice)option converts the int number into the enum type
        cout << "[" << option << "] " << menuChoiceToText((MenuChoice)option) << endl;
    }

    // Changing the game state step by step
    GameState currentState = STATE_MENU;
    cout << "\nGame state at start : " << gameStateToText(currentState) << endl;

    currentState = STATE_PLAYING;
    cout << "Game state now      : " << gameStateToText(currentState) << endl;

    // ---------------- Testing the Tile struct ----------------

    Tile emptyTile;               // default values: ' ' and false
    Tile letterTile;
    letterTile.tileLetter = 'G';
    letterTile.filled = true;

    cout << "\n--- Tile struct ---" << endl;
    cout << "Empty tile  -> letter: [" << emptyTile.tileLetter
        << "]  filled: " << emptyTile.filled << endl;
    cout << "Letter tile -> letter: [" << letterTile.tileLetter
        << "]  filled: " << letterTile.filled << endl;

    // ---------------- Testing the Location struct ----------------
    // Example from locations.txt: Kitchen gives letter 'A' at position 5
    // when the Guard is the culprit (GERM'A'NS).

    Location kitchen;
    kitchen.locName = "Kitchen";
    kitchen.letterClueLoc = 'A';
    kitchen.boardPosLoc = 5;
    kitchen.locRow = WORD_1;
    kitchen.locSearchFlag = false;

    cout << "\n--- Location struct ---" << endl;
    cout << "Name     : " << kitchen.locName << endl;
    cout << "Letter   : " << kitchen.letterClueLoc << endl;
    cout << "Position : " << kitchen.boardPosLoc << endl;
    cout << "Row      : " << wordRowToText(kitchen.locRow) << endl;
    cout << "Searched : " << (kitchen.locSearchFlag ? "Yes" : "No") << endl;

    // ---------------- Testing the Clue struct ----------------
    // Clue is a carrier: we copy the letter from the location into a clue.

    Clue foundClue;
    foundClue.clueLetter = kitchen.letterClueLoc;
    foundClue.cluePosition = kitchen.boardPosLoc;
    foundClue.clueRow = kitchen.locRow;
    kitchen.locSearchFlag = true;   // the kitchen has now been searched

    cout << "\n--- Clue struct (carried from Kitchen) ---" << endl;
    cout << "Letter " << foundClue.clueLetter
        << " collected at position " << foundClue.cluePosition
        << " of " << wordRowToText(foundClue.clueRow) << "." << endl;
    cout << "Kitchen searched now? " << (kitchen.locSearchFlag ? "Yes" : "No") << endl;

    cout << "\nModule 1 test finished successfully." << endl;
    system("pause");
    return 0;
}