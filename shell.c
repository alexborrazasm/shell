#include "shell.h"
#include "historyList.h"

#define MAX_BUFFER_INPUT 50

void printPrompt() 
{
    printf("$ -> ");
}

void readInput(tListH *history) 
{
    int numArgs = 0;
    char input[MAX_BUFFER_INPUT];

        printf("$ ");
        fgets(input, MAX_BUFFER_INPUT, stdin);

        numArgs = stringCut(input, numArgs);
}

bool processInput() 
{
    return true;
}

int stringCut(char* input, char* parts[]) 
{
    int i=1;

    if ((parts[0]=strtok(input," \n\t"))==NULL)
    return 0;
    
    while ((parts[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
    return i;
}