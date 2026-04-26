#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Canvas.h"

class InputHandler {
private:
    Canvas* canvas;
    bool lineMode;
    bool rectMode;
    bool waitingForSecondPoint;
    int lineX1, lineY1;
    int rectX1, rectY1;
    
    void showHelp();
    void showModeStatus();
    
public:
    explicit InputHandler(Canvas* c);
    void handleKeyPress(char key);
};

#endif // INPUT_HANDLER_H