#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Extra.h"

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

void chords(ChordNode** head) {
    if (token == 'N'){
        match('N');
        match('C');
    } else if (token == '%'){
        match('%');
    } else {
        do {
            Chord newChord;
            chord(&newChord);
            *head = appendChord(*head, newChord);
        }  while (token != '|');
    }
}

void Chord(Chord* chord){
    memset(chord, 0, sizeof(Chord));
    root(chord->root);
    if 
}