#include "inputUtil.h"

int isFloat(std::string str){
    std::string::iterator ch = str.begin();
    while(ch != str.end()){
        if (isdigit(*ch) || *ch == '.' || *ch == '-') {
            ch++; // increment to next character in word
        }
        else { // if a non-numerical character is present
            return 0;
        }
    }
    return 1;
}

int isInt(std::string str){
    std::string::iterator ch = str.begin();
    while(ch != str.end()){
        if (isdigit(*ch) || *ch == '-') {
            ch++;
        }
        else {
            return 0;
        }
    }
    return 1;
}