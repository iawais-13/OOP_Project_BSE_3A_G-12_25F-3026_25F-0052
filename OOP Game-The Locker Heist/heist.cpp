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

// The detective's role is "Detective" and default nationality is "Malay".
// The racks are member objects, so they are built in the initializer list
// with their sizes: 7 letters for Word 1 and 8 letters for Word 2.
Detective::Detective(string name, int startingCredibility)
    : Character(name, "Detective", "Malay"),
    word1Rack(7),
    word2Rack(8)
{
    credibility = startingCredibility;
    turns = 0;
    solvedWords = "";
}

// The racks free their own heap memory in ~TileRack(), which runs
// automatically after this destructor (composition).
Detective::~Detective()
{
}

// Returns a rack by const reference: the caller can read it, not change it
const TileRack& Detective::getRack(WordRow row) const
{
    if (row == WORD_1)
    {
        return word1Rack;
    }
    return word2Rack;
}

// Puts the clue in the rack of its own word
void Detective::collectClue(Clue clue)
{
    if (clue.clueRow == WORD_1)
    {
        word1Rack.addTile(clue);
    }
    else
    {
        word2Rack.addTile(clue);
    }
}

// Shows the letters of both racks
void Detective::displayRacks() const
{
    cout << "Rack (Word 1): ";
    word1Rack.display();
    cout << "Rack (Word 2): ";
    word2Rack.display();
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
    displayRacks();
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


// =====================================================================
//  HELPER FUNCTION : replaceUnderscores
//  Goes through the text letter by letter and puts a space wherever
//  it finds an underscore. "Bedroom_1" -> "Bedroom 1"
// =====================================================================
string replaceUnderscores(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '_')
        {
            text[i] = ' ';
        }
    }
    return text;
}


// =====================================================================
//  MODULE 4 : GAME BOARDS (DMA)  -  Part A : Cottage
// =====================================================================

// ---------------------------------------------------------------------
//  OLD RAW DMA LOGIC (before Module 8: Templates)
//  Kept as comments, as the manual asks. The new version is below.
// ---------------------------------------------------------------------
/*
// Constructor: creates the array of 7 pointers, and then one empty
// Location on the heap for every pointer.
Cottage::Cottage()
{
    locations = new Location*[TOTAL_LOCATIONS];

    for (int i = 0; i < TOTAL_LOCATIONS; i++)
    {
        locations[i] = new Location;
    }
}

// Copy constructor: "Cottage copy = original;"
// It must NOT share memory with the original, so we make a deep copy.
Cottage::Cottage(const Cottage& other)
{
    copyLocationsFrom(other);
}

// Assignment operator: "copy = original;" (both objects already exist)
Cottage& Cottage::operator=(const Cottage& other)
{
    // Self-assignment check: "cottage = cottage;" must not delete itself
    if (this != &other)
    {
        freeMemory();                 // 1. free the old memory
        copyLocationsFrom(other);     // 2. make a deep copy of the other one
    }
    return *this;                     // 3. allows chaining: a = b = c;
}

// Destructor: called automatically when a Cottage object is destroyed
Cottage::~Cottage()
{
    freeMemory();
}

// DEEP COPY: creates NEW memory and copies the VALUES into it.
// (A shallow copy would only copy the pointer, and then both cottages
// would point to the same memory -> deleting one would break the other.)
void Cottage::copyLocationsFrom(const Cottage& other)
{
    locations = new Location*[TOTAL_LOCATIONS];

    for (int i = 0; i < TOTAL_LOCATIONS; i++)
    {
        // new Location filled with a copy of the other location's data
        locations[i] = new Location(*other.locations[i]);
    }
}

// Deletes each location first, then the array of pointers itself
void Cottage::freeMemory()
{
    if (locations != nullptr)
    {
        for (int i = 0; i < TOTAL_LOCATIONS; i++)
        {
            delete locations[i];
            locations[i] = nullptr;
        }

        delete[] locations;
        locations = nullptr;   // no dangling pointer
    }
}
*/
// ---------------------------------------------------------------------
//  NEW TEMPLATE LOGIC (Module 8)
// ---------------------------------------------------------------------

// Constructor: DynamicArray creates the 7 empty locations for us
Cottage::Cottage()
    : locations(TOTAL_LOCATIONS)
{
}

// Copy constructor: "Cottage copy = original;" (deep copy)
// DynamicArray's own copy constructor makes new memory and copies the data.
Cottage::Cottage(const Cottage& other)
    : locations(other.locations)
{
}

// Assignment operator: "copy = original;" (deep copy)
Cottage& Cottage::operator=(const Cottage& other)
{
    if (this != &other)               // self-assignment check
    {
        locations = other.locations;  // DynamicArray's operator= does the deep copy
    }
    return *this;
}

// Destructor: nothing to delete here. ~DynamicArray() deletes every
// location and the array itself automatically.
Cottage::~Cottage()
{
}

// The player uses numbers 1 to 7. Anything else is an invalid position.
void Cottage::checkLocationNumber(int number) const
{
    if (number < 1 || number > TOTAL_LOCATIONS)
    {
        throw InvalidPositionException(number, TOTAL_LOCATIONS);
    }
}

// Fills a location. Number 1 is stored at index 0, and so on.
void Cottage::setLocation(int number, string name, char letter, int position)
{
    checkLocationNumber(number);

    Location& place = locations[number - 1];   // reference to the real location
    place.locName = name;
    place.letterClueLoc = letter;
    place.boardPosLoc = position;
    place.locRow = WORD_1;
    place.locSearchFlag = false;
}

// Searches a location: marks it as searched and gives back its clue.
// Searching again is allowed (the player sees the same letter again).
Clue Cottage::searchLocation(int number)
{
    checkLocationNumber(number);

    Location& place = locations[number - 1];   // reference to the real location
    place.locSearchFlag = true;

    // Clue is the carrier that takes the letter to the Scrabble board
    Clue foundClue;
    foundClue.clueLetter = place.letterClueLoc;
    foundClue.cluePosition = place.boardPosLoc;
    foundClue.clueRow = place.locRow;
    return foundClue;
}

bool Cottage::isSearched(int number) const
{
    checkLocationNumber(number);
    return locations[number - 1].locSearchFlag;
}

string Cottage::getLocationName(int number) const
{
    checkLocationNumber(number);
    return locations[number - 1].locName;
}

int Cottage::getLocationCount() const
{
    return TOTAL_LOCATIONS;
}

// Counts how many locations have been searched so far
int Cottage::getSearchedCount() const
{
    int count = 0;
    for (int i = 0; i < TOTAL_LOCATIONS; i++)
    {
        if (locations[i].locSearchFlag == true)
        {
            count++;
        }
    }
    return count;
}

// Prints the location menu, e.g.  [3] Study Room  - NOT Searched
void Cottage::displayLocations() const
{
    cout << "====== Location Menu ======" << endl;

    for (int i = 0; i < TOTAL_LOCATIONS; i++)
    {
        // Module 11: the overloaded << of Location prints name and status
        cout << "[" << (i + 1) << "] " << locations[i] << endl;
    }

    cout << "===========================" << endl;
}


// =====================================================================
//  HELPER FUNCTION : toUpperCase
//  Changes every small letter into a capital letter.
//  In the ASCII table a small letter is 32 places after its capital,
//  e.g. 'a' = 97 and 'A' = 65, so we subtract 32.
// =====================================================================
string toUpperCase(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] = text[i] - 32;
        }
    }
    return text;
}


// =====================================================================
//  MODULE 4 : GAME BOARDS (DMA)  -  Part B : Scrabble
// =====================================================================

// ---------------------------------------------------------------------
//  OLD RAW DMA LOGIC (before Module 8: Templates)
//  Kept as comments, as the manual asks. The new version is below.
// ---------------------------------------------------------------------
/*
// Constructor: creates an empty jagged board
Scrabble::Scrabble()
{
    targetWord1 = "";
    targetWord2 = "";
    createEmptyBoard();
}

// Copy constructor: "Scrabble copy = original;" (deep copy)
Scrabble::Scrabble(const Scrabble& other)
{
    copyBoardFrom(other);
}

// Assignment operator: "copy = original;" (deep copy)
Scrabble& Scrabble::operator=(const Scrabble& other)
{
    if (this != &other)          // self-assignment check
    {
        freeMemory();            // free old memory first
        copyBoardFrom(other);    // then make a deep copy
    }
    return *this;
}

// Destructor: frees both rows and the board
Scrabble::~Scrabble()
{
    freeMemory();
}

// Allocates the jagged array: 2 row pointers, then each row with its own size.
// Every Tile starts as empty (' ' and false) because of the struct defaults.
void Scrabble::createEmptyBoard()
{
    board = new Tile*[ROW_COUNT];
    board[WORD_1] = new Tile[WORD1_LENGTH];   // row 1 -> 7 slots
    board[WORD_2] = new Tile[WORD2_LENGTH];   // row 2 -> 8 slots
}

// DEEP COPY: new memory for both rows, then copy every tile one by one
void Scrabble::copyBoardFrom(const Scrabble& other)
{
    targetWord1 = other.targetWord1;
    targetWord2 = other.targetWord2;

    createEmptyBoard();

    for (int i = 0; i < WORD1_LENGTH; i++)
    {
        board[WORD_1][i] = other.board[WORD_1][i];
    }
    for (int i = 0; i < WORD2_LENGTH; i++)
    {
        board[WORD_2][i] = other.board[WORD_2][i];
    }
}

// Deletes each row first, then the array of row pointers
void Scrabble::freeMemory()
{
    if (board != nullptr)
    {
        delete[] board[WORD_1];
        delete[] board[WORD_2];
        delete[] board;
        board = nullptr;          // no dangling pointer
    }
}
*/
// ---------------------------------------------------------------------
//  NEW TEMPLATE LOGIC (Module 8)
// ---------------------------------------------------------------------

// Constructor: makes the outer array with 2 rows, then gives each row
// its own length. Every Tile starts empty because of the struct defaults.
Scrabble::Scrabble()
    : board(ROW_COUNT)
{
    targetWord1 = "";
    targetWord2 = "";

    board[WORD_1] = DynamicArray<Tile>(WORD1_LENGTH);   // row 1 -> 7 slots
    board[WORD_2] = DynamicArray<Tile>(WORD2_LENGTH);   // row 2 -> 8 slots
}

// Copy constructor (deep copy): DynamicArray copies both rows for us
Scrabble::Scrabble(const Scrabble& other)
    : board(other.board)
{
    targetWord1 = other.targetWord1;
    targetWord2 = other.targetWord2;
}

// Assignment operator (deep copy)
Scrabble& Scrabble::operator=(const Scrabble& other)
{
    if (this != &other)          // self-assignment check
    {
        board = other.board;     // DynamicArray's operator= copies every row
        targetWord1 = other.targetWord1;
        targetWord2 = other.targetWord2;
    }
    return *this;
}

// Destructor: nothing to delete here. ~DynamicArray() frees each row
// and then the outer array automatically.
Scrabble::~Scrabble()
{
}

// Row 1 accepts positions 1-7 and row 2 accepts positions 1-8
void Scrabble::checkPosition(WordRow row, int position) const
{
    int rowLength = getRowLength(row);

    if (position < 1 || position > rowLength)
    {
        throw InvalidPositionException(position, rowLength);
    }
}

// Saves the correct words after checking their length.
// A wrong length means clue.txt has wrong data.
void Scrabble::setTargetWords(string word1, string word2)
{
    if ((int)word1.length() != WORD1_LENGTH)
    {
        throw FileCorruptedException("clue.txt", "Word 1 \"" + word1 + "\" must have 7 letters.");
    }
    if ((int)word2.length() != WORD2_LENGTH)
    {
        throw FileCorruptedException("clue.txt", "Word 2 \"" + word2 + "\" must have 8 letters.");
    }

    targetWord1 = toUpperCase(word1);
    targetWord2 = toUpperCase(word2);
}

// Places a letter on the board. Three checks are done first:
//  1. the word (row) must not be complete already
//  2. the position must be inside the row
//  3. the slot must still be empty
void Scrabble::placeLetter(Clue clue)
{
    if (isRowComplete(clue.clueRow))
    {
        throw WordCompleteException(clue.clueRow);
    }

    checkPosition(clue.clueRow, clue.cluePosition);

    Tile& slot = board[clue.clueRow][clue.cluePosition - 1];   // position 1 -> index 0

    if (slot.filled)
    {
        throw SlotFilledException(clue.cluePosition);
    }

    slot.tileLetter = clue.clueLetter;
    slot.filled = true;
}

// Shows the letter that was placed at a position (used when the player
// visits a place or a person again)
char Scrabble::revealLetter(WordRow row, int position) const
{
    checkPosition(row, position);
    return board[row][position - 1].tileLetter;
}

int Scrabble::getRowLength(WordRow row) const
{
    if (row == WORD_1)
    {
        return WORD1_LENGTH;
    }
    return WORD2_LENGTH;
}

bool Scrabble::isSlotFilled(WordRow row, int position) const
{
    checkPosition(row, position);
    return board[row][position - 1].filled;
}

// Counts the filled slots of one row
int Scrabble::getFilledCount(WordRow row) const
{
    int count = 0;
    for (int i = 0; i < getRowLength(row); i++)
    {
        if (board[row][i].filled)
        {
            count++;
        }
    }
    return count;
}

// A row is complete when every slot is filled
bool Scrabble::isRowComplete(WordRow row) const
{
    return getFilledCount(row) == getRowLength(row);
}

// Joins the placed letters into a string, e.g. "GER_A_S" (empty = '_')
string Scrabble::getPlacedWord(WordRow row) const
{
    string word = "";
    for (int i = 0; i < getRowLength(row); i++)
    {
        if (board[row][i].filled)
        {
            word = word + board[row][i].tileLetter;
        }
        else
        {
            word = word + '_';
        }
    }
    return word;
}

string Scrabble::getTargetWord(WordRow row) const
{
    if (row == WORD_1)
    {
        return targetWord1;
    }
    return targetWord2;
}

// The detective can type a word only when its row is complete.
// Small letters are allowed, so both words are changed to capitals first.
bool Scrabble::isWordCorrect(WordRow row, string typedWord) const
{
    if (isRowComplete(row) == false)
    {
        throw WordIncompleteException(row);
    }

    typedWord = toUpperCase(typedWord);
    string correctWord = getTargetWord(row);

    // Different length -> it cannot be the same word
    if (typedWord.length() != correctWord.length())
    {
        return false;
    }

    // Module 11: compare letter by letter with the overloaded != of Tile
    // (each letter of the typed word against the letter from clue.txt)
    for (int i = 0; i < (int)correctWord.length(); i++)
    {
        Tile typedTile;
        typedTile.tileLetter = typedWord[i];

        Tile correctTile;
        correctTile.tileLetter = correctWord[i];

        if (typedTile != correctTile)
        {
            return false;
        }
    }
    return true;
}

// Prints one row like:  Word 1: [G] [E] [ ] [M] [ ] [ ] [S]
void Scrabble::displayWord(WordRow row) const
{
    cout << "  " << wordRowToText(row) << ": ";

    for (int i = 0; i < getRowLength(row); i++)
    {
        cout << board[row][i] << " ";     // Module 11: Tile's << prints [G]
    }

    if (isRowComplete(row))
    {
        cout << "-> COMPLETE";
    }
    cout << endl;
}

// Prints the whole board (both rows) with the overloaded << of Scrabble
void Scrabble::displayBoard() const
{
    cout << *this;
}


// =====================================================================
//  MODULE 5 : CLUE SYSTEM  -  TileRack
// =====================================================================

// ---------------------------------------------------------------------
//  OLD RAW DMA LOGIC (before Module 8: Templates)
//  Kept as comments, as the manual asks. The new version is below.
// ---------------------------------------------------------------------
/*
// Constructor: creates two empty arrays of the given size
TileRack::TileRack(int rackCapacity)
{
    if (rackCapacity < 1)
    {
        rackCapacity = 1;    // a rack must have space for at least one letter
    }

    capacity = rackCapacity;
    count = 0;
    tiles = new Tile[capacity];
    positions = new int[capacity];

    for (int i = 0; i < capacity; i++)
    {
        positions[i] = 0;    // 0 = no position yet
    }
}

// Copy constructor: "TileRack copy = original;" (deep copy)
TileRack::TileRack(const TileRack& other)
{
    copyRackFrom(other);
}

// Assignment operator: "copy = original;" (deep copy)
TileRack& TileRack::operator=(const TileRack& other)
{
    if (this != &other)          // self-assignment check
    {
        freeMemory();
        copyRackFrom(other);
    }
    return *this;
}

// Destructor: frees both arrays
TileRack::~TileRack()
{
    freeMemory();
}

// DEEP COPY: new arrays of the same size, then copy every value
void TileRack::copyRackFrom(const TileRack& other)
{
    capacity = other.capacity;
    count = other.count;
    tiles = new Tile[capacity];
    positions = new int[capacity];

    for (int i = 0; i < capacity; i++)
    {
        tiles[i] = other.tiles[i];
        positions[i] = other.positions[i];
    }
}

void TileRack::freeMemory()
{
    delete[] tiles;
    delete[] positions;
    tiles = nullptr;
    positions = nullptr;
}
*/
// ---------------------------------------------------------------------
//  NEW TEMPLATE LOGIC (Module 8)
// ---------------------------------------------------------------------

// Constructor: the two DynamicArray members are created in the
// initializer list with the rack's size (DynamicArray does the "new").
TileRack::TileRack(int rackCapacity)
    : tiles(rackCapacity), positions(rackCapacity)
{
    capacity = tiles.getSize();   // DynamicArray makes the size at least 1
    count = 0;

    for (int i = 0; i < capacity; i++)
    {
        positions[i] = 0;    // 0 = no position yet
    }
}

// Copy constructor (deep copy): DynamicArray's own copy constructor
// copies the arrays, we only copy the two numbers.
TileRack::TileRack(const TileRack& other)
    : tiles(other.tiles), positions(other.positions)
{
    capacity = other.capacity;
    count = other.count;
}

// Assignment operator (deep copy): DynamicArray's operator= copies the arrays
TileRack& TileRack::operator=(const TileRack& other)
{
    if (this != &other)          // self-assignment check
    {
        tiles = other.tiles;
        positions = other.positions;
        capacity = other.capacity;
        count = other.count;
    }
    return *this;
}

// Destructor: nothing to delete here. The DynamicArray members free
// their own memory in ~DynamicArray().
TileRack::~TileRack()
{
}

// Adds a letter at the end of the rack. Two checks first:
//  1. the rack must not be full
//  2. the same board position must not be collected twice
void TileRack::addTile(Clue clue)
{
    if (isFull())
    {
        throw RackFullException();
    }

    if (hasPosition(clue.cluePosition))
    {
        throw AlreadyVisitedException("position " + to_string(clue.cluePosition));
    }

    tiles[count].tileLetter = clue.clueLetter;
    tiles[count].filled = true;
    positions[count] = clue.cluePosition;
    count++;
}

// Removes the last letter (like taking the top plate from a pile)
char TileRack::removeLastTile()
{
    if (isEmpty())
    {
        throw RackEmptyException();
    }

    count--;
    char removedLetter = tiles[count].tileLetter;

    // clear the slot so it is empty again
    tiles[count].tileLetter = ' ';
    tiles[count].filled = false;
    positions[count] = 0;

    return removedLetter;
}

bool TileRack::isFull() const
{
    return count == capacity;
}

bool TileRack::isEmpty() const
{
    return count == 0;
}

int TileRack::getCount() const
{
    return count;
}

int TileRack::getCapacity() const
{
    return capacity;
}

// Checks if a letter for this board position is already in the rack
bool TileRack::hasPosition(int position) const
{
    for (int i = 0; i < count; i++)
    {
        if (positions[i] == position)
        {
            return true;
        }
    }
    return false;
}

char TileRack::getLetterAt(int index) const
{
    if (index < 0 || index >= count)
    {
        throw InvalidPositionException(index + 1, count);
    }
    return tiles[index].tileLetter;
}

int TileRack::getPositionAt(int index) const
{
    if (index < 0 || index >= count)
    {
        throw InvalidPositionException(index + 1, count);
    }
    return positions[index];
}

// Letters in the order they were collected, e.g. "RGA"
string TileRack::getLettersInOrder() const
{
    string letters = "";
    for (int i = 0; i < count; i++)
    {
        letters = letters + tiles[i].tileLetter;
    }
    return letters;
}

// Letters arranged by their board position. Missing letters are '_'.
// Example: R at 3, G at 1, A at 5 in a rack of 7 -> "G_R_A__"
string TileRack::getLettersByPosition() const
{
    string word(capacity, '_');      // a string of '_' with the rack's length

    for (int i = 0; i < count; i++)
    {
        int index = positions[i] - 1;          // position 1 -> index 0
        if (index >= 0 && index < capacity)
        {
            word[index] = tiles[i].tileLetter;
        }
    }
    return word;
}

// Shows every letter with its position, or "(empty)"
void TileRack::display() const
{
    if (isEmpty())
    {
        cout << "(empty)" << endl;
        return;
    }

    for (int i = 0; i < count; i++)
    {
        cout << "[" << tiles[i].tileLetter << " at " << positions[i] << "] ";
    }
    cout << " (" << count << "/" << capacity << ")" << endl;
}


// =====================================================================
//  MODULE 11 : OPERATOR OVERLOADING AND FRIEND FUNCTIONS
// ---------------------------------------------------------------------
//  OPERATOR OVERLOADING gives a new meaning to a C++ operator (==, +,
//  <<, >>) for OUR OWN types, so the code reads like normal math:
//      tile1 == tile2        rack1 + rack2        cout << scrabble
//
//  A FRIEND FUNCTION is not a member of the class, but the class allows
//  it to use its private / protected data.
//  Why are << and >> friends and not members?  In  "cout << tile"  the
//  LEFT side is cout (an ostream), not our class. A member operator only
//  works when our class is on the left side, so these operators are
//  written outside the class and made friends.
// =====================================================================

// ---------------- Tile : == and != ----------------

// Two tiles are equal when their letters are the same
bool Tile::operator==(const Tile& other) const
{
    return tileLetter == other.tileLetter;
}

// "Not equal" simply uses the == operator above
bool Tile::operator!=(const Tile& other) const
{
    return !(*this == other);
}

// ---------------- Tile : << and >> ----------------

// Prints a tile like  [G]  or  [ ]
ostream& operator<<(ostream& out, const Tile& tile)
{
    out << "[" << tile.tileLetter << "]";
    return out;      // returning the stream allows: cout << a << b << c;
}

// Reads one letter into a tile (small letters become capital)
istream& operator>>(istream& in, Tile& tile)
{
    char letter;
    in >> letter;

    if (letter >= 'a' && letter <= 'z')
    {
        letter = letter - 32;
    }

    tile.tileLetter = letter;
    tile.filled = true;
    return in;
}

// ---------------- Location : << and >> ----------------

// Prints a location like  "Study Room   - NOT Searched"
ostream& operator<<(ostream& out, const Location& place)
{
    string nameToShow = replaceUnderscores(place.locName);
    out << nameToShow;

    // spaces so that the status column lines up
    for (int space = (int)nameToShow.length(); space < 13; space++)
    {
        out << " ";
    }

    if (place.locSearchFlag)
    {
        out << "- Searched";
    }
    else
    {
        out << "- NOT Searched";
    }
    return out;
}

// Reads a location as:  name letter position   (e.g.  Garden M 4)
istream& operator>>(istream& in, Location& place)
{
    in >> place.locName >> place.letterClueLoc >> place.boardPosLoc;

    if (place.letterClueLoc >= 'a' && place.letterClueLoc <= 'z')
    {
        place.letterClueLoc = place.letterClueLoc - 32;
    }

    place.locRow = WORD_1;          // locations always give letters for Word 1
    place.locSearchFlag = false;    // a new location is not searched yet
    return in;
}

// ---------------- Character : << and >> ----------------

// Prints any character. display() is VIRTUAL, so for a Cook object the
// Cook version runs, for a PA object the PA version runs (polymorphism),
// even though the parameter type is Character&.
// (display() writes to cout, so this operator is meant to be used with cout.)
ostream& operator<<(ostream& out, const Character& person)
{
    person.display();
    return out;
}

// Reads a new name and nationality. Being a friend, this function can
// change the protected data of Character directly.
istream& operator>>(istream& in, Character& person)
{
    in >> person.characterName >> person.nationality;
    return in;
}

// ---------------- TileRack : + ----------------

// Joins two racks into a NEW rack:  letters of this rack (in board order),
// one space, then the letters of the other rack (in board order).
// "GERMANS" + "SHEPHERD" -> "GERMANS SHEPHERD" (16 tiles)
// Neither of the two racks is changed.
TileRack TileRack::operator+(const TileRack& other) const
{
    string fullClue = getLettersByPosition() + " " + other.getLettersByPosition();

    TileRack mergedRack((int)fullClue.length());

    for (int i = 0; i < (int)fullClue.length(); i++)
    {
        Clue letterClue;
        letterClue.clueLetter = fullClue[i];
        letterClue.cluePosition = i + 1;     // positions 1, 2, 3 ... in the merged clue
        letterClue.clueRow = WORD_1;         // not used for the merged rack
        mergedRack.addTile(letterClue);
    }

    return mergedRack;
}

// ---------------- Scrabble : << and >> ----------------

// Prints the whole board. As a friend it reads the private board directly.
ostream& operator<<(ostream& out, const Scrabble& scrabble)
{
    out << "Scrabble:" << endl;

    for (int row = WORD_1; row <= WORD_2; row++)
    {
        WordRow currentRow = (WordRow)row;
        out << "  " << wordRowToText(currentRow) << ": ";

        for (int i = 0; i < scrabble.getRowLength(currentRow); i++)
        {
            out << scrabble.board[currentRow][i] << " ";   // uses Tile's <<
        }

        if (scrabble.isRowComplete(currentRow))
        {
            out << "-> COMPLETE";
        }
        out << endl;
    }
    return out;
}

// Reads a move as:  letter position row   (e.g.  G 1 1)  and places it.
// All the normal checks of placeLetter() still run (exceptions).
istream& operator>>(istream& in, Scrabble& scrabble)
{
    char letter;
    int position;
    int rowNumber;
    in >> letter >> position >> rowNumber;

    if (in.fail())
    {
        in.clear();
        in.ignore(1000, '\n');
        throw InvalidInputException("type a letter and two numbers, e.g. G 1 1");
    }

    if (rowNumber != 1 && rowNumber != 2)
    {
        throw InvalidInputException("row must be 1 or 2.");
    }

    if (letter >= 'a' && letter <= 'z')
    {
        letter = letter - 32;
    }

    Clue newClue;
    newClue.clueLetter = letter;
    newClue.cluePosition = position;
    newClue.clueRow = (rowNumber == 1) ? WORD_1 : WORD_2;

    scrabble.placeLetter(newClue);
    return in;
}
