#include "shell.h"

void printPrompt() 
{
    printf("\033[1;32m$ \033[0m");
}

void readInput(char* input, tArgs *args, tListH *history)
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
    if (!insertItem(item, LNULL, history))
        puts("Malloc error");
    // Get command arguments
    args->len = stringCut(input, args->array);
}

void processInput(tArgs args, tListH historic, bool *end)
{
    // string name , void name
    tCommand commands[] = {
    {"date", cmdDate},
    {"authors", cmdAuthors},
    {"pid", cmdPid},
    {"ppid", cmdPpid},
    {"historic", cmdHistoric},
    {"chdir", cmdChdir},
    };

    const int nCommands = sizeof(commands) / sizeof(commands[0]);
    
    if (args.len > 0) 
    {
        for (int i = 0; i < nCommands; ++i) 
        {
            if (strcmp(commands[i].name, args.array[0]) == 0) 
            {
                commands[i].func(args, historic);
                return;
            }
        }

        if (strcmp("bye",  args.array[0]) == 0 ||
            strcmp("exit", args.array[0]) == 0 ||
            strcmp("quit", args.array[0]) == 0 ) 
        {
            cmdExit(end);
            return;
        }

        tItemH input = getItem(last(historic), historic);

        printf("\033[1;31mUnknown command: %s\033[0m\n", input.command);
    }
}

int stringCut(char* input, char* parts[]) 
{
    int i=1;

    if ((parts[0]=strtok(input," \n\t"))==NULL)
    return 0;
    
    while ((parts[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}

void freeHistoryList(tListH* list) 
{
    tPosH node;

    while (!isEmptyList(*list))
    {
        node = last(*list);
        deleteAtPosition(node, list);
    }
}