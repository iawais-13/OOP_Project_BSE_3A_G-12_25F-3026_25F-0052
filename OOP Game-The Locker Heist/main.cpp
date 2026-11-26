// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 3 - Part B) it is used to TEST
//              the complete Character hierarchy and the diamond problem.
// =====================================================================

#include "heist.h"

int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 3 (PART B) TEST    " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // All 8 people of the cottage, with the letters of the GUARD
    // profile from characters.txt (Word 2 = S H E P H E R D).
    // ------------------------------------------------------------
    const int TOTAL_PEOPLE = 8;
    Character* people[TOTAL_PEOPLE];

    people[0] = new Victim("Mr.Leonardo", "French", 'S', 1);
    people[1] = new Cook("Mr.Enzo", "Italian", 'H', 2);
    people[2] = new Guard("Mr.Lukas", "German", 'E', 3);
    people[3] = new Servant("Mr.Toby", "British", 'P', 4);
    people[4] = new OwnerChild("Ms.Celine", "French", 'H', 5);
    people[5] = new Partner("Ms.Ruslana", "Partner 1", "Ukrainian", 'E', 6);
    people[6] = new Partner("Mr.Dmitry", "Partner 2", "Russian", 'R', 7);
    people[7] = new PA("Mr.Jad", "Egyptian", 'D', 8);

    // ------------------------------------------------------------
    // TEST 1 : display() of the 4 new characters (polymorphism)
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : The new characters of Part B ---\n" << endl;
    for (int i = 4; i < TOTAL_PEOPLE; i++)
    {
        people[i]->display();
        cout << endl;
    }

    // ------------------------------------------------------------
    // TEST 2 : Interview conditions of all 8 people at different
    // credibility levels. "YES" = can be interviewed.
    // ------------------------------------------------------------
    cout << "--- TEST 2 : Interview table ---\n" << endl;

    const int LEVELS = 5;
    int credibilityLevels[LEVELS] = { 100, 75, 50, 35, 20 };

    cout << "Person (Role)              ";
    for (int c = 0; c < LEVELS; c++)
    {
        cout << credibilityLevels[c] << "\t";
    }
    cout << endl;
    cout << "-----------------------------------------------------------" << endl;

    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        string label = people[i]->getName() + " (" + people[i]->getRole() + ")";
        cout << label;

        // add spaces so that all columns line up
        for (int space = (int)label.length(); space < 27; space++)
        {
            cout << " ";
        }

        for (int c = 0; c < LEVELS; c++)
        {
            if (people[i]->canBeInterviewed(credibilityLevels[c]))
            {
                cout << "YES\t";
            }
            else
            {
                cout << "no\t";
            }
        }
        cout << endl;
    }

    // ------------------------------------------------------------
    // TEST 3 : Building Word 2 from the 8 clues.
    // Every letter is put at its own position, so the order of
    // interviews does not matter.
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Word 2 from all 8 clues ---" << endl;

    char word2[TOTAL_PEOPLE + 1];      // 8 letters + '\0'
    word2[TOTAL_PEOPLE] = '\0';

    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        Clue personClue = people[i]->getClue();
        word2[personClue.cluePosition - 1] = personClue.clueLetter;   // position 1 -> index 0
    }
    cout << "Word 2 is: " << word2 << "  (Guard profile)" << endl;

    // ------------------------------------------------------------
    // TEST 4 : Proof that the diamond problem is solved.
    // We reach the Suspect part of PA through BOTH parents.
    // If there were two copies, the two addresses would be different.
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Diamond problem check (PA) ---" << endl;

    PA testPA("Mr.Jad", "Egyptian", 'D', 8);

    BusinessRelations* throughBusiness = &testPA;
    CloseRelations* throughClose = &testPA;

    Suspect* suspectPartFromBusiness = throughBusiness;
    Suspect* suspectPartFromClose = throughClose;

    cout << "Name through BusinessRelations : " << throughBusiness->getName() << endl;
    cout << "Name through CloseRelations    : " << throughClose->getName() << endl;
    cout << "Only ONE Suspect part inside PA? "
        << (suspectPartFromBusiness == suspectPartFromClose ? "YES (diamond solved)" : "NO")
        << endl;
    cout << "PA calls getName() directly    : " << testPA.getName()
        << "  (no ambiguity error)" << endl;

    // ------------------------------------------------------------
    // TEST 5 : Deleting all objects through base class pointers
    // ------------------------------------------------------------
    for (int i = 0; i < TOTAL_PEOPLE; i++)
    {
        delete people[i];
        people[i] = nullptr;
    }
    cout << "\n--- TEST 5 : All 8 character objects deleted safely ---" << endl;

    cout << "\nModule 3 (Part B) test finished successfully." << endl;
    system("pause");
    return 0;
}
