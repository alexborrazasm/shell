#include "shell.h"

int getInput(char* input);

void printPrompt() 
{
    printf("\033[1;32m$ \033[0m");
}

bool readInput(tLists *L) 
{
    char input[MAX_BUFFER_INPUT]; int len; tItemH item;

    len = getInput(input);

    if (len > 1)
    {
        // Copy console input to history item
        strcpy(item.command, input);

        if (isEmptyList(L->historic)) // The first element on historic
        {
            item.n = 1;
        }
        else
        {
            tPosH p = last(L->historic);
            tItemH last = getItem(p, L->historic);

            item.n = last.n + 1;
        }

        if (!insertItem(item, LNULL, &L->historic)) 
        {
            puts("Error: historic inset");
            return false;
        }
            return true;
    }       
    return false;
}

int getInput(char* input)
{
    int len;

    // Get new console line
    if (fgets(input, MAX_BUFFER_INPUT, stdin) == NULL)
        perror("Error reading buffer\n");

    len = strlen(input);
    
    if (len > 0 && input[len - 1] == '\n') 
    {
        input[len - 1] = '\0';  // Remove '\n'
    }
    return len;
}

void processInput(tLists L, bool *end)
{
    // string name , void name
    tCommand commands[] = 
    {
        {"date", cmdDate},
        {"authors", cmdAuthors},
        {"pid", cmdPid},
        {"ppid", cmdPpid},
        {"historic", cmdHistoric},
        {"chdir", cmdChdir},
        {"infosys", cmdInfosys},
    };

    if (!isEmptyList(L.historic)) 
    {
        tArgs args;
        tItemH input = getItem(last(L.historic), L.historic);
        
        args.len = stringCut(input.command, args.array);

        const int nCommands = sizeof(commands) / sizeof(commands[0]);
        
        if (args.len > 0) 
        {
            for (int i = 0; i < nCommands; ++i) 
            {
                if (strcmp(commands[i].name, args.array[0]) == 0) 
                {
                    commands[i].func(args, L);
                    return;
                }
            }

            if (strcmp("bye",  args.array[0]) == 0 ||
                strcmp("exit", args.array[0]) == 0 ||
                strcmp("quit", args.array[0]) == 0 ) 
            {
                cmdExit(args, end);
                return;
            }

            printf("\033[1;31mshell: %s: command not found...\033[0m\n", 
                    input.command);
        }
    }
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