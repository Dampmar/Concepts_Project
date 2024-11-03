#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Program.h"

FILE *inputFile;
char token;

void error(const char* message);
void getToken();
void match(char c);
void input();
void song(ChordNode** head);
void bar(ChordNode** head);
void meter();
int numerator();
int denominator();
void chords(ChordNode** head);
void chord(Chord* chord);
void root(Note* root);
void note(Note* res);
char letter();
char acc();
void description(Chord* chord);
Quality qual();
Extension qnum();
void add(Chord* chord);
Suspension sus();
Omission omit();
void bass(Note* bass);
void freeChords(ChordNode* head);
ChordNode* appendChord(ChordNode* head, Chord chord);
void printChords(ChordNode* head);
void alt(Chord* chord);
void printChord(const Chord* chord); // Added declaration for printChord

int main(void) {
    char filepath[365];
    printf("Enter the path of the file to be parsed: ");
    scanf("%[^\n]s", filepath);
    
    // Checking if the file is indeed a .txt file 
    if (strlen(filepath) < 4 || strcmp(filepath + strlen(filepath) - 4, ".txt") != 0) {
        printf("Error: Only .txt files are supported\n");
        exit(1);
    }

    // Opening the file to handle it 
    inputFile = fopen(filepath, "r");
    if (inputFile == NULL){
        printf("Error: cannot open file\n");
        exit(1);
    }
    
    printf("The following characters demonstrate the tokens being parsed.\n\n");
    getToken();
    input();
    printf("\nParsing completed successfully\n");
    fclose(inputFile);
    return 0;
}

/*  File Parsing functions */
void error(const char* message) {
    printf("\nParse error: %s\n", message);
    exit(1);
}

void getToken(){
    // tokens are characters
    token = getc(inputFile);
    if (token == EOF) 
        return; 
    while(token == ' ' || token == '\n' || token == '\t'){
        token = getc(inputFile);
    }
}

// Checking if the result is the expected one 
void match(char c){
    if (token == c) {
        getToken();
    } else if (token == EOF) {
        error("Unexpected end of input");
    } else {
        error("Unexpected character");
    }
}

/*  Parser functions */
void input() {
    ChordNode* head = NULL;
    song(&head);
    match(EOF);
    printChords(head);
    freeChords(head);
}

void song(ChordNode** head) {
    do {
        bar(head);
    } while (token != EOF && token != '|');
    match('|');
}

void bar(ChordNode** head) {
    if (isdigit(token)) meter();
    chords(head);
    match('|');
}

void meter(){
    numerator();
    match('/');
    denominator();
}

int numerator() {
    int value = 0;
    if (!isdigit(token)) error("Expected number");
    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        getToken();
    }
    if (value < 1 || value > 15) error("Invalid numerator");
    return value;
}

int denominator(){
    int value = 0;
    if (!isdigit(token)) error("Expected number");
    while (isdigit(token)){
        value = value * 10 + (token - '0');
        getToken();
    }
    if (value != 1 && value != 2 && value != 4 && value != 8 && value != 16)
        error("Invalid denominator");
    return value;
}

void chords(ChordNode** head) {
    if (token == 'N') {
        match('N');
        match('C');
    } else if (token == '%') {
        match('%');
    } else {
        do {
            Chord newChord;
            chord(&newChord);  
            *head = appendChord(*head, newChord);
        } while (token != '|' && token != EOF);
    } 
}

void chord(Chord* chord) {
    memset(chord, 0, sizeof(Chord));
    root(&chord->root);
    if (strchr("-+o51679^", token) != NULL) {
        description(chord);
    } else {
        chord->qual = QUAL_MAJOR;
        chord->sus = SUS_NONE;
        chord->qnum = EXT_NONE;
        chord->add.accid = '\0';
        chord->add.add_type = -1;
        chord->om = OM_NONE;
    }

    if (token == '/') {
        bass(&chord->bass);
    } else {
        chord->bass = (Note) { .letter = '\0', .accidental = '\0' };
    }
}

void root(Note* root) {
    note(root);
}

void note(Note* res) {
    res->letter = letter();
    res->accidental = (token == 'b' || token == '#') ? acc(): '\0';
}

char letter() {
    char temp; 
    if (strchr("ABCDEFG", token) != NULL) {
        temp = token;
        getToken();
    } else {
        error("Invalid letter found");
    }
    return temp;
}

char acc() {
    char temp;
    if (strchr("#b", token) != NULL) {
        temp = token;
        getToken();
    } else {
        error("Invalid accidental");
    }
    return temp;
}

void description(Chord* chord) {
    bool hasQual = false, hasQnum = false, hasSus = false, hasAdd = false, hasOmit = false;
    if (strchr("-o51", token) != NULL) {
        chord->qual = qual();
        hasQual = true;
    } else { chord->qual = QUAL_MAJOR; }

    if (strchr("^67891", token) != NULL) {
        chord->qnum = qnum();
        hasQnum = true;
    } else { chord->qnum = EXT_NONE; }

    if (strchr("#b(", token) != NULL) {
        add(chord);
        hasAdd = true;
    } else {
        chord->add.accid = '\0';
        chord->add.add_type = -1;
    }

    if (token == 's') {
        chord->sus = sus();
        hasSus = true;
    } else { chord->sus = SUS_NONE; }

    if (token == 'n') {
        chord->om = omit();
        hasOmit = true;
    } else { chord->om = OM_NONE; }

    if (hasQual && hasSus)
        error("Invalid sequence of description");

    if (!hasQual && !hasSus && !hasQnum && !hasAdd && !hasOmit)
        error("Expected at least one description to be handled.");
}

Quality qual() {
    switch(token) {
        case '-': match('-'); return QUAL_MINOR;
        case '+': match('+'); return QUAL_AUG;
        case 'o': match('o'); return QUAL_DIM;
        case '5': match('5'); return QUAL_POW;
        case '1': match('1'); return QUAL_UNISON;
        default: error("Invalid quality");
    }
}

Extension qnum() {
    Extension temp;
    if (token == '^') {
        match('^');
        switch (token) {
            case '7':
                temp = EXT_SSEV;
                break;
            case '9':
                temp = EXT_SNIN;
                break;
            case '1':
                getToken();
                if (token == '1') temp = EXT_SELE;
                else if (token == '3') temp = EXT_STHR;
                else error("Expected a '1' or '3' after '^1'");
                break;
            default:
                error("Invalid extended type after '^'");
                break;
        }
    } else {
        switch (token) {
            case '6':
                temp = EXT_SIX;
                break;
            case '7':
                temp = EXT_SEV;
                break;
            case '9':
                temp = EXT_NIN;
                break;
            case '1':
                getToken();
                if (token == '1') temp = EXT_ELE;
                else if (token == '3') temp = EXT_THR;
                else error("Expected a '1' or '3' after '1'");
                break;
            default:
                error("Invalid extended type");
                break;
        }
    }
    getToken();
    return temp;
}

Suspension sus() {
    Suspension result = SUS_NONE;

    if (token == '2') {
        match('2');
        result = SUS2;
    } else if (token == '4') {
        match('4');
        result = SUS4;
    }

    // Check for combined suspension
    if (result == SUS2 && token == '4') {
        match('4'); // Consume '4'
        return SUS24; // Return combined suspension
    }

    // If only one or none is matched, return the result
    return result;
}

Omission omit() {
    if (token == 'n') {
        match('n');
        return OM;
    }
    error("Invalid omission type");
    return OM_NONE; // should never reach here
}

void bass(Note* bass) {
    note(bass);
}

// Linked List Functions
ChordNode* appendChord(ChordNode* head, Chord chord) {
    ChordNode* newNode = (ChordNode*)malloc(sizeof(ChordNode));
    newNode->chord = chord;
    newNode->next = NULL;

    if (head == NULL) {
        return newNode; // New head
    } else {
        ChordNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode; // Append to the end
        return head; // Return the unchanged head
    }
}

void printChords(ChordNode* head) {
    ChordNode* current = head;
    while (current != NULL) {
        printChord(&current->chord);
        current = current->next;
    }
}

void printChord(const Chord* chord) {
    // Print logic for Chord
    printf("Chord: Root - %c, Quality - %d, Bass - %c\n", chord->root.letter, chord->qual, chord->bass.letter);
}

// Free memory for linked list
void freeChords(ChordNode* head) {
    ChordNode* current = head;
    while (current != NULL) {
        ChordNode* next = current->next;
        free(current);
        current = next;
    }
}
