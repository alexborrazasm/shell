#include "libp1.h"
#include "libp0.h"

#define FLAG_LONG  (1 << 0)  // 0001 (1)
#define FLAG_ACC   (1 << 1)  // 0010 (2)
#define FLAG_LINK  (1 << 2)  // 0100 (4)
#define FLAG_HID   (1 << 3)  // 1000 (8)

typedef enum tMode {listfile, listdir} tMode;

byte processListFlags(tArgs args, int* lastFlag, tMode mode)
{
    byte flags = 0;  // no flags

    for (int i = 1; i < args.len; i++) 
    {    
        if (strcmp(args.array[i], "-long") == 0)
        {
            flags |= FLAG_LONG;
            *lastFlag = i + 1;
        } 
        else if (strcmp(args.array[i], "-acc") == 0)
        {
            flags |= FLAG_ACC;
            *lastFlag = i + 1;
        } 
        else if (strcmp(args.array[i], "-link") == 0)
        {
            flags |= FLAG_LINK;
            *lastFlag = i + 1;
        }
        else if ((strcmp(args.array[i], "-hid") == 0) && (mode == listdir))
        {
            flags |= FLAG_HID;
            *lastFlag = i + 1;
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

void auxList(tArgs args, tLists *L, tMode mode, 
             void function(tArgs args, int n, byte flags, char* path))
{
        if (args.len == 1) // Print actual working directory
        cmdCd(args, L);
    else if (args.len > 1) // List file
    {   
        if (args.array[1][0] == '-') // 1 or more flags
        {
            int pLastFlag = 1;
            byte flags = processListFlags(args, &pLastFlag, mode);
            
            if (args.len == pLastFlag)  // Only lags but no path
            {  
                args.len = 1;  // cmdCd prints path with args.len = 1
                cmdCd(args, L);
                return;
            }

            for (int i = pLastFlag; i < args.len; i++)
            {
                function(args, i, flags, NULL);
            }
        }
        else // no flags
        {
            for (int i = 1; i < args.len; i++)
            {
                function(args, i, 0, NULL);
            }
        }
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
void auxListfile(tArgs args, int n, byte flags, char* path);

void printDate(time_t date);

void printLink(char* filepath, struct stat filestat, tArgs args);

void printLong(struct stat filestat);

void cmdListfile(tArgs args, tLists *L)
{
    auxList(args, L, listfile, auxListfile);
}

void auxListfile(tArgs args, int n, byte flags, char* path) 
{   
    struct stat filestat;
    char* filepath = args.array[n];
    char* realPath;

    // Path is for use this function on listdir 
    if (path == NULL)
        realPath = filepath;
    else
    {
        realPath = malloc(256 * sizeof(char));
        snprintf(realPath, 256, "%s/%s", path, filepath);
    }

    // Obtain file info, lstat don´t follow symbolic links
    if (lstat(realPath, &filestat) != 0) {
        pPrintErrorFile(args.array[0], realPath);
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
        printLink(realPath, filestat, args);
    }

    puts("");

    if (path != NULL)
        free(realPath);
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

    if (permissions != NULL)
    {
        printf("  %lu (%8lu)     ", filestat.st_nlink, filestat.st_ino);

        // Get user and group
        struct group *grp;
        struct passwd *pwd;

        pwd = getpwuid(filestat.st_uid);
        grp = getgrgid(filestat.st_gid);
        
        if(pwd != NULL) {
            printf("%-9s", pwd->pw_name);
        } else {
            printf("%-9ld", (long)filestat.st_uid);
        }
    
        if(grp != NULL) {
            printf("%s", grp->gr_name);}
        else{
            printf("%ld", (long)filestat.st_gid);
        }

        // size
        printf(" %7lu ", filestat.st_size);

        free(permissions);
    }
    else
    {
        printError("File permissions", "Error getting file permissions");
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
void auxListdir(tArgs args, int n, byte flags, char* path);

void cmdListdir(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxListdir);
}

void auxListdir(tArgs args, int n, byte flags, char* path)
{
    UNUSED(path);
    char *dirPath = args.array[n];
    char *realPath;

    if (path == NULL)
        realPath = dirPath;
    else
    {
        realPath = path;
    }

    // Open dir
    DIR *dir = opendir(realPath);
    if (dir == NULL) 
    {
        pPrintErrorFile(args.array[0], realPath);
        return;
    }

    tArgs fakeArgs;
    fakeArgs.len = 2;

    fakeArgs.array[0] = args.array[0];
    
    printf("**\033[1;34m%s\033[0m\n", realPath);

    // Read dir contents
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Avoid list "." and ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {   // Avoid or not .* files
            if ((flags & FLAG_HID) || entry->d_name[0] != '.')
            {
                fakeArgs.array[1] = entry->d_name;
                auxListfile(fakeArgs, 1, flags, realPath);
            }
        }
    }

    // At finish close dir
    closedir(dir);
}

/******************************************************************************/
// reclist
void auxReclist(tArgs args, int n, byte flags, char* path);

bool isDirectory(char* path);

void cmdReclist(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxReclist);
}

void auxReclist(tArgs args, int n, byte flags, char* path)
{   
    char* dirPath = args.array[n];
    char* realPath;

    if (path == NULL)
        realPath = dirPath;
    else
    {
        realPath = malloc(256 * sizeof(char));
        snprintf(realPath, 256, "%s/%s", path, dirPath);
    }

    if (isDirectory(realPath)) 
    {    
        // List actual directory
        auxListdir(args, n, flags, realPath);

        // Open dir
        DIR *dir = opendir(realPath);
        if (dir == NULL) 
        {
            pPrintErrorFile(args.array[0], realPath);
            return;
        }

        // List directory contents
        struct dirent *entry;

        tArgs newArgs;
        newArgs.array[0] = args.array[0];
        newArgs.len = 2;

        while ((entry = readdir(dir)) != NULL)
        {
            // Avoid list "." and ".."
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {   // Avoid or not .* files
                if ((flags & FLAG_HID) || entry->d_name[0] != '.')
                {
                    newArgs.array[1] = entry->d_name;
                    auxReclist(newArgs, 1, flags, realPath);
                }
            }
        }
        // At finish close dir
        closedir(dir);
        if (path != NULL)
            free(realPath);
    }
}

bool isDirectory(char* path) {
    struct stat fileStat;
    
    // Obtain info
    if (stat(path, &fileStat) != 0) {
        pPrintErrorFile("isDirectory", path);
        return false;
    }

    // Verify
    return S_ISDIR(fileStat.st_mode);
}


/******************************************************************************/
// revlist
void auxRevlist(tArgs args, int n, byte flags, char* path);

void cmdRevlist(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxRevlist);
}

void auxRevlist(tArgs args, int n, byte flags, char* path)
{
    char* dirPath = args.array[n];
    char* realPath;

    if (path == NULL)
        realPath = dirPath;
    else
    {
        realPath = malloc(256 * sizeof(char));
        snprintf(realPath, 256, "%s/%s", path, dirPath);
    }

    if (isDirectory(realPath)) 
    {
        // Open directory
        DIR *dir = opendir(realPath);
        if (dir == NULL) 
        {
            pPrintErrorFile(args.array[0], realPath);
            if (path != NULL)
                free(realPath);
            return;
        }

        // List directory contents first (subdirectories recursively)
        struct dirent *entry;

        tArgs newArgs;
        newArgs.array[0] = args.array[0];
        newArgs.len = 2;

        while ((entry = readdir(dir)) != NULL)
        {
            // Avoid listing "." and ".."
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {   
                // Avoid or not hidden files
                if ((flags & FLAG_HID) || entry->d_name[0] != '.')
                {
                    newArgs.array[1] = entry->d_name;
                    // Recursively list subdirectories before listing the current dir
                    auxRevlist(newArgs, 1, flags, realPath);
                }
            }
        }

        // After listing subdirectories, list the actual directory
        auxListdir(args, n, flags, realPath);

        // Close directory
        closedir(dir);

        if (path != NULL)
            free(realPath);
    }
}

/******************************************************************************/
// erase
void cmdErase(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
    puts("TO DO");
}

/******************************************************************************/
// delrec
void cmdDelrec(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
    puts("TO DO");
} 


/* help

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
*/