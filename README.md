# cplusplus-tetris

Game written using Juce library. Designed using Model, View, Controller pattern. Main Content Controller is auto generated file. It is where board, boardview, boardcontroller instances are created. Board contains game logic, inherit from Timer class to call update() - in which every movement of blocks is being noticed, every time intreval and includes listeners to launch Board_View's paint(). BoardView inherits from Component class and displays every change in game model. BoardController grabs keyboard focus and 
