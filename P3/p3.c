/*  José Manuel Villar García  jose.villarg@udc.es
    Alex Borrazás Mancebo      alexandre.bmancebo
*/
#include "shell.h"

void shell(tArgsMain main);

void importPath(tListP *L, tArgsMain main);

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
    createEmptyListB(&L.background);
    createEmptyListP(&L.path);

    initFList(&L.files);

    importPath(&L.path, main);
   
    // Shell loop
    while (!finished)
    {
        printPrompt();
        if (readInput(&L))
            processInput(&L, &finished, main);
    }

    freeLists(&L);
}

void importPath(tListP *L, tArgsMain main)
{
    if (main.argv[1] != NULL )
    {
        if ((main.argv[1][0] == '-') && (main.argv[1][1] == 'p'))
        {
            searchPath(L); // Add PATH to shell
            printf(YELLOW"PATH"RST" added\n");
            return;
        }  
    }   
    printf(YELLOW"PATH"RST" not imported\n"
           "Run with "YELLOW"-p"RST" to import\n");
}