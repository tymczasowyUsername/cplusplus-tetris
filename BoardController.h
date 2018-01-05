#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "BoardView.h"

class BoardController   :   private juce::KeyListener
{
public:
    BoardController (Board& model, BoardView& view);
    ~BoardController();

private:
    //juce::KeyListener
    bool keyPressed (const juce::KeyPress& key, juce::Component* originatingComponent) override;
    
    Board& board;
    BoardView& boardView;
    int64 previousTime;
};
