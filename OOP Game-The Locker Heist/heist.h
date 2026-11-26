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
protected:
    string characterName;   // e.g. "Mr.Enzo"
    string characterRole;   // e.g. "Cook"
    string nationality;     // e.g. "Italian"
    bool isCulprit;         // true only for the selected culprit

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

    // ----- Setters -----
    void setName(string name);
    void setNationality(string nationalityText);

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
//  The PLAYER. Stores credibility, number of turns and solved words.
//  NOTE: The turn count is increased by the GameEngine. The Detective
//  only stores it. The letter racks will be added in Module 5.
// ---------------------------------------------------------------------
class Detective : public Character
{
private:
    int credibility;        // starts from 100, game is lost at 0
    int turns;              // turns used so far
    string solvedWords;     // words solved correctly, e.g. "GERMANS"

public:
    Detective(string name, int startingCredibility);
    ~Detective();

    // ----- Getters -----
    int getCredibility() const;
    int getTurns() const;
    string getSolvedWords() const;

    // ----- Setters / actions -----
    void setTurns(int turnCount);
    void loseCredibility(int amount);   // never goes below 0
    void addSolvedWord(string word);

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
//  HELPER FUNCTIONS
//  An enum is stored as a number, so these functions convert an enum
//  value into readable text that we can show on the screen.
// =====================================================================
string wordRowToText(WordRow row);
string gameStateToText(GameState state);
string menuChoiceToText(MenuChoice choice);

#endif // HEIST_H
