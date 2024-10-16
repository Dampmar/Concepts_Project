#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

int token; 
int numVal;

void getToken(){

}

// Matching the token to c and fetching next token 
void match(char c, char* message) {
    if (token == c) getToken();
    else error(message);
}

void error(char* message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

void input(){
    song();
    match(EOF, "EOF expected");
}

void song(){
    bar();
    while (isBar(token))
        bar();
    match("|", "| expected");
}

void bar(){
    if (isdigit(token)) 
        meter();
    chords();
    match("|", "| expected");
}

void meter(){
    numerator();
    match("/", "/ expected");
    denominator();
}

void chords(){
    if (token == "N") {
        match("N", "NC expected");
        match("C", "NC expected");
    } else if (token == "%") {
        match("%", "% expected");
    } else {
        chord();
        while (isChord(token))
            chord();
    }
}

void chord() {
    root();
    if (isDescription(token)) description();
    if (isBass(token)) bass();
}

void root() {
    note();
}

void note() {
    letter();
    if (isAcc(token)) acc();
}

void letter() {
    if (isLetter(token)) match(token, "expected a valid letter");
    else error("uppercased letter expected");
}

void acc() {
    if (isAcc(token)) match(token, "expected an accent");
    else error("invalid accent format");
}

void description() {
    if (isQual(token)){
        qual();
        if (isQnum(token)){
            qnum();
        }
    } if (isQnum(token)){
        qnum();
        if (isSus(token)){
            
        }
    }
}