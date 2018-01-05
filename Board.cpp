#include "Board.h"

// Cell is element of matrix that has a value depended on block type. Matrix
// controls what is happening on board by storing 
Cell::Cell()
	: value(0)
{}

void Cell::setValue (int valueToUse)
{
	value = valueToUse;
}

int Cell::getValue() const
{
	return value;
}
//==============================================================================

// Creates Block and assign point values to it form generatedPoints array, sets value to 1 (O-shape Block)
Board::Block::Block (juce::Array<juce::Point<int>> points)
    : value(1) 
{
    elements.add (points.getReference (0));
    elements.add (points.getReference (1));
    elements.add (points.getReference (2));
    elements.add (points.getReference (3));
}

void Board::moveBlock (Direction d)
{
    switch (d)
    {
        case Direction::left:
            if (canBlockMoveLeft())
            {
                movingBlock.moveLeft();
                update(false);
            }
            break;

        case Direction::right:
            if (canBlockMoveRight())
            {
                movingBlock.moveRight();
                update (false);
            }
            break;

        case Direction::space:
            if (isTimerRunning())
            {
                startTimer (timerInterval);
                update();
            }
            break;
        
        case Direction::down:
        case Direction::up:
        {
            auto xys = movingBlock.getRotateXYs (d);
            if (canRotate (xys))
            {
                movingBlock.rotate (xys);
                update (false);
            }
            break;
        }
        default:
            jassertfalse;
    }
}

void Board::Block::moveDown()
{
    for (int i = 0; i < 4; ++i)
        elements.getReference (i).addXY (0, 1);
}

void Board::Block::moveLeft()
{
    for (int i = 0; i < 4; ++i)
        elements.getReference (i).addXY (-1, 0);
}

void Board::Block::moveRight()
{
    for (int i = 0; i < 4; ++i)
        elements.getReference (i).addXY (1, 0);
}

//Before any block is rotated this method returns array containing post-rotation X and Y values. 
juce::Array<juce::Point<int>> Board::Block::getRotateXYs (Direction dir) 
{
    juce::Array<juce::Point<int>> distanceFromCentreOfRotation;
    auto& dist = distanceFromCentreOfRotation;
    auto& elem = elements;
    juce::Array<juce::Point<int>> test;

    for (int i = 0; i < 4; ++i)
    {
        dist.add ( { elem.getReference (i).getX() - elem.getReference (1).getX()
                   , elem.getReference (i).getY() - elem.getReference (1).getY()  } );
    };

    if (dir == Direction::up)
    {
        for (int i = 0; i < 4; ++i)
            test.add ( { elem.getReference (1).getX() - dist.getReference (i).getY()
                       , elem.getReference (1).getY() + dist.getReference (i).getX() } );
    }

    else if (dir == Direction::down) 
    {
        for (int i = 0; i < 4; ++i)
            test.add ( { elem.getReference (1).getX() + dist.getReference (i).getY()
                       , elem.getReference (1).getY() - dist.getReference (i).getX() } );
    }


    return test;
}

void Board::Block::rotate (juce::Array<juce::Point<int>> dist)
{
    auto& elem = elements;

    for (int i = 0; i < 4; ++i)
        elem.getReference (i).setXY (dist[i].getX(), dist[i].getY());
}
// Constructor launches timer, sets Block with generated block points and sets score and level.
Board::Board()
    : movingBlock (generateBlockPoints (1))
    , timerInterval (1000)
    , score (0)
    , level (1)
{

	for (int i = 0; i < getMatrixLength(); ++i)
		matrix[i] = Cell();

    startTimer (timerInterval);
}

Board::~Board()
{
}

// Starts and stops Timer.
void Board::startStop ()
{
    if (isTimerRunning())
        stopTimer();
    else
        startTimer (timerInterval);
}

// Resets initial values and matrix cell to 0.
void Board::restart()
{
    score = 0;
    level = 1;
    timerInterval = 1000;

    for (int i = 0; i < getMatrixLength(); ++i)
	    matrix[i].setValue (0);

    generateNewBlock();

    for (int i = 0; i < 4; ++i)
        movingBlock.elements.getReference (i).addXY (0, -1);

    startTimer (timerInterval);
}

// calculates position in matrix with given row and col
int Board::indexCalc (int row, int col) const
{	
	int index = row * getRowLength() + col;
	jassert (index >= 0 && index <= getMatrixLength() - 1);
     
	index = std::min (index, getMatrixLength() - 1);
	index = std::max (index, 0);

	return	index;
}

int Board::getCellValue (int row, int col, bool includeMovinBlock) const
{	
    if (includeMovinBlock)                  // Blocks are saved to matrix when down movement is not possible,
        if (isMovingBlockIn (row, col))     // paint()loops through matrix and colour cells, 
            return movingBlock.value;       // includeMovinBlock tells which cells are occupied by block and need colour.
        
	const Cell& cell = matrix[(indexCalc (row, col))];

	return cell.getValue();
}

bool Board::isMovingBlockIn (int row, int col) const
{
    for (int i = 0; i < 4; ++i)
    {
        if (movingBlock.elements.getReference (i).getX() == col && movingBlock.elements.getReference (i).getY() == row)
            return true;
    }
    return false;
}

void Board::setCellValue (int row, int col, int value)
{
	Cell& cell = matrix[indexCalc (row, col)];
	cell.setValue (value);
}

juce::Array<int> Board::getRowValues (int row) const
{
    juce::Array<int> row_values;

    for (int a = 0; a < getRowLength(); ++a)
    {   
        const Cell& cell = matrix[indexCalc (row, a)];       
        row_values.add (cell.getValue());
    }
    return row_values;
}

// Needed for matrix update
void Board::setRowValues (int row, juce::Array<int> values)
{
    for (int a = 0; a < getRowLength(); ++a)
    {   
        Cell& cell = matrix[indexCalc (row, a)];       
        cell.setValue (values.getReference(a));
    } 
}

int Board::getRowLength() const
{
	return 10;
}

int Board::getColumnLength() const
{
	return 15;
}

int Board::getMatrixLength() const
{
	return sizeof matrix / sizeof matrix[0];
}

void Board::timerCallback()
{
    update();
}

// Called every interval with argument that prevents block  to 'jump'  when space key pressed
void Board::update (bool shallMoveDown)
{
    if (canBlockMoveDown())
    {   
        if (shallMoveDown)  
            movingBlock.moveDown();
    }
    else
    {
        saveBlockToMatrix();

        juce::Array<int> numOfFullLines = getFullLines();

        if (! numOfFullLines.isEmpty())
        {
            updateModel (numOfFullLines);
            updateScore (numOfFullLines.size());   
            listeners.call (&Listener::scoreChanged); //  observe any score changes and calls repaint()
        }

        if (! generateNewBlock())
        {
           //        listeners.call (&Listener::boardChanged);
            startStop();                        // if there's no place  for new block stops timer
        }
    }

    listeners.call (&Listener::boardChanged);   // observes any board changes and calls repaint()
}

bool Board::canBlockMoveLeft () const
{
    for (int i = 0; i < 4; ++i)
    {
        int newColumn = std::max (0, movingBlock.elements.getReference (i).getX() - 1);

        int value = getCellValue (movingBlock.elements.getReference (i).getY()
                                  , newColumn, newColumn == 0);
        if (value != 0)
            return false;
    }
    return true;
}

bool Board::canBlockMoveRight () const
{
    int numColumns = getMatrixLength() / getColumnLength();

    for (int i = 0; i < 4; ++i)
    {
        int newColumn = std::min (numColumns - 1, movingBlock.elements.getReference (i).getX() + 1);

        int value = getCellValue (movingBlock.elements.getReference (i).getY()
                                  , newColumn, newColumn == numColumns - 1);
        if (value != 0)
            return false;
    }
    return true;
}

bool Board::canBlockMoveDown () const
{
    int numRows = getMatrixLength() / getRowLength();

    for (int i = 0; i < 4; ++i)
    {
        
        int newRow = std::min (numRows - 1, movingBlock.elements.getReference (i).getY() + 1);
        int value = getCellValue (newRow, 
                                  movingBlock.elements.getReference (i).getX(), newRow == numRows - 1);

        if (value != 0)
            return false;
    }
    return true;
}

// Checks if rotation is possible for array contating blocks post-rotation XYs.
bool Board::canRotate (juce::Array<juce::Point<int>> test) const
{   
    if (! canBlockRotate)
        return false;

    for (int i = 0; i < test.size(); ++i)
    {
        DBG ("test [i]Y = " + juce::String(test[i].getY()) + " test [i]X= " + juce::String(test[i].getX()) );

        if ((test[i].getX() >= getRowLength() || test[i].getY() >= getColumnLength()) ||
            (test[i].getX() < 0               || test[i].getY() < 0))
            return false;

        if (getCellValue (test[i].getY(), test[i].getX(), false) != 0)
            return false;
    }
    
    return true;
}

bool Board::isRowFull (int row) const
{
    for (int column = 0; column < getRowLength(); ++column)
    {
        if (getCellValue (row, column, false) == 0)
            return false;
    }

    return true;
}

// Saves full rows.
juce::Array<int> Board::getFullLines() const
{
    juce::Array<int> fullLines;
    for (int row = (getColumnLength() - 1); row > 0; --row) // len array = 4
    {
        if (isRowFull (row))
            fullLines.add (row);
    }
    return fullLines;
}

// When full row is build updates matrix. 
void Board::updateModel(juce::Array<int> fullLines)
{
    int temp = fullLines[0];
    juce::Array<int> copyRow;
    juce::Array<int> zeros;
    zeros.add(0,0,0,0,0,0,0,0,0,0);

    for (int i = fullLines[0] - 1; i > 0; --i)
    {
        if (fullLines.contains(i))
            continue;

        if (getRowValues(i) != zeros)
        {
            copyRow = getRowValues(i);
        }
        else
        {
            for (int j = i + fullLines.size(); j > i; --j)
                setRowValues (j, zeros);
            return;
        }        
            
        setRowValues (temp, copyRow);
        temp -= 1;
    }
}

void Board::updateScore (int multiplier)
{
    score += multiplier * multiplier * 50;

    if (score > level * 500)
    {    
        increaseLevel();
        speedUpTime();
    }
}

void Board::increaseLevel()
{
    level += 1;
}

void Board::speedUpTime()
{
    timerInterval = int (timerInterval * 0.9);
}

// Saves value of block to matrix when further down movement is not possible.
void Board::saveBlockToMatrix()
{
    for (int i = 0; i < 4; ++i)
    {
        int x = movingBlock.elements.getReference (i).getX();
        int y = movingBlock.elements.getReference (i).getY();
        int value = movingBlock.value;

        setCellValue (y, x, value);
    }   
}

// Generates random intiger representing particular block. 
int Board::generateBlockNum ()
{
    juce::Random random (Time::currentTimeMillis());

    int pick = random.nextInt (juce::Range<int> (1, 8));
    return pick;
}

// When new block can be generated function assign values of already created block's points to new block object.
bool Board::generateNewBlock ()
{
    int pick = generateBlockNum();
    auto points = generateBlockPoints (pick);

    for (int i = 0; i < 4; ++i)
    {
        if (getCellValue (points[i].getY(), points[i].getX(), false) != 0)      //Test if there's place for new one...
        {   
            pick = 7;                                                     //... and checks if  I-shape fits
            points = generateBlockPoints (7);
        }

        if (getCellValue (points[i].getY(), points[i].getX(), false) != 0)      
            return false;
    }
        
    for (int i = 0; i < 4; ++i)
        movingBlock.elements.getReference (i) = points[i];  

    movingBlock.value = pick;
    return true;
}

// Returns points array with new block's X and Y values depending on value of random generated pick.
juce::Array<juce::Point<int>> Board::generateBlockPoints (int pick)
{
    using Pt = juce::Point<int>;
    juce::Array<Pt> points;

    canBlockRotate = true;

    switch (pick)           
    {
        default:
        case 1:
            canBlockRotate = false;
            points.addArray ( { Pt (4,1), Pt (4,0), Pt (5,0), Pt (5,1) } );     // O-shape
            break;

        case 2:
            points.addArray ( { Pt (4,1), Pt (4,0), Pt (5,0), Pt (6,0) } );     // L-shape
            break;

        case 3:
            points.addArray ( { Pt (4,0), Pt (5,0), Pt (6,0), Pt (6,1) } );     // L-shape (rev)
            break;

        case 4:
            points.addArray ( { Pt (4,0), Pt (5,0), Pt (5,1), Pt (6,1) } );     // S-shape
            break;

        case 5:
            points.addArray ( { Pt (4,1), Pt (5,1), Pt (5,0), Pt (6,0) } );     // S-shape (rev)
            break;

        case 6:
            points.addArray ( { Pt (4,0), Pt (5,0), Pt (5,1), Pt (6,0) } );     // T-shape
            break;

        case 7:
            points.addArray ( { Pt (3,0), Pt (4,0), Pt (5,0), Pt (6,0) } );     // I-shape
            break;
    }

    return points;
}


void Board::addListener (Listener* l)
{
    listeners.add(l);
}

void Board::removeListener (Listener* l)
{
    listeners.remove(l);
}
