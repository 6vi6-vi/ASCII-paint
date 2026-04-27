#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Canvas.h"
#include "InputHandler.h"

using namespace std;

int main() {
    int width, height;
    
    cout << "==========================================================" << endl;
    cout << "|                     ASCII PAINT                        |" << endl;
    cout << "==========================================================" << endl;
    cout << endl;
    cout << "Enter canvas size (from 40x20 to 200x100)" << endl;
    cout << "Width (40-200): ";
    cin >> width;
    cout << "Height (20-100): ";
    cin >> height;
    
    Canvas canvas(width, height);
    InputHandler handler(&canvas);
    
    while (true) {
        canvas.render();
        
        char key = _getch();
        
        // Обработка стрелок
        if (key == -32 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: canvas.moveCursor(0, -1); break;
                case 80: canvas.moveCursor(0, 1); break;
                case 75: canvas.moveCursor(-1, 0); break;
                case 77: canvas.moveCursor(1, 0); break;
            }
        }
        else {
            handler.handleKeyPress(key);
        }
    }
    
    return 0;
}