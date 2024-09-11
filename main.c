#include "shell.h"
#include "p0lib.h"

void shell();

tCommand getCommands();

int main() {
    
    shell();

    return 0;
}

void shell() {
    bool finished = false;
    tListH history; int nArgs = 0;
    char input[MAX_BUFFER_INPUT]; char* args[MAX_ARGS];

    // History list
    createEmptyList(&history);

    while (!finished) {
        printPrompt();
        readInput(input, args, &nArgs, &history);
        finished = processInput(args, nArgs, history);
    }
}

// falta free del histórico