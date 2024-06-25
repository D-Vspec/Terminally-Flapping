#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 20;
const int SPAWN_DELAY = 100;
const int MOVE_DELAY = 50;
const int SPACING = 5;

//All the cursor commands and moveTo rely on ANSI escape codes to work
//https://en.wikipedia.org/wiki/ANSI_escape_code

void hideCursor() {
    cout << "\033[?25l";
}

void showCursor() {
    cout << "\033[?25h";
}

void moveTo(int row, int col) {
    cout << "\033[" << row + 1 << ";" << col + 1 << "H";
}

void displayLine(int WIDTH, int HEIGHT, int column, int holeStart) {
    for (int i = 0; i < HEIGHT; ++i) {
        if (i >= holeStart && i < holeStart + 3) {
            continue;  // Skip the hole section
        }
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

bool checkCollision(int column, int holeStart) {
    int centerRow = HEIGHT / 2; // Center row
    return (column == WIDTH / 2 || column == WIDTH / 2 - 1) && !(centerRow >= holeStart && centerRow < holeStart + 3);
}

int main() {
    vector<int> columns;
    vector<int> holePositions;

    srand(time(0)); // Seed for random number generation

    hideCursor();

    displayZero(WIDTH, HEIGHT);  // Display the '0' character in the center

    bool collisionDetected = false;

    while (!collisionDetected) {
        if (columns.empty() || columns.back() <= WIDTH - SPACING - 2) {
            columns.push_back(WIDTH - 1);
            holePositions.push_back(rand() % (HEIGHT - 3)); // Random hole position
        }

        for (size_t i = 0; i < columns.size(); ++i) {
            clearLine(WIDTH, HEIGHT, columns[i] + 2);
            displayLine(WIDTH, HEIGHT, columns[i], holePositions[i]);
            columns[i]--;

            if (checkCollision(columns[i], holePositions[i])) {
                collisionDetected = true;
                break;
            }
        }

        if (!columns.empty() && columns.front() < -2) {
            columns.erase(columns.begin());
            holePositions.erase(holePositions.begin());
        }

        this_thread::sleep_for(chrono::milliseconds(MOVE_DELAY));
    }

    showCursor();

    return 0;
}
