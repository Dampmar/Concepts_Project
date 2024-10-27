#ifndef CHORDCALC_H
#define CHORDCALC_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char root[3];       // D, Db or D#
    char qual;          // -, +, o, none 
    int sus;            // sus2 or sus4
    char ext_type;      // ^ or none 
    int extension;      // 7, 9, 11, 13
    char bass[3];       // D, Cb or C
} Chord;

typedef struct ChordNode {
    Chord chord;
    struct ChordNode* next;
} ChordNode;

// Function declarations for error handling and parsing errors
void error(char* message);
void getToken();
void match(char c, char* message);

// Parsing and processing functions 
void input();
void song(ChordNode** head);
void bar(ChordNode** head);
void meter();
int numerator();
int denominator();
void chords(ChordNode** head);
void chord(Chord* chord);
void root(char* root);
void note(char* res);
char letter();
char acc();
void description(Chord* chord);
char qual();
void qnum(Chord* chord);
int num();
void sus(Chord* chord);
void bass(char* bass);

// Chord calculator functions 
void printChord(Chord* chord);
void printChords(ChordNode* head);
void createChordArr(Chord* chord, int* arr);
int noteToPitchClass(const char* root);

// Chord linked list management functions 
ChordNode* createNode(Chord chord);
ChordNode* appendChord(ChordNode* head, Chord chord);
void freeChords(ChordNode* head);

#endif 