#include "shell.h"

// string name , void name
tCommand commands[] = 
{
    {"date", cmdDate, "help"},
    {"authors", cmdAuthors, "help"},
    {"pid", cmdPid, "help"},
    {"ppid", cmdPpid, "help"},
    {"historic", cmdHistoric, "help"},
    {"cd", cmdChdir, "help"},
    {"infosys", cmdInfosys, "help"},
};


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
    int len, tabs;

    // Get new console line
    if (fgets(input, MAX_BUFFER_INPUT, stdin) == NULL)
        perror("Error reading buffer\n");

    // Remove initial tabs
    tabs = strspn(input, " \t");
    if (tabs > 0) 
    {
        // Shift the remaining characters to the left to overwrite the tabs
        memmove(input, input + tabs, strlen(input) - tabs + 1);
    }

    len = strlen(input);
    
    if (len > 0 && input[len - 1] == '\n') 
    {
        input[len - 1] = '\0';  // Remove '\n'
    }
    return len;
}

void processInput(tLists L, bool *end)
{
    if (!isEmptyList(L.historic)) 
    {
        tItemH item = getItem(last(L.historic), L.historic);
        tArgs args;
        args.len = stringCut(item.command, args.array);

        selectCommand(args, item.command, L, end);
    }
}

void selectCommand(tArgs args, char* input, tLists L, bool *end) 
{
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
                input);
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

int stringCut(char* input, char* parts[]) 
{
    int i=1;

    if ((parts[0]=strtok(input," \n\t"))==NULL)
    return 0;
    
    while ((parts[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}