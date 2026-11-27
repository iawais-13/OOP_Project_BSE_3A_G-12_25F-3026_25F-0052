// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 5) it is used to TEST the
//              TileRack class and the complete clue flow:
//              Location / Character -> Clue -> Rack -> Scrabble board
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


int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 5 TEST             " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // TEST 1 : Adding letters to a small rack (space for 3)
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Adding letters ---" << endl;
    TileRack smallRack(3);
    cout << "Rack at start: ";
    smallRack.display();

    smallRack.addTile(makeClue('R', 3, WORD_1));
    smallRack.addTile(makeClue('G', 1, WORD_1));
    cout << "After 2 letters: ";
    smallRack.display();
    cout << "Letters in order collected : " << smallRack.getLettersInOrder() << endl;
    cout << "First letter and position  : " << smallRack.getLetterAt(0)
        << " at " << smallRack.getPositionAt(0) << endl;

    // ------------------------------------------------------------
    // TEST 2 : Same position twice, and a full rack -> exceptions
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : Wrong adds ---" << endl;
    try
    {
        smallRack.addTile(makeClue('R', 3, WORD_1));   // position 3 is already in the rack
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    smallRack.addTile(makeClue('M', 4, WORD_1));       // 3rd letter -> rack is full now
    cout << "Is the rack full? " << (smallRack.isFull() ? "Yes" : "No") << endl;
    try
    {
        smallRack.addTile(makeClue('A', 5, WORD_1));   // no space left
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 3 : Removing letters until the rack is empty
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Removing letters ---" << endl;
    try
    {
        while (true)
        {
            char removed = smallRack.removeLastTile();
            cout << "Removed letter " << removed << ". Letters left: "
                << smallRack.getCount() << endl;
        }
    }
    catch (RackEmptyException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 4 : DEEP COPY of a rack
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Deep copy ---" << endl;
    TileRack originalRack(7);
    originalRack.addTile(makeClue('G', 1, WORD_1));

    TileRack copiedRack = originalRack;               // copy constructor
    copiedRack.addTile(makeClue('E', 2, WORD_1));

    TileRack assignedRack;
    assignedRack = originalRack;                      // assignment operator
    assignedRack.addTile(makeClue('S', 7, WORD_1));

    cout << "COPY     rack: " << copiedRack.getLettersByPosition() << endl;
    cout << "ASSIGNED rack: " << assignedRack.getLettersByPosition() << endl;
    cout << "ORIGINAL rack: " << originalRack.getLettersByPosition()
        << "  (original is safe)" << endl;

    // ------------------------------------------------------------
    // TEST 5 : The complete clue flow, like the real game.
    //   Cottage (location) -> Clue -> Detective's rack -> Scrabble
    //   Character (person) -> Clue -> Detective's rack -> Scrabble
    // Letters of the GUARD profile: GERMANS + SHEPHERD
    // ------------------------------------------------------------
    cout << "\n--- TEST 5 : Complete clue flow ---" << endl;

    Cottage cottage;
    cottage.setLocation(1, "Bedroom_1", 'G', 1);
    cottage.setLocation(2, "Bedroom_2", 'E', 2);
    cottage.setLocation(3, "Study_Room", 'R', 3);
    cottage.setLocation(4, "Garden", 'M', 4);
    cottage.setLocation(5, "Kitchen", 'A', 5);
    cottage.setLocation(6, "Dining_Room", 'N', 6);
    cottage.setLocation(7, "Living_Room", 'S', 7);

    Character* people[3];
    people[0] = new Victim("Mr.Leonardo", "French", 'S', 1);
    people[1] = new Cook("Mr.Enzo", "Italian", 'H', 2);
    people[2] = new PA("Mr.Jad", "Egyptian", 'D', 8);

    Detective player("Agent_Halim", 100);
    Scrabble scrabble;
    scrabble.setTargetWords("GERMANS", "SHEPHERD");

    // Visit all 7 locations (in a mixed order)
    int visitOrder[7] = { 5, 3, 1, 7, 2, 6, 4 };
    for (int i = 0; i < 7; i++)
    {
        Clue foundClue = cottage.searchLocation(visitOrder[i]);
        player.collectClue(foundClue);       // 1. save in the rack
        scrabble.placeLetter(foundClue);     // 2. place on the board
    }

    // Interview 3 people
    for (int i = 0; i < 3; i++)
    {
        Clue personClue = people[i]->getClue();
        player.collectClue(personClue);
        scrabble.placeLetter(personClue);
    }

    cout << endl;
    player.display();
    cout << endl;
    scrabble.displayBoard();

    cout << "\nRack 1 by position: " << player.getRack(WORD_1).getLettersByPosition() << endl;
    cout << "Rack 2 by position: " << player.getRack(WORD_2).getLettersByPosition() << endl;

    // ------------------------------------------------------------
    // TEST 6 : DEEP COPY of a Detective.
    // The Detective has no copy constructor of its own. C++ copies it
    // member by member, and for each TileRack member it calls the
    // TileRack copy constructor, which makes a deep copy.
    // ------------------------------------------------------------
    cout << "\n--- TEST 6 : Copying a Detective ---" << endl;
    Detective copiedPlayer = player;
    copiedPlayer.collectClue(makeClue('E', 3, WORD_2));

    cout << "COPY     Word 2 rack: " << copiedPlayer.getRack(WORD_2).getLettersByPosition() << endl;
    cout << "ORIGINAL Word 2 rack: " << player.getRack(WORD_2).getLettersByPosition()
        << "  (original is safe)" << endl;

    for (int i = 0; i < 3; i++)
    {
        delete people[i];
        people[i] = nullptr;
    }

    cout << "\nModule 5 test finished successfully." << endl;
    return 0;
}
