#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *inputFile; /* file pointer for reading input */
int token; /* holds the current input character for the parse */
char *fileContents; /* variable to store file contents */
size_t fileSize; /* size of the file */
char filePath[256]; /* buffer to hold the file path */
size_t currentIndex = 0; /* index for the current position in fileContents */

/* function declarations */
void command();
void song();
void bar();
void meter();
void chords();
void chord();
void root();
void description();
void bass();
void error(char *message);
void getToken();
void match(char c, char *message);
void parse();
void readFileContents();
int number(int min, int max);

/* Error handling function */
void error(char *message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

/* Function to read the entire contents of the input file into fileContents */
void readFileContents() {
    inputFile = fopen(filePath, "r");
    if (!inputFile) {
        perror("Error opening file");
        exit(1);
    }

    fseek(inputFile, 0, SEEK_END);
    fileSize = ftell(inputFile);
    rewind(inputFile);

    fileContents = malloc(fileSize + 1);
    if (!fileContents) {
        perror("Memory allocation failed");
        fclose(inputFile);
        exit(1);
    }

    fread(fileContents, sizeof(char), fileSize, inputFile);
    fileContents[fileSize] = '\0'; // Null-terminate the string

    fclose(inputFile);
}

/* Function to get the next token from the file */
void getToken() {
    // Skip whitespace
    while (isspace(fileContents[currentIndex])) {
        currentIndex++;
    }

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
    song();
    match('|', "Expected end of song"); // Expect end of song
}

/* Parsing song */
void song() {
    bar(); // Parse the first bar
    while (token != EOF && token != '|') {
        bar(); // Parse subsequent bars
    }
}

/* Parsing a bar */
void bar() {
    if (isdigit(token)) {
        meter(); // Parse the meter
    }
    chords(); // Parse the chords
    match('|', "Expected end of bar");
}

/* Parsing meter */
void meter() {
    number(1, 15); // Parse numerator (1-15)
    match('/', "Expected '/'");

    int denom = number(1, 16); // Parse denominator (valid: 1, 2, 4, 8, 16)
    
    // Check if the denominator is valid
    if (denom != 1 && denom != 2 && denom != 4 && denom != 8 && denom != 16) {
        error("Invalid denominator. Only 1, 2, 4, 8, and 16 are allowed.");
    }
}

/* Function to handle a number in a specific range */
int number(int min, int max) {
    int value = 0;
    if (!isdigit(token)) error("Expected a number");

    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        getToken();
    }

    if (value < min || value > max) {
        error("Number out of range");
    }
    return value;
}

/* Parsing chords */
void chords() {
    if (token == 'N') { // "NC" is represented as 'N'
        match('N', "Expected 'N'");
        match('C', "Expected 'C'");
    } else if (token == '%') {
        match('%', "Expected '%'");
    } else {
        chord(); // Parse a chord
        while (token != '|' && token != EOF) {
            chord(); // Parse additional chords
        }
    }
}

/* Parsing a chord */
void chord() {
    root(); // Parse the root note
    description(); // Parse optional description (quality, qnum, sus)
    if (token == '/') {
        bass(); // Parse optional bass note
    }
}

/* Parsing a root note */
void root() {
    if (token >= 'A' && token <= 'G') {
        getToken();
        if (token == '#' || token == 'b') { // Optional accidental
            getToken();
        }
    } else {
        error("Expected root note (A-G)");
    }
}

/* Parsing chord description */
void description() {
    if (token == '-' || token == '+' || token == 'o') { // Qualifier (optional)
        getToken();
    }

    if (isdigit(token)) { // qnum (optional)
        number(7, 13);
    }

    if (token == 's') { // Suspensions (optional)
        match('s', "Expected 's'");
        if (token == 'u') {
            match('u', "Expected 'u'");
            match('s', "Expected 's'");
            if (token == '2' || token == '4') {
                getToken();
            } else {
                error("Expected 'sus2' or 'sus4'");
            }
        }
    }
}

/* Parsing a bass note */
void bass() {
    match('/', "Expected '/' for bass");
    root(); // Parse the bass note
}

/* Main parse function */
void parse() {
    readFileContents(); // Read the contents of the input file
    getToken(); // Get the first token
    command(); // Call the parsing procedure for the start symbol
    free(fileContents); // Free allocated memory for file contents
}

/* Main function */
int main() {
    printf("Enter the file path: ");
    fgets(filePath, sizeof(filePath), stdin); // Get the file path from the user
    filePath[strcspn(filePath, "\n")] = 0; // Remove the newline character if present

    parse(); // Start parsing
    return 0;
}
