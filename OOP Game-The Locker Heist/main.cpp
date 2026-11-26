// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 7) it is used to TEST that
//              our custom exception classes are working correctly.
// =====================================================================

#include "heist.h"

// ---------------------------------------------------------------------
//  TEST HELPER 1 : reads a Main Menu choice (1 to 5) from the user.
//  It THROWS an exception if the input is not a number or is outside
//  the range. The exception is not caught here; it goes back to main().
// ---------------------------------------------------------------------
int readMenuChoice()
{
    int choice;
    cout << "Choice: ";
    cin >> choice;

    // cin fails when the user types letters instead of a number
    if (cin.fail())
    {
        cin.clear();              // remove the error state of cin
        cin.ignore(1000, '\n');   // throw away the wrong input
        throw InvalidInputException("please type a number, not letters.");
    }

    // A number was typed, but it must be between 1 and 5
    if (choice < MENU_NEW_GAME || choice > MENU_EXIT)
    {
        throw InvalidPositionException(choice, MENU_EXIT);
    }

    return choice;
}

// ---------------------------------------------------------------------
//  TEST HELPER 2 : a tiny "rack" with space for only 3 letters.
//  When a 4th letter is added, it THROWS a RackFullException.
//  (The real TileRack class will be made in Module 5.)
// ---------------------------------------------------------------------
void addLetterToSmallRack(char rack[], int& count, char letter)
{
    const int RACK_SIZE = 3;

    if (count == RACK_SIZE)
    {
        throw RackFullException();
    }

    rack[count] = letter;
    count++;
    cout << "Letter " << letter << " added to the rack." << endl;
}


int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 7 TEST             " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // TEST 1 : Each exception type is thrown and caught one by one.
    // All of them are caught by ONE catch block of the BASE class,
    // and still the correct error type is printed (polymorphism).
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : Throwing every exception type ---" << endl;

    for (int testNumber = 1; testNumber <= 12; testNumber++)
    {
        try
        {
            switch (testNumber)
            {
            case 1:  throw GameException();
            case 2:  throw InvalidInputException("name must not contain spaces.");
            case 3:  throw InvalidPositionException(9, 7);
            case 4:  throw SlotFilledException(3);
            case 5:  throw WordIncompleteException(WORD_2);
            case 6:  throw WordCompleteException(WORD_1);
            case 7:  throw InvalidWordException("GERMAN");
            case 8:  throw RackFullException();
            case 9:  throw RackEmptyException();
            case 10: throw AlreadyVisitedException("Kitchen");
            case 11: throw InterviewDeniedException("Mr.Leonardo", 80);
            case 12: throw FileNotFoundException("clue.txt");
            }
        }
        catch (GameException& error)   // caught by REFERENCE, so no copy is made
        {
            cout << testNumber << ". " << error.getFullMessage() << endl;
        }
    }

    // ------------------------------------------------------------
    // TEST 2 : Exception thrown inside a FUNCTION and caught in main().
    // The rack has space for 3 letters, we try to add 4.
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : Rack full (thrown from a function) ---" << endl;

    char smallRack[3];
    int lettersInRack = 0;
    char lettersToAdd[4] = { 'G', 'E', 'R', 'M' };

    try
    {
        for (int i = 0; i < 4; i++)
        {
            addLetterToSmallRack(smallRack, lettersInRack, lettersToAdd[i]);
        }
    }
    catch (RackFullException& error)   // catching a SPECIFIC exception type
    {
        cout << "Caught: " << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 3 : Wrong user input. The program does NOT crash; it shows
    // the error and asks again until a correct choice is entered.
    // Try typing: 9, then abc, then 2
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Enter a Main Menu choice (1 to 5) ---" << endl;
    cout << "(Try a wrong value first, e.g. 9 or abc)" << endl;

    bool validChoice = false;
    while (validChoice == false)
    {
        try
        {
            int choice = readMenuChoice();
            cout << "You selected: " << menuChoiceToText((MenuChoice)choice) << endl;
            validChoice = true;
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
            cout << "Please try again." << endl;
        }
    }

    // ------------------------------------------------------------
    // TEST 4 : catch(...) catches ANY kind of error, even one that is
    // not a GameException. It is our last safety net.
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : Unknown error with catch(...) ---" << endl;
    try
    {
        throw 404;   // an int, not a GameException
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }
    catch (...)
    {
        cout << "An unknown error was caught safely." << endl;
    }

    cout << "\nModule 7 test finished successfully." << endl;
    system("pause");
    return 0;
}