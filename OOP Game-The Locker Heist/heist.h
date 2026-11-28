//  This is the header file , it only declares things  (enums, structs, classes and function prototypes)

#ifndef HEIST_H     // Header guard: stops this file from being
#define HEIST_H     // included more than once in the same program

#include <iostream>
#include <string>
#include <fstream>     // ifstream / ofstream for the .txt files (Module 6)
#include <cstdlib>     // rand() and srand() to pick a random culprit
#include <ctime>       // time() for the random seed and today date

using namespace std;

enum WordRow
{
    WORD_1,     // letters collected from the 7 locations
    WORD_2      // letters collected from the 8 people
};

// gamestate stores the current state of the player in the game.
enum GameState
{
    STATE_MENU,      // player is on the main menu
    STATE_PLAYING,   // a game is running
    STATE_WON,       // player caught the culprit
    STATE_LOST,      // credibility became zero or turns finished
    STATE_EXIT       // player wants to close the program
};

// menuchoice is used for the Main Menu options.
enum MenuChoice
{
    MENU_NEW_GAME = 1,   // 1 new Game
    MENU_LEADERBOARD,    // 2 view Leader/Score Board
    MENU_ABOUT,          // 3 about
    MENU_RULES,          // 4 rules
    MENU_EXIT            // 5 exit
};

// Tile is one box slot of the scrabble board or of a tileRack.
struct Tile
{
    char tileLetter = ' ';   // letter placed in this box
    bool filled = false;     // true when a letter has been placed here
    bool operator==(const Tile& other) const;
    bool operator!=(const Tile& other) const;
    friend ostream& operator<<(ostream& out, const Tile& tile);
    friend istream& operator>>(istream& in, Tile& tile);
};

// clue is a carrier, it carries one letter and its position
// from a location or a character to the scrabble board.
struct Clue
{
    char clueLetter = ' ';      // the letter given as a clue
    int cluePosition = 0;       // board position of the letter 
    WordRow clueRow = WORD_1;   // which word  this letter belongs to
};

// Location is one place of the cottage 
struct Location
{
    string locName = "";           // name of the place e.g. study room
    char letterClueLoc = ' ';      // letter hidden at this place
    int boardPosLoc = 0;           // position of that letter in Word 1
    WordRow locRow = WORD_1;       // always word 1 for locations
    bool locSearchFlag = false;    // true after the detective searches it

    friend ostream& operator<<(ostream& out, const Location& place);
    friend istream& operator>>(istream& in, Location& place);
};

// base class for every error of the game
class GameException
{
protected:
    string message;     // details of what went wrong

public:
    GameException(string errorMessage = "Something went wrong in the game.");

    // Returns the details of the error
    string getMessage() const;

    // Returns the name type of the error.
    virtual string getErrorType() const;

    // returns type + message together ready to print on screen
    string getFullMessage() const;

    // virtual destructor so derived exception objects are destroyed properly
    virtual ~GameException();
};

// thrown when the user types something wrong
class InvalidInputException : public GameException
{
public:
    InvalidInputException(string details);
    string getErrorType() const override;
};

// thrown when a position or menu number is outside the allowed range
class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(int position, int maxPosition);
    string getErrorType() const override;
};

// thrown when a letter is placed on a board slot that is already filled
class SlotFilledException : public GameException
{
public:
    SlotFilledException(int position);
    string getErrorType() const override;
};

// thrown when the detective tries to form a word but the row is not full yet
class WordIncompleteException : public GameException
{
public:
    WordIncompleteException(WordRow row);
    string getErrorType() const override;
};

// thrown when a letter is added to a word row that is already complete
class WordCompleteException : public GameException
{
public:
    WordCompleteException(WordRow row);
    string getErrorType() const override;
};

// thrown when the detective types a wrong word
class InvalidWordException : public GameException
{
public:
    InvalidWordException(string typedWord);
    string getErrorType() const override;
};

// thrown when a letter is added to a TileRack that has no space left
class RackFullException : public GameException
{
public:
    RackFullException();
    string getErrorType() const override;
};

// thrown when we try to take a letter out of an empty tilerack
class RackEmptyException : public GameException
{
public:
    RackEmptyException();
    string getErrorType() const override;
};

// Thrown when the same clue letter is collected a second time
class AlreadyVisitedException : public GameException
{
public:
    AlreadyVisitedException(string placeOrPerson);
    string getErrorType() const override;
};

// thrown when the detective credibility is too low for an interview
class InterviewDeniedException : public GameException
{
public:
    InterviewDeniedException(string personName, int requiredCredibility);
    string getErrorType() const override;
};

// thrown when a .txt file cannot be opened
class FileNotFoundException : public GameException
{
public:
    FileNotFoundException(string fileName);
    string getErrorType() const override;
};

// thrown when a .txt file is opened but its data is in a wrong format
class FileCorruptedException : public GameException
{
public:
    FileCorruptedException(string fileName, string details);
    string getErrorType() const override;
};

//  This class does all the new / delete / deep copy work in one place, so cottage, scrabble and tilerack become shorter and safer.
template <class T>
class DynamicArray
{
private:
    T* data;      // the array on the heap
    int size;     // number of elements

public:
    // constructor, creates an array of the given size , at least 1
    DynamicArray(int arraySize = 1)
    {
        if (arraySize < 1)
        {
            arraySize = 1;
        }
        size = arraySize;
        data = new T[size];
    }

    // copy constructor: dEEP copy, new memory,  then copy every element
    DynamicArray(const DynamicArray<T>& other)
    {
        size = other.size;
        data = new T[size];
        for (int i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    // assignment operator: deep copy
    DynamicArray<T>& operator=(const DynamicArray<T>& other)
    {
        if (this != &other)              // self assignment check
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

    // destructor, frees the heap memory
    ~DynamicArray()
    {
        delete[] data;
        data = nullptr;
    }

    // it checks the index so we can never go outside the array
    T& operator[](int index)
    {
        if (index < 0 || index >= size)
        {
            throw InvalidPositionException(index + 1, size);
        }
        return data[index];
    }

    // const objects , read only
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

//  Stores the letters collected by the detective, together with the board position of every letter.
class TileRack
{
private:
    DynamicArray<Tile> tiles;       // letters
    DynamicArray<int> positions;    // board positions 
    int capacity;       // maximum letters 
    int count;          // letters stored so far

public:
    TileRack(int rackCapacity = 7);
    TileRack(const TileRack& other);              // copy constructor
    TileRack& operator=(const TileRack& other);   // assignment operator 
    ~TileRack();                                  // frees heap memory

    // stores the letter and position carried by a clue
    void addTile(Clue clue);

    // removes the last added letter and returns it
    char removeLastTile();

    //  checks and getters 
    bool isFull() const;
    bool isEmpty() const;
    int getCount() const;
    int getCapacity() const;
    bool hasPosition(int position) const;     // is this position already collected?
    char getLetterAt(int index) const;        // index 0 = first collected letter
    int getPositionAt(int index) const;

    // string manipulation
    string getLettersInOrder() const;         // order of collection
    string getLettersByPosition() const;      // board order

    // shows every letter with its position
    void display() const;

    TileRack operator+(const TileRack& other) const;
};

//  Holds the common data of every person in the game.
class Character
{
    friend class GameEngine;
    friend ostream& operator<<(ostream& out, const Character& person);
    friend istream& operator>>(istream& in, Character& person);

protected:
    string characterName;   
    string characterRole;   
    string nationality;     
    bool isCulprit;         // true only for the selected culprit
    bool interviewed;       // true after the detective interviews this person 

    // prints name, role and nationality.
    void displayBasicInfo() const;

public:
    Character(string name, string role, string nationalityText);

    virtual ~Character();

    // getters 
    string getName() const;
    string getRole() const;
    string getNationality() const;
    bool getIsCulprit() const;
    bool isInterviewed() const;

    // setters 
    void setName(string name);
    void setNationality(string nationalityText);
    void setInterviewed(bool value);

    // minimum credibility the detective needs to interview this person.
    virtual int getRequiredCredibility() const;

    bool canBeInterviewed(int credibility) const;
    virtual Clue getClue() const;
    virtual bool canBeAccused() const;
    virtual void display() const = 0;
};

class Victim : public Character
{
private:
    WordRow clueRowChar;   
    char letterClueChar;    
    int boardPosChar;       

public:
    // static const one fixed value shared by all victim objects
    static const int REQUIRED_CREDIBILITY = 80;

    Victim(string name, string nationalityText, char letter, int position);

    char getLetter() const;
    int getPosition() const;
    WordRow getRow() const;

    Clue getClue() const override;
    int getRequiredCredibility() const override;
    void display() const override;
};


class Detective : public Character
{
private:
    int credibility;        // starts from 100, game is lost at 0
    int turns;              // turns used so far
    string solvedWords;     // words solved correctly
    TileRack word1Rack;     // 7 letters collected from the locations
    TileRack word2Rack;     // 8 letters collected from the people

public:
    Detective(string name, int startingCredibility);
    ~Detective();

    //  getters 
    int getCredibility() const;
    int getTurns() const;
    string getSolvedWords() const;
    const TileRack& getRack(WordRow row) const;   // read only access to a rack

    // setters 
    void setTurns(int turnCount);
    void loseCredibility(int amount);   // never goes below 0
    void addSolvedWord(string word);
    
    void collectClue(Clue clue);
    void displayRacks() const;
    void display() const override;
};

class Suspect : public Character
{
protected:
    WordRow clueRowChar;    // always word 2
    char letterClueChar;    // letter given by this suspect
    int boardPosChar;       // position of that letter in word 2

public:
    Suspect(string name, string role, string nationalityText, char letter, int position);

    char getLetter() const;
    int getPosition() const;
    WordRow getRow() const;

    Clue getClue() const override;

    // every suspect can be accused
    bool canBeAccused() const override;

    virtual void display() const = 0;   // still pure virtual
};

class HouseHelp : public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 0;

    HouseHelp(string name, string role, string nationalityText, char letter, int position);

    int getRequiredCredibility() const override;
};

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

class BusinessRelations : virtual public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 50;
    BusinessRelations();
    int getRequiredCredibility() const override;
};

class CloseRelations : virtual public Suspect
{
public:
    static const int REQUIRED_CREDIBILITY = 75;
    CloseRelations();
    int getRequiredCredibility() const override;
};

class Partner : public BusinessRelations
{
public:
    Partner(string name, string role, string nationalityText, char letter, int position);
    void display() const override;
};

class OwnerChild : public CloseRelations
{
public:
    OwnerChild(string name, string nationalityText, char letter, int position);
    void display() const override;
};

class PA : public BusinessRelations, public CloseRelations
{
public:
    static const int REQUIRED_CREDIBILITY = 35;

    PA(string name, string nationalityText, char letter, int position);

    int getRequiredCredibility() const override;
    void display() const override;
};


class Cottage
{
private:
    DynamicArray<Location> locations;
    void checkLocationNumber(int number) const;     // throws if not 1 7

public:
    static const int TOTAL_LOCATIONS = 7;
    Cottage();                                   // creates 7 empty locations
    Cottage(const Cottage& other);               // copy constructor 
    Cottage& operator=(const Cottage& other);    // assignment operator
    ~Cottage();                                  // frees all heap memory

    void setLocation(int number, string name, char letter, int position);

    Clue searchLocation(int number);

    // getters 
    bool isSearched(int number) const;
    string getLocationName(int number) const;
    int getLocationCount() const;
    int getSearchedCount() const;

    // shows all 7 locations with their search status
    void displayLocations() const;
};

class Scrabble
{
private:
    DynamicArray< DynamicArray<Tile> > board;
    string targetWord1;     // correct Word 1 from clue.txt
    string targetWord2;     // correct Word 2 from clue.txt
    void checkPosition(WordRow row, int position) const;  // throws if wrong

public:
    // static const values shared by every Scrabble object
    static const int ROW_COUNT = 2;
    static const int WORD1_LENGTH = 7;
    static const int WORD2_LENGTH = 8;

    Scrabble();                                     // creates an empty board
    Scrabble(const Scrabble& other);                // copy constructor 
    Scrabble& operator=(const Scrabble& other);     // assignment operator
    ~Scrabble();                                    // frees all heap memory

    // saves the 2 correct words of the selected culprit 
    void setTargetWords(string word1, string word2);

    // places the letter of a clue at its position on the board
    void placeLetter(Clue clue);

    // returns the letter at a position , if the slot is still empty
    char revealLetter(WordRow row, int position) const;

    // checks and getters 
    int getRowLength(WordRow row) const;
    bool isSlotFilled(WordRow row, int position) const;
    bool isRowComplete(WordRow row) const;
    int getFilledCount(WordRow row) const;
    string getPlacedWord(WordRow row) const;        // letters placed so far
    string getTargetWord(WordRow row) const;

    // compares the word typed by the detective with the correct word
    bool isWordCorrect(WordRow row, string typedWord) const;

    //  display 
    void displayWord(WordRow row) const;            // one row
    void displayBoard() const;                      // both rows

    friend ostream& operator<<(ostream& out, const Scrabble& scrabble);
    friend istream& operator>>(istream& in, Scrabble& scrabble);
};

// names of the data files 
const string CLUE_FILE = "clue.txt";
const string LOCATIONS_FILE = "locations.txt";
const string CHARACTERS_FILE = "characters.txt";
const string RULES_FILE = "rules.txt";
const string SCORE_FILE = "scoreBoard.txt";     
const string ABOUT_FILE = "about.txt";

struct GameRules
{
    int maxTurns = 25;
    int startingCredibility = 100;
    int wrongTurnPenalty = 5;
    int wrongWordPenalty = 10;
    string winCondition = "Accuse_Correct_Culprit";
    string loseCondition = "Credibility_Zero_Or_Turns_Exhausted";
};


class GameFileReader
{
private:
    // throws if the profile number is not between 1 and 7
    static void checkProfileNumber(int profileNumber);

    // makes the correct character object from its role
    static Character* createCharacter(string role, string name, string nationalityText,char letter, int position);

public:
    static const int TOTAL_PROFILES = 7;
    static int countProfiles(string fileName = CLUE_FILE);
    static void readClueProfile(int profileNumber, string& culpritRole,string& word1, string& word2,string fileName = CLUE_FILE);
    static void loadLocations(Cottage& cottage, int profileNumber,string fileName = LOCATIONS_FILE);
    static int loadCharacters(Character* people[], int maxPeople, int profileNumber,string fileName = CHARACTERS_FILE);
    static GameRules readRules(string fileName = RULES_FILE);
    static void showTextFile(string fileName);
};

class ScoreBoard
{
private:
    string fileName;
    string getTodayDate() const;     
    bool fileHasData() const;       

public:
    ScoreBoard(string scoreFileName = SCORE_FILE);
    void addScore(string detectiveName, bool won, int credibilityLeft,int turnsLeft, string culpritRole);
    void displayScores() const;
};

class GameEngine
{
private:
    static GameEngine* instance;     
    GameEngine();                    
    ~GameEngine();                   
    Detective detective;            
    Cottage cottage;                 
    Scrabble scrabble;               
    TileRack finalClueRack;         

    // the 8 people of the cottage 
    static const int MAX_PEOPLE = 8;
    Character* people[MAX_PEOPLE];
    int peopleCount;
    ScoreBoard* scoreBoard;

    //  game data 
    GameRules rules;                 
    GameState state;                 
    int profileNumber;               
    string culpritRole;             
    string targetWord1;              
    string targetWord2;             

    // screen and input helpers
    void clearScreen() const;
    void pressEnterToContinue() const;
    int readNumber(int minimum, int maximum) const;    // throws on wrong input
    string readDetectiveName() const;                  // no empty name, no spaces

    // main menu actions 
    void showTitle() const;
    void showMainMenu() const;
    void showLeaderBoard() const;
    void showAbout() const;
    void showRules();

    void loadRules();              
    void chooseRandomCulprit();      
    void markCulprit();              
    void freePeople();               
    void startNewGame();
    void showGameStatus() const;     

    
    void playGame();               
    void showGameMenu() const;
    bool askYesNo(string question) const;
    void giveWrongStepPenalty(string reason);   
    void useTurn();                             
    void checkGameOver();                       
    bool visitLocation();           
    bool interviewPerson();         
    void showPeopleMenu() const;
    void viewBoard() const;          

   
    bool word1Solved;              
    bool word2Solved;                

    bool formWord();                 
    bool accuseSuspect();           
    void showFinalClue() const;      
    Character* findCulprit() const;  
    void finishGame();              

public:
    GameEngine(const GameEngine& other) = delete;
    GameEngine& operator=(const GameEngine& other) = delete;
    static GameEngine* getInstance();
    static void destroyInstance();
    void setScoreBoard(ScoreBoard* board);
    void run();
};

string wordRowToText(WordRow row);
string gameStateToText(GameState state);
string menuChoiceToText(MenuChoice choice);
string replaceUnderscores(string text);
string toUpperCase(string text);
string trimSpaces(string text);

int splitLine(string line, char separator, DynamicArray<string>& parts);
bool textToNumber(string text, int& number);
string twoDigits(int number);
string padRight(string text, int width);

#endif // HEIST_H
