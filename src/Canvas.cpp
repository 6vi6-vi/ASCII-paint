#include "Canvas.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
#include <windows.h>

using namespace std;

Canvas::Canvas(int w, int h) : width(w), height(h), cursorX(0), cursorY(0), currentChar('@') {
    // Ограничения размеров
    if (width < 40) width = 40;
    if (width > 200) width = 200;
    if (height < 20) height = 20;
    if (height > 100) height = 100;
    
    // Инициализация холста
    grid = vector<vector<char>>(height, vector<char>(width, '.'));
}

void Canvas::saveToHistory() {
    if (history.size() >= 20) {
        history.erase(history.begin());
    }
    history.push_back(Memento(grid));
}

int Canvas::getWidth() const { return width; }
int Canvas::getHeight() const { return height; }
int Canvas::getCursorX() const { return cursorX; }
int Canvas::getCursorY() const { return cursorY; }
char Canvas::getCurrentChar() const { return currentChar; }
char Canvas::getPixel(int x, int y) const { return grid[y][x]; }

void Canvas::setCurrentChar(char ch) { currentChar = ch; }

void Canvas::setPixel(int x, int y, char ch) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = ch;
    }
}

void Canvas::moveCursor(int dx, int dy) {
    cursorX += dx;
    cursorY += dy;
    if (cursorX < 0) cursorX = 0;
    if (cursorX >= width) cursorX = width - 1;
    if (cursorY < 0) cursorY = 0;
    if (cursorY >= height) cursorY = height - 1;
}

void Canvas::setCursorPosition(int x, int y) {
    if (x >= 0 && x < width) cursorX = x;
    if (y >= 0 && y < height) cursorY = y;
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, char ch) {
    saveToHistory();
    
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    while (true) {
        setPixel(x, y, ch);
        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

void Canvas::drawRect(int x1, int y1, int x2, int y2, bool fill, char ch) {
    saveToHistory();
    
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);
    
    if (fill) {
        for (int y = y1; y <= y2; y++) {
            for (int x = x1; x <= x2; x++) {
                setPixel(x, y, ch);
            }
        }
    }
    else {
        // Верхняя и нижняя границы
        for (int x = x1; x <= x2; x++) {
            setPixel(x, y1, ch);
            setPixel(x, y2, ch);
        }
        // Левая и правая границы
        for (int y = y1; y <= y2; y++) {
            setPixel(x1, y, ch);
            setPixel(x2, y, ch);
        }
    }
}

void Canvas::floodFill(int x, int y, char newChar) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    
    char targetChar = grid[y][x];
    if (targetChar == newChar) return;
    
    saveToHistory();
    
    stack<pair<int, int>> st;
    st.push({x, y});
    
    while (!st.empty()) {
        auto [cx, cy] = st.top();
        st.pop();
        
        if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
        if (grid[cy][cx] != targetChar) continue;
        
        grid[cy][cx] = newChar;
        
        st.push({cx + 1, cy});
        st.push({cx - 1, cy});
        st.push({cx, cy + 1});
        st.push({cx, cy - 1});
    }
}

void Canvas::clear() {
    saveToHistory();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = '.';
        }
    }
}

void Canvas::undo() {
    if (history.empty()) return;
    grid = history.back().restore();
    history.pop_back();
}

bool Canvas::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file << grid[y][x];
        }
        file << endl;
    }
    return true;
}

bool Canvas::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    
    vector<vector<char>> newGrid(height, vector<char>(width, '.'));
    string line;
    int y = 0;
    
    while (getline(file, line) && y < height) {
        for (int x = 0; x < width && x < (int)line.length(); x++) {
            newGrid[y][x] = line[x];
        }
        y++;
    }
    
    grid = newGrid;
    return true;
}

void Canvas::render(bool lineModeActive, bool rectModeActive) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Очищаем экран
    system("cls");
    
    // Выводим верхнюю границу
    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;
    
    // Выводим холст
    for (int y = 0; y < height; y++) {
        cout << "| ";
        for (int x = 0; x < width; x++) {
            if (x == cursorX && y == cursorY) {
                SetConsoleTextAttribute(hConsole, 112);
                cout << grid[y][x];
                SetConsoleTextAttribute(hConsole, 7);
            }
            else {
                cout << grid[y][x];
            }
        }
        cout << " |" << endl;
    }
    
    // Выводим нижнюю границу
    cout << "=";
    for (int x = 0; x < width + 2; x++) cout << "=";
    cout << "=" << endl;
    
    // Выводим панель управления
    cout << " ===============================================================================" << endl;
    cout << "| Current symbol: [" << currentChar << "]                                       |" << endl;
    
    if (lineModeActive) {
        SetConsoleTextAttribute(hConsole, 14);
        cout << "| LINE MODE: (Enter - select point, Escape - cancel)                  |" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else if (rectModeActive) {
        SetConsoleTextAttribute(hConsole, 14);
        cout << "| RECTANGLE MODE: (Enter - select corner, Escape - cancel)         |" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else {
        cout << "|                                                                               |" << endl;
    }
    
    cout << "| Controls:                                                                   |" << endl;
    cout << "|   [Arrows] - move cursor     [L] - line mode      [R] - rectangle mode        |" << endl;
    cout << "|   [F] - flood fill           [C] - clear canvas   [U] - undo                  |" << endl;
    cout << "|   [S] - save to file         [O] - load from file [H] - help                  |" << endl;
    cout << "|   [1-9, any char] - select drawing symbol                                    |" << endl;
    cout << "|   [Q] - exit                                                                |" << endl;
    cout << " ===============================================================================" << endl;
}