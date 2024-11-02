#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Program.h"

FILE *inputFile;
char token;

/*  File Parsing functions */
void error(char* message) {
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
    if (token == c) 
        getToken();
    else 
        error(c);
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
    } while (token != '|');
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
        } while (token != '|');
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
        chord->bass = (Note) { .letter = '\0', .accidental = '\0'};
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

