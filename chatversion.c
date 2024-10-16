#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *inputFile; /* file pointer for reading input */
int token; /* holds the current input character for the parse */
char *fileContents; /* variable to store file contents */
size_t fileSize; /* size of the file */

/* function declarations */
void command();
void song();
void bar();
void meter();
void chords();
void chord();
void error(char *message);
void getToken();
void match(char c, char *message);
void parse();
void readFileContents();

/* Error handling function */
void error(char *message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

/* Function to read the entire contents of the input file into fileContents */
void readFileContents() {
    // Open the input file for reading
    inputFile = fopen("input.txt", "r");
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

/* Function to get the next token from the file */
void getToken() {
    // Get the next character from the string instead of the file
    static size_t currentIndex = 0; // Keep track of the current index in the string
    if (currentIndex < fileSize) {
        token = fileContents[currentIndex++];
    } else {
        token = EOF; // End of string
    }
}

/* Function to match the expected token */
void match(char c, char *message) {
    if (token == c) {
        getToken();
    } else {
        error(message);
    }
}

/* Parsing commands */
void command() {
    song(); /* start parsing a song */
    match('|', "Expected end of song"); /* expect end of song */
}

/* Parsing song */
void song() {
    bar(); /* parse the first bar */
    while (token != EOF && token != '|') {
        bar(); /* parse subsequent bars */
    }
}

/* Parsing a bar */
void bar() {
    meter(); /* parse the meter */
    chords(); /* parse the chords */
    match('|', "Expected end of bar");
}

/* Parsing meter */
void meter() {
    match('1', "Expected numerator (1-15)"); /* placeholder for numerator */
    match('/', "Expected '/'");
    match('4', "Expected denominator (1, 2, 4, 8, 16)"); /* placeholder for denominator */
}

/* Parsing chords */
void chords() {
    if (token == 'N') { /* "NC" is represented as 'N' */
        match('N', "Expected 'N'");
        match('C', "Expected 'C'");
    } else if (token == '%') {
        match('%', "Expected '%'");
    } else {
        chord(); /* parse a chord */
        while (token != '|' && token != EOF) {
            chord(); /* parse additional chords */
        }
    }
}

/* Parsing a chord */
void chord() {
    match('A', "Expected root note (A-G)"); /* placeholder for root note */
    /* Handle optional description and bass */
    if (token == '-') match('-', "Expected quality");
    if (token == 's') match('s', "Expected 'sus'");
    if (token == '/') {
        match('/', "Expected '/' for bass");
        match('A', "Expected bass note (A-G)"); /* placeholder for bass note */
    }
}

/* Main parse function */
void parse() {
    readFileContents(); /* read the contents of the input file */
    getToken(); /* get the first token */
    command(); /* call the parsing procedure for the start symbol */
    free(fileContents); /* free allocated memory for file contents */
}

/* Main function */
int main() {
    parse(); /* start parsing */
    return 0;
}