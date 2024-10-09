#include "shell.h"

// string name , void name, string help, string help plus
tCommand commands[] =
{
    {"authors", cmdAuthors, "authors [-l|-n]",
     "Prints the names and/or logins of the program authors"},
    {"pid", cmdPid, "pid",
     "Prints the pid of the process executing the shell"},
    {"ppid", cmdPpid, "ppid",
     "Prints the pid of the shell´s parent process"},
    {"cd", cmdCd, "cd [dir]",
     "Changes or print the current working directory of the shell"},
    {"date", cmdDate, "date [-t|-d]",
     "Prints the current date and/or the time."},
    {"historic", cmdHistoric, "historic [N|-N]",
     "Shows the historic of commands executed by this shell.\n\t\t   "
     "-N: Prints only the lastN commands\n\t\t    "
     "N: Repeats command number N"},
    {"open", cmdOpen, "open [file] mode",
     "Opens a file and adds it the to the list of shell open files\n\t\t    "
     "Open without arguments lists the shell open files\nmodes:\n"
     "cr: O_CREAT\tap: O_APPEND\tex: O_EXCL\tro: O_RDONLY\n"
     "rw: O_RDWR\two O_WRONLY\ttr: O_TRUNC"},
    {"close", cmdClose, "close [df]",
     "Closes the df file descriptor and eliminates the corresponding item "
     "from the list"},
    {"dup", cmdDup, "dup [df]",
     "Duplicates the df file descriptor creating the corresponding new "
     "entry on the file list"},
    {"infosys", cmdInfosys, "infosys",
     "Prints information on the machine running the shell"},
    {"help", cmdHelp, "help [cmd]",
     "Gives a brief help on the usage of command"},
    {"cwd", cmdCwd, "cwd [dir]", "Prints current working directory"},
    {"listdir", cmdListdir, "listdir [dir]", "Lists directories contents"},
    {"makefile", cmdMakefile, "makefile [name]", "Creates a file"},
    {"makedir", cmdMakedir, "makedir [name]", "creates a directory"}
};

int getInput(char *input);

void printPrompt()
{
    printf("\033[1;32m$ \033[0m");
}

bool readInput(tLists *L)
{
    char input[MAX_BUFFER_INPUT];
    int len;
    tItemH item;

    len = getInput(input);

    if (len > 1)
    {
        // Copy console input to history item
        strcpy(item.command, input);

        if (isEmptyListH(L->historic)) // The first element on historic
        {
            item.n = 1;
        }
        else
        {
            tPosH p = lastH(L->historic);
            tItemH last = getItemH(p, L->historic);

            item.n = last.n + 1;
        }

        if (!insertItemH(item, HNULL, &L->historic))
        {
            puts("Error: historic inset");
            return false;
        }
        return true;
    }
    return false;
}

int getInput(char *input)
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
        input[len - 1] = '\0'; // Remove '\n'
    }
    return len;
}

void processInput(tLists *L, bool *end)
{
    if (!isEmptyListH(L->historic))
    {
        tItemH item = getItemH(lastH(L->historic), L->historic);
        tArgs args;
        args.len = stringCut(item.command, args.array);

        selectCommand(args, item.command, L, end);
    }
}

void selectCommand(tArgs args, char *input, tLists *L, bool *end)
{
    const int nCommands = getCommandsLen();

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

        if (strcmp("bye", args.array[0]) == 0 ||
            strcmp("exit", args.array[0]) == 0 ||
            strcmp("quit", args.array[0]) == 0)
        {
            cmdExit(args, end);
            return;
        }

        printError(input, "command not found...");
    }
}

void freeHistoryList(tListH *list)
{
    tPosH node;

    while (!isEmptyListH(*list))
    {
        node = lastH(*list);
        deleteAtPositionH(node, list);
    }
}

void freeFileList(tListF *list)
{
   tPosF p;

    while (!isEmptyListF(*list)) {
        p = lastF(*list); 
        deleteAtPositionF(p, list);
    }
}

int stringCut(char *input, char *parts[])
{
    int i = 1;

    if ((parts[0] = strtok(input, " \n\t")) == NULL)
        return 0;

    while ((parts[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

void printError(char *name, char *msg)
{
    fprintf(stderr, "\033[1;31mError: %s: %s\033[0m\n", name, msg);
}

void pPrintError(char *name)
{
    fprintf(stderr, "\033[1;31mError: %s: %s\033[0m\n", name, strerror(errno));
}

int getCommandsLen()
{
    return sizeof(commands) / sizeof(commands[0]);
}

void getCommands(tCommand *copy)
{
    int n = getCommandsLen();

    for (int i = 0; i < n; i++)
    {
        copy[i] = commands[i];
    }
}

void initFList(tListF *L)
{
    tItemF input;
    tItemF output;
    tItemF error;
    tItemF empty;

    input.df=0;
    strcpy(input.info, "standard input");
    input.mode = O_RDWR;

    output.df=1;
    strcpy(output.info, "standard output");
    output.mode = O_RDWR;

    error.df=2;
    strcpy(error.info, "standard error");
    error.mode = O_RDWR;

    insertItemF(input, FNULL, L);
    insertItemF(output, FNULL, L);
    insertItemF(error, FNULL, L);

    strcpy(empty.info, "unused");
    empty.mode = MNULL;

    for (int i = 3; i <= 9; i++)
    {
        empty.df = i;
        insertItemF(empty, FNULL, L);
    }
}

bool stringToInt(char *str, int *n)
{
    char *endptr = NULL;
    // Convert
    *n = strtol(str, &endptr, 10);

    // Error check
    if (*endptr != '\0') {
        return false;
    }

    return true;
}

