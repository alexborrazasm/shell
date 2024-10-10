#include "libp1.h"
#include "libp0.h"

#define FLAG_LONG  (1 << 0)  // 0001 (1)
#define FLAG_ACC   (1 << 1)  // 0010 (2)
#define FLAG_LINK  (1 << 2)  // 0100 (4)

byte processListFlags(tArgs args, bool itsFlag[MAX_ARGS])
{
    byte flags = 0;  // no flags

    for (int i = 1; i < args.len; i++) 
    {   
        if (args.array[i][0] != '-')
        {
            break;
        } 
        else if (strcmp(args.array[i], "-long") == 0)
        {
            flags |= FLAG_LONG;
            itsFlag[i] = true;
        } else if (strcmp(args.array[i], "-acc") == 0)
        {
            flags |= FLAG_ACC;
            itsFlag[i] = true;
        } else if (strcmp(args.array[i], "-link") == 0)
        {
            flags |= FLAG_LINK;
            itsFlag[i] = true;
        }
    }

    return flags; 
}

/******************************************************************************/
// makefile
void cmdMakefile(tArgs args, tLists *L){
    int df;
    switch (args.len)
    {
    case 1:
        cmdCwd(args, L);
    case 2:
        if ((df = open(args.array[1], O_CREAT, 0755)) != -1) //Also we could use 0777
        {
            close(df);  //Probably it's ok?¿ idk
            printf("El archivo ha sido creado\n"); //modify
        } 
        else 
        {
            printf("No se ha podido crear el archivo o el directorio\n"); //modify
        }
        break;
    
    default:
        printf("error sintaxis"); //modify
        break;
    }
}

/******************************************************************************/
// makedir

// makedir
void makedirAux(char *path);

void cmdMakedir(tArgs args, tLists *L)
{
    switch (args.len)
    {
    case 2:
        makedirAux(args.array[1]);
        break;

    default:
        printf("error sintaxis"); // modify
        break;
    }
}

void makedirAux(char *path)
{
    int df;
    df = open(path, O_CREAT, 0755); // Also we could use 0777
    if (df != -1)
    {
        close(df);                             // Probably it's ok?¿ idk
        printf("El archivo ha sido creado\n"); // placeholder
    }
    else
    {
        printf("No se ha podido crear el archivo o el directorio\n"); // placeholder
    }
}


/******************************************************************************/
// listfile
void auxListfile(tArgs args, int n);

void auxListfileFlags(tArgs args);

void cmdListfile(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len == 1) // Print actual working directory
        cmdCd(args, L);
    else if (args.len > 1) // List file
    {   
        if (args.array[1][0] == '-') // 1 or more flags
        {
            auxListfileFlags(args);
        }
        else // no flags
        {
            for (int i = 1; i < args.len; i++)
            {
                auxListfile(args, i);
            }
        }
    }
}

void auxListfile(tArgs args, int n) { // to do
    char* filepath = args.array[n];
    struct stat fileStat;

    // Obtain file info
    if (stat(filepath, &fileStat) != 0) {
        pPrintError(args.array[0]);
        return;
    }

    // Print size and name
    printf("%10ld  %s\n", fileStat.st_size, filepath);
}

void auxListfileFlags(tArgs args)
{   
    bool itsFlag[args.len];

    for (int i = 0; i < args.len; i++) {
        itsFlag[i] = false;
    }

    byte flags = processListFlags(args, itsFlag);

    if (flags & FLAG_LONG) 
    {
        puts("flag long");
    }
    else 
    {
        if (flags & FLAG_ACC)
        {
            puts("flag acc");
        }
        if (flags & FLAG_LINK)
        {
            puts("flag link");
        }
    }
}

/******************************************************************************/
// cwd
void cmdCwd(tArgs args, tLists *L)
{
    if (args.len == 1)
        cmdCd(args, L);
    else
        printError(args.array[0], "Invalid num of arguments");
}

/******************************************************************************/
// listdir
void auxListdir(tArgs args);

void cmdListdir(tArgs args, tLists *L) // to do
{
    if (args.len == 1)
        cmdCd(args, L);
    else
        auxListdir(args);
}

void auxListdir(tArgs args)
{
    puts("HOLA");
}

/******************************************************************************/
// reclist

//void cmdReclist(tArgs args, tLists *L); 

/******************************************************************************/
// revlist

//void cmdRevlist(tArgs args, tLists *L); 

/******************************************************************************/
// erase

//void cmdErase(tArgs args, tLists *L); 

/******************************************************************************/
// delrec

//void cmdDelrec(tArgs args, tLists *L); 


/* help

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { // and bit a bit con los bits de formato,0170000 
        case S_IFSOCK: return 's'; // socket
        case S_IFLNK: return 'l'; // symbolic link
        case S_IFREG: return '-'; // fichero normal
        case S_IFBLK: return 'b'; // block device
        case S_IFDIR: return 'd'; // directorio  
        case S_IFCHR: return 'c'; // char device
        case S_IFIFO: return 'p'; // pipe
        default: return '?'; // desconocido, no debería aparecer
     }
}
// las siguientes funciones devuelven los permisos de un fichero en formato rwx----
// a partir del campo st_mode de la estructura stat 
// las tres son correctas pero usan distintas estrategias de asignación de memoria

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    // propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    // grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    // resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    // setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}


char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    // propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    // grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    // resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    // setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}

char * ConvierteModo3 (mode_t m)
{
    char *permisos;

    if ((permisos=(char *) malloc (12))==NULL)
        return NULL;
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    // propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    // grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    // resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    // setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}    
*/