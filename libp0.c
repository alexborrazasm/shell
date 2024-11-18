#include "libp0.h"

/******************************************************************************/
// authors [-l|-n]
void autName();
void autLogin();

void cmdAuthors(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        autName();
        autLogin();
        break;

    case 2:
        if (strcmp(args.array[1], "-l") == 0)
        {
            autLogin();
        }
        else if (strcmp(args.array[1], "-n") == 0)
        {
            autName();
        }
        else
        {
            printError(args.array[0], "Invalid argument");
        }
        break;

    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void autName()
{
    printf("José Manuel Villar \t Alex Borrazás\n");
}

void autLogin()
{
    printf("jose.villarg@udc.es \t alexandre.bmancebo\n");
}

/******************************************************************************/
// pid
void cmdPid(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len != 1)
    {
        printError(args.array[0], "Invalid argument");
    }
    else
    {
        int pid;
        pid = getpid();
        printf("%d\n", pid);
    }
}

/******************************************************************************/
// ppid
void cmdPpid(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len != 1)
    {
        printError(args.array[0], "Invalid argument");
    }
    else
    {
        int ppid;
        ppid = getppid();
        printf("%d\n", ppid);
    }
}

/******************************************************************************/
// cd [path]
void getcwdAux(char *path);

void chdirAux(tArgs args);

void cmdCd(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        getcwdAux(args.array[1]);
        break;

    case 2:
        chdirAux(args);
        break;

    default: // args.len > 2
        printError(args.array[0], "Invalid arguments");
        break;
    }
}

void getcwdAux(char *path)
{
    long max_path_length;

    // Consultar el valor de _PC_PATH_MAX para el directorio actual (tamaño máximo
    // de un path)
    max_path_length = pathconf(".", _PC_PATH_MAX);

    char currentDir[max_path_length];
    if (getcwd(currentDir, sizeof(currentDir)) != NULL)
    {
        printf(BLUE"%s\n"RST, currentDir);
    }
    else
    {
        pPrintError(path);
    }
}

void chdirAux(tArgs args)
{
    if (chdir(args.array[1]) == 0)
    {
        printf(BLUE"%s\n"RST, args.array[1]);
    }
    else
    {
        pPrintError(args.array[0]);
    }
}

/******************************************************************************/
// date [-t|-d]
void date(tArgs args);
void timeC(tArgs args);

void cmdDate(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        timeC(args);
        date(args);
        break;

    case 2:
        if (strcmp(args.array[1], "-t") == 0)
        {
            timeC(args);
        }
        else if (strcmp(args.array[1], "-d") == 0)
        {
            date(args);
        }
        else
        {
            printError(args.array[0], "Invalid argument");
        }
        break;

    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void date(tArgs args)
{
    time_t now = getTime(args);

    if (now == -1)
        return;

    printDate(now);
    puts("");
}

void timeC(tArgs args)
{
    time_t now = getTime(args);
    
    if (now == -1)
        return;

    printTime(now);
    puts("");
}

/******************************************************************************/
// historic [N|-N]
void printHistoric(tArgs args, tListH historic);
void printHistoricN(tArgs args, tListH historic, int n);
void callHistoric(tArgs args, tLists *L, int n);

void cmdHistoric(tArgs args, tLists *L)
{
    if (args.len == 1) // Do not have argument
    {
        printHistoric(args, L->historic);
        return;
    }
    else if (args.len == 2) // Have 1 argument
    {
        if (args.array[1][0] != '-') // historic N
        {
            int n;
            if(stringToInt(args.array[1], &n))
                callHistoric(args, L, n);
            else 
                printError(args.array[0], "Invalid argument");
            return;
        }
        else // historic -N
        {
            if (strlen(args.array[1]) > 1)
            {
                int n;
                if(stringToInt(&args.array[1][1], &n))
                    printHistoricN(args, L->historic, n);
                else
                    printError(args.array[0], "Invalid argument");
                return;
            }
        }
    }
    // Remaining cases
    printError(args.array[0], "Invalid argument");
}

void printHistoric(tArgs args, tListH historic)
{
    tItemH item;
    int acc = 1;

    if (!isEmptyListH(historic))
    {
        tPosH p = firstH(historic);

        while (p != HNULL)
        {
            item = getItemH(p, historic);
            printf("%d  %s\n", item.n, item.command);

            acc++;
            p = nextH(p, historic);
        }
    }
    else
    {
        printError(args.array[0], "empty list");
    }
}

void printHistoricN(tArgs args, tListH historic, int n)
{
    tItemH item;
    tPosH p;
    int i = 1;

    if (n > 0)
    {
        if (!isEmptyListH(historic))
        {
            // Search -N command
            p = lastH(historic);

            for (; i < n; i++)
            {
                if (p != HNULL)
                    p = previousH(p, historic);
                else // Loop list
                {
                    p = firstH(historic);
                    break;
                }
            }
            // Print
            for (; p != HNULL; p = nextH(p, historic))
            {
                item = getItemH(p, historic);
                printf("%d  %s\n", item.n, item.command);
            }
        }
        else
        {
            printError(args.array[0], "empty list");
        }
    }
    else
    {
        printError(args.array[0], "Invalid argument");
    }
}

void callHistoric(tArgs args, tLists *L, int n)
{
    if (!isEmptyListH(L->historic))
    {
        tPosH p = firstH(L->historic);
        tItemH item = getItemH(p, L->historic);

        // Search command by historic number
        for (; p != HNULL; p = nextH(p, L->historic))
        { // can't call actual cmd command
            if (item.n == n)
                break;

            item = getItemH(p, L->historic);
        }

        if (p == HNULL) // Not found command
        {
            printError(args.array[0], "Command number not found");
            return;
        }

        // Call command
        tArgs args;

        args.len = stringCut(item.command, args.array);

        // Avoid looping reclusion
        if (strcmp(args.array[0], "historic") == 0)
        {
            if (args.len == 2) // have 1 argument
            {
                if (args.array[1][0] != '-') // not historic -N is historic N
                {
                    printError(args.array[0], "Avoid infinite looping");
                    return;
                }
            }
        }
        // All right
        selectCommand(args, item.command, L, false);
    }
}

/******************************************************************************/
// open [file] mode
void openList(tListF L);
void Cmd_open(tArgs args, tListF *L);
void printItemF(tItemF item);
int getMode(tArgs args);
const char* modeToString(int mode);

void cmdOpen(tArgs args, tLists *L)
{
    if (args.len == 1)
    {
        openList(L->files);
    }
    else if (args.len < 1)
    {
        printError(args.array[0], "Invalid argument");
    }
    else
        Cmd_open(args, &L->files);
}

void openList(tListF L)
{
    if (!isEmptyListF(L))
    {
        tPosF p;
        tItemF item;

        for (p = firstF(L); p != FNULL; p = nextF(p, L))
        {
            item = getItemF(p, L);
            printItemF(item);
        }
    }
    else
    {
        printError("open", "filesList is empty");
    }
}

void Cmd_open(tArgs args, tListF *L)
{
    int df, mode;
    tItemF item;
    tPosF p;

    mode = getMode(args);

    if ((df = open(args.array[1], mode, 0777)) == -1)
        pPrintError(args.array[0]);
    else
    {
        item.df = df;
        strcpy(item.info, args.array[1]);
        item.mode = mode;

        p = findItemF(df, *L);

        if (p == FNULL)
        {
            if (!insertItemF(item, L))
            {
                printError(args.array[0], "Can´t insert on filesList");
                return;
            }
        }
        else // p exits
            updateItemF(item, p, L);

        printf("Added %d entry to the open files table\n", df);
    }
}

void printItemF(tItemF item)
{
    if (item.mode == MODE_NULL)
    {
        printf("descriptor: %d -> %s\n", item.df, item.info);
    }
    else if (strcmp("unused", item.info) != 0)
    {
        printf("descriptor: %d -> "BLUE"%s "GREEN"%s\n"RST,
        item.df, item.info, modeToString(item.mode));
    }
    else
        printf("descriptor: %d -> %s\n", item.df, item.info);
}

int getMode(tArgs args)
{
    int mode = 0;

    for (int i = 2; i < args.len; i++)
    {
        if (!strcmp(args.array[i], "cr"))
        {
            mode |= O_CREAT;
        }
        else if (!strcmp(args.array[i], "ex"))
        {
            mode |= O_EXCL;
        }
        else if (!strcmp(args.array[i], "ro"))
        {
            mode |= O_RDONLY;
        }
        else if (!strcmp(args.array[i], "wo"))
        {
            mode |= O_WRONLY;
        }
        else if (!strcmp(args.array[i], "rw"))
        {
            mode |= O_RDWR;
        }
        else if (!strcmp(args.array[i], "ap"))
        {
            mode |= O_APPEND;
        }
        else if (!strcmp(args.array[i], "tr"))
        {
            mode |= O_TRUNC;
        }
        else
            break;
    }
    return mode;
}

const char* modeToString(int mode) {
    static char buffer[64] = {0};

    buffer[0] = '\0';

    if (mode & O_CREAT) {
        strcat(buffer, "O_CREATE ");
    }
    if (mode & O_EXCL) {
        strcat(buffer, "O_EXCL ");
    }
    if (mode & O_RDONLY) {
        strcat(buffer, "O_RDONLY ");
    }
    if (mode & O_WRONLY) {
        strcat(buffer, "O_WRONLY ");
    }
    if (mode & O_RDWR) {
        strcat(buffer, "O_RDWR ");
    }
    if (mode & O_APPEND) {
        strcat(buffer, "O_APPEND ");
    }
    if (mode & O_TRUNC) {
        strcat(buffer, "O_TRUNC ");
    }

    // Remove the trailing space if buffer is not empty
    int len = strlen(buffer);
    if (len > 0) {
        buffer[len - 1] = '\0';
    }

    return buffer;
}


/******************************************************************************/
// close [df]
void auxClose(int df, tArgs args, tListF *L);

void cmdClose(tArgs args, tLists *L)
{
    switch (args.len)
    {
    case 1:
        openList(L->files);
        break;
    case 2:
        int df;

        if (!stringToInt(args.array[1], &df))
        {
            printError(args.array[0], "Invalid argument");
        }
        else if (df < 0)
        {
            openList(L->files);
        }
        else
        {
            auxClose(df, args, &L->files);
        }
        break;
    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void auxClose(int df, tArgs args, tListF *L)
{
    if (close(df) == -1)
    {
        pPrintError(args.array[0]);
    }
    else
    {
        tItemF item; tPosF p;

        p = findItemF(df, *L);
        
        if (p != FNULL)
        {
            if (df <= 19)
            {
                item = getItemF(p, *L);
                strcpy(item.info, "unused");
                item.mode = MODE_NULL;
                updateItemF(item, p, L);
            }
            else 
            {
                deleteAtPositionF(p, L);
            }
        }
        else
        {
            printError(args.array[0], "Bad file descriptor");
        }
    }
}

/******************************************************************************/
// dup [df]
void auxDup(int df, tArgs args, tListF *L);

void cmdDup(tArgs args, tLists *L)
{
    switch (args.len)
    {
    case 1:
        openList(L->files);
        break;
    case 2:
        int df;

        if (!stringToInt(args.array[1], &df))
        {
            printError(args.array[0], "Invalid argument");
        }
        else if (df < 0)
        {
            openList(L->files);
        }
        else
        {
            auxDup(df, args, &L->files);
        }
        break;
    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void auxDup(int df, tArgs args, tListF *L)
{
    tPosF p; char newInfo[MAX_BUFFER_INPUT + 9]; int newDf;

    if ((newDf = dup(df)) == -1)
    {
        pPrintError(args.array[0]);
        return;
    }

    p = findItemF(df, *L);

    if (p != FNULL)
    {
        tItemF item;
        
        item = getItemF(p, *L);
                 
        sprintf(newInfo, "dup %d (%s)", df, item.info);
        
        // Change item to insert
        item.df = newDf;
        strcpy(item.info, newInfo);

        // Search p to insert
        p = findItemF(newDf, *L);

        if (p == FNULL)
        {
            if (!insertItemF(item, L))
            {
                printError(args.array[0], "Can´t insert on filesList");
                return;
            }
        }
        else // p exits
            updateItemF(item, p, L);
    }
    else
    {
        printError(args.array[0], "Bad file descriptor");
    }
}
/******************************************************************************/
// infosys
void infosysAux();

void cmdInfosys(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len != 1)
    {
        printError(args.array[0], "Invalid argument");
    }
    else
    {
        infosysAux(args);
    }
}

void infosysAux(tArgs args)
{
    struct utsname info_sistema; // Estructura que guarda info del sistema ;)
    if (uname(&info_sistema) == 0)
    {
        printf("SO:\t\t"BLUE"%s\n"RST
               "Hostname:\t"BLUE"%s\n"RST
               "Kernel:\t\t"BLUE"%s\n"RST
               "Version:\t"BLUE"%s\n"RST
               "Arch:\t\t"BLUE"%s\n"RST,
               info_sistema.sysname, info_sistema.nodename,
               info_sistema.release, info_sistema.version,
               info_sistema.machine);
    }
    else
    {
        pPrintError(args.array[0]);
    }
}

/******************************************************************************/
// help [cmd]
void help();

void helpCommand(tArgs args);

void cmdHelp(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        help();
        break;
    case 2:
        helpCommand(args);
        break;
    default:
        printError(args.array[0], "Invalid argument");
        break;
    }
}

void help()
{
    const int n = getCommandsLen();
    tCommand commands[n];

    getCommands(commands);

    puts("help [cmd]     Gives a brief help on the usage of command\n\n"
         "List of commands:");

    for (int i = 0; i < n; i++)
    {
        printf("%15s", commands[i].name);
        if ((i + 1) % 5 == 0)
            puts("");
    }
    if (n % 5 != 0)
        puts("");
}

void helpCommand(tArgs args)
{
    const int n = getCommandsLen();
    tCommand commands[n];

    getCommands(commands);

    for (int i = 0; i < n; i++)
    {
        for (int i = 0; i < n; ++i)
        {
            if (strcmp(commands[i].name, args.array[1]) == 0)
            {
                printf("%s    %s\n", commands[i].help0, commands[i].help1);
                return;
            }
        }
    }

    if (strcmp("bye", args.array[1]) == 0 ||
        strcmp("exit", args.array[1]) == 0 ||
        strcmp("quit", args.array[1]) == 0)
    {

        printf("%s    %s\n", args.array[1], "End the shell");
        return;
    }
    printError(args.array[0], "Invalid argument");
}

/******************************************************************************/
// Quit
// Exit
// Bye
void cmdExit(tArgs args, bool *end)
{
    if (args.len != 1)
        printError(args.array[0], "Invalid argument");
    else
        *end = true;
}