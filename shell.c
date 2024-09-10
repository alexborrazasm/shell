#include "shell.h"

void printPrompt() 
{
    printf("$ -> ");
}

void readInput() 
{

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