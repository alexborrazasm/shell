#include "libp1.h"
/******************************************************************************/
// makefile

void cmdMakefile(tArgs args, tLists *L){
    int df;
    switch (args.len)
    {
    case 2:
        if ((df = open(args.array[1], O_CREAT, 0755)) != -1) //Also we could use 0777
        {
            close(df);  //Probably it's ok?¿ idk
            printf("El archivo ha sido creado\n"); //modify
        } else {
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

//void cmdMakedir(tArgs args, tLists *L);

/******************************************************************************/
// listfile

//void cmdListfile(tArgs args, tLists *L);

/******************************************************************************/
// cwd
void cmdCwd(tArgs args, tLists *L)
{
    if (args.len == 1)
        cmdChdir(args, L);
    else
        printError("cwd", "Invalid num of arguments");
}

/******************************************************************************/
// listdir

//void cmdListdir(tArgs args, tLists *L); 

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
        default: return '?'; // desconocido, no deberia aparecer
     }
}
// las siguientes funciones devuelven los permisos de un fichero en formato rwx----
// a partir del campo st_mode de la estructura stat 
// las tres son correctas pero usan distintas estrategias de asignaciÃ³n de memoria

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