// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 8) it is used to TEST the
//              DynamicArray<T> template, and to check that Cottage,
//              Scrabble and TileRack still work after using it.
// =====================================================================

#include "heist.h"

int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 8 TEST             " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // TEST 1 : ONE template, THREE different types
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Same template, different types ---" << endl;

    DynamicArray<int> turnNumbers(5);            // array of int
    for (int i = 0; i < turnNumbers.getSize(); i++)
    {
        turnNumbers[i] = (i + 1) * 5;
    }
    cout << "DynamicArray<int>      : ";
    for (int i = 0; i < turnNumbers.getSize(); i++)
    {
        cout << turnNumbers[i] << " ";
    }
    cout << endl;

    DynamicArray<string> names(3);               // array of string
    names[0] = "Mr.Enzo";
    names[1] = "Mr.Lukas";
    names[2] = "Mr.Toby";
    cout << "DynamicArray<string>   : ";
    for (int i = 0; i < names.getSize(); i++)
    {
        cout << names[i] << " ";
    }
    cout << endl;

    DynamicArray<Location> places(2);            // array of our own struct
    places[0].locName = "Garden";
    places[1].locName = "Kitchen";
    cout << "DynamicArray<Location> : " << places[0].locName << " "
        << places[1].locName << endl;

    // ------------------------------------------------------------
    // TEST 2 : The index is checked -> no going outside the array
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : Wrong index ---" << endl;
    try
    {
        cout << turnNumbers[10] << endl;         // only 5 elements
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 3 : Deep copy of the template itself
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Deep copy of DynamicArray ---" << endl;
    DynamicArray<string> copiedNames = names;    // copy constructor
    copiedNames[0] = "CHANGED";

    DynamicArray<string> assignedNames;
    assignedNames = names;                       // assignment operator
    assignedNames[1] = "ALSO_CHANGED";

    cout << "COPY     : " << copiedNames[0] << " " << copiedNames[1] << endl;
    cout << "ASSIGNED : " << assignedNames[0] << " " << assignedNames[1] << endl;
    cout << "ORIGINAL : " << names[0] << " " << names[1] << "  (original is safe)" << endl;

    // ------------------------------------------------------------
    // TEST 4 : Cottage, Scrabble and TileRack now use the template.
    // We run the complete clue flow again; the result must be the
    // same as in Module 5 (GERMANS on the board).
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Classes after the template refactor ---" << endl;

    Cottage cottage;
    cottage.setLocation(1, "Bedroom_1", 'G', 1);
    cottage.setLocation(2, "Bedroom_2", 'E', 2);
    cottage.setLocation(3, "Study_Room", 'R', 3);
    cottage.setLocation(4, "Garden", 'M', 4);
    cottage.setLocation(5, "Kitchen", 'A', 5);
    cottage.setLocation(6, "Dining_Room", 'N', 6);
    cottage.setLocation(7, "Living_Room", 'S', 7);

    Detective player("Agent_Halim", 100);
    Scrabble scrabble;
    scrabble.setTargetWords("GERMANS", "SHEPHERD");

    for (int number = 1; number <= cottage.getLocationCount(); number++)
    {
        Clue foundClue = cottage.searchLocation(number);
        player.collectClue(foundClue);
        scrabble.placeLetter(foundClue);
    }

    Victim owner("Mr.Leonardo", "French", 'S', 1);
    player.collectClue(owner.getClue());
    scrabble.placeLetter(owner.getClue());

    player.displayRacks();
    scrabble.displayBoard();
    cottage.displayLocations();

    // ------------------------------------------------------------
    // TEST 5 : Deep copy still works for the three classes
    // ------------------------------------------------------------
    cout << "\n--- TEST 5 : Deep copy of the refactored classes ---" << endl;

    Cottage copiedCottage = cottage;
    copiedCottage.setLocation(1, "Changed_Room", 'X', 1);
    cout << "Cottage  -> copy: " << copiedCottage.getLocationName(1)
        << " | original: " << cottage.getLocationName(1) << endl;

    Scrabble copiedBoard = scrabble;
    Clue cookClue;
    cookClue.clueLetter = 'H';
    cookClue.cluePosition = 2;
    cookClue.clueRow = WORD_2;
    copiedBoard.placeLetter(cookClue);
    cout << "Scrabble -> copy: " << copiedBoard.getPlacedWord(WORD_2)
        << " | original: " << scrabble.getPlacedWord(WORD_2) << endl;

    TileRack copiedRack = player.getRack(WORD_2);
    copiedRack.addTile(cookClue);
    cout << "TileRack -> copy: " << copiedRack.getLettersByPosition()
        << " | original: " << player.getRack(WORD_2).getLettersByPosition() << endl;

    // ------------------------------------------------------------
    // TEST 6 : The old checks still work (they now come from the classes
    // AND from DynamicArray's index check)
    // ------------------------------------------------------------
    cout << "\n--- TEST 6 : Old checks still work ---" << endl;
    try
    {
        scrabble.placeLetter(cookClue);          // fine: slot 2 of Word 2 is empty
        scrabble.placeLetter(cookClue);          // same slot again -> error
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    try
    {
        cottage.searchLocation(8);               // only 7 locations
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    cout << "\nModule 8 test finished successfully." << endl;
    return 0;
}
