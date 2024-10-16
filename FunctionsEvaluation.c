
/*
// Declaring the functions used for the parser
void input();
void song();
void bar();
void meter();
void numerator();
void denominator();
void chords();
void chord();
void root();
void note();
void letter();
void acc();
void description();
void qual();
void qnum();
void num();
void sus();
void bass();

// Function in charge of token handling
void getToken();
void match(char c, char* message);
void error(char* message);
int isBar(char c);

// Defining the functions used for the parser

// Matching the token to c and fetching next token 
void match(char c, char *message) {
    if (token == c) getToken();
    else error(message);
}

// Exit the parsing whenever you find an error
void error(char* message) {
    printf("Parse error: %s\n", message);
    exit(1);
}

char 
int isBar(char c) {
    if ()
}
// Function in charge of 
void input(){
    song();
    match(EOF, "EOF expected\n");
}

void song(){
    bar();
    while (isBar(token)) 
        bar();
}

*/