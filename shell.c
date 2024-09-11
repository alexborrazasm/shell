#include "shell.h"

void printPrompt() 
{
    printf("$ ");
}

void readInput(char* input, char* args[], int *nArgs, tListH *history)
{
    tItemH item; int len;

    // Get new console line
    fgets(input, MAX_BUFFER_INPUT, stdin);

    len = strlen(input);
    
    if (len > 0 && input[len - 1] == '\n') 
    {
    input[len - 1] = '\0';  // Remove '\n'
    }

    // Copy console input to history item
    strcpy(item.command, input);
    // Insert it
    insertItem(item, LNULL, history);

    // Get command arguments
    *nArgs = stringCut(input, args);
}

bool processInput(char* args[], int nArgs, tListH history) 
{
    // string name , void name
    tCommand commands[] = {
    {"Date", dateCmd}
    };

    const int nCommands = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < nCommands; ++i) 
    {
        if (strcmp(commands[i].name, args[0]) == 0) 
        {
            commands[i].func(nArgs, args);
            return false;
        }
    }

    printf("Unknown command: %s\n", args[0]);

    return false;
}

int stringCut(char* input, char* parts[]) 
{
    int i=1;

    if ((parts[0]=strtok(input," \n\t"))==NULL)
    return 0;
    
    while ((parts[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}