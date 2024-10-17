int isValidDenominator(int value, int* validValues, int size) {
    for (int i = 0; i < size; i++) {
        if (value == validValues[i]) {
            return 1;  // Valid denominator found
        }
    }
    return 0; 
}

bool isDescription(char c, char* firstDescription) {
    for (int i = 0; i < 8; i++) 
        if (c == firstDescription[i])
            return 1;
    return 0;
}

bool isAcc(char c) {
    if (c == '#' || c == 'b')
        return 1;
    return 0;
}

bool isBar(char c) {
    static char firstSetBar[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'N', 'C', '%', 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
    for (int i = 0; i < sizeof(firstSetBar) / sizeof(char); i++) 
        if (c == firstSetBar[i])
            return 1;
    return 0;
}