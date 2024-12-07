/*  José Manuel Villar García  jose.villarg@udc.es
    Alex Borrazás Mancebo      alexandre.bmancebo
*/
#include "shell.h"

void shell(tArgsMain main);

int main(int argc, char *argv[], char *envp[])
{
    tArgsMain main;

    main.argc = argc;
    main.argv = argv;
    main.envp = envp;

    shell(main);

    return 0;
}

void shell(tArgsMain main)
{
    bool finished = false;
    tLists L;

    // Init lists
    createEmptyListH(&L.historic);
    createEmptyListF(&L.files);
    createEmptyListM(&L.memory);
    createEmptyListB(&L.backgroud);
    createEmptyListP(&L.path);

    initFList(&L.files);

    while (!finished)
    {
        printPrompt();
        if (readInput(&L))
            processInput(&L, &finished, main);
    }

    freeLists(&L);
}