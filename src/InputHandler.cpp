#include "InputHandler.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

InputHandler::InputHandler(Canvas* c) : canvas(c), lineMode(false), rectMode(false),
    waitingForSecondPoint(false), lineX1(0), lineY1(0),
    rectX1(0), rectY1(0) {
}

void InputHandler::showHelp() {
    system("cls");
    cout << "==================== HELP ====================" << endl;
    cout << "Cursor control:" << endl;
    cout << "  Arrow keys - move cursor" << endl;
    cout << endl;
    cout << "Tools:" << endl;
    cout << "  L - line drawing mode" << endl;
    cout << "  R - rectangle drawing mode" << endl;
    cout << "  F - flood fill" << endl;
    cout << "  C - clear canvas" << endl;
    cout << "  U - undo last action" << endl;
    cout << endl;
    cout << "File operations:" << endl;
    cout << "  S - save to file (.ascii or .txt)" << endl;
    cout << "  O - load from file (.ascii or .txt)" << endl;
    cout << endl;
    cout << "Symbols:" << endl;
    cout << "  Any printable character - set current symbol" << endl;
    cout << "  1-9 - quick select: @ # % * + - = | /" << endl;
    cout << endl;
    cout << "Other:" << endl;
    cout << "  H - show this help" << endl;
    cout << "  Q - exit" << endl;
    cout << endl;
    cout << "================================================" << endl;
    cout << "Press any key to continue...";
    _getch();
}

void InputHandler::showModeStatus() {
    if (lineMode) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\n\n[LINE MODE] - Press Enter to select first point, then Enter for second point" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
    else if (rectMode) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\n\n[RECTANGLE MODE] - Press Enter to select first corner, then Enter for second corner" << endl;
        SetConsoleTextAttribute(hConsole, 7);
    }
}

void InputHandler::handleKeyPress(char key) {
    // Обработка числовых клавиш
    switch (key) {
        case '1': canvas->setCurrentChar('@'); break;
        case '2': canvas->setCurrentChar('#'); break;
        case '3': canvas->setCurrentChar('%'); break;
        case '4': canvas->setCurrentChar('*'); break;
        case '5': canvas->setCurrentChar('+'); break;
        case '6': canvas->setCurrentChar('-'); break;
        case '7': canvas->setCurrentChar('='); break;
        case '8': canvas->setCurrentChar('|'); break;
        case '9': canvas->setCurrentChar('/'); break;
    }
    
    // Режим линии
    if (lineMode) {
        if (key == 13) { // Enter
            if (!waitingForSecondPoint) {
                lineX1 = canvas->getCursorX();
                lineY1 = canvas->getCursorY();
                waitingForSecondPoint = true;
                
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "\n[First point selected] Move cursor to second point and press Enter" << endl;
                SetConsoleTextAttribute(hConsole, 7);
                _getch();
            }
            else {
                canvas->drawLine(lineX1, lineY1, canvas->getCursorX(), canvas->getCursorY(), canvas->getCurrentChar());
                lineMode = false;
                waitingForSecondPoint = false;
                
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "\n[Line drawn! Line mode finished]" << endl;
                SetConsoleTextAttribute(hConsole, 7);
                _getch();
            }
        }
        else if (key == 27) { // Escape
            lineMode = false;
            waitingForSecondPoint = false;
            cout << "\n[Line mode cancelled]" << endl;
            _getch();
        }
        return;
    }
    
    // Режим прямоугольника
    if (rectMode) {
        if (key == 13) { // Enter
            if (!waitingForSecondPoint) {
                rectX1 = canvas->getCursorX();
                rectY1 = canvas->getCursorY();
                waitingForSecondPoint = true;
                
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "\n[First corner selected] Move cursor to second corner and press Enter" << endl;
                SetConsoleTextAttribute(hConsole, 7);
                _getch();
            }
            else {
                canvas->drawRect(rectX1, rectY1, canvas->getCursorX(), canvas->getCursorY(), false, canvas->getCurrentChar());
                rectMode = false;
                waitingForSecondPoint = false;
                
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 10);
                cout << "\n[Rectangle drawn! Rectangle mode finished]" << endl;
                SetConsoleTextAttribute(hConsole, 7);
                _getch();
            }
        }
        else if (key == 27) { // Escape
            rectMode = false;
            waitingForSecondPoint = false;
            cout << "\n[Rectangle mode cancelled]" << endl;
            _getch();
        }
        return;
    }
    
    // Обычные команды
    switch (key) {
        case 'L': case 'l':
            lineMode = true;
            waitingForSecondPoint = false;
            showModeStatus();
            _getch();
            break;
        case 'R': case 'r':
            rectMode = true;
            waitingForSecondPoint = false;
            showModeStatus();
            _getch();
            break;
        case 'F': case 'f':
            canvas->floodFill(canvas->getCursorX(), canvas->getCursorY(), canvas->getCurrentChar());
            break;
        case 'C': case 'c':
            canvas->clear();
            break;
        case 'U': case 'u':
            canvas->undo();
            break;
        case 'S': case 's': {
            string filename;
            canvas->render();
            cout << "File name to save (.ascii): ";
            cin >> filename;
            if (filename.find('.') == string::npos) filename += ".ascii";
            if (canvas->saveToFile(filename)) {
                cout << "Saved to " << filename << endl;
            }
            else {
                cout << "Save error!" << endl;
            }
            _getch();
            break;
        }
        case 'O': case 'o': {
            string filename;
            canvas->render();
            cout << "File name to load (.ascii or .txt): ";
            cin >> filename;
            if (canvas->loadFromFile(filename)) {
                cout << "Loaded from " << filename << endl;
            }
            else {
                cout << "Load error!" << endl;
            }
            _getch();
            break;
        }
        case 'H': case 'h':
            showHelp();
            break;
        case 'Q': case 'q':
            exit(0);
            break;
        default:
            if (key >= 32 && key <= 126) {
                canvas->setCurrentChar(key);
            }
            break;
    }
}