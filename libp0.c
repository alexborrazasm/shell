#include "libp0.h"

int stringCut(char* input, char* parts[]) 
{
    int i=1;

    if ((parts[0]=strtok(input," \n\t"))==NULL)
    return 0;
    
    while ((parts[i]=strtok(NULL," \n\t"))!=NULL) i++;
    return i;
}

/******************************************************************************/
// date [-t|-d]

void date();

void timeC();

void cmdDate (tArgs args, tListH historic){
    UNUSED(historic);
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

void date(){

    int day, month, year;
    time_t now;
    time(&now);
    if (now == -1){
        perror("Error al obtener la hora:");
    }

    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf("Hoy es %d/%d/%d\n", day, month, year);
}

void timeC(){   //Lo nombro así para que no choque con
                //la función time
    int hours, minutes, seconds;
    time_t now;
    time(&now);

    if (now == -1){
        perror("Error al obtener la fecha:");
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

void cmdAuthors (tArgs args, tListH historic){
    UNUSED(historic);
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

void cmdPid(tArgs args, tListH historic){
    UNUSED(historic);
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

void cmdPpid(tArgs args, tListH historic){
    UNUSED(historic);
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

void callHistoric(tListH historic, int n);

void cmdHistoric(tArgs args, tListH historic)  // revisar
{
    if (args.len == 1) // Do not have argument
    {
        printHistoric(historic);
        return;
    }
    else if (args.len == 2) // Have argument
    {
        if (args.array[1][0] != '-') // historic N
        {
            callHistoric(historic, atoi(&args.array[1][0]));
            return;
        }
        else // historic -N
        {
            if (strlen(args.array[1]) == 2) 
            {
                printHistoricN(historic, atoi(&args.array[1][1]));
                return;
            }
        }
    }
    // Remaining cases
    printf("\033[1;31mError: %s: Invalid arguments\033[0m\n", 
                args.array[0]);
}

void printHistoric(tListH historic)
{
    tItemH item; int acc = 1;

    if (!isEmptyList(historic))
    {
        tPosH p = first(historic);

        while (p != last(historic))
        {   
            item = getItem(p, historic);
            printf("%d  %s\n", acc, item.command);

            acc++;
            p = next(p, historic);
        }
    }
    else
    {
        puts("\033[1;31mError: historic: empty list\033[0m");
    }
}

void printHistoricN(tListH historic, int n) // mal
{
    UNUSED(historic);
    UNUSED(n);
}

void callHistoric(tListH historic, int n) // cooking
{
    UNUSED(historic);
    UNUSED(n);
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