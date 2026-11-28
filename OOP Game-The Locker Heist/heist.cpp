#include "heist.h"

// Converts a wordrow value into text
string wordRowToText(WordRow row)
{
    if (row == WORD_1)
    {
        return "Word 1";
    }
    return "Word 2";
}

// converts a gamestate value into text so it can be printed
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

// converts a menuchoice value into the text shown on the main menu
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

// constructor, saves the error message
GameException::GameException(string errorMessage)
{
    message = errorMessage;
}

// returns only the error details
string GameException::getMessage() const
{
    return message;
}

// base class name. derived classes override this function.
string GameException::getErrorType() const
{
    return "Game Error";
}

// Joins the error type and the message
string GameException::getFullMessage() const
{
    return "[" + getErrorType() + "] " + message;
}

// Destructor: nothing to free, but it is virtual for safe deletion
GameException::~GameException()
{
}

// base class constructor gameexception using the initializer list.
InvalidInputException::InvalidInputException(string details) : GameException("Invalid input: " + details)
{
}

string InvalidInputException::getErrorType() const
{
    return "Invalid Input";
}

InvalidPositionException::InvalidPositionException(int position, int maxPosition) : GameException("Position " + to_string(position) + " is not valid. Please choose between 1 and " +to_string(maxPosition) + ".")
{
}

string InvalidPositionException::getErrorType() const
{
    return "Invalid Position";
}

SlotFilledException::SlotFilledException(int position): GameException("Slot " + to_string(position) + " on the board is already filled.")
{
}

string SlotFilledException::getErrorType() const
{
    return "Slot Filled";
}

WordIncompleteException::WordIncompleteException(WordRow row) : GameException(wordRowToText(row) +" is not complete yet. Collect all its letters first.")
{
}

string WordIncompleteException::getErrorType() const
{
    return "Word Incomplete";
}

WordCompleteException::WordCompleteException(WordRow row): GameException(wordRowToText(row) + " is already complete. No more letters can be placed.")
{
}

string WordCompleteException::getErrorType() const
{
    return "Word Complete";
}

InvalidWordException::InvalidWordException(string typedWord) : GameException("\"" + typedWord + "\" is not the correct word.")
{
}

string InvalidWordException::getErrorType() const
{
    return "Wrong Word";
}

RackFullException::RackFullException() : GameException("The tile rack is full. No more letters can be added.")
{
}

string RackFullException::getErrorType() const
{
    return "Rack Full";
}

RackEmptyException::RackEmptyException() : GameException("The tile rack is empty. There is no letter to remove.")
{
}

string RackEmptyException::getErrorType() const
{
    return "Rack Empty";
}

AlreadyVisitedException::AlreadyVisitedException(string placeOrPerson): GameException("The clue from " + placeOrPerson + " has already been collected.")
{
}

string AlreadyVisitedException::getErrorType() const
{
    return "Already Visited";
}

InterviewDeniedException::InterviewDeniedException(string personName, int requiredCredibility) : GameException(personName + " refuses to talk. You need at least " +to_string(requiredCredibility) + " credibility.")
{
}

string InterviewDeniedException::getErrorType() const
{
    return "Interview Denied";
}

FileNotFoundException::FileNotFoundException(string fileName) : GameException("The file \"" + fileName + "\" was not found.")
{
}

string FileNotFoundException::getErrorType() const
{
    return "File Not Found";
}

FileCorruptedException::FileCorruptedException(string fileName, string details) : GameException("The file \"" + fileName + "\" is corrupted: " + details)
{
}

string FileCorruptedException::getErrorType() const
{
    return "File Corrupted";
}

Character::Character(string name, string role, string nationalityText)
{
    characterName = name;
    characterRole = role;
    nationality = nationalityText;
    isCulprit = false;
    interviewed = false;    // nobody has been interviewed at the start
}
// Virtual destructor
Character::~Character()
{
}

// Prints the common details. Used by every childs display().
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

bool Character::isInterviewed() const
{
    return interviewed;
}

void Character::setInterviewed(bool value)
{
    interviewed = value;
}

// base version
int Character::getRequiredCredibility() const
{
    return 0;
}

// Checks the condition with the help of the virtual function above
bool Character::canBeInterviewed(int credibility) const
{
    return credibility >= getRequiredCredibility();
}

// base version returns an empty clue 
Clue Character::getClue() const
{
    Clue emptyClue;
    return emptyClue;
}

// base version, a Character cannot be accused 
bool Character::canBeAccused() const
{
    return false;
}

Victim::Victim(string name, string nationalityText, char letter, int position): Character(name, "Victim", nationalityText)
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

// packs the letter, position and row into a clue
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


Detective::Detective(string name, int startingCredibility) : Character(name, "Detective", "Malay"),word1Rack(7),word2Rack(8)
{
    credibility = startingCredibility;
    turns = 0;
    solvedWords = "";
}

Detective::~Detective()
{
}

const TileRack& Detective::getRack(WordRow row) const
{
    if (row == WORD_1)
    {
        return word1Rack;
    }
    return word2Rack;
}

// puts the clue in the rack of its own word
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

// shows the letters of both racks
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

// gameEngine counts the turns and stores the new value here
void Detective::setTurns(int turnCount)
{
    turns = turnCount;
}

// reduces credibility after a wrong step it never becomes negative
void Detective::loseCredibility(int amount)
{
    credibility = credibility - amount;

    if (credibility < 0)
    {
        credibility = 0;
    }
}

// adds a correctly solved word, separated by a space
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

Suspect::Suspect(string name, string role, string nationalityText, char letter, int position): Character(name, role, nationalityText)
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

// Packs the suspect letter into a clue
Clue Suspect::getClue() const
{
    Clue suspectClue;
    suspectClue.clueLetter = letterClueChar;
    suspectClue.cluePosition = boardPosChar;
    suspectClue.clueRow = clueRowChar;
    return suspectClue;
}

// all 7 suspects can be accused by the detective
bool Suspect::canBeAccused() const
{
    return true;
}

HouseHelp::HouseHelp(string name, string role, string nationalityText, char letter, int position): Suspect(name, role, nationalityText, letter, position)
{
}

// house help can be interviewed at any time
int HouseHelp::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 0
}

Cook::Cook(string name, string nationalityText, char letter, int position): HouseHelp(name, "Cook", nationalityText, letter, position)
{
}

void Cook::display() const
{
    cout << "------ House Help : Cook ------" << endl;
    displayBasicInfo();
    cout << "Note        : Prepared the food for the party." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}

Guard::Guard(string name, string nationalityText, char letter, int position): HouseHelp(name, "Guard", nationalityText, letter, position)
{
}

void Guard::display() const
{
    cout << "------ House Help : Guard ------" << endl;
    displayBasicInfo();
    cout << "Note        : Was guarding the cottage gate." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}

Servant::Servant(string name, string nationalityText, char letter, int position): HouseHelp(name, "Servant", nationalityText, letter, position)
{
}

void Servant::display() const
{
    cout << "------ House Help : Servant ------" << endl;
    displayBasicInfo();
    cout << "Note        : Served the guests in every room." << endl;
    cout << "Interview   : Allowed at any time" << endl;
}

// empty ,  the shared suspect part is built by partner or PA
BusinessRelations::BusinessRelations()
{
}

int BusinessRelations::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 50
}
// empoty,  the shared Suspect part is built by OwnerChild or PA
CloseRelations::CloseRelations()
{
}

int CloseRelations::getRequiredCredibility() const
{
    return REQUIRED_CREDIBILITY;   // 75
}

Partner::Partner(string name, string role, string nationalityText, char letter, int position): Suspect(name, role, nationalityText, letter, position),BusinessRelations()
{
}

void Partner::display() const
{
    cout << "------ Business Relation : " << characterRole << " ------" << endl;
    displayBasicInfo();
    cout << "Note        : Owns a share in the owner's business." << endl;
    cout << "Interview   : Needs credibility >= " << getRequiredCredibility() << endl;
}

OwnerChild::OwnerChild(string name, string nationalityText, char letter, int position): Suspect(name, "Child", nationalityText, letter, position),CloseRelations()
{
}

void OwnerChild::display() const
{
    cout << "------ Close Relation : Owner's Child ------" << endl;
    displayBasicInfo();
    cout << "Note        : A teenager (15-19), panicked after the theft." << endl;
    cout << "Interview   : Needs credibility >= " << getRequiredCredibility() << endl;
}

PA::PA(string name, string nationalityText, char letter, int position): Suspect(name, "PA", nationalityText, letter, position),BusinessRelations(),CloseRelations()
{
}

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

// Constructor ,  DynamicArray creates the 7 empty locations for us
Cottage::Cottage(): locations(TOTAL_LOCATIONS)
{
}

// Copy constructor, "Cottage copy = original;" (deep copy)
Cottage::Cottage(const Cottage& other): locations(other.locations)
{
}

Cottage& Cottage::operator=(const Cottage& other)
{
    if (this != &other)               // self assignment check
    {
        locations = other.locations;  // DynamicArray operator= does the deep copy
    }
    return *this;
}

// Destructor: nothing to delete here ~DynamicArray() deletes every  location and the array itself automatically.
Cottage::~Cottage()
{
}

// The player uses numbers 1 to 7 ,  anything else is an invalid position
void Cottage::checkLocationNumber(int number) const
{
    if (number < 1 || number > TOTAL_LOCATIONS)
    {
        throw InvalidPositionException(number, TOTAL_LOCATIONS);
    }
}

// fills a location,  Number 1 is stored at index 0 and so on
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

Clue Cottage::searchLocation(int number)
{
    checkLocationNumber(number);
    Location& place = locations[number - 1];   // reference to the real location
    place.locSearchFlag = true;
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

// counts how many locations have been searched so far
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

void Cottage::displayLocations() const
{
    cout << "====== Location Menu ======" << endl;
    for (int i = 0; i < TOTAL_LOCATIONS; i++)
    {
        cout << "[" << (i + 1) << "] " << locations[i] << endl;
    }
    cout << "===========================" << endl;
}

//  helper function
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

// Constructor, makes the outer array with 2 rows, then gives each row
// its own length
// Every Tile starts empty because of the struct defaults
Scrabble::Scrabble() : board(ROW_COUNT)
{
    targetWord1 = "";
    targetWord2 = "";
    board[WORD_1] = DynamicArray<Tile>(WORD1_LENGTH);   
    board[WORD_2] = DynamicArray<Tile>(WORD2_LENGTH);   
}

// Copy constructor ,  DynamicArray copies both rows for us
Scrabble::Scrabble(const Scrabble& other): board(other.board)
{
    targetWord1 = other.targetWord1;
    targetWord2 = other.targetWord2;
}

// Assignment operator 
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

// Destructor , nothing to delete here. ~DynamicArray() frees each row and then the outer array automatically.
Scrabble::~Scrabble()
{
}

void Scrabble::checkPosition(WordRow row, int position) const
{
    int rowLength = getRowLength(row);

    if (position < 1 || position > rowLength)
    {
        throw InvalidPositionException(position, rowLength);
    }
}

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

// places a letter on the board. Three checks are done first
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

// Shows the letter that was placed at a position ,used when the player visits a place or a person again
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

// a row is complete when every slot is filled
bool Scrabble::isRowComplete(WordRow row) const
{
    return getFilledCount(row) == getRowLength(row);
}

// joins the placed letters into a string 
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

// The detective can type a word only when its row is complete
bool Scrabble::isWordCorrect(WordRow row, string typedWord) const
{
    if (isRowComplete(row) == false)
    {
        throw WordIncompleteException(row);
    }
    typedWord = toUpperCase(typedWord);
    string correctWord = getTargetWord(row);

    if (typedWord.length() != correctWord.length())
    {
        return false;
    }

    // compare letter by letter 
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

// prints one row 
void Scrabble::displayWord(WordRow row) const
{
    cout << "  " << wordRowToText(row) << ": ";

    for (int i = 0; i < getRowLength(row); i++)
    {
        cout << board[row][i] << " ";    
    }

    if (isRowComplete(row))
    {
        cout << "-> COMPLETE";
    }
    cout << endl;
}

// prints the whole board 
void Scrabble::displayBoard() const
{
    cout << *this;
}

// constructor, the two DynamicArray members are created in the
TileRack::TileRack(int rackCapacity): tiles(rackCapacity), positions(rackCapacity)
{
    capacity = tiles.getSize();   // DynamicArray makes the size at least 1
    count = 0;
    for (int i = 0; i < capacity; i++)
    {
        positions[i] = 0;    // 0 = no position yet
    }
}

// Copy constructor , DynamicArray own copy constructor
TileRack::TileRack(const TileRack& other) : tiles(other.tiles), positions(other.positions)
{
    capacity = other.capacity;
    count = other.count;
}

// Assignment operator 
TileRack& TileRack::operator=(const TileRack& other)
{
    if (this != &other)        
    {
        tiles = other.tiles;
        positions = other.positions;
        capacity = other.capacity;
        count = other.count;
    }
    return *this;
}

// Destructor, nothing to delete here. The DynamicArray members free their own memory in ~DynamicArray()
TileRack::~TileRack()
{
}

// Adds a letter at the end of the rack. Two checks first:
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

// Removes the last letter
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

// Letters in the order they were collected 
string TileRack::getLettersInOrder() const
{
    string letters = "";
    for (int i = 0; i < count; i++)
    {
        letters = letters + tiles[i].tileLetter;
    }
    return letters;
}

// Letters arranged by their board position
string TileRack::getLettersByPosition() const
{
    string word(capacity, '_');     
    for (int i = 0; i < count; i++)
    {
        int index = positions[i] - 1;          
        if (index >= 0 && index < capacity)
        {
            word[index] = tiles[i].tileLetter;
        }
    }
    return word;
}

// Shows every letter with its position, or empty
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

// Two tiles are equal when their letters are the same
bool Tile::operator==(const Tile& other) const
{
    return tileLetter == other.tileLetter;
}

// Not equal simply uses the == operator above
bool Tile::operator!=(const Tile& other) const
{
    return !(*this == other);
}

// Prints a tile 
ostream& operator<<(ostream& out, const Tile& tile)
{
    out << "[" << tile.tileLetter << "]";
    return out;      
}

// Reads one letter into a tile 
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

// Prints a location 
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

// Reads a location 
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

// Prints any character
ostream& operator<<(ostream& out, const Character& person)
{
    person.display();
    return out;
}

// Reads a new name and nationality, being a friend this function can change the protected data of Character directly.
istream& operator>>(istream& in, Character& person)
{
    in >> person.characterName >> person.nationality;
    return in;
}

// Joins two racks into a new rack 
TileRack TileRack::operator+(const TileRack& other) const
{
    string fullClue = getLettersByPosition() + " " + other.getLettersByPosition();
    TileRack mergedRack((int)fullClue.length());

    for (int i = 0; i < (int)fullClue.length(); i++)
    {
        Clue letterClue;
        letterClue.clueLetter = fullClue[i];
        letterClue.cluePosition = i + 1;     
        letterClue.clueRow = WORD_1;         
        mergedRack.addTile(letterClue);
    }
    return mergedRack;
}

// Prints the whole board,  As a friend it reads the private board directly
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

// Reads a move
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

// Removes spaces
string trimSpaces(string text)
{
    int start = 0;
    int end = (int)text.length() - 1;

    while (start <= end && (text[start] == ' ' || text[start] == '\t' || text[start] == '\r'))
    {
        start++;
    }
    while (end >= start && (text[end] == ' ' || text[end] == '\t' || text[end] == '\r'))
    {
        end--;
    }

    if (start > end)
    {
        return "";
    }
    return text.substr(start, end - start + 1);
}

int splitLine(string line, char separator, DynamicArray<string>& parts)
{
    line = trimSpaces(line);
    if (line.length() > 0 && line[0] == separator)
    {
        line = line.substr(1);                      
    }
    if (line.length() > 0 && line[line.length() - 1] == separator)
    {
        line = line.substr(0, line.length() - 1);    
    }

    int partCount = 0;
    string currentPart = "";

    for (int i = 0; i < (int)line.length(); i++)
    {
        if (line[i] == separator)
        {
            if (partCount < parts.getSize())
            {
                parts[partCount] = trimSpaces(currentPart);
            }
            partCount++;
            currentPart = "";
        }
        else
        {
            currentPart = currentPart + line[i];
        }
    }

    if (partCount < parts.getSize())
    {
        parts[partCount] = trimSpaces(currentPart);
    }
    partCount++;
    return partCount;
}

bool textToNumber(string text, int& number)
{
    text = trimSpaces(text);
    if (text == "")
    {
        return false;
    }

    int index = 0;
    bool negative = false;

    if (text[0] == '-' || text[0] == '+')
    {
        negative = (text[0] == '-');
        index = 1;
        if (text.length() == 1)
        {
            return false;     
        }
    }

    int result = 0;
    for (; index < (int)text.length(); index++)
    {
        if (text[index] < '0' || text[index] > '9')
        {
            return false;     // not a digit
        }
        result = result * 10 + (text[index] - '0');
    }

    number = negative ? -result : result;
    return true;
}

// 5 -> "05"
string twoDigits(int number)
{
    if (number >= 0 && number < 10)
    {
        return "0" + to_string(number);
    }
    return to_string(number);
}

string padRight(string text, int width)
{
    while ((int)text.length() < width)
    {
        text = text + " ";
    }
    return text;
}

void GameFileReader::checkProfileNumber(int profileNumber)
{
    if (profileNumber < 1 || profileNumber > TOTAL_PROFILES)
    {
        throw InvalidPositionException(profileNumber, TOTAL_PROFILES);
    }
}

Character* GameFileReader::createCharacter(string role, string name, string nationalityText,char letter, int position)
{
    if (role == "Victim")
    {
        return new Victim(name, nationalityText, letter, position);
    }
    if (role == "Cook")
    {
        return new Cook(name, nationalityText, letter, position);
    }
    if (role == "Guard")
    {
        return new Guard(name, nationalityText, letter, position);
    }
    if (role == "Servant")
    {
        return new Servant(name, nationalityText, letter, position);
    }
    if (role == "Child")
    {
        return new OwnerChild(name, nationalityText, letter, position);
    }
    if (role == "Partner 1" || role == "Partner 2")
    {
        return new Partner(name, role, nationalityText, letter, position);
    }
    if (role == "PA")
    {
        return new PA(name, nationalityText, letter, position);
    }
    throw FileCorruptedException(CHARACTERS_FILE, "unknown role \"" + role + "\".");
}

// Counts the data lines of clue.txt 
int GameFileReader::countProfiles(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    string line;
    getline(file, line);              // skip the heading line

    int profiles = 0;
    while (getline(file, line))
    {
        if (trimSpaces(line) != "")   // empty lines are not counted
        {
            profiles++;
        }
    }

    file.close();
    return profiles;
}

// Reads line number 
void GameFileReader::readClueProfile(int profileNumber, string& culpritRole,string& word1, string& word2, string fileName)
{
    checkProfileNumber(profileNumber);

    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    string line;
    getline(file, line);              // skip the heading line

    DynamicArray<string> parts(3);
    int lineNumber = 0;

    while (getline(file, line))
    {
        if (trimSpaces(line) == "")
        {
            continue;
        }
        lineNumber++;

        if (lineNumber == profileNumber)
        {
            if (splitLine(line, '|', parts) != 3)
            {
                file.close();
                throw FileCorruptedException(fileName, "line " + to_string(lineNumber) +" must have 3 columns.");
            }

            culpritRole = parts[0];
            word1 = toUpperCase(parts[1]);
            word2 = toUpperCase(parts[2]);
            file.close();
            return;
        }
    }

    file.close();
    throw FileCorruptedException(fileName, "profile " + to_string(profileNumber) + " is missing.");
}

void GameFileReader::loadLocations(Cottage& cottage, int profileNumber, string fileName)
{
    checkProfileNumber(profileNumber);

    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    const int COLUMNS = 9;            
    DynamicArray<string> parts(COLUMNS);
    string line;
    getline(file, line);             

    int locationNumber = 0;
    while (getline(file, line))
    {
        if (trimSpaces(line) == "")
        {
            continue;
        }

        if (locationNumber == Cottage::TOTAL_LOCATIONS)
        {
            file.close();
            throw FileCorruptedException(fileName, "it has more than 7 locations.");
        }

        if (splitLine(line, '|', parts) != COLUMNS)
        {
            file.close();
            throw FileCorruptedException(fileName, "a line must have 9 columns: " + line);
        }

        int position;
        string letterText = parts[profileNumber + 1];

        if (!textToNumber(parts[1], position) || letterText.length() != 1)
        {
            file.close();
            throw FileCorruptedException(fileName, "wrong position or letter in: " + line);
        }

        locationNumber++;
        cottage.setLocation(locationNumber, parts[0], toUpperCase(letterText)[0], position);
    }

    file.close();

    if (locationNumber != Cottage::TOTAL_LOCATIONS)
    {
        throw FileCorruptedException(fileName, "it must have exactly 7 locations.");
    }
}

int GameFileReader::loadCharacters(Character* people[], int maxPeople, int profileNumber, string fileName)
{
    checkProfileNumber(profileNumber);

    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    const int COLUMNS = 12;
    DynamicArray<string> parts(COLUMNS);
    string line;
    getline(file, line);              // skip the heading line

    int peopleCount = 0;

    try
    {
        while (getline(file, line))
        {
            if (trimSpaces(line) == "")
            {
                continue;
            }

            if (peopleCount == maxPeople)
            {
                throw FileCorruptedException(fileName, "it has too many people.");
            }

            if (splitLine(line, '|', parts) != COLUMNS)
            {
                throw FileCorruptedException(fileName, "a line must have 12 columns: " + line);
            }

            int position;
            string letterText = parts[profileNumber + 3];

            if (!textToNumber(parts[3], position) || letterText.length() != 1)
            {
                throw FileCorruptedException(fileName, "wrong position or letter in: " + line);
            }

            people[peopleCount] = createCharacter(parts[1], parts[0], parts[2],
                toUpperCase(letterText)[0], position);
            peopleCount++;
        }
    }
    catch (...)
    {
        // Something went wrong in the middle delete the people that were
        // already created (no memory leak), then pass the error on.
        for (int i = 0; i < peopleCount; i++)
        {
            delete people[i];
            people[i] = nullptr;
        }
        file.close();
        throw;      // re-throw the same exception to the caller
    }

    file.close();
    return peopleCount;
}

// Reads lines 
GameRules GameFileReader::readRules(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    GameRules rules;              // starts with the default values
    DynamicArray<string> parts(2);
    string line;

    while (getline(file, line))
    {
        if (trimSpaces(line) == "")
        {
            continue;
        }

        if (splitLine(line, '=', parts) != 2)
        {
            file.close();
            throw FileCorruptedException(fileName, "a rule must look like Key=Value: " + line);
        }

        string key = parts[0];
        string value = parts[1];
        int number = 0;

        if (key == "Win_Condition")
        {
            rules.winCondition = value;
        }
        else if (key == "Lose_Condition")
        {
            rules.loseCondition = value;
        }
        else if (key == "Max_Turns" || key == "Starting_Credibility" || key == "Every_Wrong_Turn" || key == "Every_Wrong_Word")
        {
            if (!textToNumber(value, number))
            {
                file.close();
                throw FileCorruptedException(fileName, "\"" + value + "\" is not a number.");
            }

            if (number < 0)
            {
                number = -number;  
            }

            if (key == "Max_Turns")
            {
                rules.maxTurns = number;
            }
            else if (key == "Starting_Credibility")
            {
                rules.startingCredibility = number;
            }
            else if (key == "Every_Wrong_Turn")
            {
                rules.wrongTurnPenalty = number;
            }
            else
            {
                rules.wrongWordPenalty = number;
            }
        }
        // any other key is simply ignored
    }

    file.close();

    if (rules.maxTurns <= 0 || rules.startingCredibility <= 0)
    {
        throw FileCorruptedException(fileName, "turns and credibility must be more than 0.");
    }
    return rules;
}

// Prints every line of a text file
void GameFileReader::showTextFile(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    file.close();
}

ScoreBoard::ScoreBoard(string scoreFileName)
{
    fileName = scoreFileName;
}

// Today date as dd-mm-yy
string ScoreBoard::getTodayDate() const
{
    time_t now = time(0);                 // seconds since 1970
    tm dateParts;
    localtime_s(&dateParts, &now);        // split into day, month, year
    int day = dateParts.tm_mday;
    int month = dateParts.tm_mon + 1;     
    int year = dateParts.tm_year % 100; 

    return twoDigits(day) + "-" + twoDigits(month) + "-" + twoDigits(year);
}

// true if the file exists and has at least one line
bool ScoreBoard::fileHasData() const
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        return false;
    }

    string firstLine;
    bool hasLine = (bool)getline(file, firstLine);
    file.close();
    return hasLine;
}

void ScoreBoard::addScore(string detectiveName, bool won, int credibilityLeft, int turnsLeft, string culpritRole)
{
    bool writeHeading = !fileHasData();

    ofstream file(fileName, ios::app);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    if (writeHeading)
    {
        file << "Detective|Result|Credibility|Turns_Left|Culprit|Date" << endl;
    }

    // "Partner 2" is saved as "Partner_2" (like the sample file)
    for (int i = 0; i < (int)culpritRole.length(); i++)
    {
        if (culpritRole[i] == ' ')
        {
            culpritRole[i] = '_';
        }
    }
    file << detectiveName << "|"<< (won ? "WIN" : "LOSE") << "|"<< credibilityLeft << "|"<< twoDigits(turnsLeft) << "|"<< culpritRole << "|"<< getTodayDate() << endl;
    file.close();
}

// Shows all the results as a table
void ScoreBoard::displayScores() const
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw FileNotFoundException(fileName);
    }

    cout << "================= LEADER / SCORE BOARD =================" << endl;
    cout << padRight("Detective", 16) << padRight("Result", 8) << padRight("Cred.", 7) << padRight("Turns", 7) << padRight("Culprit", 11) << "Date" << endl;
    cout << "--------------------------------------------------------" << endl;

    const int COLUMNS = 6;
    DynamicArray<string> parts(COLUMNS);
    string line;
    getline(file, line);              // skip the heading line

    int games = 0;
    while (getline(file, line))
    {
        if (trimSpaces(line) == "")
        {
            continue;
        }

        if (splitLine(line, '|', parts) != COLUMNS)
        {
            file.close();
            throw FileCorruptedException(fileName, "a line must have 6 columns: " + line);
        }

        cout << padRight(parts[0], 16) << padRight(parts[1], 8) << padRight(parts[2], 7)<< padRight(parts[3], 7) << padRight(parts[4], 11) << parts[5] << endl;
        games++;
    }

    if (games == 0)
    {
        cout << "No games have been played yet." << endl;
    }
    cout << "========================================================" << endl;
    file.close();
}

GameEngine* GameEngine::instance = nullptr;

// PRIVATE constructor ,  The member objects are built in the initializer
// list  The Detective gets a temporary name; the real name
// is asked when a new game starts.
GameEngine::GameEngine(): detective("Detective", 100),finalClueRack(16)                // 7 letters + 1 space + 8 letters
{
    for (int i = 0; i < MAX_PEOPLE; i++)
    {
        people[i] = nullptr;
    }
    peopleCount = 0;

    scoreBoard = nullptr;
    state = STATE_MENU;
    profileNumber = 0;
    culpritRole = "";
    targetWord1 = "";
    targetWord2 = "";
    word1Solved = false;
    word2Solved = false;

    srand((unsigned int)time(0));      // new random culprit in every run
}

// Private destructor, frees the 8 character objects
GameEngine::~GameEngine()
{
    freePeople();
}

// Gives the one and only engine, It is created on the first call only.
GameEngine* GameEngine::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GameEngine();
    }
    return instance;
}

// Deletes the engine
void GameEngine::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

// ASSOCIATION, the engine only remembers where the score board is
void GameEngine::setScoreBoard(ScoreBoard* board)
{
    scoreBoard = board;
}

// Clears the console window
void GameEngine::clearScreen() const
{
    system("cls");
}

void GameEngine::pressEnterToContinue() const
{
    cout << "\nPress Enter to continue...";
    string ignoreLine;
    getline(cin, ignoreLine);
}

// Reads a whole line and turns it into a number between minimum and maximum.
int GameEngine::readNumber(int minimum, int maximum) const
{
    string input;
    getline(cin, input);

    int number;
    if (!textToNumber(input, number))
    {
        throw InvalidInputException("please type a number.");
    }

    if (number < minimum || number > maximum)
    {
        throw InvalidPositionException(number, maximum);
    }

    return number;
}

// Asks for the detective's name until a valid one is typed
string GameEngine::readDetectiveName() const
{
    while (true)
    {
        cout << "Enter detective name (use _ or . instead of spaces): ";
        string name;
        getline(cin, name);
        name = trimSpaces(name);

        try
        {
            if (name == "")
            {
                throw InvalidInputException("the name cannot be empty.");
            }
            for (int i = 0; i < (int)name.length(); i++)
            {
                if (name[i] == ' ')
                {
                    throw InvalidInputException("no spaces allowed, write it like Agent_007 or Insp.Jamshad");
                }
            }
            return name;
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }
}


// Main menu 

// shown once when the program starts , game title and a short scenario
void GameEngine::showTitle() const
{
    clearScreen();
    cout << "======================================================" << endl;
    cout << "                  THE LOCKER HEIST" << endl;
    cout << "        A Detective Game backed by OOP Principles" << endl;
    cout << "======================================================" << endl;
    cout << "A locker hidden behind a bookshelf in the Study Room" << endl;
    cout << "was robbed during a party at a wealthy business" << endl;
    cout << "owner's cottage. Eight people were there, and one of" << endl;
    cout << "them is the CULPRIT. You are the private detective" << endl;
    cout << "hired to find out who did it." << endl;
    cout << "======================================================" << endl;
    pressEnterToContinue();
}

// The Main Menu is made from the menuchoice enum 
void GameEngine::showMainMenu() const
{
    cout << "======================================================" << endl;
    cout << "            THE LOCKER HEIST - MAIN MENU" << endl;
    cout << "======================================================" << endl;

    for (int option = MENU_NEW_GAME; option <= MENU_EXIT; option++)
    {
        cout << "      [" << option << "] " << menuChoiceToText((MenuChoice)option) << endl;
    }
    cout << "======================================================" << endl;
    cout << "Choice: ";
}

// Score Board uses the associated ScoreBoard object
void GameEngine::showLeaderBoard() const
{
    clearScreen();
    if (scoreBoard == nullptr)
    {
        cout << "The score board is not connected." << endl;
    }
    else
    {
        try
        {
            scoreBoard->displayScores();
        }
        catch (FileNotFoundException&)
        {
            cout << "No games have been played yet." << endl;
        }
    }
    pressEnterToContinue();
}

// read from about.txt
void GameEngine::showAbout() const
{
    clearScreen();
    try
    {
        GameFileReader::showTextFile(ABOUT_FILE);
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
    }
    pressEnterToContinue();
}

// numbers from rules.txt, plus how to play
void GameEngine::showRules()
{
    clearScreen();
    loadRules();

    cout << "===================== GAME RULES =====================" << endl;
    cout << "Maximum turns        : " << rules.maxTurns << endl;
    cout << "Starting credibility : " << rules.startingCredibility << endl;
    cout << "Every wrong step     : -" << rules.wrongTurnPenalty << " credibility" << endl;
    cout << "Every wrong word     : -" << rules.wrongWordPenalty << " credibility" << endl;
    cout << "WIN  : " << replaceUnderscores(rules.winCondition) << endl;
    cout << "LOSE : " << replaceUnderscores(rules.loseCondition) << endl;
    cout << "--------------------- HOW TO PLAY --------------------" << endl;
    cout << "1. Visit the 7 locations    -> letters of Word 1." << endl;
    cout << "2. Interview the 8 people   -> letters of Word 2." << endl;
    cout << "3. Every letter is placed at its position on the board." << endl;
    cout << "4. When a row is full, type its word (Form a Word)." << endl;
    cout << "5. The two words point to the culprit. Accuse him/her!" << endl;
    cout << "------------------ INTERVIEW RULES -------------------" << endl;
    cout << "Business Owner (Victim) : credibility >= " << Victim::REQUIRED_CREDIBILITY << endl;
    cout << "Owner's Child           : credibility >= " << CloseRelations::REQUIRED_CREDIBILITY << endl;
    cout << "Business Partners       : credibility >= " << BusinessRelations::REQUIRED_CREDIBILITY << endl;
    cout << "PA                      : credibility >= " << PA::REQUIRED_CREDIBILITY << endl;
    cout << "Cook, Guard, Servant    : any time" << endl;
    cout << "Viewing the Scrabble board does NOT use a turn." << endl;
    cout << "======================================================" << endl;
    pressEnterToContinue();
}

// Reads rules.txt. If the file is missing or corrupted, the default values of GameRules are used and the game still works.
void GameEngine::loadRules()
{
    try
    {
        rules = GameFileReader::readRules();
    }
    catch (GameException& error)
    {
        cout << error.getFullMessage() << endl;
        cout << "The default rules will be used." << endl;
        rules = GameRules();          // fresh object = default values
    }
}

// Picks a random line profile of clue.txt and reads its 2 words
void GameEngine::chooseRandomCulprit()
{
    int totalProfiles = GameFileReader::countProfiles();

    if (totalProfiles < 1)
    {
        throw FileCorruptedException(CLUE_FILE, "it has no culprit profiles.");
    }
    if (totalProfiles > GameFileReader::TOTAL_PROFILES)
    {
        totalProfiles = GameFileReader::TOTAL_PROFILES;
    }
    profileNumber = rand() % totalProfiles + 1;      // 1 to 7
    GameFileReader::readClueProfile(profileNumber, culpritRole, targetWord1, targetWord2);
}

// Sets isCulprit = true on the character whose role matches the culprit
void GameEngine::markCulprit()
{
    bool culpritFound = false;

    for (int i = 0; i < peopleCount; i++)
    {
        if (people[i]->characterRole == culpritRole)
        {
            people[i]->isCulprit = true;
            culpritFound = true;
        }
        else
        {
            people[i]->isCulprit = false;
        }
    }

    if (!culpritFound)
    {
        throw FileCorruptedException(CHARACTERS_FILE, "no person has the role \"" + culpritRole + "\".");
    }
}

// Deletes the character objects made by GameFileReader loadCharacters()
void GameEngine::freePeople()
{
    for (int i = 0; i < peopleCount; i++)
    {
        delete people[i];
        people[i] = nullptr;
    }
    peopleCount = 0;
}

// New Game
void GameEngine::startNewGame()
{
    clearScreen();
    cout << "======================================================" << endl;
    cout << "            THE LOCKER HEIST - NEW GAME" << endl;
    cout << "======================================================" << endl;
    string playerName = readDetectiveName();
    loadRules();
    chooseRandomCulprit();

    // A fresh detective, cottage and board for every new game
    detective = Detective(playerName, rules.startingCredibility);
    cottage = Cottage();
    scrabble = Scrabble();
    finalClueRack = TileRack(16);
    word1Solved = false;
    word2Solved = false;

    // Load the letters of the selected culprit from the files
    scrabble.setTargetWords(targetWord1, targetWord2);
    GameFileReader::loadLocations(cottage, profileNumber);

    freePeople();
    peopleCount = GameFileReader::loadCharacters(people, MAX_PEOPLE, profileNumber);
    markCulprit();

    state = STATE_PLAYING;

    cout << "\nA culprit has been secretly chosen. The game begins!" << endl;
    cout << "Max turns: " << rules.maxTurns << "  |  Starting credibility: " << rules.startingCredibility << endl;
    cout << "WIN : " << replaceUnderscores(rules.winCondition) << endl;
    cout << "LOSE: " << replaceUnderscores(rules.loseCondition) << endl;

    pressEnterToContinue();
    playGame();
    state = STATE_MENU;      // back to the main menu after the game
}

// Shows the detective's status, the racks and the board (like the sample output)
void GameEngine::showGameStatus() const
{
    cout << "Detective: " << detective.getName()<< "  |  Credibility: " << detective.getCredibility()<< "  |  Turns used: " << detective.getTurns() << " / " << rules.maxTurns << endl;
    detective.displayRacks();
    cout << scrabble;
}

// runs the whole program  title, then the main menu until Exit
void GameEngine::run()
{
    showTitle();
    state = STATE_MENU;

    while (state != STATE_EXIT)
    {
        clearScreen();
        showMainMenu();
        try
        {
            int choice = readNumber(MENU_NEW_GAME, MENU_EXIT);

            // The number typed by the user becomes a menuchoice enum value
            switch ((MenuChoice)choice)
            {
            case MENU_NEW_GAME:
                startNewGame();
                break;
            case MENU_LEADERBOARD:
                showLeaderBoard();
                break;
            case MENU_ABOUT:
                showAbout();
                break;
            case MENU_RULES:
                showRules();
                break;
            case MENU_EXIT:
                state = STATE_EXIT;
                break;
            }
        }
        catch (FileNotFoundException& error)
        {
            // A game file is missing, the game cannot start, back to the menu
            cout << error.getFullMessage() << endl;
            cout << "Please put all the .txt files in the project folder." << endl;
            state = STATE_MENU;
            pressEnterToContinue();
        }
        catch (GameException& error)
        {
            // wrong menu input or corrupted data, show it and ask again
            cout << error.getFullMessage() << endl;
            state = STATE_MENU;
            pressEnterToContinue();
        }
    }
    clearScreen();
    cout << "Thank you for playing THE LOCKER HEIST. Goodbye, Detective!" << endl;
}

// The Game Menu shown on every turn
void GameEngine::showGameMenu() const
{
    cout << "\n==== Game Menu ====" << endl;
    cout << "[1] Visit a Location" << endl;
    cout << "[2] Interview a Person" << endl;
    cout << "[3] View Scrabble Board  (free - no turn)" << endl;
    cout << "[4] Form a Word" << endl;
    cout << "[5] Accuse a Suspect" << endl;
    cout << "[6] Quit" << endl;
    cout << "Choice: ";
}

// Asks a Y/N question until the player types Y or N small letters too
bool GameEngine::askYesNo(string question) const
{
    while (true)
    {
        cout << question << " (Y/N): ";
        string answer;
        getline(cin, answer);
        answer = toUpperCase(trimSpaces(answer));

        if (answer == "Y" || answer == "YES")
        {
            return true;
        }
        if (answer == "N" || answer == "NO")
        {
            return false;
        }
        cout << "Please type Y or N." << endl;
    }
}

// A wrong step costs credibility value from rules.txt, normally 5
void GameEngine::giveWrongStepPenalty(string reason)
{
    detective.loseCredibility(rules.wrongTurnPenalty);
    cout << "Wrong step: " << reason << "  (-" << rules.wrongTurnPenalty << " credibility, now " << detective.getCredibility() << ")" << endl;
}

// The GameEngine counts the turns; the Detective only stores the number
void GameEngine::useTurn()
{
    detective.setTurns(detective.getTurns() + 1);
}

// Ends the game when the detective has no credibility or no turns left
void GameEngine::checkGameOver()
{
    if (state != STATE_PLAYING)
    {
        return;
    }
    if (detective.getCredibility() <= 0)
    {
        cout << "\nYour credibility is ZERO. Nobody trusts you anymore!" << endl;
        state = STATE_LOST;
    }
    else if (detective.getTurns() >= rules.maxTurns)
    {
        cout << "\nYou have used all " << rules.maxTurns << " turns!" << endl;
        state = STATE_LOST;
    }
}

// The loop of one game
void GameEngine::playGame()
{
    state = STATE_PLAYING;

    while (state == STATE_PLAYING)
    {
        clearScreen();
        cout << "--- TURN " << (detective.getTurns() + 1) << " ---" << endl;
        showGameStatus();
        showGameMenu();

        // read the choice 
        int choice;
        try
        {
            choice = readNumber(1, 6);
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
            giveWrongStepPenalty("invalid menu choice");
            checkGameOver();
            pressEnterToContinue();
            continue;
        }

        // do the action
        bool turnUsed = false;
        try
        {
            switch (choice)
            {
            case 1:
                turnUsed = visitLocation();
                break;
            case 2:
                turnUsed = interviewPerson();
                break;
            case 3:
                viewBoard();             
                break;
            case 4:
                turnUsed = formWord();
                break;
            case 5:
                turnUsed = accuseSuspect();
                break;
            case 6:
                if (askYesNo("Are you sure you want to quit this game?"))
                {
                    cout << "You left the investigation." << endl;
                    state = STATE_LOST;
                }
                break;
            }
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }

        // count the turn and check the end of the game
        if (turnUsed)
        {
            useTurn();
        }
        checkGameOver();
        pressEnterToContinue();
    }
    finishGame();
}

//  Visit a Location
bool GameEngine::visitLocation()
{
    while (true)
    {
        cout << endl;
        cottage.displayLocations();
        cout << "[0] Back to the Game Menu" << endl;
        cout << "Pick: ";

        int number;
        try
        {
            number = readNumber(0, cottage.getLocationCount());
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
            continue;                         // ask again
        }
        if (number == 0)
        {
            return false;                     // back, no turn used
        }
        string placeName = replaceUnderscores(cottage.getLocationName(number));

        if (!cottage.isSearched(number))
        {
            // first visit 
            cout << "You have NOT searched the " << placeName << " yet." << endl;
            if (askYesNo("Search?"))
            {
                Clue foundClue = cottage.searchLocation(number);   
                detective.collectClue(foundClue);               
                scrabble.placeLetter(foundClue);                   
                cout << "Letter " << foundClue.clueLetter << " collected at position " << foundClue.cluePosition << " of Word 1." << endl;
                cout << "Letter has been placed." << endl;
                return true;
            }
        }
        else
        {
            //visit again
            cout << "The " << placeName << " has already been searched." << endl;
            if (askYesNo("Do you want to search it again?"))
            {
                Clue oldClue = cottage.searchLocation(number);
                cout << "You found letter " << oldClue.clueLetter << " at position " << oldClue.cluePosition << " of Word 1 here before." << endl;
                return true;
            }
        }
        cout << "Going back to the locations..." << endl;
    }
}

// The list of the 8 people with their interview status
void GameEngine::showPeopleMenu() const
{
    cout << "\n====== Character Menu ======" << endl;
    for (int i = 0; i < peopleCount; i++)
    {
        string label = people[i]->getName() + " (" + people[i]->getRole() + ")";
        cout << "[" << (i + 1) << "] " << padRight(label, 26) << "- " << (people[i]->isInterviewed() ? "Interviewed" : "NOT Interviewed") << endl;
    }
    cout << "============================" << endl;
}

// 2 ,  Interview a Person
bool GameEngine::interviewPerson()
{
    while (true)
    {
        showPeopleMenu();
        cout << "[0] Back to the Game Menu" << endl;
        cout << "Pick: ";
        int number;
        try
        {
            number = readNumber(0, peopleCount);
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
            continue;
        }
        if (number == 0)
        {
            return false;
        }
        Character* person = people[number - 1];

        // Credibility condition
        if (!person->canBeInterviewed(detective.getCredibility()))
        {
            InterviewDeniedException denied(person->getName(), person->getRequiredCredibility());
            cout << denied.getFullMessage() << endl;
            giveWrongStepPenalty("interview refused");
            return false;
        }

        if (!person->isInterviewed())
        {
            // first interview 
            cout << "You have NOT interviewed " << person->getName() << " yet." << endl;
            if (askYesNo("Interview?"))
            {
                cout << endl << *person;                 
                Clue personClue = person->getClue();     
                detective.collectClue(personClue);      
                scrabble.placeLetter(personClue);        
                person->setInterviewed(true);
                cout << "\nLetter " << personClue.clueLetter << " collected at position " << personClue.cluePosition << " of Word 2." << endl;
                cout << "Letter has been placed." << endl;
                return true;
            }
        }
        else
        {
            //  interview again 
            cout << person->getName() << " has already been interviewed." << endl;
            if (askYesNo("Do you want to interview again?"))
            {
                Clue oldClue = person->getClue();
                cout << person->getName() << " repeats: letter " << oldClue.clueLetter << " at position " << oldClue.cluePosition << " of Word 2." << endl;
                return true;
            }
        }
        cout << "Going back to the characters..." << endl;
    }
}

// 3 View Scrabble Board 
void GameEngine::viewBoard() const
{
    cout << endl;
    detective.displayRacks();
    cout << scrabble;
    cout << "Word 1: " << scrabble.getFilledCount(WORD_1) << "/" << Scrabble::WORD1_LENGTH<< " letters   |   Word 2: " << scrabble.getFilledCount(WORD_2) << "/"<< Scrabble::WORD2_LENGTH << " letters" << endl;

    if (word1Solved && word2Solved)
    {
        showFinalClue();
    }
}

// Finds the person whose isCulprit flag is true
Character* GameEngine::findCulprit() const
{
    for (int i = 0; i < peopleCount; i++)
    {
        if (people[i]->getIsCulprit())
        {
            return people[i];
        }
    }
    return nullptr;
}

// 4 Form a Word
bool GameEngine::formWord()
{
    int pick;
    while (true)
    {
        cout << "\n====== Form a Word ======" << endl;
        cout << "[1] Word 1 (7 letters) - " << (word1Solved ? "SOLVED" : "not solved") << endl;
        cout << "[2] Word 2 (8 letters) - " << (word2Solved ? "SOLVED" : "not solved") << endl;
        cout << "[0] Back to the Game Menu" << endl;
        cout << "Pick: ";

        try
        {
            pick = readNumber(0, 2);
            break;
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }

    if (pick == 0)
    {
        return false;
    }

    WordRow row = (pick == 1) ? WORD_1 : WORD_2;
    bool alreadySolved = (row == WORD_1) ? word1Solved : word2Solved;

    if (alreadySolved)
    {
        cout << wordRowToText(row) << " is already solved: " << scrabble.getTargetWord(row) << endl;
        return false;
    }

    // The row must be full before a word can be typed exception otherwise
    if (!scrabble.isRowComplete(row))
    {
        throw WordIncompleteException(row);
    }

    scrabble.displayWord(row);
    cout << "Type the word: ";
    string typedWord;
    getline(cin, typedWord);
    typedWord = trimSpaces(typedWord);

    if (typedWord == "")
    {
        throw InvalidInputException("the word cannot be empty.");
    }

    if (scrabble.isWordCorrect(row, typedWord))
    {
        // correct word
        if (row == WORD_1)
        {
            word1Solved = true;
        }
        else
        {
            word2Solved = true;
        }
        detective.addSolvedWord(scrabble.getTargetWord(row));
        cout << "Correct! " << wordRowToText(row) << " is " << scrabble.getTargetWord(row) << "." << endl;

        // Both words solved
        if (word1Solved && word2Solved)
        {
            finalClueRack = detective.getRack(WORD_1) + detective.getRack(WORD_2);
            showFinalClue();
        }
    }
    else
    {
        //  wrong word -10 credibility 
        detective.loseCredibility(rules.wrongWordPenalty);
        InvalidWordException wrongWord(typedWord);
        cout << wrongWord.getFullMessage() << "  (-" << rules.wrongWordPenalty
            << " credibility, now " << detective.getCredibility() << ")" << endl;
        cout << "You can try again in your next turn." << endl;
    }
    return true;      // typing a word right or wrong uses a turn
}

// Shows the meaning of the two solved words.The culprit's role is the
// one read from clue.txt, and the nationality comes from the culprit object.
void GameEngine::showFinalClue() const
{
    Character* culprit = findCulprit();
    string nationalityText = (culprit != nullptr) ? culprit->getNationality() : "Unknown";

    cout << "\n****************************************************" << endl;
    cout << "Both words solved:" << endl;
    cout << "  " << targetWord1 << " + " << targetWord2 << " = " << nationalityText << "  ->  " << culpritRole << " is the Culprit!" << endl;
    cout << "  Final clue (Rack 1 + Rack 2): " << finalClueRack.getLettersInOrder() << endl;
    cout << "Now use [5] Accuse a Suspect to catch the culprit." << endl;
    cout << "****************************************************" << endl;
}

// 5 Accuse a Suspect
bool GameEngine::accuseSuspect()
{
    DynamicArray<int> suspectIndex(MAX_PEOPLE);    // list number 
    int suspectCount = 0;

    int pick;
    while (true)
    {
        cout << "\n====== Accuse a Suspect ======" << endl;
        suspectCount = 0;
        for (int i = 0; i < peopleCount; i++)
        {
            if (people[i]->canBeAccused())
            {
                suspectIndex[suspectCount] = i;
                suspectCount++;
                cout << "[" << suspectCount << "] " << people[i]->getName()
                    << " (" << people[i]->getRole() << ")" << endl;
            }
        }
        cout << "[0] Back to the Game Menu" << endl;
        cout << "Pick: ";

        try
        {
            pick = readNumber(0, suspectCount);
            break;
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }

    if (pick == 0)
    {
        return false;
    }

    Character* accused = people[suspectIndex[pick - 1]];

    if (!askYesNo("Accuse " + accused->getName() + " (" + accused->getRole() + ")?"))
    {
        cout << "Going back..." << endl;
        return false;
    }

    cout << "Accuse: " << accused->getName() << " (" << accused->getRole() << ")" << endl;

    if (accused->getIsCulprit())
    {
        cout << accused->getName() << " is the culprit!" << endl;
        state = STATE_WON;
    }
    else
    {
        cout << accused->getName() << " is NOT the culprit." << endl;
        giveWrongStepPenalty("wrong accusation");
    }

    return true;      // an accusation uses a turn
}

// WIN / LOSE screen
void GameEngine::finishGame()
{
    clearScreen();

    bool won = (state == STATE_WON);
    int turnsLeft = rules.maxTurns - detective.getTurns();
    if (turnsLeft < 0)
    {
        turnsLeft = 0;
    }
    Character* culprit = findCulprit();
    string culpritText = culpritRole;
    if (culprit != nullptr)
    {
        culpritText = culpritRole + " " + culprit->getName() + " (" + culprit->getNationality() + ")";
    }

    cout << "==============================================================" << endl;
    if (won)
    {
        cout << "                   " << detective.getName() << " WIN!" << endl;
        cout << "        You caught the culprit. Case closed, Detective!" << endl;
    }
    else
    {
        cout << "                   " << detective.getName() << " LOSE!" << endl;
        cout << "              The culprit got away this time..." << endl;
    }
    cout << "==============================================================" << endl;
    cout << " Final credibility : " << detective.getCredibility() << endl;
    cout << " Turns used        : " << detective.getTurns() << " / " << rules.maxTurns << "   (turns left: " << turnsLeft << ")" << endl;
    cout << " Culprit           : " << culpritText << endl;
    cout << " Words             : " << targetWord1 << " + " << targetWord2 << endl;
    cout << "==============================================================" << endl;

    // Save the result 
    if (scoreBoard != nullptr)
    {
        try
        {
            scoreBoard->addScore(detective.getName(), won, detective.getCredibility(), turnsLeft, culpritRole);
            cout << "Your result has been saved in the Leader/Score Board." << endl;
        }
        catch (GameException& error)
        {
            cout << error.getFullMessage() << endl;
        }
    }
    state = STATE_MENU;
    pressEnterToContinue();      // after this run() clears the screen and shows the Main Menu
}
