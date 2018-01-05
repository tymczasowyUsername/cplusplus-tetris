#include "BoardView.h"



BoardView::BoardView (const String& name, Board& boardToUse)
	: juce::Component (name)
	, board (boardToUse)
{

    board.addListener (this);   //knows when to call paint()
}


BoardView::~BoardView()
{
    board.removeListener (this);
}

// Fuction is called whenever something needs to be redrawn (like item move)
void BoardView::paint (juce::Graphics& g)
{
	g.fillAll (juce::Colours::brown);

    int w = getWidth();
    int h = getHeight();

    Font font (float (std::min (w, h) / 25));
    g.setFont (font);
    g.setColour (Colours::white);
    g.drawText ("Level " + String (board.level), Rectangle<int> (w * 2/3, h / 60, w * 1/3, 100), Justification::centred, true);
    g.drawText ("Score " + String (board.score), Rectangle<int> (w * 2/3, h / 10, w * 1/3, 100), Justification::centred, true);

    Font font2 (float (std::min (w, h) / 40));
    g.setFont (font2);
    g.drawMultiLineText ("space                 - boost\n"
                         "left, right arrow   - move\n"
                         "up, down arrow   - rotate\n"
                         "esc                     - restart game\n"
                         "F11                    - pause game ", w * 65/100, h * 2/3, 300);


	int colWidth = (w - 200) / 10;
	int rowWidth = h / 15;
    
	// br¹zowy pasek
	g.setColour (juce::Colours::black);


    // Follow every cell in matrix and colours it 
	for (int row = 0; row < 15; ++row)
	{
		for (int col = 0; col < 10; ++col)
		{
			juce::Rectangle<int> rect (col * colWidth, row * rowWidth
                                       , colWidth, rowWidth);

            int cellValue = board.getCellValue (row, col, true);

			if (cellValue == 0)
            {
				g.setColour (juce::Colours::black);
            }
			else
            {
                switch (cellValue)
                {
                    case 1:
                        g.setColour (juce::Colours::white);
                        break;

                    case 2:
                        g.setColour (juce::Colours::red);
                        break;

                    case 3:
                        g.setColour (juce::Colours::yellow);
                        break;

                    case 4:
                        g.setColour (juce::Colours::green);
                        break;

                    case 5:
                        g.setColour (juce::Colours::violet);
                        break;

                    case 6:
                        g.setColour (juce::Colours::blue);
                        break;

                    case 7:
                        g.setColour (juce::Colours::pink);
                        break;
                }
            }
			g.fillRect (rect.reduced (1));

        }
	}
}

void BoardView::scoreChanged()
{
    repaint (getWidth() * 2/3 , 0, getWidth(), getHeight());
}

void BoardView::boardChanged()
{
    repaint (0, 0, getWidth(), getHeight());
}
