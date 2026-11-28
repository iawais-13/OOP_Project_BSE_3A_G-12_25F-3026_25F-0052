// =====================================================================
//  File      : main.cpp
//  Project   : THE LOCKER HEIST - A Detective Game backed by OOP Principles
//  Purpose   : This is the DRIVER file. It only starts the GameEngine.
//              All the game logic is inside the classes (heist.h / heist.cpp).
// =====================================================================

#include "heist.h"

int main()
{
    // The leader/score board belongs to main(). The GameEngine only
    // USES it through a pointer (ASSOCIATION).
    ScoreBoard leaderBoard;

    try
    {
        // SINGLETON: we cannot write "GameEngine engine;" because the
        // constructor is private. getInstance() is the only way.
        GameEngine* engine = GameEngine::getInstance();

        engine->setScoreBoard(&leaderBoard);
        engine->run();
    }
    catch (GameException& error)
    {
        // Any game error that was not handled inside the game ends here
        cout << "\nThe game had to stop: " << error.getFullMessage() << endl;
    }
    catch (...)
    {
        // Last safety net for any unknown error
        cout << "\nAn unknown error happened. The game had to stop." << endl;
    }

    // Free the engine's memory before the program ends
    GameEngine::destroyInstance();

    return 0;
}
