#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class Cell
{
public:
	Cell();

	void setValue (int valueToUse);
	int getValue () const;

private:
	int value;
};

//==============================================================================
class Board     : private juce::Timer
{
public:
	Board();
	~Board();

	int getCellValue (int row, int col, bool includeMovingBlock) const;
	void setCellValue (int row, int col, int value);

    juce::Array<int> getRowValues (int row) const;
    void setRowValues (int row, juce::Array<int>);

    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void boardChanged() = 0;
        virtual void scoreChanged() = 0;
    };
    
    void startStop();
    void restart();

    void addListener (Listener* l);
    void removeListener (Listener* l);

    enum class Direction
    {
        left,
        right,
        down,
        up,
        space
    };

    void moveBlock (Direction d);

    int score;
    int level;

private:
    //==============================================================================
    class Block
    {
    public:
        Block (juce::Array<juce::Point<int>>);
    
        void moveDown();
        void moveLeft();
        void moveRight();
        juce::Array<juce::Point<int>> getRotateXYs (Direction dir); 
        void rotate (juce::Array<juce::Point<int>> dist);

        juce::Array<juce::Point<int>> elements;
        int value;
    };
    bool isMovingBlockIn (int row, int column) const;

	void update(bool shallmoveDown=true);
    bool canBlockMoveDown() const;
    bool canBlockMoveLeft() const;
    bool canBlockMoveRight() const;
    bool canRotate (juce::Array<juce::Point<int>> dist) const; 

    void saveBlockToMatrix();
    int generateBlockNum ();
    bool generateNewBlock();
    juce::Array<juce::Point<int>> generateBlockPoints (int blockNum);

	int getRowLength() const;
    int getColumnLength() const;

	int indexCalc (int row, int col) const;
    int getMatrixLength() const;

    bool isRowFull (int row) const;
    juce::Array<int> getFullLines() const;


    void updateModel (juce::Array<int> fullLines);
    void updateScore (int multiplier);
    void increaseLevel();
    void speedUpTime();

    //juce::Timer
    void timerCallback() override;

	Cell matrix[150];

    Block movingBlock;

    int timerInterval;

    bool canBlockRotate;

    juce::ListenerList<Listener> listeners;
};

