#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

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

void displayZero(int width, int height) {
    int row = height / 2; // Calculate the center row
    int col = width / 2;  // Calculate the center column

    moveTo(row, col);
    cout << '0';
}

bool checkCollision(int column, int holeStart) {
    int centerRow = 20 / 2; // Assuming height is 20
    return (column == 0 || column == -1) && !(centerRow >= holeStart && centerRow < holeStart + 3);
}

int main() {
    const int width = 80;
    const int height = 20;
    const int spawnDelay = 100;
    const int moveDelay = 50;
    const int spacing = 5;

    vector<int> columns;
    vector<int> holePositions;

    srand(time(0)); // Seed for random number generation

    hideCursor();

    displayZero(width, height);  // Display the '0' character in the center

    bool collisionDetected = false;

    while (!collisionDetected) {
        if (columns.empty() || columns.back() <= width - spacing - 2) {
            columns.push_back(width - 1);
            holePositions.push_back(rand() % (height - 3)); // Random hole position
        }

        for (size_t i = 0; i < columns.size(); ++i) {
            clearLine(width, height, columns[i] + 2);
            displayLine(width, height, columns[i], holePositions[i]);
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

        this_thread::sleep_for(chrono::milliseconds(moveDelay));
    }

    showCursor();

    return 0;
}
