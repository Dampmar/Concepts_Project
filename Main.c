#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *inputFile;
int token; 

// Allows for backtracking 
char* fileContents;
size_t fileSize;
int currentPosition;
char filePath[256];
size_t currentIndex = 0;

// Function Declarations
void input();
void song();
void bar();
void meter();
void numerator();
void denominator();
void chords();
void chord();
void root();
void note();
void letter();
void acc();
void description();
void qual();
void qnum();
void num();
void sus();
void bass();
void getToken();
void match(char c, char* message);
void error(char* message);
void readFileContents();
void getAdjacentToken(char* c, int n);

// Helper functions 
void error(char* message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

void match(char c, char *message) {
    if (token == c) getToken();
    else error(message);
}

void getToken() {
    static size_t currentIndex = 0;
    if (currentIndex < fileSize) {
        token = fileContents[currentIndex++];
    } else {
        token = EOF;
    }
}

void readFileContents(char* filePath) {
    // Open the input file for reading
    inputFile = fopen(filePath, "r");
    if (!inputFile) {
        perror("Error opening file");
        exit(1);
    }

    // Move the file pointer to the end of the file to determine the size
    fseek(inputFile, 0, SEEK_END);
    fileSize = ftell(inputFile); // Get the size of the file
    rewind(inputFile); // Move the pointer back to the beginning

    // Allocate memory for fileContents
    fileContents = malloc(fileSize + 1); // +1 for null terminator
    if (!fileContents) {
        perror("Memory allocation failed");
        fclose(inputFile);
        exit(1);
    }

    // Read the file contents into fileContents
    fread(fileContents, sizeof(char), fileSize, inputFile);
    fileContents[fileSize] = '\0'; // Null-terminate the string

    fclose(inputFile); // Close the input file
}

int main() {
    printf("Enter the file name: ");
    fgets(filePath, sizeof(filePath), stdin);
    filePath[strcspn(filePath, "\n")] = 0;

    readFileContents(filePath);
    printf("%s", fileContents);
    return 0;
}
