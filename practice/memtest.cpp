#include <iostream>
#include <cstring>

const int chunkSize = 16;

// Function to dynamically allocate memory for a string while reading from input
char* readStringFromInput() {
    char* buffer = nullptr;
    int bufferSize = 0;
    int currentSize = 0;
    char ch;

    std::cout << "Enter a string (Ctrl+D to finish input):\n";

    while (std::cin.get(ch)) {
        if (currentSize >= bufferSize - 1) {
            // The current buffer is full, reallocate memory
            bufferSize += chunkSize;
            char* newBuffer = new char[bufferSize];
            if (buffer) {
                std::strcpy(newBuffer, buffer);
                delete[] buffer;
            }
            buffer = newBuffer;
        }

        buffer[currentSize] = ch;
        currentSize++;
        buffer[currentSize] = '\0'; // Null-terminate the string

        if (ch == '\n') {
            break; // Stop reading on newline (Enter key)
        }
    }

    return buffer;
}

int main() {
    char* inputString = readStringFromInput();

    if (inputString) {
        std::cout << "You entered: " << inputString << std::endl;
        delete[] inputString; // Free the allocated memory
    }

    return 0;
}

