#include "shell.h"

void shell();

int main() 
{
    shell();

    return 0;
}

void shell() 
{
    bool finished = false;
    tListH history;
    char input[MAX_BUFFER_INPUT]; 
    tArgs args; args.len = 0;

    // History list
    createEmptyList(&history);

    while (!finished) 
    {
        printPrompt();
        readInput(input, &args, &history);
        finished = processInput(args, history);
    }
}

// falta free del histórico