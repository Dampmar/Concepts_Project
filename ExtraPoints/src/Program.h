#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdbool.h>
#include <stdio.h>

// Enum for chord qualities
typedef enum {
    QUAL_MAJOR,     // None
    QUAL_MINOR,     // -
    QUAL_AUG,       // +
    QUAL_DIM,       // o
    QUAL_POW,       // 5
    QUAL_UNISON,    // 1
} Quality;

// Enum for chord suspensions
typedef enum {
    SUS_NONE,       // None
    SUS2,           // sus2
    SUS4,           // sus4
    SUS24,          // sus24
} Suspension;

// Enum for qnum 
typedef enum {
    EXT_SIX,        // 6
    EXT_SEV,        // 7
    EXT_NIN,        // 9
    EXT_ELE,        // 11
    EXT_THR,        // 13
    EXT_SSEV,       // ^7
    EXT_SNIN,       // ^9
    EXT_SELE,       // ^11
    EXT_STHR,       // ^13
    EXT_NONE,       // None
} Extension;

// Enum for omissions
typedef enum {
    OM_NONE,        // None
    OM_NO3,         // no3 
    OM_NO5,         // no5
    OM_NO35,        // no35
} Omission;

// Enum for addition type 
typedef enum {
    ADD_5, 
    ADD_9,
    ADD_11,
    ADD_13
} AddType;

// Enum for additions 
typedef struct {
    char accid;     // Accidental for the alternative (e.g., '#', 'b', or '\0)
    int add_type;   // 5, 9, 11, 13 
} Addition;

// Musical Note 
typedef struct {
    char letter;
    char accidental;
} Note;

// Structure of a chord
typedef struct {
    Note root;
    Quality qual;
    Extension qnum;
    Addition add;
    Suspension sus;
    Omission om;
    Note bass;
} Chord;

typedef struct ChordNode {
    Chord chord;
    struct ChordNode* next;
} ChordNode;

// Function declarations for error handling and parsing errors 
void error(char* message);
void getToken();
void match(char c);


#endif