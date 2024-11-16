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
    {"listdir", cmdListdir, "listdir [-hid][-long][-link][-acc] n1 n2 ..",
     "List directory contents"
     "\n\t-hid: includes hidden files"
     "\n\t-long: long listing\n\t-hid: include hidden files"
	 "\n\t-acc: acesstime"
     "\n\t-link: if it is a symbolic link, the path content"},
    {"listfile", cmdListfile, "listfile [-long][-link][-acc] name1 name2 ..",
     "Lists files"
     "\n\t-long: long list"
     "\n\t-acc: acesstime"
	 "\n\t-link: if it is a symbolic link, the path contained"},
    {"makefile", cmdMakefile, "makefile [name]", "Creates a file"},
    {"makedir", cmdMakedir, "makedir [name]", "creates a directory"},
    {"reclist", cmdReclist, "reclist [-hid][-long][-link][-acc] n1 n2 ..",
     "Recursively list contents of directories (subdirs after)"
     "Lists files"
     "\n\t-hid: includes hidden files"
     "\n\t-long: long list"
     "\n\t-acc: acesstime"
	 "\n\t-link: if it is a symbolic link, the path contained\n"},
    {"revlist", cmdRevlist, "revlist [-hid][-long][-link][-acc] n1 n2 ..",
     "Recursively list contents of directories (subdirs before)"
     "Lists files"
     "\n\t-hid: includes hidden files"
     "\n\t-long: long list"
     "\n\t-acc: acesstime"
	 "\n\t-link: if it is a symbolic link, the path contained\n"},
    {"erase", cmdErase, "erase [name1 name2 ..]",
     "Delete empty files or directories"},
    {"delrec", cmdDelrec, "delrec [name1 name2 ..]",
     "Delete files or non empty directories recursively"},
    {"allocate", cmdAllocate, "allocate [-malloc|-shared|-createshared|-mmap]..",
     "Allocates a block of memory"
     "\n\t-malloc n: allocates a block of malloc memory of size n bytes"
     "\n\t-createshared cl n: creates a block of shared memory with key cl and size n"
     "\n\t-shared cl: allocates the (already existing) shared memory block of key cl"
     "\n\t-mmap file perm: maps a fille to memory with permissions perm."},
    {"deallocate", cmdDeallocate, "deallocate [-malloc|-shared|-delkey|-mmap|addr]..",
     "Deallocates a block of memory"
     "\n\t-malloc n: deallocates a block of malloc memory of size n bytes"
     "\n\t-shared cl: deallocates the (already existing) shared memory block of key cl"
     "\n\t-delkey cl: removes from the system (without unmapping) the memory key cl"
     "\n\t-mmap f: unmaps the mapped file f"
     "\n\t-addr: deallocates the memory block at address addr"},
    {"memfill", cmdMemfill, "memfill addr cont byte",
     "Fill memory starting from addr with byte"},
    {"memdump", cmdMemdump, "memdump addr cont", 
     "Dumps the contents of cont bytes of memory at address addr to the screen"},
    {"memory", cmdMemory, "memory [-blocks|-funcs|-vars|-all|-pmap] ..",
     "Show details of the process memory"
     "\n\t-blocks: prints the list of allocated blocks"
     "\n\t-funcs: the addresses of the functions"
     "\n\t-vars: the addresses of the variables"
     "\n\t-all: prints all of the above (-funcs, -vars and -blocks)"
     "\n\t-pmap: shows the output of the command pmap for the shell process"},
    {"readfile", cmdReadfile, "readfile file addr cont",
     "Reads cont bytes of a file into memory address addr"},
    {"writefile", cmdWritefile, "writefile [-o] file addr cont", 
     "Writes to a file cont bytes starting at memory address addr (-o overwrite)"},
    {"read", cmdRead, "read df addr cont",
     "Transfer cont bytes of the file described by df to address addr"},
    {"write", cmdWrite, "write df addr cont",
     "The same as writefile but we use a (already opened) file descriptor"},
    {"recurse", cmdRecurse, "recurse n",
     "Transfer cont bytes from address addr to the file described by df"}
};

int getInput(char *input);

void cmdClear();

void printPrompt()
{
    printf(GREEN"$ "RST);
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

        if (strcmp("clear", args.array[0]) == 0)
        {
            cmdClear();
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

void freeMenList(tListM *list)
{
   tPosM p;

    while (!isEmptyListM(*list)) {
        p = lastM(*list); 
        deleteAtPositionM(p, list);
    }
}

void freeLists(tLists *L)
{
    freeHistoryList(&L->historic);
    freeFileList(&L->files);
    freeMenList(&L->memory);
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
    fprintf(stderr, RED"Error: %s: %s\n"RST, name, msg);
}

void pPrintError(char *name)
{
    fprintf(stderr, RED"Error: %s: %s\n"RST, name, strerror(errno));
}

void pPrintErrorFile(char *name, char *filename)
{
    fprintf(stderr, "Error: %s: %s: %s\n"RST,
     name, filename, strerror(errno));
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

    insertItemF(input, L);
    insertItemF(output, L);
    insertItemF(error, L);

    strcpy(empty.info, "unused");
    empty.mode = MODE_NULL;

    for (int i = 3; i <= 19; i++)
    {
        empty.df = i;
        insertItemF(empty, L);
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

void cmdClear()
{
    pid_t pid;       
    char *argv[4]={"clear", NULL}; // clear don't need any argument

    if ((pid=fork())==-1)
    {
        pPrintError("Can't create process");
        return;
    }

    if (pid==0)
    {
        if (execvp(argv[0], argv)==-1)
        pPrintError("Cannot execute clear");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}
