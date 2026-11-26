// =====================================================================
//  File      : heist.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the IMPLEMENTATION file. It contains the bodies
//              of all functions that were declared in heist.h
// =====================================================================

#include "heist.h"


// =====================================================================
//  HELPER FUNCTIONS (for enums)
// =====================================================================

// Converts a WordRow value into text, e.g. WORD_1 -> "Word 1"
string wordRowToText(WordRow row)
{
    if (row == WORD_1)
    {
        return "Word 1";
    }
    return "Word 2";
}

// Converts a GameState value into text so it can be printed
string gameStateToText(GameState state)
{
    switch (state)
    {
    case STATE_MENU:
        return "Menu";
    case STATE_PLAYING:
        return "Playing";
    case STATE_WON:
        return "Won";
    case STATE_LOST:
        return "Lost";
    case STATE_EXIT:
        return "Exit";
    default:
        return "Unknown";
    }
}

// Converts a MenuChoice value into the text shown on the Main Menu
string menuChoiceToText(MenuChoice choice)
{
    switch (choice)
    {
    case MENU_NEW_GAME:
        return "New Game";
    case MENU_LEADERBOARD:
        return "View Leader/Score Board";
    case MENU_ABOUT:
        return "About";
    case MENU_RULES:
        return "Rules";
    case MENU_EXIT:
        return "Exit";
    default:
        return "Unknown";
    }
}


// =====================================================================
//  MODULE 7 : EXCEPTION HANDLING
// =====================================================================

// ---------------- Base class : GameException ----------------

// Constructor: saves the error message
GameException::GameException(string errorMessage)
{
    message = errorMessage;
}

// Returns only the error details
string GameException::getMessage() const
{
    return message;
}

// Base class name. Derived classes override this function.
string GameException::getErrorType() const
{
    return "Game Error";
}

// Joins the error type and the message, e.g. "[Rack Full] The rack ..."
// getErrorType() is virtual, so the derived class version is called here.
string GameException::getFullMessage() const
{
    return "[" + getErrorType() + "] " + message;
}

// Destructor: nothing to free, but it is virtual for safe deletion
GameException::~GameException()
{
}


// ---------------- Derived exception classes ----------------
// Each constructor builds its own message and passes it to the
// base class constructor (GameException) using the initializer list.

InvalidInputException::InvalidInputException(string details)
    : GameException("Invalid input: " + details)
{
}

string InvalidInputException::getErrorType() const
{
    return "Invalid Input";
}


InvalidPositionException::InvalidPositionException(int position, int maxPosition)
    : GameException("Position " + to_string(position) +
        " is not valid. Please choose between 1 and " +
        to_string(maxPosition) + ".")
{
}

string InvalidPositionException::getErrorType() const
{
    return "Invalid Position";
}


SlotFilledException::SlotFilledException(int position)
    : GameException("Slot " + to_string(position) +
        " on the board is already filled.")
{
}

string SlotFilledException::getErrorType() const
{
    return "Slot Filled";
}


WordIncompleteException::WordIncompleteException(WordRow row)
    : GameException(wordRowToText(row) +
        " is not complete yet. Collect all its letters first.")
{
}

string WordIncompleteException::getErrorType() const
{
    return "Word Incomplete";
}


WordCompleteException::WordCompleteException(WordRow row)
    : GameException(wordRowToText(row) +
        " is already complete. No more letters can be placed.")
{
}

string WordCompleteException::getErrorType() const
{
    return "Word Complete";
}


InvalidWordException::InvalidWordException(string typedWord)
    : GameException("\"" + typedWord + "\" is not the correct word.")
{
}

string InvalidWordException::getErrorType() const
{
    return "Wrong Word";
}


RackFullException::RackFullException()
    : GameException("The tile rack is full. No more letters can be added.")
{
}

string RackFullException::getErrorType() const
{
    return "Rack Full";
}


RackEmptyException::RackEmptyException()
    : GameException("The tile rack is empty. There is no letter to remove.")
{
}

string RackEmptyException::getErrorType() const
{
    return "Rack Empty";
}


AlreadyVisitedException::AlreadyVisitedException(string placeOrPerson)
    : GameException("The clue from " + placeOrPerson +
        " has already been collected.")
{
}

string AlreadyVisitedException::getErrorType() const
{
    return "Already Visited";
}


InterviewDeniedException::InterviewDeniedException(string personName, int requiredCredibility)
    : GameException(personName + " refuses to talk. You need at least " +
        to_string(requiredCredibility) + " credibility.")
{
}

string InterviewDeniedException::getErrorType() const
{
    return "Interview Denied";
}


FileNotFoundException::FileNotFoundException(string fileName)
    : GameException("The file \"" + fileName + "\" was not found.")
{
}

string FileNotFoundException::getErrorType() const
{
    return "File Not Found";
}


FileCorruptedException::FileCorruptedException(string fileName, string details)
    : GameException("The file \"" + fileName + "\" is corrupted: " + details)
{
}

string FileCorruptedException::getErrorType() const
{
    return "File Corrupted";
}


// =====================================================================
//  MODULE 3 : CHARACTER HIERARCHY  (Part A)
// =====================================================================

// ---------------- Class: Character ----------------

// Constructor: saves the common data. Nobody is the culprit at the start;
// the GameEngine will select the culprit later.
Character::Character(string name, string role, string nationalityText)
{
    characterName = name;
    characterRole = role;
    nationality = nationalityText;
    isCulprit = false;
}

// Virtual destructor (no heap memory in Character itself)
Character::~Character()
{
}

// Prints the common details. Used by every child's display().
void Character::displayBasicInfo() const
{
    cout << "Name        : " << characterName << endl;
    cout << "Role        : " << characterRole << endl;
    cout << "Nationality : " << nationality << endl;
}

string Character::getName() const
{
    return characterName;
}

string Character::getRole() const
{
    return characterRole;
}

string Character::getNationality() const
{
    return nationality;
}

bool Character::getIsCulprit() const
{
    return isCulprit;
}

void Character::setName(string name)
{
    characterName = name;
}

void Character::setNationality(string nationalityText)
{
    nationality = nationalityText;
}

// Base version: no condition (0). Child classes override this.
int Character::getRequiredCredibility() const
{
    return 0;
}

// Checks the condition with the help of the virtual function above.
// The correct child version of getRequiredCredibility() is called at runtime.
bool Character::canBeInterviewed(int credibility) const
{
    return credibility >= getRequiredCredibility();
}

// Base version: returns an empty clue (used by the Detective)
Clue Character::getClue() const
{
    Clue emptyClue;
    return emptyClue;
}


// ---------------- Class: Victim ----------------

// The role is always "Victim" and the letter always belongs to Word 2
Victim::Victim(string name, string nationalityText, char letter, int position)
    : Character(name, "Victim", nationalityText)
{
    clueRowChar = WORD_2;
    letterClueChar = letter;
    boardPosChar = position;
}

char Victim::getLetter() const
{
    return letterClueChar;
}

int Victim::getPosition() const
{
    return boardPosChar;
}

WordRow Victim::getRow() const
{
    return clueRowChar;
}

// Packs the letter, position and row into a Clue (the carrier)
Clue Victim::getClue() const
{
    Clue victimClue;
    victimClue.clueLetter = letterClueChar;
    victimClue.cluePosition = boardPosChar;
    victimClue.clueRow = clueRowChar;
    return victimClue;
}

int Victim::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 80
}

void Victim::display() const
{
    cout << "------ Victim (Business Owner) ------" << endl;
    displayBasicInfo();
    cout << "Note        : Owner of the robbed locker. Not a suspect." << endl;
    cout << "Interview   : Needs credibility >= " << REQUIRED_CREDIBILITY << endl;
}


// ---------------- Class: Detective ----------------

// The detective's role is "Detective" and default nationality is "Malay"
Detective::Detective(string name, int startingCredibility)
    : Character(name, "Detective", "Malay")
{
    credibility = startingCredibility;
    turns = 0;
    solvedWords = "";
}

// Nothing on the heap yet. In Module 5 the racks will be added and this
// destructor will make sure their memory is freed.
Detective::~Detective()
{
}

int Detective::getCredibility() const
{
    return credibility;
}

int Detective::getTurns() const
{
    return turns;
}

string Detective::getSolvedWords() const
{
    return solvedWords;
}

// GameEngine counts the turns and stores the new value here
void Detective::setTurns(int turnCount)
{
    turns = turnCount;
}

// Reduces credibility after a wrong step. It never becomes negative.
void Detective::loseCredibility(int amount)
{
    credibility = credibility - amount;

    if (credibility < 0)
    {
        credibility = 0;
    }
}

// Adds a correctly solved word, separated by a space
void Detective::addSolvedWord(string word)
{
    if (solvedWords == "")
    {
        solvedWords = word;
    }
    else
    {
        solvedWords = solvedWords + " " + word;
    }
}

void Detective::display() const
{
    cout << "------ Detective (Player) ------" << endl;
    displayBasicInfo();
    cout << "Credibility : " << credibility << endl;
    cout << "Turns used  : " << turns << endl;
    cout << "Solved words: " << (solvedWords == "" ? "(none yet)" : solvedWords) << endl;
}


// ---------------- Class: Suspect ----------------

Suspect::Suspect(string name, string role, string nationalityText, char letter, int position)
    : Character(name, role, nationalityText)
{
    clueRowChar = WORD_2;
    letterClueChar = letter;
    boardPosChar = position;
}

char Suspect::getLetter() const
{
    return letterClueChar;
}

int Suspect::getPosition() const
{
    return boardPosChar;
}

WordRow Suspect::getRow() const
{
    return clueRowChar;
}

// Packs the suspect's letter into a Clue (the carrier)
Clue Suspect::getClue() const
{
    Clue suspectClue;
    suspectClue.clueLetter = letterClueChar;
    suspectClue.cluePosition = boardPosChar;
    suspectClue.clueRow = clueRowChar;
    return suspectClue;
}


// ---------------- Class: HouseHelp ----------------

HouseHelp::HouseHelp(string name, string role, string nationalityText, char letter, int position)
    : Suspect(name, role, nationalityText, letter, position)
{
}

// House Help can be interviewed at any time
int HouseHelp::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 0
}


// ---------------- Classes: Cook, Guard, Servant ----------------
// Each constructor fixes its own role name and passes the rest to HouseHelp.
// Each class writes its OWN display() (method overriding).

Cook::Cook(string name, string nationalityText, char letter, int position)
    : HouseHelp(name, "Cook", nationalityText, letter, position)
{
}

void Cook::display() const
{
    cout << "------ House Help : Cook ------" << endl;
    displayBasicInfo();
    cout << "Note        : Prepared the food for the party." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}

Guard::Guard(string name, string nationalityText, char letter, int position)
    : HouseHelp(name, "Guard", nationalityText, letter, position)
{
}

void Guard::display() const
{
    cout << "------ House Help : Guard ------" << endl;
    displayBasicInfo();
    cout << "Note        : Was guarding the cottage gate." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}

Servant::Servant(string name, string nationalityText, char letter, int position)
    : HouseHelp(name, "Servant", nationalityText, letter, position)
{
}

void Servant::display() const
{
    cout << "------ House Help : Servant ------" << endl;
    displayBasicInfo();
    cout << "Note        : Served the guests in every room." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}


// =====================================================================
//  MODULE 3 : CHARACTER HIERARCHY  (Part B)
// ---------------------------------------------------------------------
//  IMPORTANT RULE OF VIRTUAL INHERITANCE:
//  A virtual base class (Suspect) is always built by the LAST class in
//  the chain (the object we actually create: Partner, OwnerChild or PA).
//  That is why Partner, OwnerChild and PA call Suspect(...) directly,
//  and the middle classes (BusinessRelations, CloseRelations) do not.
// =====================================================================

// ---------------- Class: BusinessRelations ----------------

// Empty: the shared Suspect part is built by Partner or PA
BusinessRelations::BusinessRelations()
{
}

int BusinessRelations::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 50
}


// ---------------- Class: CloseRelations ----------------

// Empty: the shared Suspect part is built by OwnerChild or PA
CloseRelations::CloseRelations()
{
}

int CloseRelations::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 75
}


// ---------------- Class: Partner ----------------
// Suspect(...) is called here because it is a virtual base class.

Partner::Partner(string name, string role, string nationalityText, char letter, int position)
    : Suspect(name, role, nationalityText, letter, position),
    BusinessRelations()
{
}

void Partner::display() const
{
    cout << "------ Business Relation : " << characterRole << " ------" << endl;
    displayBasicInfo();
    cout << "Note        : Owns a share in the owner's business." << endl;
    cout << "Interview   : Needs credibility >= " << getRequiredCredibility() << endl;
}


// ---------------- Class: OwnerChild ----------------

OwnerChild::OwnerChild(string name, string nationalityText, char letter, int position)
    : Suspect(name, "Child", nationalityText, letter, position),
    CloseRelations()
{
}

void OwnerChild::display() const
{
    cout << "------ Close Relation : Owner's Child ------" << endl;
    displayBasicInfo();
    cout << "Note        : A teenager (15-19), panicked after the theft." << endl;
    cout << "Interview   : Needs credibility >= " << getRequiredCredibility() << endl;
}


// ---------------- Class: PA (the diamond) ----------------
// PA builds the ONE shared Suspect part itself, and then its two
// parents. Because of virtual inheritance, Suspect is created only once.

PA::PA(string name, string nationalityText, char letter, int position)
    : Suspect(name, "PA", nationalityText, letter, position),
    BusinessRelations(),
    CloseRelations()
{
}

// PA has its own condition (35). Without this function the program
// would not compile, because PA gets two different versions (50 and 75)
// from its two parents.
int PA::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 35
}

void PA::display() const
{
    cout << "------ Business + Close Relation : PA ------" << endl;
    displayBasicInfo();
    cout << "Note        : Right-hand of the owner, knows all his secrets." << endl;
    cout << "Interview   : Needs credibility >= " << REQUIRED_CREDIBILITY << endl;
}
