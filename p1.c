/*  José Manuel Villar García  jose.villarg@udc.es
    Alex Borrazás Mancebo      alexandre.bmancebo
*/



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
    createEmptyListH(&L.historic);
    createEmptyListF(&L.files);

    initFList(&L.files);

    while (!finished)
    {
        printPrompt();
        if (readInput(&L))
            processInput(&L, &finished);
    }
    freeHistoryList(&L.historic);
    freeFileList(&L.files);
}