#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <vector>
#include "Memento.h"

class Canvas {
private:
    int width;
    int height;
    std::vector<std::vector<char>> grid;
    std::vector<Memento> history;
    int cursorX;
    int cursorY;
    char currentChar;

    void saveToHistory();

public:
    Canvas(int w, int h);
    
    // Getters
    int getWidth() const;
    int getHeight() const;
    int getCursorX() const;
    int getCursorY() const;
    char getCurrentChar() const;
    char getPixel(int x, int y) const;
    
    // Setters
    void setCurrentChar(char ch);
    void setPixel(int x, int y, char ch);
    void setCursorPosition(int x, int y);
    
    // Operations
    void moveCursor(int dx, int dy);
    void drawLine(int x1, int y1, int x2, int y2, char ch);
    void drawRect(int x1, int y1, int x2, int y2, bool fill, char ch);
    void floodFill(int x, int y, char newChar);
    void clear();
    void undo();
    
    // File I/O
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    // Rendering
    void render(bool lineModeActive = false, bool rectModeActive = false);
};

#endif // CANVAS_H