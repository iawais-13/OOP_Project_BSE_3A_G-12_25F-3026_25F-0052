# THE LOCKER HEIST
### A Detective Game backed by OOP Principles

A console-based, menu-driven detective game written in C++ for the course
**CL1004 - Object Oriented Programming** (Fall 2026).

The player is a private detective. A locker was robbed during a party at a
business owner's cottage. The detective visits 7 locations and interviews
8 people. Every location and person gives one letter and its position on a
Scrabble-style board. When both words are complete and typed correctly,
they point to the culprit. The detective must accuse the right suspect
before credibility or turns run out.

---

## Team Members

| Member | GitHub Account | Roll Number | Commits (Modules) |
|--------|----------------|-------------|-------------------|
| Member 1 - [Full Name] | iawais-13 | 25F-3026 | Module 1, Module 7, Module 3 (Part A and B), Module 4 (Cottage), Module 6, Module 10 |
| Member 2 - Ammar Haider | ammarhaider3538-design | 25F-0052 | Module 4 (Scrabble), Module 5, Module 8, Module 11, Module 2 + 12, Module 13, Module 14 |

Program: BSE - Section: 3A - Group: 12 - Lab Instructor: [Instructor Name]

---

## Project Files

| File | Purpose |
|------|---------|
| `heist.h` | Header file: all enums, structs and class declarations (and the `DynamicArray<T>` template) |
| `heist.cpp` | Implementation file: bodies of all functions |
| `main.cpp` | Driver file: starts the Singleton `GameEngine` |
| `clue.txt` | 7 culprit profiles: role, Word 1, Word 2 |
| `locations.txt` | 7 locations: name, Word 1 position, letter for every culprit |
| `characters.txt` | 8 people: name, role, nationality, Word 2 position, letter for every culprit |
| `rules.txt` | Max turns, starting credibility, penalties, win/lose conditions |
| `scoreBoard.txt` | Leader/Score Board (results are appended after every game) |
| `about.txt` | Development team and game details |
| `ClassDiagram.png` | UML class diagram of the whole game (image) |
| `ClassDiagram.drawio` | The same class diagram as a Draw.io file (can be edited) |
| `ClassDiagram.mmd` | The same class diagram as Mermaid code |

The code and the `.txt` files are inside the `OOP Game-The Locker Heist` folder.
The class diagram files and this README are in the main folder of the repository.

---

## Build and Run Instructions (Visual Studio)

1. Open Visual Studio and create an **Empty Project (C++)** named `TheLockerHeist`
   (or clone this repository and open the `.sln` file).
2. Add `heist.h` to **Header Files**, and `heist.cpp` and `main.cpp` to **Source Files**.
3. Copy all six `.txt` files into the **project folder** (the folder that contains
   the `.vcxproj` file and `heist.cpp`). The game reads the files from there.
4. Press **Ctrl + F5** (Start Without Debugging).
5. If you get error `LNK1168`, close the old game window and run again.

No extra libraries are needed. Only `<iostream>`, `<string>`, `<fstream>`,
`<cstdlib>` and `<ctime>` are used. No STL containers or algorithms are used.

---

## How to Play

1. Main Menu -> **[1] New Game** -> type your name (use `_` or `.` instead of spaces, e.g. `Agent_007`).
2. **[1] Visit a Location** -> letters of Word 1 (7 letters).
3. **[2] Interview a Person** -> letters of Word 2 (8 letters).
   - Business Owner: credibility >= 80, Owner's Child: >= 75,
     Partners: >= 50, PA: >= 35, Cook / Guard / Servant: any time.
4. **[3] View Scrabble Board** is free (no turn).
5. **[4] Form a Word** when a row is full. Wrong word = -10 credibility.
6. When both words are solved, the culprit's role is shown.
7. **[5] Accuse a Suspect**. Right person = **WIN**. Wrong person = -5 credibility.
8. **LOSE** if credibility becomes 0, all 25 turns are used, or you quit.
9. The result is saved in the Leader/Score Board (**[2]** in the Main Menu).

---

## OOP Concepts and Where They Are Used

| Concept | Where |
|---------|-------|
| 3-file structure | `heist.h`, `heist.cpp`, `main.cpp` |
| Struct, Enum | `Tile`, `Clue`, `Location`, `GameRules`; `WordRow`, `GameState`, `MenuChoice` |
| Encapsulation, getters/setters | All classes keep their data private/protected |
| Inheritance (single, hierarchical, multi-level, multiple, virtual) | `Character` hierarchy, `GameException` hierarchy |
| Diamond problem | `PA` inherits `BusinessRelations` and `CloseRelations` (virtual inheritance of `Suspect`) |
| Abstract class, pure virtual | `Character`, `Suspect` (`display() = 0`) |
| Runtime polymorphism | `display()`, `getRequiredCredibility()`, `getClue()`, `canBeAccused()`, `getErrorType()` |
| DMA, jagged array | `Cottage`, `Scrabble` (2 rows of 7 and 8), `TileRack` |
| Deep copy | Copy constructors and `operator=` of `Cottage`, `Scrabble`, `TileRack`, `DynamicArray` |
| Virtual destructor | `Character`, `GameException` |
| Template | `DynamicArray<T>` (old raw DMA code is kept as comments) |
| File handling | `GameFileReader`, `ScoreBoard` |
| Custom exceptions | `GameException` + 12 derived classes |
| Operator overloading | `Tile ==, !=`, `TileRack +`, `<<` and `>>` |
| Friend function / friend class | `<<`, `>>` friends; `GameEngine` is a friend class of `Character` |
| Composition / Aggregation / Association | `GameEngine` <>- parts; `Cottage` <>- `Location`; `GameEngine` -> `ScoreBoard` |
| Singleton | `GameEngine::getInstance()` |

---

## Design Decisions (Deviations from the Manual)

As required by Section 5.2, every change from the manual is listed here with a short reason.

| # | Decision | Reason |
|---|----------|--------|
| 1 | Enum values have prefixes (`STATE_EXIT`, `MENU_EXIT`, ...) | `GameState` and `MenuChoice` both have "Exit"; two normal enums cannot use the same name in C++. |
| 2 | The clue attributes of the suspects are `protected` in `Suspect` (private in `Victim`) | Follows the sample class diagram (Figure 1) of the manual and avoids writing the same code in 7 classes. |
| 3 | `characters.txt` has 8 data lines (not 7) | The sample file includes the Victim, who gives letter 1 of Word 2. |
| 4 | The score file is named `scoreBoard.txt` | A `/` cannot be used in a Windows file name ("leader/scoreBoard.txt" is the label of the board). |
| 5 | `Turns_Left` is saved in the score file (not turns used) | The sample file of Appendix B uses the `Turns_Left` column. |
| 6 | Word 2 of the Cook is `SPAGHETI` (one T) | Kept as in the manual, because Word 2 must have exactly 8 letters. |
| 7 | A wrong accusation costs -5 credibility and the game continues | The lose conditions in `rules.txt` are only "credibility zero or turns exhausted". |
| 8 | A wrong menu choice and a refused interview cost -5 credibility, but no turn | They are wrong steps (-5), but no real action was done. |
| 9 | Answering N to "Search? / Interview?" or going back uses no turn | No action was done. Searching or interviewing again (Y) uses a turn. |
| 10 | Typing a word uses a turn (right or wrong) | It is an action of the detective. |
| 11 | Quitting a game saves it as LOSE | The score file only has WIN or LOSE. |
| 12 | Credibility never goes below 0 | Clean display; 0 already means LOSE. |
| 13 | Typed words are accepted in small letters too | User friendly; the word is changed to capital letters before comparing. |
| 14 | A new random culprit is chosen for every New Game | Every game is different. |
| 15 | The Detective's nationality is always "Malay" | The manual says it "can be set"; only the name is asked to keep input short. |
| 16 | The Y/N prompt for searched locations is in `GameEngine` (not inside `Cottage`) | Single responsibility: `Cottage` manages data, `GameEngine` talks to the user. |
| 17 | The `DynamicArray<T>` template is fully written in `heist.h` | C++ needs the full template code where it is used; otherwise there are linker errors. |
| 18 | The Partner 1 name is "Ms.Ruslana" | Taken from the sample `characters.txt` (Appendix A shows "Ms.Rulana"). |

---

## Known Limitations

1. The game is console-based only (no graphics or mouse input).
2. It is single player; only one detective can play at a time.
3. Screen clearing uses `system("cls")`, so it works on Windows only.
4. The `.txt` files must be in the project folder; the game cannot find them anywhere else.
5. The words are always the same for the same culprit (7 fixed profiles from `clue.txt`).
6. The Leader/Score Board is not sorted; results are shown in the order they were played.
