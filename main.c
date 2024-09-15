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
    tLists L;

    // History list
    createEmptyList(&L.historic);

    while (!finished) 
    {
        printPrompt();
        if (readInput(&L))
            processInput(L, &finished);
    }
    freeHistoryList(&L.historic);
}