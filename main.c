#include<stdbool.h>

void shell();1

int main() {

    shell();

    return 0;
}

void shell() {
    bool finished = false;

    while (!finished) {
        printPrompt();
        readInput();
        finished = processInput();
    }
    
}