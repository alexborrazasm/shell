#include "libp1.h"
#include "libp0.h"

#define FLAG_LONG  (1 << 0)  // 0001 (1)
#define FLAG_ACC   (1 << 1)  // 0010 (2)
#define FLAG_LINK  (1 << 2)  // 0100 (4)

byte processListFlags(tArgs args, int* lastFlag)
{
    byte flags = 0;  // no flags

    for (int i = 1; i < args.len; i++) 
    {    
        if (strcmp(args.array[i], "-long") == 0)
        {
            flags |= FLAG_LONG;
            *lastFlag = i;
        } 
        else if (strcmp(args.array[i], "-acc") == 0)
        {
            flags |= FLAG_ACC;
            *lastFlag = i;
        } 
        else if (strcmp(args.array[i], "-link") == 0)
        {
            flags |= FLAG_LINK;
            *lastFlag = i;
        }
        else 
        {
            break;
        }
    }

    return flags; 
}

char getFileTypeChar(mode_t m)
{
    switch (m & S_IFMT) { // bitwise AND with the format bits, 0170000
    case S_IFSOCK: return 's'; // socket
    case S_IFLNK: return 'l';  // symbolic link
    case S_IFREG: return '-';  // regular file
    case S_IFBLK: return 'b';  // block device
    case S_IFDIR: return 'd';  // directory  
    case S_IFCHR: return 'c';  // character device
    case S_IFIFO: return 'p';  // pipe
    default: return '?';       // unknown, shouldn't appear
    }
}

// free(permissions)!!!!
char* getFilePermissions(mode_t m)
{
    char *permissions;

    if ((permissions=(char *) malloc (12))==NULL)
        return NULL;
    strcpy (permissions,"---------- ");
    
    permissions[0]=getFileTypeChar(m);
    if (m&S_IRUSR) permissions[1]='r';    // proprietary
    if (m&S_IWUSR) permissions[2]='w';
    if (m&S_IXUSR) permissions[3]='x';
    if (m&S_IRGRP) permissions[4]='r';    // group
    if (m&S_IWGRP) permissions[5]='w';
    if (m&S_IXGRP) permissions[6]='x';
    if (m&S_IROTH) permissions[7]='r';    // rest
    if (m&S_IWOTH) permissions[8]='w';
    if (m&S_IXOTH) permissions[9]='x';
    if (m&S_ISUID) permissions[3]='s';    // setuid, setgid y stickybit
    if (m&S_ISGID) permissions[6]='s';
    if (m&S_ISVTX) permissions[9]='t';
    
    return permissions;
}

/******************************************************************************/
// makefile
void cmdMakefile(tArgs args, tLists *L){
    int df;
    
    switch (args.len)   
    {
    case 1:
        cmdCwd(args, L);
        break;
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
    UNUSED(L);

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
void auxListfile(tArgs args, int n, byte flags);

void printDate(time_t date);

void printLink(char* filepath, struct stat filestat, tArgs args);

void printLong(struct stat filestat);

void cmdListfile(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len == 1) // Print actual working directory
        cmdCd(args, L);
    else if (args.len > 1) // List file
    {   
        if (args.array[1][0] == '-') // 1 or more flags
        {
            int pLastFlag = 0; byte flags = processListFlags(args, &pLastFlag);
            
            for (int i = pLastFlag + 1; i < args.len; i++)
            {
                auxListfile(args, i, flags);
            }
            
        }
        else // no flags
        {
            for (int i = 1; i < args.len; i++)
            {
                auxListfile(args, i, 0);
            }
        }
    }
}

void auxListfile(tArgs args, int n, byte flags) 
{   
    char* filepath = args.array[n];
    struct stat filestat;

    // Obtain file info, lstat don´t follow symbolic links
    if (lstat(filepath, &filestat) != 0) {
        pPrintErrorFile(args.array[0], filepath);
        return;
    }

    if ((flags & FLAG_LONG) && (flags & FLAG_ACC))
    {
        printDate(filestat.st_atime);
        printLong(filestat);
    }
    else if (flags & FLAG_LONG)
    {
        printDate(filestat.st_mtime);
        printLong(filestat);
    } 
    else if (flags & FLAG_ACC)
    {
        printf("%10ld  ", filestat.st_size);
        printDate(filestat.st_atime);  
    }
    else 
    {
        printf("%10ld  ", filestat.st_size);
    }

    printf("%s", filepath);
        
    if (flags & FLAG_LINK)
    {
        printLink(filepath, filestat, args);
    }

    puts("");
}

void printDate(time_t date)
{
    char buffer[20];
    struct tm *tm_info;

    // Convert time to readable format 
    tm_info = localtime(&date);
    strftime(buffer, sizeof(buffer), "%Y/%m/%d-%H:%M", tm_info);

    // Print size, date and name
    printf("%s ", buffer);
}

void printLink(char* filepath, struct stat filestat, tArgs args)
{
    if (S_ISLNK(filestat.st_mode)) // It is a symbolic link?
    {
        char buffer[1024]; // To store the path
        // Read symbolic link
        ssize_t len = readlink(filepath, buffer, sizeof(buffer) - 1);

        if (len != -1) 
        {
            buffer[len] = '\0'; // "to string"
            // print the path of symbolic link
            printf(" -> %s", buffer);
        }
        else 
        {
            printError(args.array[0], "Can´t read the symbolic link");
        }
    }
}

void printLong(struct stat filestat) 
{   
    char* permissions = getFilePermissions(filestat.st_mode);

    // Obtain proprietary
    struct passwd *pw = getpwuid(filestat.st_uid);
    char *owner = pw ? pw->pw_name : "unknown";

    // Obtain group
    struct group *gr = getgrgid(filestat.st_gid);
    char *group = gr ? gr->gr_name : "unknown";

    printf("  %lu (%lu)     %-9s %s %s %7lu ", filestat.st_nlink,
           filestat.st_ino, owner, group, permissions, filestat.st_size);

    free(permissions);
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
    UNUSED(L);

    if (args.len == 1)
        cmdCd(args, L);
    else
        auxListdir(args);
}

void auxListdir(tArgs args)
{
    UNUSED(args);
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
*/