// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 3 - Part A) it is used to TEST
//              the Character hierarchy.
// =====================================================================

#include "heist.h"

int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 3 (PART A) TEST    " << endl;
    cout << "==================================================" << endl;

    // NOTE: The line below will NOT compile, because Character is an
    // abstract class (it has a pure virtual function).
    // Character someone("Test", "Test", "Test");

    // ------------------------------------------------------------
    // Creating characters with the letters of the GUARD profile
    // (Word 2 = SHEPHERD). Values are taken from characters.txt:
    //   Mr.Leonardo -> S at 1,  Mr.Enzo -> H at 2,
    //   Mr.Lukas    -> E at 3,  Mr.Toby -> P at 4
    // All objects are stored in an array of BASE CLASS pointers.
    // ------------------------------------------------------------
    const int TOTAL_PEOPLE = 4;
    Character* people[TOTAL_PEOPLE];

    people[0] = new Victim("Mr.Leonardo", "French", 'S', 1);
    people[1] = new Cook("Mr.Enzo", "Italian", 'H', 2);
    people[2] = new Guard("Mr.Lukas", "German", 'E', 3);
    people[3] = new Servant("Mr.Toby", "British", 'P', 4);

    // ------------------------------------------------------------
    // TEST 1 : Runtime polymorphism
    // The same line  people[i]->display()  calls a DIFFERENT display()
    // for every object, chosen at runtime.
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : display() of every character ---\n" << endl;
    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        people[i]->display();
        cout << endl;
    }

    // ------------------------------------------------------------
    // TEST 2 : Interview conditions (no if-else on the role!)
    // ------------------------------------------------------------
    cout << "--- TEST 2 : Who can be interviewed? ---" << endl;
    int testCredibility[2] = { 100, 70 };

    for (int c = 0; c < 2; c++)
    {
        cout << "\nWith credibility " << testCredibility[c] << ":" << endl;
        for (int i = 0; i < TOTAL_PEOPLE; i++)
        {
            cout << "  " << people[i]->getName() << " (" << people[i]->getRole() << ") : ";
            if (people[i]->canBeInterviewed(testCredibility[c]))
            {
                cout << "Can be interviewed" << endl;
            }
            else
            {
                cout << "REFUSES (needs " << people[i]->getRequiredCredibility() << ")" << endl;
            }
        }
    }

    // ------------------------------------------------------------
    // TEST 3 : Collecting the Word 2 clue from every person
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Clues for Word 2 ---" << endl;
    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        Clue personClue = people[i]->getClue();
        cout << people[i]->getName() << " gives letter " << personClue.clueLetter
            << " for position " << personClue.cluePosition
            << " of " << wordRowToText(personClue.clueRow) << endl;
    }

    // ------------------------------------------------------------
    // TEST 4 : Detective (the player)
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Detective ---\n" << endl;
    Detective player("Agent_Halim", 100);
    player.display();

    cout << "\nWrong step (-5) and wrong word (-10)..." << endl;
    player.loseCredibility(5);
    player.loseCredibility(10);
    player.setTurns(3);
    player.addSolvedWord("GERMANS");
    player.display();

    cout << "\nA very big penalty (-500) ..." << endl;
    player.loseCredibility(500);
    cout << "Credibility now: " << player.getCredibility()
        << "  (it stops at 0, never negative)" << endl;

    // ------------------------------------------------------------
    // TEST 5 : Deleting objects through BASE CLASS pointers.
    // Because ~Character() is virtual, the full object is destroyed.
    // ------------------------------------------------------------
    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        delete people[i];
        people[i] = nullptr;
    }
    cout << "\n--- TEST 5 : All character objects deleted safely ---" << endl;

    cout << "\nModule 3 (Part A) test finished successfully." << endl;
    return 0;
}
