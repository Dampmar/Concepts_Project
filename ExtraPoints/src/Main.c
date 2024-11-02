#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Chord.h"

FILE *inputFile;
char token;

// Main Function 
int main(void) {

}

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
void input(){
    ChordNode* head = NULL;
    song(&head);
    match(EOF);
    printChords(head);
    freeChords(head);
}

void song(ChordNode** head){
    do {
        bar(head);
    } while (token != '|');
    match('|');
}

void bar(ChordNode** head) {
    if (isdigit(token))
        meter();
    chords(head);
    match('|');
}

void meter(){
    int x = numerator();
    match('/');
    int y = denominator();
}

int numerator(){
    int value = 0;
    // if not a digit call error 
    if (!isdigit(token))
        error("Expected number");

    // continue retrieving adjacent digits 
    while (isdigit(token)){
        value = value * 10 + (token - '0');
        getToken();
    }

    // check if the value is in the correct range 
    if (value < 0 || value > 15)
        error("Invalid numerator");
    return value;
}

int denominator(){
    int value = 0;
    // if not a digit call error 
    if (!isdigit(token))
        error("Expected a number");
    // continue retrieving adjacent digits 
    while (isdigit(token)){
        value = value * 10 + (token - '0');
        getToken();
    }
    // check if the value is in the correct range 
    if (value != 1 && value != 2 && value != 4 && value != 8 && value != 16)
        error("Invalid denominator");
    return value;
}

void chords(ChordNode** head){
    if (token == 'N') {
        match('N');
        match('C');
    } else if (token == '%') {
        match('%');
    } else {
        do { 
            Chord newChord;
            chord(&newChord);                       // Fill newChord with parsed data
            *head = appendChord(*head, newChord);   // Add chord to lsit
        } while (token != '|');
    } 
}

void chord(Chord* chord){
    memset(chord, 0, sizeof(Chord));
    root(chord->root);
    if (token == '-' || token == '+' || token == 'o' || token == '5' || token == '1' || token == '6' || token == '7' || 
        token == '9' || token == '1' || token == '^' || token == '#' || token == '(' || token == '5' || token == 'b' || token == 's' || token == 'n') {
            description(chord);
    } else {
        chord->qual = ' ';
        chord->sus = 0;
        chord->ext_type = ' ';
        chord->extension = 0;
        chord->add[0] = '\0';
        chord->omit = 0;
    }

    if (token == '/') {
        bass(chord->bass);
    } else {
        chord->bass[0] = '\0';
    }
}

void root(char* root){
    note(root);
}

void note(char *res){
    res[0] = letter();
    if (token == 'b' || token == '#'){
        res[1] = acc();
        res[2] = '\0';
    } else {
        res[1] = '\0';
    }
}

char letter(){
    char temp; 
    switch(token){
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
            temp = token;
            getToken();
            break;
        default:
            error("invalid character found");
            break;
    }
    return temp;
}

char acc(){
    char temp;
    switch(token){
        case '#':
        case 'b':
            temp = token;
            getToken();
            break;
        default:
            error("invalid accidental");
            break;
    }
    return temp;
}

void description(Chord* chord){
    bool hasQual = false, hasQnum = false, hasSus = false, hasAdd = false, hasOmit = false;
    if (token == '-' || token == '+' || token == 'o' || token == '5' || token == '1'){
        chord->qual = qual();
        hasQual = true;
    } else { chord->qual = ' ';}

    if (token == '^' || token == '6' || token == '7' || token == '9' || token == '1'){
        qnum(chord);
        hasQnum = true;
    } else { chord->extension = 0; chord->ext_type = ' '; }

    if (token == '#' || token == 'b' || token == '('){
        add(chord);
        hasAdd = true;
    } else { chord->add[0] = '\0'; }

    if (token == 's') {
        sus(chord);
        hasSus = true;
    } else { chord->sus = 0; }

    if (token == 'n'){
        omit(chord);
        hasOmit = true;
    } else { chord->omit = 0; }

    // Checking if more than 0 of the routes where taken 
    if (hasQual && hasSus)
        error("Invalid sequence of description");

    if (!hasQual && !hasSus && !hasQnum && !hasAdd && !hasOmit)
        error("Expected at least one description to be handled.");
}

char qual(){
    char temp;
    switch(token){
        case '-':
        case '+':
        case 'o':
        case '1':
        case '5':
            temp = token;
            getToken();
            break;
        default:
            error("invalid qual");
            break;
    }
    return temp;
}

void qnum(Chord* chord){
    char temp = ' ';
    if (token == '^') {
        temp - token;
        match('^');
    }
    chord->ext_type = temp;
    int x = ext1();
    chord->extension = x;
}

int ext1(){
    // num => "7" | "9" | "11" | "13"
    int value = 0; 
    if (!isdigit(token))
        error("Expected a number");
    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        match(token);
    }
    if (!(value == 7 || value == 9 || value == 11 || value == 13 || value == 6))
        error("invalid num");
    return value;
}

void add(Chord* chord){
    if (token == '('){
        match('(');
        alt(chord);
        match(')');
    }  else { alt(chord); }
}

void alt(Chord* chord){
    if (token == '#' || token == 'b')
        chord->add[] = acc();
    
    int ext;
    if (token == '5') {
        ext = atoi(token);
        match('5');
    } else {
        ext = ext1();
    }
    if 

}