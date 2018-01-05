#include "BoardController.h"

//Board Controller class listens to input from keyboard.
BoardController::BoardController (Board& model, BoardView& view)
    : board (model)
    , boardView (view)
    , previousTime (Time::currentTimeMillis())
{
    boardView.addKeyListener (this);
    boardView.setWantsKeyboardFocus (true);
}
BoardController::~BoardController()
{
    boardView.removeKeyListener (this);
}

// Calls function assigned to a key value. Frequency of calling is limited to 100 ms.
bool BoardController::keyPressed (const juce::KeyPress& key
                                  , juce::Component* originatingComponent)

{
    if (originatingComponent == &boardView)
    {
        int keyCode = key.getKeyCode();

        if (keyCode == juce::KeyPress::leftKey)
        {   
             int64 timePassed = Time::currentTimeMillis() - previousTime;
             if (timePassed > 100)
             {
                board.moveBlock (Board::Direction::left);
                previousTime = Time::currentTimeMillis();
             }
             
             return true;
        }

        if (keyCode == juce::KeyPress::rightKey)
        {
             int64 timePassed = Time::currentTimeMillis() - previousTime;
             if (timePassed > 100)
             {
                 board.moveBlock (Board::Direction::right);
                 previousTime = Time::currentTimeMillis();
             }
             return true;
        }

        if (keyCode == juce::KeyPress::downKey)
        {
             int64 timePassed = Time::currentTimeMillis() - previousTime;
             if (timePassed > 100)
             {
                 board.moveBlock (Board::Direction::down);
                 previousTime = Time::currentTimeMillis();
             }
             return true;
        }

        if (keyCode == juce::KeyPress::upKey)
        {
             int64 timePassed = Time::currentTimeMillis() - previousTime;
             if (timePassed > 100)
             {
                 board.moveBlock (Board::Direction::up);
                 previousTime = Time::currentTimeMillis();
             }
             return true;
        }

        if (keyCode == juce::KeyPress::spaceKey)
        {
             int64 timePassed = Time::currentTimeMillis() - previousTime;
             if (timePassed > 100)
             {
                 board.moveBlock (Board::Direction::space);
                 previousTime = Time::currentTimeMillis();
             }
             return true;
        }

        if (keyCode == juce::KeyPress::escapeKey)
        {
             board.restart();
             return true;
        }

        if (keyCode == juce::KeyPress::F11Key)
        {
             board.startStop();
             return true;
        }        
    }

    return false;
}
