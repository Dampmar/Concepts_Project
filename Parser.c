#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

FILE *inputFile;
char token;

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

// Main function
int main(void){
    // Use entire filepaths for this section
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
    song();
    match(EOF, "EOF expected");
}

void song(){
    // song -> bar {bar} '|'
    // The process of getting bars in a song ends whenever the token after a bar is '|'
    do {
        bar();
    } while (token != '|');
    printf("%c", token);
    match('|', "| expected");
}

void bar(){
    // bar -> [meter] chords '|'
    if (isdigit(token))
        meter();
    chords();
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

void chords(){
    // chords => "NC" | "%" | chord {chord}
    if (token == 'N') {
        match('N', "N expected");
        match('C', "C expected");
        printf("NC");
    } else if (token == '%') {
        match('%', "% expected");
        printf("%%");
    } else {
        do { chord(); } while (token != '|');
    } 
}

void chord(){
    // chord => root [description] [bass]
    root();
    if (token == 's' || token == '-' || token == '+' || token == 'o'|| 
        token == '7' || token == '9' || token == '1' || token == '^')
        description();
    if (token == '/')
        bass();
}

void root(){
    // root => note
    note();
}

void note(){
    // note => letter [acc]
    letter();
    if (token == 'b' || token == '#') 
        acc();
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

void description(){
    // description => qual | qual qnum | qnum | qnum sus | sus 
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

void qnum(){
    // qnum => ["^"] num
    char temp = ' '; 
    if (token == '^') {
        temp = token;
        match('^', "^ expected");
    }
    int x = num();
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

void sus(){
    // sus => "sus2" | "sus4"
    if(token == 's'){
        match('s', "invalid sus sequence");
        match('u', "invalid sus sequence");
        match('s', "invalid sus sequence");
        switch(token){
            case '2':
                printf("sus2");
                match('2', "2 expected");
                break;
            case '4':
                printf("sus4");
                match('4', "4 expected");
                break;
            default:
                error("invalid suspended sequence");
        }
    } else error("invalid input as sus");
}

void bass(){
    // bass => "/" note
    printf("/");
    match('/', "/ expected");
    note();
}
