// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. Later it will only start the
//              GameEngine. For now (Module 6) it is used to TEST reading
//              the .txt files and the Leader/Score Board.
//  NOTE      : The 6 .txt files must be in the project folder.
// =====================================================================

#include "heist.h"

int main()
{
    cout << "==================================================" << endl;
    cout << "     THE LOCKER HEIST - MODULE 6 TEST             " << endl;
    cout << "==================================================" << endl;

    // ------------------------------------------------------------
    // TEST 1 : about.txt
    // ------------------------------------------------------------
    cout << "\n--- TEST 1 : about.txt ---" << endl;
    try
    {
        GameFileReader::showTextFile(ABOUT_FILE);
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 2 : rules.txt, and the default values when the file is missing
    // ------------------------------------------------------------
    cout << "\n--- TEST 2 : rules.txt ---" << endl;
    GameRules rules;
    try
    {
        rules = GameFileReader::readRules();
        cout << "Rules loaded from " << RULES_FILE << endl;
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
        cout << "Using the default rules." << endl;
    }
    cout << "Max turns            : " << rules.maxTurns << endl;
    cout << "Starting credibility : " << rules.startingCredibility << endl;
    cout << "Wrong turn penalty   : -" << rules.wrongTurnPenalty << endl;
    cout << "Wrong word penalty   : -" << rules.wrongWordPenalty << endl;
    cout << "Win condition        : " << replaceUnderscores(rules.winCondition) << endl;
    cout << "Lose condition       : " << replaceUnderscores(rules.loseCondition) << endl;

    cout << "\nNow trying a file that does not exist:" << endl;
    GameRules backupRules;
    try
    {
        backupRules = GameFileReader::readRules("missing_rules.txt");
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
        cout << "Fall back to defaults -> max turns " << backupRules.maxTurns
            << ", credibility " << backupRules.startingCredibility << endl;
    }

    // ------------------------------------------------------------
    // TEST 3 : Pick a RANDOM culprit profile from clue.txt
    // ------------------------------------------------------------
    cout << "\n--- TEST 3 : Random culprit from clue.txt ---" << endl;
    srand((unsigned int)time(0));         // new random numbers every run

    int profileNumber = 2;                // used if something goes wrong
    string culpritRole;
    string word1;
    string word2;

    try
    {
        int totalProfiles = GameFileReader::countProfiles();
        profileNumber = rand() % totalProfiles + 1;          // 1 to 7
        GameFileReader::readClueProfile(profileNumber, culpritRole, word1, word2);

        cout << "Profiles in clue.txt : " << totalProfiles << endl;
        cout << "Random profile       : " << profileNumber << endl;
        cout << "Culprit role         : " << culpritRole << endl;
        cout << "Word 1 / Word 2      : " << word1 << " / " << word2 << endl;
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 4 : locations.txt -> Cottage (letters of the random culprit)
    // ------------------------------------------------------------
    cout << "\n--- TEST 4 : locations.txt ---" << endl;
    Cottage cottage;
    Scrabble scrabble;

    try
    {
        scrabble.setTargetWords(word1, word2);
        GameFileReader::loadLocations(cottage, profileNumber);
        cottage.displayLocations();

        // search every location and put the letters on the board
        for (int number = 1; number <= cottage.getLocationCount(); number++)
        {
            scrabble.placeLetter(cottage.searchLocation(number));
        }
        cout << "Word 1 built from the file: " << scrabble.getPlacedWord(WORD_1)
            << "  (expected " << word1 << ")" << endl;
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 5 : characters.txt -> 8 people (letters of the random culprit)
    // ------------------------------------------------------------
    cout << "\n--- TEST 5 : characters.txt ---" << endl;
    const int MAX_PEOPLE = 8;
    Character* people[MAX_PEOPLE];
    int peopleCount = 0;

    try
    {
        peopleCount = GameFileReader::loadCharacters(people, MAX_PEOPLE, profileNumber);
        cout << "People loaded: " << peopleCount << endl;

        for (int i = 0; i < peopleCount; i++)
        {
            Clue personClue = people[i]->getClue();
            cout << "  " << padRight(people[i]->getName(), 13)
                << padRight(people[i]->getRole(), 11)
                << padRight(people[i]->getNationality(), 11)
                << "letter " << personClue.clueLetter << " at " << personClue.cluePosition;

            if (people[i]->getRole() == culpritRole)
            {
                cout << "   <-- culprit";
            }
            cout << endl;

            scrabble.placeLetter(personClue);
        }
        cout << "Word 2 built from the file: " << scrabble.getPlacedWord(WORD_2)
            << "  (expected " << word2 << ")" << endl;
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    cout << endl << scrabble;

    // ------------------------------------------------------------
    // TEST 6 : Leader / Score Board
    // The real scoreBoard.txt is only READ here. The test result is
    // written to a separate file, so the real board stays clean.
    // ------------------------------------------------------------
    cout << "\n--- TEST 6 : Leader / Score Board ---" << endl;
    try
    {
        ScoreBoard realBoard;                         // scoreBoard.txt
        realBoard.displayScores();

        ScoreBoard testBoard("scoreBoard_test.txt");
        testBoard.addScore("Test_Detective", true, 70, 12, culpritRole);
        cout << "\nOne test result was saved in scoreBoard_test.txt:" << endl;
        testBoard.displayScores();
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // ------------------------------------------------------------
    // TEST 7 : A file that does not exist
    // ------------------------------------------------------------
    cout << "\n--- TEST 7 : Missing file ---" << endl;
    try
    {
        GameFileReader::showTextFile("no_such_file.txt");
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }

    // free the people created by loadCharacters()
    for (int i = 0; i < peopleCount; i++)
    {
        delete people[i];
        people[i] = nullptr;
    }

    cout << "\nModule 6 test finished successfully." << endl;
    return 0;
}
