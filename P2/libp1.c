#include "libp1.h"
#include "libp0.h"

#define FLAG_LONG  (1 << 0)  // 0001 (1)
#define FLAG_ACC   (1 << 1)  // 0010 (2)
#define FLAG_LINK  (1 << 2)  // 0100 (4)
#define FLAG_HID   (1 << 3)  // 1000 (8)
#define FLAG_AVOID (1 << 4)

typedef enum tMode {listfile, listdir} tMode;

/**************************************************************************8* */
// Auxiliary functions to all file

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
    switch (m & S_IFMT)  // bitwise AND with the format bits, 0170000
    { 
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

// Return true if path is a directory
bool isDirectory(tArgs args, char* path) 
{
    struct stat fileStat;
    
    // Obtain info, lstat don´t follow symbolic links
    if (lstat(path, &fileStat) != 0) 
    {
        pPrintErrorFile(args.array[0], path);
        return false;
    }

    // Verify
    return S_ISDIR(fileStat.st_mode);
}

void auxList(tArgs args, tLists *L, tMode mode, 
             void function(tArgs args, int n, byte flags, char* fullPath))
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

void auxDel(tArgs args,
            void function(tArgs args, int n, byte flags, char* fullPath))
{
    if (args.len == 1) // Print actual working directory
        printError(args.array[0], "missing operand");
    else if (args.len > 1) // Delete files
    {   
        // Looping arguments
        for (int i = 1; i < args.len; i++)
        {
            function(args, i, 0, NULL);
        }
    }
}

void auxMake(tArgs args, tLists *L, void function (tArgs args))
{
    switch (args.len)
    {
        case 1:
            cmdCd(args, L);
            break;
        case 2:
            function(args);
            break;
        default:
            printError(args.array[0], "Invalid num of arguments");
            break;
    }
}

// free me!!!
char* buildPath(char* relativePath, char* fullPath)
{
    char *newPath;
    
    if (fullPath == NULL) // It's not necessary
    {
        newPath = strdup(relativePath);
    }
    else // Build full path
    {
        // +1 for '/' and for para '\0'
        size_t length = strlen(fullPath) + strlen(relativePath) + 2;  
        newPath = malloc(length * sizeof(char));
        snprintf(newPath, length, "%s/%s", fullPath, relativePath);
    }

    return newPath;
}

// free me!!!!
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
    if (m&S_ISUID) permissions[3]='s';    // setuid, setgid and stickybit
    if (m&S_ISGID) permissions[6]='s';
    if (m&S_ISVTX) permissions[9]='t';
    
    return permissions;
}

// Open dir and apply a function
void openDir(tArgs args, char* path, byte flags,
             void function(tArgs args, int n, byte flags, char* path))
{
    // Open dir
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        pPrintErrorFile(args.array[0], path);
        return;
    }

    tArgs newArgs;
    newArgs.array[0] = args.array[0];
    newArgs.len = 2;

    // Read dir contents
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Avoid list "." and ".."
        if (!(flags & FLAG_AVOID) || 
             (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0))
        {   // Avoid or not .* files
            if ((flags & FLAG_HID) || entry->d_name[0] != '.')
            {
                newArgs.array[1] = entry->d_name;
                function(newArgs, 1, flags, path);
            }
        }
    }
    // At finish close dir
    closedir(dir);
}

/******************************************************************************/
// makefile
void auxMakefile(tArgs args);

void cmdMakefile(tArgs args, tLists *L)
{
    auxMake(args, L, auxMakefile);
}

void auxMakefile(tArgs args)
{
    int df;

    df = open(args.array[1], O_CREAT, 0755); // Also we could use 0777
    if (df != -1)
    {
        close(df);  // Close file descriptor
    }
    else
    {
        pPrintErrorFile(args.array[0], args.array[1]);
    }
}

/******************************************************************************/
// makedir
void auxMakedir(tArgs args);

void cmdMakedir(tArgs args, tLists *L)
{
    auxMake(args, L, auxMakedir);
}

void auxMakedir(tArgs args)
{
    if(mkdir(args.array[1], 0755) == -1)  // Also we could use 0777
    {
        pPrintErrorFile(args.array[0], args.array[1]);
    }
}
/******************************************************************************/
// listfile
void auxListfile(tArgs args, int n, byte flags, char* fullPath);

void printDateFile(time_t date);

void printLink(char* filepath, struct stat filestat, tArgs args);

void printLong(struct stat filestat);

void cmdListfile(tArgs args, tLists *L)
{
    auxList(args, L, listfile, auxListfile);
}

void auxListfile(tArgs args, int n, byte flags, char* fullPath) 
{   
    struct stat filestat;
    char* filename = args.array[n];
    char* path = buildPath(filename, fullPath);

    // Obtain file info, lstat don´t follow symbolic links
    if (lstat(path, &filestat) != 0) 
    {
        pPrintErrorFile(args.array[0], path);
        return;
    }

    if ((flags & FLAG_LONG) && (flags & FLAG_ACC))
    {
        printDateFile(filestat.st_atime);
        printLong(filestat);
    }
    else if (flags & FLAG_LONG)
    {
        printDateFile(filestat.st_mtime);
        printLong(filestat);
    } 
    else if (flags & FLAG_ACC)
    {
        printf("%10ld  ", filestat.st_size);
        printDateFile(filestat.st_atime);  
    }
    else 
    {
        printf("%10ld  ", filestat.st_size);
    }

    printf("%s", filename);
        
    if (flags & FLAG_LINK)
    {
        printLink(path, filestat, args);
    }

    puts("");

    free(path);
}

void printDateFile(time_t date)
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
        struct group *grp = NULL;
        struct passwd *pwd = NULL;

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

        // permissions
        printf(" %s", permissions);

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

void auxListdir(tArgs args, int n, byte flags, char* fullPath);

void cmdListdir(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxListdir);
}

void auxListdir(tArgs args, int n, byte flags, char* fullPath)
{
    char *path;

    if (fullPath == NULL)
        path = args.array[n];
    else // Use absolute path
    {
        path = fullPath;
    }

    if (isDirectory(args, path))
    {
        // Print directory name
        printf("******"BLUE"%s\n"RST, path);

        // Avoid call .. and .
        if (flags & FLAG_AVOID)
            flags &= ~FLAG_AVOID;
        // Open dir and prints content
        openDir(args, path, flags, auxListfile);
    }
    else  // It's a file
        auxListfile(args, n, flags, fullPath);
}

/******************************************************************************/
// reclist

void auxReclist(tArgs args, int n, byte flags, char* fullPath);

void cmdReclist(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxReclist);
}

void auxReclist(tArgs args, int n, byte flags, char* fullPath)
{   
    char* path = buildPath(args.array[n], fullPath);

    if (isDirectory(args, path)) 
    {    
        // List actual directory before
        auxListdir(args, n, flags, path);

        // To avoid infinite loop with . and ..
        flags |= FLAG_AVOID;

        // List subdirectories after recursively
        openDir(args, path, flags, auxReclist);
    }

    free(path);
}

/******************************************************************************/
// revlist
void auxRevlist(tArgs args, int n, byte flags, char* fullPath);

void cmdRevlist(tArgs args, tLists *L)
{
    auxList(args, L, listdir, auxRevlist);
}

void auxRevlist(tArgs args, int n, byte flags, char* fullPath)
{   
    char* path = buildPath(args.array[n], fullPath);

    if (isDirectory(args, path)) 
    {   
        // To avoid infinite loop with . and ..
        flags |= FLAG_AVOID;
        
        // List subdirectories before recursively
        openDir(args, path, flags, auxRevlist);
        
        flags &= ~FLAG_AVOID;

        // List actual directory after
        auxListdir(args, n, flags, path);
    }

    free(path);
}

/******************************************************************************/
// erase
void auxErase(tArgs args, int n, byte flags, char* fullPath);

void cmdErase(tArgs args, tLists *L)
{
    UNUSED(L);
    auxDel(args, auxErase);
}

void auxErase(tArgs args, int n, byte flags, char* fullPath)
{
    UNUSED(flags);

    char* path = buildPath(args.array[n], fullPath);

    if (remove(path) == -1)
    {
        pPrintErrorFile(args.array[0], path);
    }
    free(path);
}

/******************************************************************************/
// delrec
void auxDelrec(tArgs args, int n, byte flags, char* fullPath);

void cmdDelrec(tArgs args, tLists *L)
{
    UNUSED(L);
    auxDel(args, auxDelrec);
}

void auxDelrec(tArgs args, int n, byte flags, char* fullPath)
{
    UNUSED(flags);

    char* path = buildPath(args.array[n], fullPath);

    if (remove(path) == -1) // Try to remove dir
    {
        // Dir it's not empty
        openDir(args, path, 0, auxDelrec); // Remove recursively
        if (remove(path) == -1) // Finally remove dir
        {
            pPrintErrorFile(args.array[0], path);
        }
    }

    free(path);
}
