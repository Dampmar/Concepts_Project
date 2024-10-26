#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

FILE *inputFile;
char token;

<<<<<<< Updated upstream:Debug.c
=======
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

>>>>>>> Stashed changes:ChordCalc.c
// Function declarations 
void error(char* message);
void getToken();
void match(char c, char* message);
void input();
void song();
void bar();
void meter();
int numerator();
int denominator();
void chords();
void chord();
void root();
void note();
char letter();
char acc();
void description();
char qual();
void qnum();
int num();
void sus();
void bass();

<<<<<<< Updated upstream:Debug.c
// Main function
int main(void){
    char filename[100];
    printf("Enter the name of the file to be parsed: ");
    scanf("%s", filename);
    inputFile = fopen(filename, "r");
=======
// Chord calculator functions 
void printChord(Chord* chord);
void printChords (ChordNode* head);
void createChordArr(Chord* chord, int* arr);
int noteToPitchClass(const char* root);

// Main Function 
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
>>>>>>> Stashed changes:ChordCalc.c
    if (inputFile == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    getToken();
    input();
    printf("Parsing completed successfully\n");
    fclose(inputFile);
    return 0;
}
<<<<<<< Updated upstream:Debug.c
=======

// Functions for handling chords 
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

void printChords(ChordNode* head) {
    ChordNode* temp = head; 
    int chordNum = 1;
    int pitchClassNum[12] = {0};
    printf("\n");
    printf("      \t 0 \t 1 \t 2 \t 3 \t 4 \t 5 \t 6 \t 7 \t 8 \t 9 \t A \t B \t");
    printf("\n");
    printf("      \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t");
    printf("\n");
    while (temp != NULL) {
        int arr[12] = {0};
        printf("%4d. ", chordNum);
        createChordArr(&temp->chord, arr);  // Calculating pitch class
        // Printing the pitch class of the chord 
        printf("\t ");
        for (int i = 0; i < 12; i++) {
            if (arr[i]) {
                printf("* \t");
                pitchClassNum[i]++;
            }
            else {
                printf("  \t");
            }
        }
        printf("- ");
        printChord(&temp->chord);
        printf("\n");
        temp = temp->next;
        chordNum++;
    }
    printf("      \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t - \t");
    printf("\n");
    printf("      \t");
    for (int i = 0; i < 12; i++) {
        printf("%3d\t", pitchClassNum[i]);
    }
    printf("\n");
}

// Creating the pitch class of a chord in an array 
void createChordArr(Chord* chord, int* arr) {
    memset(arr, 0, sizeof(int) * 12); 

    // Getting Pitch Class
    int rootPC = noteToPitchClass(chord->root);
    if (rootPC == -1) {
        printf("Invalid note");
        exit(1);
    }

    arr[rootPC] = 1; // Mark the root note

    // Handle chord quality
    switch (chord->qual) {
        case '-':
            arr[(rootPC + 3) % 12] = 1;
            arr[(rootPC + 7) % 12] = 1;
            break;
        case '+':
            arr[(rootPC + 4) % 12] = 1;
            arr[(rootPC + 8) % 12] = 1;
            break;
        case 'o':
            arr[(rootPC + 3) % 12] = 1;
            arr[(rootPC + 6) % 12] = 1;
            break;
        default: 
            arr[(rootPC + 4) % 12] = 1;
            arr[(rootPC + 7) % 12] = 1;
            break;
    }
    
    // Handle chord qnum 
    switch (chord->extension) {
        case 7:
            if (chord->ext_type == '^') {
                arr[(rootPC + 11) % 12] = 1;
            } else {
                arr[(rootPC + 10) % 12] = 1;
            }
            break;
        case 9:
            if (chord->ext_type == '^') {
                arr[(rootPC + 2) % 12] = 1;
                arr[(rootPC + 11) % 12] = 1;
            } else {
                arr[(rootPC + 2) % 12] = 1;
                arr[(rootPC + 10) % 12] = 1;
            }
            break;
        case 11:
            if (chord->ext_type == '^') {
                arr[(rootPC + 5) % 12] = 1;
                arr[(rootPC + 11) % 12] = 1;
            } else {
                arr[(rootPC + 5) % 12] = 1;
                arr[(rootPC + 10) % 12] = 1;
            }
            break;
        case 13:
            if (chord->ext_type == '^') {
                arr[(rootPC + 9) % 12] = 1;
                arr[(rootPC + 11) % 12] = 1;
            } else {
                arr[(rootPC + 9) % 12] = 1;
                arr[(rootPC + 10) % 12] = 1;
            }
        default:
            break;
    }

    // Handle chord suspension
    switch (chord->sus) {
        case 2:
            arr[(rootPC + 2) % 12] = 1;
            arr[(rootPC + 7) % 12] = 1;
            break;
        case 4:
            arr[(rootPC + 5) % 12] = 1;
            arr[(rootPC + 7) % 12] = 1;
            break;
        default:
            break;
    }

    // Handle chord bass
    if (chord->bass[0] != '\0') {
        int bassPC = noteToPitchClass(chord->bass);
        if (bassPC != -1) arr[bassPC] = 1;
    }

}

int noteToPitchClass(const char* root) {
    switch (root[0]) {
        case 'C': 
            if (root[1] == 'b') return 11;
            if (root[1] == '\0') return 0;
            if (root[1] == '#') return 1;
        case 'D':
            if (root[1] == 'b') return 1;
            if (root[1] == '\0') return 2;
            if (root[1] == '#') return 3;
        case 'E':
            if (root[1] == 'b') return 3;
            if (root[1] == '\0') return 4;
            if (root[1] == '#') return 5;
        case 'F':
            if (root[1] == 'b') return 4;
            if (root[1] == '\0') return 5;
            if (root[1] == '#') return 6;
        case 'G':
            if (root[1] == 'b') return 6;
            if (root[1] == '\0') return 7;
            if (root[1] == '#') return 8;
        case 'A':
            if (root[1] == 'b') return 8;
            if (root[1] == '\0') return 9;
            if (root[1] == '#') return 10;
        case 'B':
            if (root[1] == 'b') return 10;
            if (root[1] == '\0') return 11;
            if (root[1] == '#') return 0;
        default:
            return -1;
    }
}

// Function to convert root note 
void printChord(Chord* chord) {
    printf("%s", chord->root);
    // Checking for the description of this chord
    if (chord->qual != ' ') { printf("%c", chord->qual); }                              // if it has a quality
    if (chord->ext_type == '^') { printf("%c%d", chord->ext_type, chord->extension); }  // if it has a qnum starting with '^'
    else if (chord->extension != 0) { printf("%d", chord->extension); }                 // if it has a qnum with no '^'
    if (chord->sus != 0) { printf("sus%d", chord->sus); }                               // if it has a sus 
    // Checking for the bass of the chord
    if (chord->bass[0] != '\0') { printf("/%s", chord->bass); }
}

void printCho(ChordNode* head) {
    ChordNode* temp = head;
    while (temp != NULL) {
        printf("Chord: %s", temp->chord.root);
        if (temp->chord.qual != ' ') printf("%c", temp->chord.qual);

        if (temp->chord.ext_type == '^') printf("%c%d", temp->chord.ext_type, temp->chord.extension);
        else if (temp->chord.extension != 0) printf("%d", temp->chord.extension);

        if (temp->chord.sus != 0) printf("sus%d", temp->chord.sus);

        if (temp->chord.bass[0] != '\0') printf(" / %s\n", temp->chord.bass);
        else printf("\n");
        temp = temp->next;
    }
}

void freeChords(ChordNode* head) {
    ChordNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
>>>>>>> Stashed changes:ChordCalc.c

// Program functions
void error(char* message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

void getToken(){
    // tokens are characters
    token = getc(inputFile);
    if (token == EOF) 
        exit(1); 
    while(token == ' ' || token == '\n' || token == '\t'){
        token = getc(inputFile);
    }
}

// Checking if the result is the expected one 
void match(char c, char* message){
    if (token == c) 
        getToken();
    else 
        error(message);
}

void input(){
    // input -> song 'EOF'
    // printf("Debug: input: Starting input parsing.\n");
    song();
    match(EOF, "EOF expected");
    // printf("Debug: input: Finished input parsing.\n");
}

void song(){
    // song -> bar {bar} '|'
    // printf("Debug: song: Starting song parsing.\n");
    // The process of getting bars in a song ends whenever the token after a bar is '|'
    do {
        bar();
    } while (token != '|');
    printf("%c successfully matched in song\n", token);
    match('|', "| expected");
    // printf("Debug: song: Finished song parsing.\n");
}

void bar(){
    // bar -> [meter] chords '|'
    // printf("Debug: bar: Starting bar parsing\n");
    if (isdigit(token))
        meter();
    chords();
    printf("%c successfully matched in bar\n", token);
    match('|', "| expected");
    // printf("Debug: bar: Finished bar parsing\n");
}

void meter(){
    // meter -> numerator "/" denominator 
    // printf("Debug: meter: Starting meter parsing\n");
    int x = numerator();
    printf("Meter: %d %c", x, token);
    match('/', "/ expected in meter");
    int y = denominator();
    printf("%d\n", y);
    // printf("Debug: meter: Finished meter parsing\n");
}

int numerator(){
    // numerator -> "1" | "2" | ... | "15"
    // printf("Debug: numerator: Starting numerator parsing\n");
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
    if (value < 0 || value > 15)
        error("Invalid numerator");
    return value;
    // printf("Debug: numerator: Finished numerator parsing\n");
}

int denominator(){
    // denominator => "1" | "2" | "4" | "8" | "16"
    // printf("Debug: denominator: Starting denominator parsing\n");
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
    // printf("Debug: denominator: Finished denominator parsing\n");
    return value;
}

void chords(){
    // chords => "NC" | "%" | chord {chord}
    // printf("Debug: chords: Starting chords parsing\n");
    if (token == 'N') {
        match('N', "N expected");
        match('C', "C expected");
        printf("Chords: NC");
    } else if (token == '%') {
        match('%', "\% expected");
        printf("Chords: %");
    } else {
        do { chord(); } while (token != '|');
    } 
    // printf("Debug: chords: Finished chords parsing\n");
}

void chord(){
    // chord => root [description] [bass]
    // printf("Debug: chord: Starting chord parsing\n");
    root();
    if (token == 's' || token == '-' || token == '+' || token == 'o'|| 
        token == '7' || token == '9' || token == '1' || token == '^')
        description();
    if (token == '/')
        bass();
    // printf("Debug: chord: Finished chord parsing\n");
}

void root(){
    // root => note
    // printf("Debug: root: Starting root parsing\n");
    note();
    // printf("Debug: root: Finished root parsing\n");
}

void note(){
    // note => letter [acc]
    // printf("Debug: note: Starting note parsing\n");
    letter();
    if (token == 'b' || token == '#') 
        acc();
    // printf("Debug: note: Finished note parsing\n");
}

char letter(){
    // letter => "A" | "B" | ... | "G"
    // printf("Debug: letter: Starting letter parsing, token is '%c'\n", token);
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
            printf("Letter: %c\n", token);
            getToken();
            break;
        default:
            error("invalid note (letter)");
            break;
    }
    return temp;
    // printf("Debug: letter: Parsed letter \n");
}

char acc(){
    // acc => "#" | "b"
    // printf("Debug: acc: Starting accidental parsing, token is '%c'\n", token);
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
    printf("Acc: %c\n", temp);
    // printf("Debug: acc: Finished accidental parsing");
    return temp;
}

void description(){
    // description => qual | qual qnum | qnum | qnum sus | sus 
    // printf("Debug: description: Starting description parsing\n");
    bool hasQual = false, hasQnum = false, hasSus = false;
    if (token == '-' || token == '+' || token == 'o'){ 
        qual(); 
        hasQual = true;
    }
    if (token == '^' || token == '7' || token == '9' || token == '1'){
        qnum();
        hasQnum = true;
    }
    if (token == 's' && !hasQual) {
        sus();
        hasSus = true;
    }
    // Checking if more than 0 of the routes where taken 
    if (!hasQual && !hasQnum && !hasSus)
        error("Invalid description: at least one of qual, qnum, or sus expected");
    // printf("Debug: description: Finished description parsing\n");   
}

char qual(){
    // qual => "-" | "+" | "o"
    // printf("Debug: qual: Starting qual parsing, token is '%c'\n", token);
    char temp;
    switch(token){
        case '-':
        case '+':
        case 'o':
            temp = token;
            getToken();
            break;
        default:
            error("invalid qual");
            break;
    }
    printf("Qual: %c\n", temp);
    return temp;
    // printf("Debug: qual: Parsed qual\n");
}

void qnum(){
    // qnum => ["^"] num
    // printf("Debug: qnum: Starting qnum parsing\n");
    char temp = ' '; 
    if (token == '^') {
        temp = token;
        match('^', "^ expected");
<<<<<<< Updated upstream:Debug.c
    }
=======
    } 
    chord->ext_type = temp;
>>>>>>> Stashed changes:ChordCalc.c
    int x = num();
    printf("Qnum: %c %d\n", temp, x);
    // printf("Debug: qnum: Finished qnum parsing\n");
}

int num(){
    // num => "7" | "9" | "11" | "13"
    int value = 0; 
    if (!isdigit(token))
        error("Expected a number");
    while (isdigit(token)) {
        value = value * 10 + (token - '0');
        match(token, "");
    }
    if (!(value == 7 || value == 9 || value == 11 || value == 13))
        error("invalid num");
    return value;
}

void sus(){
    // sus => "sus2" | "sus4"
    // printf("Debug: sus: Starting sus parsing\n");
    if(token == 's'){
        match('s', "invalid sus sequence");
        match('u', "invalid sus sequence");
        match('s', "invalid sus sequence");
        switch(token){
            case '2':
                printf("Suspension: sus2\n");
                match('2', "2 expected");
                break;
            case '4':
                printf("Suspension: sus4\n");
                match('4', "4 expected");
                break;
            default:
                error("invalid suspended sequence");
        }
    } else error("invalid input as sus");
    // printf("Debug: sus: Finished sus parsing\n");
}

void bass(){
    // bass => "/" note
    match('/', "/ expected");
    note();
}
