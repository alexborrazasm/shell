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
        processInput(args, history, &finished);
    }
    freeHistoryList(&history);
}

// falta free del histórico