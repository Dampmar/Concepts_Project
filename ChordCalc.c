#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

FILE *inputFile;
char token;

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

// Program functions
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
void match(char c, char* message){
    if (token == c) 
        getToken();
    else 
        error(message);
}

void input(){
    // input -> song 'EOF'
    ChordNode* head = NULL;
    song(&head);
    match(EOF, "EOF expected");
    printChords(head); 
    freeChords(head);
}

void song(ChordNode** head){
    // song -> bar {bar} '|'
    // The process of getting bars in a song ends whenever the token after a bar is '|'
    do {
        bar(head);
    } while (token != '|');
    printf("%c", token);
    match('|', "| expected");
}

void bar(ChordNode** head){
    // bar -> [meter] chords '|'
    if (isdigit(token))
        meter();
    chords(head);
    printf("%c", token);
    match('|', "| expected");
}

void meter(){
    // meter -> numerator "/" denominator 
    int x = numerator();
    printf("%d%c", x, token);
    match('/', "/ expected in meter");
    int y = denominator();
    printf("%d", y);
}

int numerator(){
    // numerator -> "1" | "2" | ... | "15"
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
}

int denominator(){
    // denominator => "1" | "2" | "4" | "8" | "16"
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
    // chords => "NC" | "%" | chord {chord}
    if (token == 'N') {
        match('N', "N expected");
        match('C', "C expected");
        printf("NC");
    } else if (token == '%') {
        match('%', "% expected");
        printf("%%");
    } else {
        do { 
            Chord newChord;
            chord(&newChord);                       // Fill newChord with parsed data
            *head = appendChord(*head, newChord);   // Add chord to lsit
        } while (token != '|');
    } 
}

void chord(Chord* chord){
    // chord => root [description] [bass]
    memset(chord, 0, sizeof(Chord));
    root(chord->root);
    if (token == 's' || token == '-' || token == '+' || token == 'o'|| 
        token == '7' || token == '9' || token == '1' || token == '^')
        description(chord);
    else {
        chord->extension = 0;
        chord->qual = ' ';
        chord->sus = 0;
    }
    if (token == '/') bass(chord->bass);
    else chord->bass[0] = '\0';
}

void root(char* root){
    // root => note
    note(root);
}

void note(char *res){
    // note => letter [acc]
    res[0] = letter();
    if (token == 'b' || token == '#') {
        res[1] = acc();
        res[2] = '\0';
    } else {
        res[1] = '\0';
    }
}

char letter(){
    // letter => "A" | "B" | ... | "G"
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
            printf("%c", token);
            getToken();
            break;
        default:
            error("invalid character found");
            break;
    }
    return temp;
}

char acc(){
    // acc => "#" | "b"
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
    printf("%c", temp);
    return temp;
}

void description(Chord* chord){
    // description => qual | qual qnum | qnum | qnum sus | sus 
    bool hasQual = false, hasQnum = false, hasSus = false;
    if (token == '-' || token == '+' || token == 'o'){ 
        chord->qual = qual(); 
        hasQual = true;
    } else { chord->qual = ' '; }
    if (token == '^' || token == '7' || token == '9' || token == '1'){
        qnum(chord);
        hasQnum = true;
    } else { chord->extension = 0; }
    if (token == 's' && !hasQual) {
        sus(chord);
        hasSus = true;
    } else { chord->sus = 0; }
    // Checking if more than 0 of the routes where taken 
    if (!hasQual && !hasQnum && !hasSus)
        error("Invalid description: at least one of qual, qnum, or sus expected");
}

char qual(){
    // qual => "-" | "+" | "o"
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
    printf("%c", temp);
    return temp;
}

void qnum(Chord* chord){
    // qnum => ["^"] num
    char temp = ' '; 
    if (token == '^') {
        temp = token;
        match('^', "^ expected");
    } 
    chord->ext_type = temp;
    int x = num();
    chord->extension = x;
    if (temp == ' ') printf("%d", x);
    else printf("%c%d", temp, x);
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

void sus(Chord* chord){
    // sus => "sus2" | "sus4"
    if(token == 's'){
        match('s', "invalid sus sequence");
        match('u', "invalid sus sequence");
        match('s', "invalid sus sequence");
        switch(token){
            case '2':
                printf("sus2");
                match('2', "2 expected");
                chord->sus = 2;
                break;
            case '4':
                printf("sus4");
                match('4', "4 expected");
                chord->sus = 4;
                break;
            default:
                error("invalid suspended sequence");
        }
    } else error("invalid input as sus");
}

void bass(char *bass){
    // bass => "/" note
    printf("/");
    match('/', "/ expected");
    note(bass);
}
