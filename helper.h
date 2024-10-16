int isAcc(char c){
    return (c == '#' || c == 'b');
}
int isLetter(char c){
    return (c >= 'A' && c <= 'G');
}
int isQual(char c){

}
int isQnum(char c){
    return (token == 'ˆ' || isNum(c));
}
int isNum(char c) {
    return (token == '7' || token == '9' || token == '1' || token == '3' );
}
int isSus(char c){

}