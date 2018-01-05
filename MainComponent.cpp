/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

// Main component is window's content. All visible elements are part of class Component.
//==============================================================================
MainContentComponent::MainContentComponent()
	: juce::Component()
	, board()
	, boardView ("View", board)
    , boardController (board, boardView)
{

    addAndMakeVisible (boardView);
    setSize (600, 600);
}

MainContentComponent::~MainContentComponent()
{
}

// By default every Componemnt has virtual functions paint and resized.
// Paint determines how Component objects are rendered on the screen.
void MainContentComponent::paint (Graphics& g)
{

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

	Font font;

    g.setFont (font);
    g.setColour (Colours::white);
    g.drawText ("Tetris", Rectangle<int> (0, 0, getWidth(), 10), Justification::centred, true);

    boardView.grabKeyboardFocus();

}

// Resized is called automatically whenever anything happens that lead to change component size.
void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	
	auto boundsReduced = getLocalBounds().reduced (50, 50);
	int dim = std::min (boundsReduced.getWidth(), boundsReduced.getHeight());

	boardView.setBounds (boundsReduced.getX(), boundsReduced.getY(), dim, dim);

}
