int isValidDenominator(int value, int* validValues, int size) {
    for (int i = 0; i < size; i++) {
        if (value == validValues[i]) {
            return 1;  // Valid denominator found
        }
    }
    return 0; 
}