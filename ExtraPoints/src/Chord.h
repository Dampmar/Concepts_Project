#ifndef EXTRA_H
#define EXTRA_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char root[3];         // Root note: D, Db, or D#
    char qual;            // Chord quality: '-', '+', 'o', '5', '1', or none
    int sus;              // Suspended chord type: 2 for sus2, 4 for sus4, 24 for sus24
    char ext_type;        // Extension type: '^' for major seventh or none
    int extension;        // Extension: 6, 7, 9, 11, or 13
    char add[5];          // Additional notes or alterations, e.g., "#5", "(b9)"
    int omit;             // Indicates omitted notes: 3 for no3, 5 for no5, 35 for no35
    char bass[3];         // Bass note: D, Cb, or C
} Chord;

typedef struct ChordNode {
    Chord chord;
    struct ChordNode* next;
} ChordNode;

// Function declarations for error handling and parsing errors 
void error(char* message);
void getToken();
void match(char c);

// Parsing and processing 
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
int ext1();
void add(Chord* chord);
int ext2();
void alt(Chord* chord);
void sus(Chord* chord);
void omit(Chord* chord);
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