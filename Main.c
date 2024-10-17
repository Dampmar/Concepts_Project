#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "library.h"

FILE *inputFile;
int token; 

// The contents of the file are turned into a string for easier handling in the program
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
void numerator(int min, int max);
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

void match(char c, char *message) {
    // If the token is the appropiate move on to the next
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

// Implementation of actual grammar functions
void input() {
    song();
    match(EOF, "EOF expected");
}

// Parsing a song and checking for repeated bars
void song() {
    bar();
    while (isBar(token)) bar();
    match("|", "'|' expected");
}

// Parsing a bar 
void bar() {
    if (isdigit(token)) meter();
    chords();
    match("|", "'|' expected");
}

// Parsing the meter 
void meter() {
    numerator(1, 16);
    match("/", "'/' expected");
    denominator();
}

// Parsing numerator and checking for validity 
void numerator(int min, int max) {
    int value = 0;
    if (!isdigit(token)) 
        error("Expected a number");
    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        match(token, "");
    }
    if (value < min || value > max)
        error("Number out of range");
}

// Parsing the denominator and checking for validity 
void denominator() {
    int value = 0;
    if (!isdigit(token))
        error("Expected a number");
    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        match(token, "");
    }
    static int validDenominatorValues = [1,2,4,8,16];
    if (!isValidDenominator(value, 
                            validDenominatorValues, 
                            sizeof(validDenominatorValues) / sizeof(int)))
        error("Invalid denominator value");
}

// Parsing the chords section 
void chords() {
    if (token == 'N') {
        match('N', "expected 'N'");
        match('C', "expected 'C'");
    } else if (token == '%') {
        match('%', "expected '%'");
    } else {
        chord();
        while (token != '|' && token != EOF) {
            chord();
        }
    }
}


int main() {
    printf("Enter the file name: ");
    fgets(filePath, sizeof(filePath), stdin);
    filePath[strcspn(filePath, "\n")] = 0;

    readFileContents(filePath);
    printf("%s", fileContents);
    return 0;
}
