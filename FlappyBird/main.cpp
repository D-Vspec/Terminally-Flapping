#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 20;
const int SPAWN_DELAY = 100;
const int MOVE_DELAY = 50;
const int SPACING = 5;

void hideCursor() {
    cout << "\033[?25l";
}

void showCursor() {
    cout << "\033[?25h";
}

void moveTo(int row, int col) {
    cout << "\033[" << row + 1 << ";" << col + 1 << "H";
}

void displayLine(int WIDTH, int HEIGHT, int column) {
    for (int i = 0; i < HEIGHT; ++i) {
        moveTo(i, column);
        if (column >= 0 && column < WIDTH) cout << 'a';
        if (column + 1 >= 0 && column + 1 < WIDTH) cout << 'a';
    }
}

void clearLine(int WIDTH, int HEIGHT, int column) {
    for (int i = 0; i < HEIGHT; ++i) {
        moveTo(i, column);
        if (column >= 0 && column < WIDTH) cout << ' ';
        if (column + 1 >= 0 && column + 1 < WIDTH) cout << ' ';
    }
}

void displayZero(int WIDTH, int HEIGHT) {
    int row = HEIGHT / 2; // Calculate the center row
    int col = WIDTH / 2;  // Calculate the center column

    moveTo(row, col);
    cout << '0';
}

bool checkCollision(int column) {
    return column == WIDTH / 2 || column == -1;
}

int main() {

    vector<int> columns;

    hideCursor();

    displayZero(WIDTH, HEIGHT);  // Display the '0' character in the center

    bool collisionDetected = false;

    while (!collisionDetected) {
        if (columns.empty() || columns.back() <= WIDTH - SPACING - 2) {
            columns.push_back(WIDTH - 1);
        }

        for (size_t i = 0; i < columns.size(); ++i) {
            clearLine(WIDTH, HEIGHT, columns[i] + 2);
            displayLine(WIDTH, HEIGHT, columns[i]);
            columns[i]--;

            if (checkCollision(columns[i])) {
                collisionDetected = true;
                break;
            }
        }

        if (!columns.empty() && columns.front() < -2) {
            columns.erase(columns.begin());
        }

        this_thread::sleep_for(chrono::milliseconds(MOVE_DELAY));
    }

    showCursor();

    return 0;
}
