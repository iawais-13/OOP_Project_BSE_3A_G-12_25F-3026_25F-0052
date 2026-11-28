// =====================================================================
//  File      : heist.h
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the HEADER file. It only DECLARES things
//              (enums, structs, classes and function prototypes).
//              All function bodies are written in heist.cpp.
//              In this way the interface is kept separate from
//              the implementation (3-file structure).
// =====================================================================

#ifndef HEIST_H     // Header guard: stops this file from being
#define HEIST_H     // included more than once in the same program

#include <iostream>
#include <string>
#include <fstream>     // ifstream / ofstream for the .txt files (Module 6)
#include <cstdlib>     // rand() and srand() to pick a random culprit
#include <ctime>       // time() for the random seed and today's date

using namespace std;


// =====================================================================
//  MODULE 1 : ENUMERATIONS
// ---------------------------------------------------------------------
//  An enum gives meaningful NAMES to a fixed set of integer values.
//  Instead of remembering that 0 means "Word 1" and 1 means "Word 2",
//  we simply write WORD_1 and WORD_2. This makes the code easy to read
//  and protects us from using wrong numbers by mistake.
// =====================================================================

// WordRow tells us on which row of the Scrabble board a letter belongs.
// Row 1 holds the 7 letters from locations, Row 2 holds the 8 letters
// from characters.
// NOTE: WordRow is written first because the Clue and Location structs
// below use it.
enum WordRow
{
    WORD_1,     // letters collected from the 7 locations
    WORD_2      // letters collected from the 8 people
};

// GameState stores the current state of the player in the game.
// GameEngine will use it to decide what to do next.
// We added the prefix "STATE_" because MenuChoice also has an "Exit"
// value, and two normal enums cannot have the same name in C++.
enum GameState
{
    STATE_MENU,      // player is on the main menu
    STATE_PLAYING,   // a game is running
    STATE_WON,       // player caught the culprit
    STATE_LOST,      // credibility became zero or turns finished
    STATE_EXIT       // player wants to close the program
};

// MenuChoice is used for the Main Menu options.
// We start it from 1 so that the number the user types (1 to 5)
// matches the enum value directly.
enum MenuChoice
{
    MENU_NEW_GAME = 1,   // [1] New Game
    MENU_LEADERBOARD,    // [2] View Leader/Score Board
    MENU_ABOUT,          // [3] About
    MENU_RULES,          // [4] Rules
    MENU_EXIT            // [5] Exit
};


// =====================================================================
//  MODULE 1 : STRUCTURES
// ---------------------------------------------------------------------
//  A struct groups related data under ONE name. Instead of keeping a
//  letter, a position and a row in three separate variables, we keep
//  them together in one struct. Structs improve code reusability,
//  because the same struct can be used again and again in many classes
//  (Cottage, Scrabble, TileRack, Characters...).
//  Every member has a default value so a new struct is never filled
//  with garbage values.
// =====================================================================

// Tile is one box (slot) of the Scrabble board or of a TileRack.
struct Tile
{
    char tileLetter = ' ';   // letter placed in this box (' ' = no letter)
    bool filled = false;     // true when a letter has been placed here

    // ----- MODULE 11 : Operator Overloading -----
    // Two tiles are EQUAL when they have the same letter.
    // Used to compare the letters with the letters of clue.txt.
    bool operator==(const Tile& other) const;
    bool operator!=(const Tile& other) const;

    // ----- MODULE 11 : Friend functions -----
    // cout << tile  prints  [G]  (or  [ ]  when empty)
    // cin  >> tile  reads one letter into the tile
    friend ostream& operator<<(ostream& out, const Tile& tile);
    friend istream& operator>>(istream& in, Tile& tile);
};

// Clue is a "carrier". It carries one letter and its position
// from a Location or a Character to the Scrabble board.
struct Clue
{
    char clueLetter = ' ';      // the letter given as a clue
    int cluePosition = 0;       // board position of the letter (starts from 1)
    WordRow clueRow = WORD_1;   // which word (row) this letter belongs to
};

// Location is one place of the cottage (Garden, Kitchen, Bedroom_1 ...).
// Every location gives one letter for Word 1.
struct Location
{
    string locName = "";           // name of the place, e.g. "Study_Room"
    char letterClueLoc = ' ';      // letter hidden at this place
    int boardPosLoc = 0;           // position of that letter in Word 1
    WordRow locRow = WORD_1;       // always WORD_1 for locations
    bool locSearchFlag = false;    // true after the detective searches it

    // ----- MODULE 11 : Friend functions -----
    // cout << location  prints  "Study Room   - NOT Searched"
    // cin  >> location  reads   name letter position  (e.g. Garden M 4)
    friend ostream& operator<<(ostream& out, const Location& place);
    friend istream& operator>>(istream& in, Location& place);
};


// =====================================================================
//  MODULE 7 : EXCEPTION HANDLING
// ---------------------------------------------------------------------
//  An exception is an error that happens while the program is running
//  (wrong input, full rack, missing file ...). Instead of crashing, we
//  THROW an exception object and CATCH it somewhere else, show a
//  message and let the game continue safely.
//
//  GameException is the BASE class of all our game errors. Every
//  special error is a DERIVED class of it. Because of this, one
//  catch block  "catch (GameException& error)"  can catch ALL of them.
// =====================================================================

// Base class for every error of the game
class GameException
{
protected:
    string message;     // details of what went wrong

public:
    GameException(string errorMessage = "Something went wrong in the game.");

    // Returns the details of the error
    string getMessage() const;

    // Returns the name (type) of the error.
    // It is VIRTUAL, so every derived class gives its own name and the
    // correct version is chosen at runtime (polymorphism).
    virtual string getErrorType() const;

    // Returns type + message together, ready to print on screen
    string getFullMessage() const;

    // Virtual destructor so derived exception objects are destroyed properly
    virtual ~GameException();
};

// Thrown when the user types something wrong
// (e.g. menu choice 9, letters instead of a number, name with a space)
class InvalidInputException : public GameException
{
public:
    InvalidInputException(string details);
    string getErrorType() const override;
};

// Thrown when a position (or menu number) is outside the allowed range
class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(int position, int maxPosition);
    string getErrorType() const override;
};

// Thrown when a letter is placed on a board slot that is already filled
class SlotFilledException : public GameException
{
public:
    SlotFilledException(int position);
    string getErrorType() const override;
};

// Thrown when the detective tries to form a word but the row is not full yet
class WordIncompleteException : public GameException
{
public:
    WordIncompleteException(WordRow row);
    string getErrorType() const override;
};

// Thrown when a letter is added to a word (row) that is already complete
class WordCompleteException : public GameException
{
public:
    WordCompleteException(WordRow row);
    string getErrorType() const override;
};

// Thrown when the detective types a wrong word
class InvalidWordException : public GameException
{
public:
    InvalidWordException(string typedWord);
    string getErrorType() const override;
};

// Thrown when a letter is added to a TileRack that has no space left
class RackFullException : public GameException
{
public:
    RackFullException();
    string getErrorType() const override;
};

// Thrown when we try to take a letter out of an empty TileRack
class RackEmptyException : public GameException
{
public:
    RackEmptyException();
    string getErrorType() const override;
};

// Thrown when the same clue (letter) is collected a second time
class AlreadyVisitedException : public GameException
{
public:
    AlreadyVisitedException(string placeOrPerson);
    string getErrorType() const override;
};

// Thrown when the detective's credibility is too low for an interview
class InterviewDeniedException : public GameException
{
public:
    InterviewDeniedException(string personName, int requiredCredibility);
    string getErrorType() const override;
};

// Thrown when a .txt file cannot be opened
class FileNotFoundException : public GameException
{
public:
    FileNotFoundException(string fileName);
    string getErrorType() const override;
};

// Thrown when a .txt file is opened but its data is in a wrong format
class FileCorruptedException : public GameException
{
public:
    FileCorruptedException(string fileName, string details);
    string getErrorType() const override;
};


// =====================================================================
//  MODULE 8 : TEMPLATES  -  DynamicArray<T>
// ---------------------------------------------------------------------
//  A TEMPLATE is a "blueprint" class that works with ANY data type.
//  We write the code once with a placeholder type T, and the compiler
//  makes a real class for every type we use:
//      DynamicArray<Location>  -> array of locations  (Cottage)
//      DynamicArray<Tile>      -> array of tiles      (Scrabble, TileRack)
//      DynamicArray<int>       -> array of integers   (TileRack positions)
//  We cannot use the STL (vector), so this is our own simple version.
//
//  IMPORTANT: The WHOLE template (including the function bodies) is
//  written here in the header file and NOT in heist.cpp. The compiler
//  needs the complete code at the place where DynamicArray<Tile> etc.
//  is used; if the bodies were in heist.cpp we would get linker errors.
//
//  This class does all the new / delete / deep copy work in ONE place,
//  so Cottage, Scrabble and TileRack become shorter and safer.
// =====================================================================

template <class T>
class DynamicArray
{
private:
    T* data;      // the array on the heap
    int size;     // number of elements

public:
    // Constructor: creates an array of the given size (at least 1)
    DynamicArray(int arraySize = 1)
    {
        if (arraySize < 1)
        {
            arraySize = 1;
        }
        size = arraySize;
        data = new T[size];
    }

    // Copy constructor: DEEP copy (new memory, then copy every element)
    DynamicArray(const DynamicArray<T>& other)
    {
        size = other.size;
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    // Assignment operator: DEEP copy
    DynamicArray<T>& operator=(const DynamicArray<T>& other)
    {
        if (this != &other)              // self-assignment check
        {
            delete[] data;               // free the old array

            size = other.size;
            data = new T[size];
            for (int i = 0; i < size; i++)
            {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    // Destructor: frees the heap memory
    ~DynamicArray()
    {
        delete[] data;
        data = nullptr;
    }

    // Index operator: lets us write  array[2]  like a normal array.
    // It also CHECKS the index, so we can never go outside the array.
    T& operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw InvalidPositionException(index + 1, size);
        }
        return data[index];
    }

    // Same as above, but for const objects (read only)
    const T& operator[](int index) const
    {
        if (index < 0 || index >= size)
        {
            throw InvalidPositionException(index + 1, size);
        }
        return data[index];
    }

    int getSize() const
    {
        return size;
    }
};


// =====================================================================
//  MODULE 5 : CLUE SYSTEM  -  TileRack
// ---------------------------------------------------------------------
//  NOTE: TileRack is written BEFORE the Character classes, because the
//  Detective class (Module 3) keeps two TileRack objects inside it, and
//  C++ must know a class before it can be used as a member.
// =====================================================================

// ---------------------------------------------------------------------
//  Class: TileRack
//  Stores the letters collected by the detective, together with the
//  board position of every letter.
//  It owns a Tile array on the heap (DMA). Because a Tile has no
//  position, a second (parallel) int array keeps the positions:
//      tiles[i]     -> letter number i
//      positions[i] -> board position of that letter
// ---------------------------------------------------------------------
class TileRack
{
private:
    // ----- OLD RAW DMA LOGIC (before Module 8: Templates) -----
    // Tile* tiles;        // dynamic array of letters
    // int* positions;     // dynamic array of board positions (parallel to tiles)
    // void copyRackFrom(const TileRack& other);   // makes a DEEP copy
    // void freeMemory();                          // deletes both arrays
    // ------------------------------------------------------------

    // NEW (Module 8): the template does new / delete / deep copy for us
    DynamicArray<Tile> tiles;       // letters
    DynamicArray<int> positions;    // board positions (parallel to tiles)

    int capacity;       // maximum letters (7 for Word 1, 8 for Word 2)
    int count;          // letters stored so far

public:
    TileRack(int rackCapacity = 7);
    TileRack(const TileRack& other);              // copy constructor (deep copy)
    TileRack& operator=(const TileRack& other);   // assignment operator (deep copy)
    ~TileRack();                                  // frees heap memory

    // Stores the letter and position carried by a Clue
    void addTile(Clue clue);

    // Removes the last added letter and returns it
    char removeLastTile();

    // ----- Checks and getters -----
    bool isFull() const;
    bool isEmpty() const;
    int getCount() const;
    int getCapacity() const;
    bool hasPosition(int position) const;     // is this position already collected?
    char getLetterAt(int index) const;        // index 0 = first collected letter
    int getPositionAt(int index) const;

    // String manipulation:
    string getLettersInOrder() const;         // order of collection, e.g. "RGA"
    string getLettersByPosition() const;      // board order, e.g. "G_R_A__"

    // Shows every letter with its position, e.g. [R at 3] [G at 1]
    void display() const;

    // ----- MODULE 11 : Operator Overloading -----
    // rack1 + rack2  joins both racks into ONE new rack, with a space
    // between them: "GERMANS" + "SHEPHERD" -> "GERMANS SHEPHERD"
    // (the final clue that points to the culprit)
    TileRack operator+(const TileRack& other) const;
};


// =====================================================================
//  MODULE 3 : CHARACTER HIERARCHY  (Part A)
// ---------------------------------------------------------------------
//  INHERITANCE lets a child class reuse the data and functions of a
//  parent class. Every person in the cottage has a name, a role and a
//  nationality, so we write these things ONCE in the Character class
//  and all other classes inherit them.
//
//  Classes made in Part A:
//
//      Character  (abstract)
//      ├── Victim          (Business Owner)
//      ├── Detective       (Player)
//      └── Suspect  (abstract)
//          └── HouseHelp
//              ├── Cook
//              ├── Guard
//              └── Servant
//
//  BusinessRelations, CloseRelations, Partner, OwnerChild and PA
//  (the diamond problem) are added in Part B (see below).
// =====================================================================

// ---------------------------------------------------------------------
//  Class: Character  (ABSTRACT BASE CLASS)
//  Holds the common data of every person in the game.
//  It has a PURE VIRTUAL function display(), so no object of
//  Character can be made. Every child class must write its own display().
// ---------------------------------------------------------------------
class Character
{
    // ----- MODULE 11 : Friend class -----
    // GameEngine is a FRIEND of Character, so it can reach the private and
    // protected data directly (e.g. set isCulprit = true for the culprit)
    // without a public setter that any other class could misuse.
    friend class GameEngine;

    // ----- MODULE 11 : Friend functions -----
    // cout << person  calls the VIRTUAL display(), so the right version runs
    // cin  >> person  reads a new name and nationality
    friend ostream& operator<<(ostream& out, const Character& person);
    friend istream& operator>>(istream& in, Character& person);

protected:
    string characterName;   // e.g. "Mr.Enzo"
    string characterRole;   // e.g. "Cook"
    string nationality;     // e.g. "Italian"
    bool isCulprit;         // true only for the selected culprit
    bool interviewed;       // true after the detective interviews this person (Module 13)

    // Prints name, role and nationality.
    // Child classes call it inside their own display() (code reuse).
    void displayBasicInfo() const;

public:
    Character(string name, string role, string nationalityText);

    // VIRTUAL destructor: when a child object is deleted through a
    // Character pointer, the child's destructor is also called.
    virtual ~Character();

    // ----- Getters -----
    string getName() const;
    string getRole() const;
    string getNationality() const;
    bool getIsCulprit() const;
    bool isInterviewed() const;

    // ----- Setters -----
    void setName(string name);
    void setNationality(string nationalityText);
    void setInterviewed(bool value);

    // Minimum credibility the detective needs to interview this person.
    // VIRTUAL: every child class returns its own value (polymorphism),
    // so we never need a long if-else chain on the role.
    virtual int getRequiredCredibility() const;

    // Returns true if the detective's credibility is high enough
    bool canBeInterviewed(int credibility) const;

    // Returns the letter clue of this person (for Word 2).
    // The Detective has no clue, so the base version returns an empty clue.
    virtual Clue getClue() const;

    // PURE VIRTUAL function ( = 0 ). It makes Character an abstract class.
    virtual void display() const = 0;
};


// ---------------------------------------------------------------------
//  Class: Victim  (inherits Character)
//  The French Business Owner. He is interviewed but is NOT a suspect.
//  He gives letter number 1 of Word 2.
// ---------------------------------------------------------------------
class Victim : public Character
{
private:
    WordRow clueRowChar;    // always WORD_2
    char letterClueChar;    // letter given by the victim
    int boardPosChar;       // position of that letter in Word 2

public:
    // static const: one fixed value shared by all Victim objects
    static const int REQUIRED_CREDIBILITY = 80;

    Victim(string name, string nationalityText, char letter, int position);

    char getLetter() const;
    int getPosition() const;
    WordRow getRow() const;

    Clue getClue() const override;
    int getRequiredCredibility() const override;
    void display() const override;
};


// ---------------------------------------------------------------------
//  Class: Detective  (inherits Character)
//  The PLAYER. Stores credibility, number of turns, solved words and the
//  collected letters.
//  NOTE: The turn count is increased by the GameEngine. The Detective
//  only stores it.
//
//  COMPOSITION (Detective <>--- TileRack, filled diamond):
//  The two racks are member OBJECTS. They are created with the Detective
//  and destroyed with it; they cannot live without the Detective.
// ---------------------------------------------------------------------
class Detective : public Character
{
private:
    int credibility;        // starts from 100, game is lost at 0
    int turns;              // turns used so far
    string solvedWords;     // words solved correctly, e.g. "GERMANS"

    TileRack word1Rack;     // 7 letters collected from the locations
    TileRack word2Rack;     // 8 letters collected from the people

public:
    Detective(string name, int startingCredibility);
    ~Detective();

    // ----- Getters -----
    int getCredibility() const;
    int getTurns() const;
    string getSolvedWords() const;
    const TileRack& getRack(WordRow row) const;   // read-only access to a rack

    // ----- Setters / actions -----
    void setTurns(int turnCount);
    void loseCredibility(int amount);   // never goes below 0
    void addSolvedWord(string word);

    // Saves a collected clue in the correct rack (Word 1 or Word 2)
    void collectClue(Clue clue);

    // Shows the letters of both racks
    void displayRacks() const;

    void display() const override;
};


// ---------------------------------------------------------------------
//  Class: Suspect  (inherits Character)  -  ABSTRACT
//  Parent of all seven suspects. The clue data is PROTECTED so that
//  every suspect class can use it directly.
//  display() is still pure virtual, so Suspect is also abstract.
// ---------------------------------------------------------------------
class Suspect : public Character
{
protected:
    WordRow clueRowChar;    // always WORD_2
    char letterClueChar;    // letter given by this suspect
    int boardPosChar;       // position of that letter in Word 2

public:
    Suspect(string name, string role, string nationalityText, char letter, int position);

    char getLetter() const;
    int getPosition() const;
    WordRow getRow() const;

    Clue getClue() const override;

    virtual void display() const = 0;   // still pure virtual
};


// ---------------------------------------------------------------------
//  Class: HouseHelp  (inherits Suspect)
//  Group of the house workers: Cook, Guard and Servant.
//  They can be interviewed at ANY time (no credibility needed).
// ---------------------------------------------------------------------
class HouseHelp : public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 0;

    HouseHelp(string name, string role, string nationalityText, char letter, int position);

    int getRequiredCredibility() const override;
};


// ----- The three House Help members (multi-level inheritance) -----
// Character -> Suspect -> HouseHelp -> Cook / Guard / Servant

class Cook : public HouseHelp
{
public:
    Cook(string name, string nationalityText, char letter, int position);
    void display() const override;
};

class Guard : public HouseHelp
{
public:
    Guard(string name, string nationalityText, char letter, int position);
    void display() const override;
};

class Servant : public HouseHelp
{
public:
    Servant(string name, string nationalityText, char letter, int position);
    void display() const override;
};


// =====================================================================
//  MODULE 3 : CHARACTER HIERARCHY  (Part B)
// ---------------------------------------------------------------------
//  Classes made in Part B:
//
//      Suspect
//      ├── BusinessRelations   (virtual)
//      │   ├── Partner         (Partner 1, Partner 2)
//      │   └── PA  ──┐
//      └── CloseRelations      (virtual)
//          ├── OwnerChild    │
//          └── PA  ──────────┘   <- PA inherits from BOTH groups
//
//  THE DIAMOND PROBLEM:
//  PA inherits from BusinessRelations AND CloseRelations, and both of
//  them inherit from Suspect. With normal inheritance, PA would get
//  TWO copies of Suspect (two names, two letters ...) and the compiler
//  would not know which one to use.
//
//  SOLUTION: VIRTUAL INHERITANCE.
//  BusinessRelations and CloseRelations inherit Suspect as "virtual",
//  so PA gets only ONE shared copy of Suspect (and of Character).
// =====================================================================

// ---------------------------------------------------------------------
//  Class: BusinessRelations  (inherits Suspect VIRTUALLY)
//  Group of people related to the owner's business: Partners and PA.
//  Default condition for this group: credibility >= 50.
// ---------------------------------------------------------------------
class BusinessRelations : virtual public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 50;

    // No Suspect data is passed here: Suspect is a VIRTUAL base class,
    // so it is always built by the last class (Partner or PA).
    BusinessRelations();

    int getRequiredCredibility() const override;
};


// ---------------------------------------------------------------------
//  Class: CloseRelations  (inherits Suspect VIRTUALLY)
//  Group of people close to the owner: Owner's Child and PA.
//  Default condition for this group: credibility >= 75.
// ---------------------------------------------------------------------
class CloseRelations : virtual public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 75;

    // No Suspect data is passed here (same reason as BusinessRelations)
    CloseRelations();

    int getRequiredCredibility() const override;
};


// ---------------------------------------------------------------------
//  Class: Partner  (inherits BusinessRelations)
//  Business Partner 1 and Business Partner 2 are both objects of this
//  class. Only their data (name, role, letter ...) is different.
//  Interview condition: credibility >= 50 (taken from BusinessRelations).
// ---------------------------------------------------------------------
class Partner : public BusinessRelations
{
public:
    Partner(string name, string role, string nationalityText, char letter, int position);
    void display() const override;
};


// ---------------------------------------------------------------------
//  Class: OwnerChild  (inherits CloseRelations)
//  The owner's teenage child (aged 15-19), panicked after the theft.
//  Interview condition: credibility >= 75 (taken from CloseRelations).
// ---------------------------------------------------------------------
class OwnerChild : public CloseRelations
{
public:
    OwnerChild(string name, string nationalityText, char letter, int position);
    void display() const override;
};


// ---------------------------------------------------------------------
//  Class: PA  (inherits BusinessRelations AND CloseRelations)
//  MULTIPLE INHERITANCE. The PA is the owner's right-hand and bestie,
//  so he is a business relation and a close relation at the same time.
//
//  Two important rules of the diamond:
//  1. PA's constructor must call the Suspect constructor ITSELF,
//     because Suspect is a virtual base class.
//  2. PA MUST override getRequiredCredibility(), because both parents
//     give a different version (50 and 75) and the compiler cannot
//     choose one by itself.
//  Interview condition: credibility >= 35.
// ---------------------------------------------------------------------
class PA : public BusinessRelations, public CloseRelations
{
public:
    static const int REQUIRED_CREDIBILITY = 35;

    PA(string name, string nationalityText, char letter, int position);

    int getRequiredCredibility() const override;
    void display() const override;
};


// =====================================================================
//  MODULE 4 : GAME BOARDS (DMA)  -  Part A : Cottage
// ---------------------------------------------------------------------
//  DMA (Dynamic Memory Allocation) means creating memory at RUNTIME
//  with "new" and freeing it ourselves with "delete". This memory is
//  on the HEAP, so we must be careful: every "new" needs one "delete",
//  otherwise we get a memory leak.
// =====================================================================

// ---------------------------------------------------------------------
//  Class: Cottage
//  Holds and manages the 7 locations of the cottage (for Word 1).
//  All locations can be visited at any time (no conditions).
//
//  AGGREGATION (Cottage <>--- Location):
//  The cottage "has" locations. A Location is a separate struct that
//  is created from the data of locations.txt and can exist on its own,
//  the Cottage only keeps and manages them.
//
//  Memory design: an array of POINTERS, and every pointer points to one
//  Location on the heap. So the destructor first deletes each location
//  and then deletes the array itself.
// ---------------------------------------------------------------------
class Cottage
{
private:
    // ----- OLD RAW DMA LOGIC (before Module 8: Templates) -----
    // Location** locations;   // dynamic array of 7 Location pointers
    // void copyLocationsFrom(const Cottage& other);   // makes a DEEP copy
    // void freeMemory();                              // deletes everything
    // ------------------------------------------------------------

    // NEW (Module 8): template array of 7 locations
    DynamicArray<Location> locations;

    void checkLocationNumber(int number) const;     // throws if not 1-7

public:
    // static const: the cottage always has 7 locations
    static const int TOTAL_LOCATIONS = 7;

    Cottage();                                   // creates 7 empty locations
    Cottage(const Cottage& other);               // copy constructor (deep copy)
    Cottage& operator=(const Cottage& other);    // assignment operator (deep copy)
    ~Cottage();                                  // frees all heap memory

    // Fills one location (number 1 to 7) with data from locations.txt
    void setLocation(int number, string name, char letter, int position);

    // Marks the location as searched and returns its clue (letter + position)
    Clue searchLocation(int number);

    // ----- Getters -----
    bool isSearched(int number) const;
    string getLocationName(int number) const;
    int getLocationCount() const;
    int getSearchedCount() const;

    // Shows all 7 locations with their search status
    void displayLocations() const;
};


// =====================================================================
//  MODULE 4 : GAME BOARDS (DMA)  -  Part B : Scrabble
// ---------------------------------------------------------------------
//  A JAGGED ARRAY is a 2D array whose rows have DIFFERENT lengths.
//  Our board has 2 rows:
//
//      Row 1 (Word 1):  [ ][ ][ ][ ][ ][ ][ ]        <- 7 slots (locations)
//      Row 2 (Word 2):  [ ][ ][ ][ ][ ][ ][ ][ ]     <- 8 slots (people)
//
//  board is a pointer to 2 row pointers, and each row is created
//  separately with its own size.
// =====================================================================

// ---------------------------------------------------------------------
//  Class: Scrabble
//  The letter board. It places each collected letter at its position,
//  shows the two words and checks the word typed by the detective.
//  The position of a tile is its index in the row (position 1 = index 0).
// ---------------------------------------------------------------------
class Scrabble
{
private:
    // ----- OLD RAW DMA LOGIC (before Module 8: Templates) -----
    // Tile** board;           // jagged array: 2 rows of different length
    // void createEmptyBoard();                        // allocates both rows
    // void copyBoardFrom(const Scrabble& other);      // makes a DEEP copy
    // void freeMemory();                              // deletes both rows + board
    // ------------------------------------------------------------

    // NEW (Module 8): a jagged array made of templates.
    // The outer array has 2 rows, and every row is a DynamicArray<Tile>
    // with its own length (7 and 8). We still write board[row][index].
    DynamicArray< DynamicArray<Tile> > board;

    string targetWord1;     // correct Word 1 from clue.txt, e.g. "GERMANS"
    string targetWord2;     // correct Word 2 from clue.txt, e.g. "SHEPHERD"

    void checkPosition(WordRow row, int position) const;  // throws if wrong

public:
    // static const values shared by every Scrabble object
    static const int ROW_COUNT = 2;
    static const int WORD1_LENGTH = 7;
    static const int WORD2_LENGTH = 8;

    Scrabble();                                     // creates an empty board
    Scrabble(const Scrabble& other);                // copy constructor (deep copy)
    Scrabble& operator=(const Scrabble& other);     // assignment operator (deep copy)
    ~Scrabble();                                    // frees all heap memory

    // Saves the 2 correct words of the selected culprit (from clue.txt)
    void setTargetWords(string word1, string word2);

    // Places the letter of a clue at its position on the board
    void placeLetter(Clue clue);

    // Returns the letter at a position (' ' if the slot is still empty)
    char revealLetter(WordRow row, int position) const;

    // ----- Checks and getters -----
    int getRowLength(WordRow row) const;
    bool isSlotFilled(WordRow row, int position) const;
    bool isRowComplete(WordRow row) const;
    int getFilledCount(WordRow row) const;
    string getPlacedWord(WordRow row) const;        // letters placed so far
    string getTargetWord(WordRow row) const;

    // Compares the word typed by the detective with the correct word
    bool isWordCorrect(WordRow row, string typedWord) const;

    // ----- Display -----
    void displayWord(WordRow row) const;            // one row
    void displayBoard() const;                      // both rows

    // ----- MODULE 11 : Friend functions -----
    // cout << scrabble  prints the whole board
    // cin  >> scrabble  reads  letter position row  (e.g. G 1 1) and places it
    friend ostream& operator<<(ostream& out, const Scrabble& scrabble);
    friend istream& operator>>(istream& in, Scrabble& scrabble);
};


// =====================================================================
//  MODULE 6 : FILE I/O AND LEADER / SCORE BOARD
// ---------------------------------------------------------------------
//  FILE HANDLING lets the game keep its data in .txt files:
//    - ifstream  reads a file   (input file stream)
//    - ofstream  writes a file  (output file stream)
//  So the characters, locations, clues and rules can be changed without
//  changing the code, and the scores stay saved after the game closes.
//
//  The files must be in the same folder as the project files
//  (the folder that has the .vcxproj file).
// =====================================================================

// Names of the data files (const = they never change)
const string CLUE_FILE = "clue.txt";
const string LOCATIONS_FILE = "locations.txt";
const string CHARACTERS_FILE = "characters.txt";
const string RULES_FILE = "rules.txt";
const string SCORE_FILE = "scoreBoard.txt";     // "leader/scoreBoard.txt" in the manual
const string ABOUT_FILE = "about.txt";

// ---------------------------------------------------------------------
//  Struct: GameRules
//  Keeps the numbers read from rules.txt. The default values are used
//  when rules.txt is missing or corrupted.
//  NOTE: penalties are stored as POSITIVE numbers (5 and 10), because
//  Detective::loseCredibility() subtracts them.
// ---------------------------------------------------------------------
struct GameRules
{
    int maxTurns = 25;
    int startingCredibility = 100;
    int wrongTurnPenalty = 5;
    int wrongWordPenalty = 10;
    string winCondition = "Accuse_Correct_Culprit";
    string loseCondition = "Credibility_Zero_Or_Turns_Exhausted";
};

// ---------------------------------------------------------------------
//  Class: GameFileReader
//  Reads the game data from the .txt files.
//  All functions are STATIC: they belong to the class itself, so we can
//  call them without making an object, e.g.
//      GameFileReader::readRules();
//  Every function throws FileNotFoundException if the file is missing
//  and FileCorruptedException if the data is in a wrong format.
//
//  "profileNumber" is the line number of the culprit in clue.txt
//  (1 = Cook, 2 = Guard, 3 = Servant, 4 = Partner 1, 5 = Partner 2,
//   6 = PA, 7 = Child). The same number tells which COLUMN of
//  locations.txt and characters.txt has the letters of that culprit.
// ---------------------------------------------------------------------
class GameFileReader
{
private:
    // Throws if the profile number is not between 1 and 7
    static void checkProfileNumber(int profileNumber);

    // Makes the correct character object from its role (used once, while loading)
    static Character* createCharacter(string role, string name, string nationalityText,
        char letter, int position);

public:
    static const int TOTAL_PROFILES = 7;

    // clue.txt : how many culprit profiles there are (should be 7)
    static int countProfiles(string fileName = CLUE_FILE);

    // clue.txt : reads the culprit role and the 2 words of one profile
    static void readClueProfile(int profileNumber, string& culpritRole,
        string& word1, string& word2,
        string fileName = CLUE_FILE);

    // locations.txt : fills the 7 locations of the cottage
    static void loadLocations(Cottage& cottage, int profileNumber,
        string fileName = LOCATIONS_FILE);

    // characters.txt : creates the 8 people (on the heap) and returns how many.
    // The caller must delete them later.
    static int loadCharacters(Character* people[], int maxPeople, int profileNumber,
        string fileName = CHARACTERS_FILE);

    // rules.txt : reads the game rules
    static GameRules readRules(string fileName = RULES_FILE);

    // Prints a whole text file on the screen (about.txt, rules.txt)
    static void showTextFile(string fileName);
};

// ---------------------------------------------------------------------
//  Class: ScoreBoard  (Leader / Score Board)
//  Saves the result of every game in scoreBoard.txt and shows all the
//  previous results. Format of one line:
//      Detective|Result|Credibility|Turns_Left|Culprit|Date
// ---------------------------------------------------------------------
class ScoreBoard
{
private:
    string fileName;

    string getTodayDate() const;     // e.g. "25-11-26"
    bool fileHasData() const;        // false if the file is missing or empty

public:
    ScoreBoard(string scoreFileName = SCORE_FILE);

    // Appends one result at the END of the file (old results are kept)
    void addScore(string detectiveName, bool won, int credibilityLeft,
        int turnsLeft, string culpritRole);

    // Shows all saved results as a table
    void displayScores() const;
};


// =====================================================================
//  MODULE 12 : SINGLETON GAME ENGINE   (+ MODULE 2 : GAME INITIALIZATION)
// ---------------------------------------------------------------------
//  SINGLETON PATTERN: only ONE object of GameEngine can ever exist,
//  because the whole game must be run by a single engine.
//  How it is done:
//    1. the constructor is PRIVATE -> nobody outside can write "new GameEngine"
//    2. copying is not allowed      -> no second engine by copying
//    3. a STATIC pointer keeps the one and only object
//    4. getInstance() creates it the first time and then always
//       returns the same object
// ---------------------------------------------------------------------
//  RELATIONSHIPS OF GAMEENGINE (Module 10):
//    COMPOSITION (GameEngine <>--- filled diamond): Detective, Cottage,
//      Scrabble and TileRack are member OBJECTS. They are born and die
//      with the engine.
//    ASSOCIATION (GameEngine ---> ScoreBoard): the engine only keeps a
//      POINTER to a ScoreBoard that main() owns. It uses it, but does not
//      create or delete it.
//    FRIEND: GameEngine is a friend class of Character (sets isCulprit).
//    DEPENDENCY (GameEngine - - -> GameException): it throws and catches
//      exceptions, but does not keep them as members.
// =====================================================================

class GameEngine
{
private:
    // ----- Singleton -----
    static GameEngine* instance;     // the one and only engine

    GameEngine();                    // PRIVATE constructor
    ~GameEngine();                   // private: only destroyInstance() deletes it

    // ----- Composition: parts owned by the engine -----
    Detective detective;             // the player
    Cottage cottage;                 // 7 locations (Word 1)
    Scrabble scrabble;               // the letter board
    TileRack finalClueRack;          // Word 1 + " " + Word 2 (made with operator+)

    // The 8 people of the cottage (created from characters.txt)
    static const int MAX_PEOPLE = 8;
    Character* people[MAX_PEOPLE];
    int peopleCount;

    // ----- Association: used, but not owned -----
    ScoreBoard* scoreBoard;

    // ----- Game data -----
    GameRules rules;                 // from rules.txt (or the defaults)
    GameState state;                 // Menu, Playing, Won, Lost, Exit
    int profileNumber;               // line of the culprit in clue.txt (1-7)
    string culpritRole;              // e.g. "Guard"
    string targetWord1;              // e.g. "GERMANS"
    string targetWord2;              // e.g. "SHEPHERD"

    // ----- Screen and input helpers -----
    void clearScreen() const;
    void pressEnterToContinue() const;
    int readNumber(int minimum, int maximum) const;    // throws on wrong input
    string readDetectiveName() const;                  // no empty name, no spaces

    // ----- Main menu actions -----
    void showTitle() const;
    void showMainMenu() const;
    void showLeaderBoard() const;
    void showAbout() const;
    void showRules();

    // ----- Module 2 : game initialization -----
    void loadRules();                // rules.txt, or the defaults if it fails
    void chooseRandomCulprit();      // random profile from clue.txt
    void markCulprit();              // sets isCulprit = true (friend access)
    void freePeople();               // deletes the 8 character objects
    void startNewGame();
    void showGameStatus() const;     // credibility, turns, racks, board

    // ----- Module 13 : game menu -----
    void playGame();                 // the turn loop of one game
    void showGameMenu() const;
    bool askYesNo(string question) const;
    void giveWrongStepPenalty(string reason);   // -5 credibility
    void useTurn();                             // turns + 1
    void checkGameOver();                       // credibility 0 or no turns left
    bool visitLocation();            // returns true if a turn was used
    bool interviewPerson();          // returns true if a turn was used
    void showPeopleMenu() const;
    void viewBoard() const;          // free: no turn

public:
    // Copying is not allowed ( = delete ): there must be only ONE engine
    GameEngine(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) = delete;

    // The ONLY way to get the engine
    static GameEngine* getInstance();

    // Deletes the engine at the end of the program (no memory leak)
    static void destroyInstance();

    // Connects the leader/score board (association)
    void setScoreBoard(ScoreBoard* board);

    // Runs the whole game (main menu loop)
    void run();
};


// =====================================================================
//  HELPER FUNCTIONS
//  An enum is stored as a number, so these functions convert an enum
//  value into readable text that we can show on the screen.
// =====================================================================
string wordRowToText(WordRow row);
string gameStateToText(GameState state);
string menuChoiceToText(MenuChoice choice);

// String manipulation: changes "Study_Room" into "Study Room" for display
string replaceUnderscores(string text);

// String manipulation: changes "germans" into "GERMANS"
string toUpperCase(string text);

// ----- Helpers for reading the .txt files (Module 6) -----

// Removes spaces, tabs and '\r' from the start and the end of a text
string trimSpaces(string text);

// Cuts a line into parts at every separator, e.g. "A|B|C" -> A, B, C.
// The parts are stored in the given array; returns how many parts were found.
int splitLine(string line, char separator, DynamicArray<string>& parts);

// Changes text like "25" or "-5" into a number. Returns false if the text
// is not a whole number (so we can report a corrupted file).
bool textToNumber(string text, int& number);

// Makes a number 2 digits long: 5 -> "05", 12 -> "12"
string twoDigits(int number);

// Adds spaces at the end of a text until it has the given width (for tables)
string padRight(string text, int width);

#endif // HEIST_H
