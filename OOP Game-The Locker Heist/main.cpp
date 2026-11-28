#include "heist.h"

int main()
{
    // The leader board belongs to main() The GameEngine only
    // uses it through a pointer  association
    ScoreBoard leaderBoard;

    try
    {
        GameEngine* engine = GameEngine::getInstance();

        engine->setScoreBoard(&leaderBoard);
        engine->run();
    }
    catch (GameException& error)
    {
        // any game error that was not handled inside the game ends here
        cout << "\nThe game had to stop: " << error.getFullMessage() << endl;
    }
    catch (...)
    {
        // last safety net for any unknown error
        cout << "\nAn unknown error happened. The game had to stop." << endl;
    }

    // free the engine's memory before the program ends
    GameEngine::destroyInstance();

    return 0;
}
