#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Board.h"

class BoardView : public juce::Component
                , private Board::Listener
{
public:
	BoardView (const String& name, Board& boardToUse);
	~BoardView();

    // juce::Component override
	void paint (juce::Graphics& g) override;

private:

    // Board::Listener
    void boardChanged() override;
    void scoreChanged() override;

	Board& board;
};

