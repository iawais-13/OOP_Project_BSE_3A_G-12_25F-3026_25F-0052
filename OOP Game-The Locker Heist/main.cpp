// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 11) it is used to TEST the
//              overloaded operators (==, !=, +, <<, >>) and friends.
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
    cout << "     THE LOCKER HEIST - MODULE 11 TEST            " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // TEST 1 : == and != on tiles (comparing letters with clue.txt)
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Tile == and != ---" << endl;
    Tile tileA;
    tileA.tileLetter = 'G';
    Tile tileB;
    tileB.tileLetter = 'G';
    Tile tileC;
    tileC.tileLetter = 'E';

    cout << tileA << " == " << tileB << " ? " << (tileA == tileB ? "Yes" : "No") << endl;
    cout << tileA << " == " << tileC << " ? " << (tileA == tileC ? "Yes" : "No") << endl;
    cout << tileA << " != " << tileC << " ? " << (tileA != tileC ? "Yes" : "No") << endl;

    // The word check of Scrabble now uses the Tile operators
    Scrabble scrabble;
    scrabble.setTargetWords("GERMANS", "SHEPHERD");
    char word1Letters[7] = { 'G', 'E', 'R', 'M', 'A', 'N', 'S' };
    for (int i = 0; i < 7; i++)
    {
        scrabble.placeLetter(makeClue(word1Letters[i], i + 1, WORD_1));
    }
    cout << "Typed \"germans\" -> " << (scrabble.isWordCorrect(WORD_1, "germans") ? "Correct" : "Wrong") << endl;
    cout << "Typed \"GERMENS\" -> " << (scrabble.isWordCorrect(WORD_1, "GERMENS") ? "Correct" : "Wrong") << endl;

    // ------------------------------------------------------------
    // TEST 2 : << for Tile, Location, Character and Scrabble
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : The << operator ---" << endl;

    Location diningRoom;
    diningRoom.locName = "Dining_Room";
    diningRoom.letterClueLoc = 'N';
    diningRoom.boardPosLoc = 6;
    cout << "Location  : " << diningRoom << endl;

    cout << "Scrabble  : " << endl << scrabble;

    // A Character pointer pointing to a PA object.
    // cout << *person runs the PA version of display() (polymorphism).
    Character* person = new PA("Mr.Jad", "Egyptian", 'D', 8);
    cout << "Character : " << endl << *person;

    // ------------------------------------------------------------
    // TEST 3 : + on TileRack -> the final clue
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : rack1 + rack2 ---" << endl;
    TileRack word1Rack(7);
    TileRack word2Rack(8);

    char word2Letters[8] = { 'S', 'H', 'E', 'P', 'H', 'E', 'R', 'D' };
    // add the letters in a mixed order, like a real game
    int order1[7] = { 5, 3, 1, 7, 2, 6, 4 };
    int order2[8] = { 8, 1, 4, 2, 6, 3, 7, 5 };
    for (int i = 0; i < 7; i++)
    {
        int position = order1[i];
        word1Rack.addTile(makeClue(word1Letters[position - 1], position, WORD_1));
    }
    for (int i = 0; i < 8; i++)
    {
        int position = order2[i];
        word2Rack.addTile(makeClue(word2Letters[position - 1], position, WORD_2));
    }

    TileRack finalClueRack = word1Rack + word2Rack;    // operator+

    cout << "Rack 1 (collected order): " << word1Rack.getLettersInOrder() << endl;
    cout << "Rack 2 (collected order): " << word2Rack.getLettersInOrder() << endl;
    cout << "Rack 1 + Rack 2         : " << finalClueRack.getLettersInOrder() << endl;
    cout << "Tiles in the final rack : " << finalClueRack.getCount() << " (7 + space + 8)" << endl;

    // ------------------------------------------------------------
    // TEST 4 : >> operators. Please type what is asked.
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : The >> operator (your input) ---" << endl;

    Tile typedTile;
    cout << "Type one letter (e.g. k): ";
    cin >> typedTile;
    cout << "You typed the tile " << typedTile << endl;

    Location typedPlace;
    cout << "\nType a location as  name letter position  (e.g. Garden m 4): ";
    cin >> typedPlace;
    cout << "Location read: " << typedPlace << "  (letter "
        << typedPlace.letterClueLoc << " at " << typedPlace.boardPosLoc << ")" << endl;

    cout << "\nType a new name and nationality for the PA (e.g. Mr.Omar Saudi): ";
    cin >> *person;
    cout << "PA is now: " << person->getName() << " (" << person->getNationality() << ")" << endl;

    cout << "\nPlace a letter on Word 2 as  letter position row  (e.g. h 2 2): ";
    try
    {
        cin >> scrabble;
        cout << scrabble;
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    delete person;
    person = nullptr;

    cout << "\nModule 11 test finished successfully." << endl;
    return 0;
}
