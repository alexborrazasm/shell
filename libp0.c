#include "libp0.h"
#include "shell.h"

/******************************************************************************/
// authors [-l|-n]
void autName();
void autLogin();

void cmdAuthors(tArgs args, tLists L)
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

    default: // args.len > 2
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
void cmdPid(tArgs args, tLists L)
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
void cmdPpid(tArgs args, tLists L)
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
void getcwdAux(tArgs args);
void chdirAux(tArgs args);

void cmdChdir(tArgs args, tLists L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        getcwdAux(args);
        break;

    case 2:
        chdirAux(args);
        break;

    default: // args.len > 2
        printError(args.array[0], "Invalid arguments");
        break;
    }
}

void getcwdAux(tArgs args)
{
    long max_path_length;

    // Consultar el valor de _PC_PATH_MAX para el directorio actual (tamaño máximo
    // de un path)
    max_path_length = pathconf(".", _PC_PATH_MAX);

    char directorio_actual[max_path_length];
    if (getcwd(directorio_actual, sizeof(directorio_actual)) != NULL)
    {
        printf("Path: \033[1;34m%s\033[0m\n", directorio_actual);
    }
    else
    {
        pPrintError(args.array[0]);
    }
}

void chdirAux(tArgs args)
{
    if (chdir(args.array[1]) == 0)
    {
        printf("Path: \033[1;34m%s\033[0m\n", args.array[1]);
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

void cmdDate(tArgs args, tLists L)
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

    default: // args,len < 2
        printError(args.array[0], "Invalid num of argument");
        break;
    }
}

void date(tArgs args)
{
    int day, month, year;
    time_t now;
    time(&now);
    if (now == -1)
    {
        pPrintError(args.array[0]);
    }

    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf("\033[1;34m%d/%d/%d\033[0m\n", day, month, year);
}

void timeC(tArgs args)
{ // Lo nombro así para que no choque con la función time
    int hours, minutes, seconds;
    time_t now;
    time(&now);

    if (now == -1)
    {
        pPrintError(args.array[0]);
    }

    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf("\033[1;34m%d:%d:%d\033[0m\n", hours, minutes, seconds);
}

/******************************************************************************/
// historic [N|-N]
void printHistoric(tArgs args, tListH historic);
void printHistoricN(tArgs args, tListH historic, int n);
void callHistoric(tArgs args, tLists L, int n);

void cmdHistoric(tArgs args, tLists L)
{
    if (args.len == 1) // Do not have argument
    {
        printHistoric(args, L.historic);
        return;
    }
    else if (args.len == 2) // Have 1 argument
    {
        if (args.array[1][0] != '-') // historic N
        {
            callHistoric(args, L, atoi(&args.array[1][0]));
            return;
        }
        else // historic -N
        {
            if (strlen(args.array[1]) > 1)
            {
                printHistoricN(args, L.historic, atoi(&args.array[1][1]));
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

        while (p != LNULL)
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

    if (n > 0) // atoi("letter") return 0
    {
        if (!isEmptyListH(historic))
        {
            // Search -N command
            p = lastH(historic);

            for (; i < n; i++)
            {
                if (p != LNULL)
                    p = previousH(p, historic);
                else // Loop list
                {
                    p = firstH(historic);
                    break;
                }
            }
            // Print
            for (; p != LNULL; p = nextH(p, historic))
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

void callHistoric(tArgs args, tLists L, int n)
{
    if (!isEmptyListH(L.historic))
    {
        tPosH p = firstH(L.historic);
        tItemH item = getItemH(p, L.historic);

        // Search command by historic number
        for (; p != LNULL; p = nextH(p, L.historic))
        { // can't call actual cmd command
            if (item.n == n)
                break;

            item = getItemH(p, L.historic);
        }

        if (p == LNULL) // Not found command
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
void Cmd_open(tArgs args, tListF LF);
void printItemF(tItemF item);

void cmdOpen(tArgs args, tLists L)
{
    switch (args.len)
    {
    case 1:
        openList(L.files);
        break;
    case 2:
        Cmd_open(args, L.files);
        break;
    case 3:
        Cmd_open(args, L.files);
        break;
    default:
        printError(args.array[0], "Invalid argument");
        break;
    }
}

void openList(tListF L)
{
    if (!isEmptyListF(L))
    {
        tPosF p; tItemF item;

        for (p = firstF(L); p != LNULL; p = nextF(p, L))
        {
            item = getItemF(p, L);
            printItemF(item);
        }
    }
}

void Cmd_open(tArgs args, tListF LF)
{
    int i, df, mode = 0;
    char *modeStr;
    tItemF I;
    tPosF p;

    for (i = 2; args.array[i] != NULL; i++)
    {
        if (!strcmp(args.array[i], "cr"))
        {
            mode |= O_CREAT;
            modeStr = "O_CREATE";
        }

        else if (!strcmp(args.array[i], "ex"))
        {
            mode |= O_EXCL;
            modeStr = "O_EXCL";
        }

        else if (!strcmp(args.array[i], "ro"))
        {
            mode |= O_RDONLY;
            modeStr = "O_RDONLY";
        }

        else if (!strcmp(args.array[i], "wo"))
        {
            mode |= O_WRONLY;
            modeStr = "O_WRONLY";
        }

        else if (!strcmp(args.array[i], "rw"))
        {
            mode |= O_RDWR;
            modeStr = "O_RDWR";
        }

        else if (!strcmp(args.array[i], "ap"))
        {
            mode |= O_APPEND;
            modeStr = "O_APPEND";
        }

        else if (!strcmp(args.array[i], "tr"))
        {
            mode |= O_TRUNC;
            modeStr = "O_TRUNC";
        }
        else
            break;
    }

    if ((df = open(args.array[1], mode, 0777)) == -1)
        perror("Imposible abrir fichero"); // cambiar este perror
    else
    {
        I.df = df;
        strcpy(I.info, args.array[1]);
        strcpy(I.mode, modeStr);

        p = firstF(LF);
        for (int j=0; j<df;j++ ){
            p=nextF(p, LF);
        }

        insertItemF(I, p, &LF);
        printf ("Anadida entrada %d a la tabla ficheros abiertos\n",df);
    }
}

void printItemF(tItemF item) {
    printf("Descriptor de archivo: %d\n", item.df);
    printf("Info: %s\n", item.info);
    printf("Modo: %s\n", item.mode);
    printf("----------------------------\n");
}

/******************************************************************************/
// close [df]

/******************************************************************************/
// dup [df]

/******************************************************************************/
// infosys
void infosysAux();

void cmdInfosys(tArgs args, tLists L)
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
        printf("Nombre del sistema operativo:\t\033[1;34m%s\033[0m\n"
               "Nombre del nodo (host):\t\t\033[1;34m%s\033[0m\n"
               "Versión del sistema operativo:\t\033[1;34m%s\033[0m\n"
               "Información de la versión:\t\033[1;34m%s\033[0m\n"
               "Tipo de hardware (arquitectura):\033[1;34m%s\033[0m\n",
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

void cmdHelp(tArgs args, tLists L)
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

/* examples

void Cmd_open (char * tr[])
{
    int i,df, mode=0;

    if (tr[0]==NULL) { //no hay parametro
       ..............ListarFicherosAbiertos...............
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
      if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
      else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
      else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
      else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
      else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
      else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
      else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
      else break;

    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        ...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf ("Anadida entrada a la tabla ficheros abiertos..................",......);
}

void Cmd_close (char *tr[])
{
    int df;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { //no hay parametro
      ..............ListarFicherosAbiertos............... //o el descriptor es menor que 0
        return;
    }


    if (close(df)==-1)
        perror("Inposible cerrar descriptor");
    else
       ........EliminarDeFicherosAbiertos......
}

void Cmd_dup (char * tr[])
{
    int df, duplicado;
    char aux[MAXNAME],*p;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { //no hay parametro
        ListOpenFiles(-1);                 //o el descriptor es menor que 0
        return;
    }

    duplicado=dup(df);
    p=.....NombreFicheroDescriptor(df).......;
    sprintf (aux,"dup %d (%s)",df, p);
    .......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
}

*/