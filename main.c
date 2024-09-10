#include "shell.h"

void shell();

int main() {
    
    shell();

    return 0;
}

void shell() {
    bool finished = false;
    tListH history; int nArgs = 0;
    char input[MAX_BUFFER_INPUT]; char* args[MAX_ARGS];
    
    createEmptyList(&history);

    while (!finished) {
        printPrompt();
        readInput(input, args, &nArgs, &history);
        finished = processInput();

        if (!isEmptyList(history)) {
            tItemH item = getItem(last(history), history);

            printf("%s\n",item.command);
        }
    }
}

// falta free del histórico