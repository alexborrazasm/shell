#include<stdbool.h>

void shell();

int main() {

    shell();

    return 0;
}

void shell() {
    bool finished = false;

    while (!finished) {
        printPrompt();
        readInput();
        processInput();
    }
    
}