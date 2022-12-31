#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;

const short 
    gridColumns = 10,
    gridRows = 11,
    colour1 = 15,
    colour2 = 7,
    colour3 = 8,
    colour4 = 0,
    transitionTime = 5000,
    iterTime = 100,
    maxIterCount = transitionTime / iterTime,
    matrixDensity = 5;

vector<vector<char>> clockLetters;
vector<vector<short>> matrixColours, clockColours;

void drawClock(HANDLE stdOutHandle, unsigned long sleep ) {
    for (short c = 0; c < gridColumns; c++) {
        for (short r = 0; r < gridRows; r++) {
            COORD text = { r, c };
            SetConsoleCursorPosition(stdOutHandle, text);
            SetConsoleTextAttribute(stdOutHandle, matrixColours[c][r]);

            cout << clockLetters[c][r];
        }
        cout << endl;
    }

    Sleep(sleep);
}

void transitionAdditionalColours(const short& c, const short& r, const short& currentColour, const short& newColour, const short& iterCount) {
    // Transition the current letter's colour
    matrixColours[c][r] = newColour;

    // If we're stopping the animation
    if (iterCount >= maxIterCount) {
        // If there's another letter under the current one AND 
        // we're not overwriting a letter that is supposed to be lit
        if (
            c + 1 < gridColumns
            && !(
                matrixColours[c + 1][r] == clockColours[c + 1][r] &&
                clockColours[c + 1][r] == colour1
                )
            ) {
            // Pass the colour along
            matrixColours[c + 1][r] = currentColour;
        }
    }
    // If there's another letter under the current one
    else if (c + 1 < gridColumns) {
        // Pass the coloru along
        matrixColours[c + 1][r] = currentColour;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    HANDLE stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (stdOutHandle == INVALID_HANDLE_VALUE) {
        return EXIT_FAILURE;
    }

    clockLetters = {
        { 'I', 'T', 'L', 'I', 'S', 'A', 'S', 'A', 'M', 'P', 'M' }, 
        { 'A', 'C', 'Q', 'U', 'A', 'R', 'T', 'E', 'R', 'D', 'C' },
        { 'T', 'W', 'E', 'N', 'T', 'Y', 'F', 'I', 'V', 'E', 'X' },
        { 'H', 'A', 'L', 'F', 'S', 'T', 'E', 'N', 'F', 'T', 'O' },
        { 'P', 'A', 'S', 'T', 'E', 'R', 'U', 'N', 'I', 'N', 'E' },
        { 'O', 'N', 'E', 'S', 'I', 'X', 'T', 'H', 'R', 'E', 'E' },
        { 'F', 'O', 'U', 'R', 'F', 'X', 'V', 'E', 'T', 'W', 'O' },
        { 'E', 'I', 'G', 'H', 'T', 'E', 'L', 'E', 'V', 'E', 'N' },
        { 'S', 'E', 'V', 'E', 'N', 'T', 'W', 'E', 'L', 'V', 'E' },
        { 'T', 'E', 'N', 'S', 'E', 'O', 'C', 'L', 'O', 'C', 'K' }
    };

    matrixColours = clockColours = {
        { colour1, colour1, colour4, colour1, colour1, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour1, colour1, colour1, colour1, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour1, colour1, colour1, colour1, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour1, colour1, colour1, colour1, colour1, colour4, colour4, colour4, colour4, colour4, colour4 },
        { colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4, colour4 }
    };

    drawClock(stdOutHandle, transitionTime);

    short iterCount = 0, // How many iterations of the matrix animation have we completed?
          random = 0; // Random number to "spawn" new threads in the matrix.
    bool allClockColours = false, // Are all colours in the matrix matching the clock colours?
         allLitUp = false; // Are all the litup letters from the clock also lit up in the matrix?
    
    // While the matrix does not match the clock
    while (!allClockColours) {

        // Increase the iteration and reset all flags
        iterCount++;
        allClockColours = true;
        if (iterCount >= maxIterCount) {
            allLitUp = true;
        }

        // Start scanning the matrix
        for (short c = gridColumns - 1; c >= 0; c--) {
            for (short r = 0; r < gridRows; r++) {

                // If the matrix animation is ending, check our flags first
                if (iterCount >= maxIterCount) {
                    if (matrixColours[c][r] != clockColours[c][r]) {
                        allClockColours = false;
                        if (clockColours[c][r] == colour1) {
                            allLitUp = false;
                        }
                    }
                }
                else {
                    allClockColours = allLitUp = false;
                }

                // Transition all primary coloured letters
                if (matrixColours[c][r] == colour1) {
                    // If we're still doing the animation OR the matrix letter colour does NOT match with the clock
                    if (iterCount < maxIterCount || 
                        matrixColours[c][r] != clockColours[c][r]) {
                        // Transition the current letter to the next colour
                        matrixColours[c][r] = colour2;

                        // If there's a letter beneath the current one, then pass on the colour
                        if (c + 1 < gridColumns) {
                            matrixColours[c + 1][r] = colour1;
                        }
                    }
                    // If we're stopping the animation AND 
                    // the matrix letter colour DOES match with the clock AND
                    // there's a letter beneath the current one AND
                    // the matrix letters for the current clock are not all lit up
                    else if (
                        iterCount >= maxIterCount && 
                        matrixColours[c][r] == clockColours[c][r] && 
                        c + 1 < gridColumns && 
                        matrixColours[c + 1][r] == colour4 && 
                        !allLitUp
                    ) {
                        // Pass the colour along
                        matrixColours[c + 1][r] = colour1;
                    }
                }
                // Transition all 2nd colour letters
                else if (matrixColours[c][r] == colour2) {
                    transitionAdditionalColours(c, r, colour2, colour3, iterCount);
                }
                else if (matrixColours[c][r] == colour3) {
                    transitionAdditionalColours(c, r, colour3, colour4, iterCount);
                }

                // If this is the last/top row and we don't have all clock letters lit
                if (c == 0 && !allLitUp) {
                    // Pick a random number between 1 and X
                    random = 1 + static_cast<short>(rand()) / (static_cast<short>(RAND_MAX / (matrixDensity - 1)));
                    
                    // If the random number is 1
                    if (random == 1) {
                        // Start a new thread in the matrix
                        matrixColours[c][r] = colour1;
                    }
                }
            }
        }
        
        drawClock(stdOutHandle, iterTime);
    }

    return EXIT_SUCCESS;
}