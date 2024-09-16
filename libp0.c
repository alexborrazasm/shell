#include "libp0.h"
#include "shell.h"

/******************************************************************************/
// date [-t|-d]
void date();
void timeC();

void cmdDate (tArgs args, tLists L)
{
    UNUSED(L);

    if (args.len > 2)
    {
        printf("\033[1;31mError: date [-t|-d]\033[0m\n"); 
        return;
    } 
    else 
    {
        switch (args.len)
        {
        case 1:
            date();
            timeC();
            break;

        case 2:
            if (strcmp(args.array[1], "-t") == 0)
            {
                timeC();
            } else if (strcmp(args.array[1], "-d") == 0)
            {
                date();
            } else {
                printf("\033[1;31mError: date [-t|-d]\033[0m\n");
            }

            break;
        
        default:
            break;
        }
    }  
}

void date()
{
    int day, month, year;
    time_t now;
    time(&now);
    if (now == -1){
        perror("\033[1;31mError al obtener la hora\033[0m");
    }
    

    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf("Hoy es %d/%d/%d\n", day, month, year);
}

void timeC()
{   //Lo nombro así para que no choque con la función time
    int hours, minutes, seconds;
    time_t now;
    time(&now);

    if (now == -1){
        perror("\033[1;31mError al obtener la fecha\033[0m");
    }

    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf("Son las: %d:%d:%d\n", hours, minutes, seconds);
}

/******************************************************************************/
// authors [-l|-n]
void autName();
void autLogin();

void cmdAuthors (tArgs args, tLists L)
{
    UNUSED(L);

    if (args.len > 2)
    {
        printf("\033[1;31mError: authors [-l|-n]\033[0m\n");
        return;
    } else {
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
            } else if (strcmp(args.array[1], "-n") == 0)
            {
                autName();
            } else {
                printf("\033[1;31mError: authors [-l|-n]\033[0m\n");
            }
            break;
        
        default:
            break;
        }
    }
      
}

void autName(){
    printf("José Manuel Villar \t Alex Borrazás\n");
}

void autLogin(){
    printf("jose.villarg@udc.es \t alexandre.bmancebo\n");
}

/******************************************************************************/
// pid
void cmdPid(tArgs args, tLists L)
{
    UNUSED(L);

    if (args.len != 1){
        printf("\033[1;31mError: pid\033[0m\n");
    } else {
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

    if (args.len != 1){
        printf("\033[1;31mError: ppid\033[0m\n"); 
    } else {
        int ppid;
        ppid = getppid();
        printf("%d\n", ppid);
    }
}

/******************************************************************************/
// Quit Bye Exit

void cmdExit(tArgs args, bool *end) 
{   
    if (args.len != 1)
        printf("\033[1;31mError: %s\033[0m\n", args.array[0]);
    else
        *end = true;
}

/******************************************************************************/
// historic [N|-N]
void printHistoric(tListH historic);
void printHistoricN(tListH historic, int n);
void callHistoric(tLists L, int n);

void cmdHistoric(tArgs args, tLists L)
{
    if (args.len == 1) // Do not have argument
    {
        printHistoric(L.historic);
        return;
    }
    else if (args.len == 2) // Have 1 argument
    {
        if (args.array[1][0] != '-') // historic N
        {
            callHistoric(L, atoi(&args.array[1][0]));
            return;
        }
        else // historic -N
        {
            if (strlen(args.array[1]) > 1) 
            {
                printHistoricN(L.historic, atoi(&args.array[1][1]));
                return;
            }
        }
    }
    // Remaining cases
    printf("\033[1;31mError: %s: Invalid arguments\033[0m/n", 
                args.array[0]);
}

void printHistoric(tListH historic)
{
    tItemH item; int acc = 1;

    if (!isEmptyList(historic))
    {
        tPosH p = first(historic);

        while (p != LNULL)
        {   
            item = getItem(p, historic);
            printf("%d  %s\n", item.n, item.command);

            acc++;
            p = next(p, historic);
        }
    }
    else
    {
        puts("\033[1;31mError: historic: empty list\033[0m");
    }
}

void printHistoricN(tListH historic, int n)
{
    tItemH item; tPosH p; int i = 1;

    if (n > 0)  // atoi("letter") return 0 
    {
        if (!isEmptyList(historic))
        {
            // Search -N command
            p = last(historic);
            
            for (; i < n; i++)
            {   
                if (p != LNULL)
                    p = previous(p, historic);
                else // Loop list
                {
                    p = first(historic);
                    break;
                }
            }
            // Print
            for(; p != LNULL; p = next(p, historic)) 
            {
                item = getItem(p, historic);
                printf("%d  %s\n", item.n, item.command);
            }
        }
    }
    else
    {
        puts("\033[1;31mError: historic: Invalid argument\033[0m");
    }
}

void callHistoric(tLists L, int n)
{
    if (!isEmptyList(L.historic))
    {
        tPosH p = first(L.historic);
        tItemH item = getItem(p, L.historic);

        // Search command by historic number
        for (; p != LNULL; p = next(p, L.historic))
        {   // can't call actual cmd command
            if (item.n == n)
                break;

            item = getItem(p, L.historic);
        }

        if (p == LNULL) // Not found command
        {
            printf("Not found command number %d in historic\n", n);
            return;
        }
        
        // Call command
        tArgs args;

        args.len = stringCut(item.command, args.array);

        // Avoid looping reclusion
        if (strcmp(args.array[0], "historic") == 0) 
        {
            if (args.len == 2)  // have 1 argument
            {
                if (args.array[0][0] != '-') // not historic -N is historic N
                {
                    puts("You can't call the historical with the historical");
                    return;
                }
            } 
        }
        // All right
        selectCommand(args, item.command, L, false);
    }
}

/******************************************************************************/
// chdir [path]

void getcwdAux();
void chdirAux(char *path);

void cmdChdir (tArgs args, tLists L)
{
    UNUSED(L);

    if (args.len > 2)
    {
        printf("\033[1;31mError: chdir [path]\033[0m\n");
        return;
    } else {
        switch (args.len)
        {
        case 1:
            getcwdAux();
            break;

        case 2:
            chdirAux(args.array[1]);
            break;
        
        default:
            break;
        }
    }
}

void getcwdAux()
{
    long max_path_length;

    // Consultar el valor de _PC_PATH_MAX para el directorio actual (tamaño máximo
    //de un path)
    max_path_length = pathconf(".", _PC_PATH_MAX);

    char directorio_actual[max_path_length];
    if (getcwd(directorio_actual, sizeof(directorio_actual)) != NULL){
        printf("Directorio actual \033[1;34m%s\033[0m\n", directorio_actual);
    } else {
        perror("\033[1;31mError al mostrar el directorio actual\033[0m");
    }
}

void chdirAux(char *path)
{
    if (chdir(path)==0){
        printf("Directorio actual \033[1;34m%s\033[0m\n", path);
    } else {
        fprintf(stderr,"\033[1;31mError al cambiar el directorio: %s\033[0m\n",
                strerror(errno));
    }
}

/******************************************************************************/
// infosys
void infosysAux();

void cmdInfosys (tArgs args, tLists L)
{
    UNUSED(L);

    if (args.len != 1)
    {
        printf("\033[1;31mError: infosys\033[0m\n");
    } else {
        infosysAux();
    }
}

void infosysAux()
{
    struct utsname info_sistema;    //Estructura que guarda info del sistema ;)
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
    } else {
        perror("\033[1;31mError al obtener información del sistema\033[0m\n");
    }
}


/*

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