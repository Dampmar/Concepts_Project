#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Program.h"

FILE *inputFile;
char token;

void error(char* message);
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
    printf("\n");
    printf("\nParsing completed successfully\n");
    fclose(inputFile);
    return 0;
}

/*  File Parsing functions */
void error (char* message) {
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
    else {
        char message[50];
        sprintf(message, "Expected '%c'\0", c);
        error(message);
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

void add(Chord* chord) {
    if (token == '(') {
        chord->add.parenthesis = true;
        match('(');
        alt(chord);
        match(')');
    } else {
        chord->add.parenthesis = false;
        alt(chord);
    }
}

void alt(Chord* chord) {
    if (token == '#' || token == 'b') {
        chord->add.accid = acc();
    } else {
        chord->add.accid = '\0';
    }

    switch(token) {
        case '5':
            match('5');
            chord->add.add_type = ADD_5;
            return;
        case '9':
            match('9');
            chord->add.add_type = ADD_9;
            return;
        case '1':
            match('1');
            if (token == '1') {
                chord->add.add_type = ADD_11;
                match('1');
                return;
            }
            else if (token == '3') {
                chord->add.add_type = ADD_13;
                match('3');
                return;
            }
            else error("Expected '1' or '3' after '1'");
        default:
            error("Invalid extension for alt");
            return;
    }
}

Suspension sus(){
    match('s');
    match('u');
    match('s');
    switch(token){
        case '2':
            match('2');
            if (token == '4'){
                match('4');
                return SUS24;
            }
            return SUS2;
        case '4':
            match('4');
            return SUS4;
        default:
            error("Invalid suspension");
    }
}

Omission omit(){
    match('n');
    match('o');
    switch(token){
        case '3':
            match('3');
            if (token == '5'){
                match('5');
                return OM_NO35;
            }
            return OM_NO3;
        case '5':
            match('5');
            return OM_NO5;
        default:
            error("Invalid omission");
    }
}

void bass(Note* bass) {
    match('/');
    note(root);
}

/*  Chord manipulation functions */
void printChord(const Chord* chord) {
    // Print Root note
    printf("%c", chord->root.letter);
    if (chord->root.accidental != '\0') printf("%c", chord->root.accidental);

    // Print Chord Quality 
    switch(chord->qual) {
        case QUAL_MAJOR: break;
        case QUAL_MINOR: printf("-"); break;
        case QUAL_AUG: printf("+"); break;
        case QUAL_DIM: printf("o"); break;
        case QUAL_POW: printf("5"); break;
        case QUAL_UNISON: printf("1"); break;
        default: break;
    }

    // Print Chord qnum
    switch (chord->qnum) {
        case EXT_SIX: printf("6"); break;
        case EXT_SEV: printf("7"); break;
        case EXT_NIN: printf("9"); break;
        case EXT_ELE: printf("11"); break;
        case EXT_THR: printf("13"); break;
        case EXT_SSEV: printf("^7"); break;
        case EXT_SNIN: printf("^9"); break;
        case EXT_SELE: printf("^11"); break;
        case EXT_STHR: printf("^13"); break;
        case EXT_NONE: break; // No extension
        default: break;
    }

    // Print Chord additions (if any)
    if (chord->add.add_type != -1) {
        if (chord->add.parenthesis) {
            printf("(");
            if (chord->add.accid != '\0') printf("%c", chord->add.accid);
            switch(chord->add.add_type) {
                case ADD_5: printf("5"); break;
                case ADD_9: printf("9"); break;
                case ADD_11: printf("11"); break;
                case ADD_13: printf("13"); break;
                default: break;
            }
            printf(")");
        } else {
            if (chord->add.accid != '\0') printf("%c", chord->add.accid);
            switch(chord->add.add_type) {
                case ADD_5: printf("5"); break;
                case ADD_9: printf("9"); break;
                case ADD_11: printf("11"); break;
                case ADD_13: printf("13"); break;
                default: break;
            }
        }
    }

    // Print Chord Suspension  
    switch (chord->sus) {
        case SUS2: printf("sus2"); break;
        case SUS4: printf("sus4"); break;
        case SUS24: printf("sus24"); break;
        case SUS_NONE: break; // No suspension
        default: break;
    }

    // Print Chord Omission 
    switch (chord->om) {
        case OM_NO3: printf("no3"); break;
        case OM_NO5: printf("no5"); break;
        case OM_NO35: printf("no35"); break;
        case OM_NONE: break; // No omission
        default: break;
    }

    // Print bass note (if any)
    if (chord->bass.letter != '\0') {
        printf("/%c", chord->bass.letter);
        if (chord->bass.accidental != '\0') {
            printf("%c", chord->bass.accidental);
        }
    }

    printf("\n");
}

void freeChords(ChordNode* head) {
    ChordNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

ChordNode* createNode(Chord chord) {
    ChordNode* newNode = (ChordNode*)malloc(sizeof(ChordNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->chord = chord;
    newNode->next = NULL;
    return newNode;
}

ChordNode* appendChord(ChordNode* head, Chord chord) {
    ChordNode* newNode = createNode(chord);
    if (head == NULL) {
        return newNode;
    }
    ChordNode* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return head;
}

void printChords(ChordNode* head){
    ChordNode* temp = head;
    while (temp != NULL) {
        printChord(&temp->chord);
    }
}