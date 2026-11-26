// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 4 - Part A) it is used to TEST
//              the Cottage class (DMA, deep copy, search).
// =====================================================================

#include "heist.h"

// ---------------------------------------------------------------------
//  TEST HELPER : fills a cottage with the 7 locations of locations.txt
//  using the letters of the GUARD profile (Word 1 = G E R M A N S).
//  In Module 6 this data will be read from the file instead.
// ---------------------------------------------------------------------
void fillCottageForGuard(Cottage& cottage)
{
    cottage.setLocation(1, "Bedroom_1", 'G', 1);
    cottage.setLocation(2, "Bedroom_2", 'E', 2);
    cottage.setLocation(3, "Study_Room", 'R', 3);
    cottage.setLocation(4, "Garden", 'M', 4);
    cottage.setLocation(5, "Kitchen", 'A', 5);
    cottage.setLocation(6, "Dining_Room", 'N', 6);
    cottage.setLocation(7, "Living_Room", 'S', 7);
}

// ---------------------------------------------------------------------
//  TEST HELPER : asks a Y/N question and returns true for Y.
//  Keeps asking until the user types Y or N (small or capital).
// ---------------------------------------------------------------------
bool askYesNo(string question)
{
    char answer;
    while (true)
    {
        cout << question << " (Y/N): ";
        cin >> answer;
        cin.ignore(1000, '\n');   // remove extra characters from the line

        if (answer == 'Y' || answer == 'y')
        {
            return true;
        }
        if (answer == 'N' || answer == 'n')
        {
            return false;
        }
        cout << "Please type Y or N only." << endl;
    }
}


int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 4 (PART A) TEST    " << endl;
    cout << "==================================================" << endl;

    Cottage cottage;
    fillCottageForGuard(cottage);

    // ------------------------------------------------------------
    // TEST 1 : Show all locations (all are NOT searched at start)
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Locations at the start ---" << endl;
    cottage.displayLocations();
    cout << "Total locations: " << cottage.getLocationCount() << endl;

    // ------------------------------------------------------------
    // TEST 2 : Search one location and get its clue
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : Searching the Study Room (3) ---" << endl;
    Clue studyClue = cottage.searchLocation(3);
    cout << "Letter " << studyClue.clueLetter << " collected at position "
        << studyClue.cluePosition << " of " << wordRowToText(studyClue.clueRow) << "." << endl;
    cout << "Is location 3 searched now? " << (cottage.isSearched(3) ? "Yes" : "No") << endl;

    // ------------------------------------------------------------
    // TEST 3 : Wrong location numbers throw an exception
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Wrong location number ---" << endl;
    int wrongNumbers[2] = { 0, 9 };
    for (int i = 0; i < 2; i++)
    {
        try
        {
            cottage.searchLocation(wrongNumbers[i]);
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }

    // ------------------------------------------------------------
    // TEST 4 : DEEP COPY with the copy constructor.
    // We search the Kitchen in the COPY only. If the copy is deep,
    // the ORIGINAL cottage must not change.
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Copy constructor (deep copy) ---" << endl;
    Cottage copiedCottage = cottage;      // copy constructor is called
    copiedCottage.searchLocation(5);      // search Kitchen in the copy only

    cout << "Kitchen searched in COPY?     " << (copiedCottage.isSearched(5) ? "Yes" : "No") << endl;
    cout << "Kitchen searched in ORIGINAL? " << (cottage.isSearched(5) ? "Yes" : "No")
        << "  (original is safe)" << endl;

    // ------------------------------------------------------------
    // TEST 5 : DEEP COPY with the assignment operator
    // ------------------------------------------------------------
    cout << "\n--- TEST 5 : Assignment operator (deep copy) ---" << endl;
    Cottage assignedCottage;              // a new, empty cottage
    assignedCottage = cottage;            // operator= is called
    assignedCottage.setLocation(1, "Changed_Room", 'X', 1);

    cout << "Location 1 in ASSIGNED cottage: " << assignedCottage.getLocationName(1) << endl;
    cout << "Location 1 in ORIGINAL cottage: " << cottage.getLocationName(1)
        << "  (original is safe)" << endl;

    // ------------------------------------------------------------
    // TEST 6 : Your turn! Visit locations like in the real game.
    // Already searched locations ask "Search again?"
    // ------------------------------------------------------------
    cout << "\n--- TEST 6 : Visit locations yourself ---" << endl;

    int choice = -1;
    while (choice != 0)
    {
        cout << endl;
        cottage.displayLocations();
        cout << "Searched: " << cottage.getSearchedCount() << " / "
            << cottage.getLocationCount() << endl;
        cout << "Pick a location (1-7), or 0 to finish: ";
        cin >> choice;

        if (cin.fail())                   // letters typed instead of a number
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[Invalid Input] Please type a number." << endl;
            choice = -1;
            continue;
        }

        if (choice == 0)
        {
            break;
        }

        try
        {
            string placeName = replaceUnderscores(cottage.getLocationName(choice));
            bool wantToSearch;

            if (cottage.isSearched(choice))
            {
                cout << "The " << placeName << " has already been searched." << endl;
                wantToSearch = askYesNo("Do you want to search it again?");
            }
            else
            {
                cout << "You have NOT searched the " << placeName << " yet." << endl;
                wantToSearch = askYesNo("Search?");
            }

            if (wantToSearch)
            {
                Clue foundClue = cottage.searchLocation(choice);
                cout << "Letter " << foundClue.clueLetter << " found at position "
                    << foundClue.cluePosition << " of Word 1." << endl;
            }
            else
            {
                cout << "Going back to the locations..." << endl;
            }
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }

    // All three cottages are destroyed automatically here.
    // Their destructors free all heap memory (no memory leak).
    cout << "\nModule 4 (Part A) test finished successfully." << endl;
    return 0;
}
