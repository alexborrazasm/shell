#include "shell.h"
#include "historyList.h"

void shell();

int main() {
    
    shell();

    return 0;
}

void shell() {
    bool finished = false;
    tList history;

    createEmptyList(&history);

    while (!finished) {
        printPrompt();
        readInput(&history);
        finished = processInput();
    }
}