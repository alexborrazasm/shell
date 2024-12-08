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
     "Transfer cont bytes from address addr to the file described by df"},
    {"getuid", cmdGetuid, "getuid",
     "views the process's credentials (real and efective)"},
    {"setuid", cmdSetuid, "setuid [-l] id",
     "Changes the credentials of the process running the shell"
     "\n\tid: sets the credential to the numeric value id"
     "\n\t-l id: sets the credential to login id"},
    {"showvar", cmdShowvar, "showvar v1 v2 ..", 
     "shows the value and address of environment variables v1 v2 .."},
    {"changevar", cmdChangevar, "changevar [-a|-e|-p] var val",
     "Changes the value of an environment variable"
     "\n\t-a: access via the third arg of main"
     "\n\t-e: access via environ"
     "\n\t-p: access via putenv (Can create a new variable)"},
    {"subsvar", cmdSubsvar, "subsvar [-a|-e] v1 v2 val",
     "Changes environment variable (v1) for other (v2 with value val)"
     "\n\t-a: access through the third arg of main"
     "\n\t-e: access through environ"},
    {"environ", cmdEnviron, "environ [-environ|-addr]", 
     "Displays the process environment"
     "\n\t-environ: access using environ (instead of the third arg in main)"
     "\n\t-addr: displays the value and where environ and the 3rd arg in main are stored"},
    {"fork", cmdFork, "fork", 
     "The shell does the fork system call and waits for its child to end"},
    {"search", cmdSearch, "search [-add|-del|-clear|-path]..", 
     "Shows or modifies the search list (path)"
     "\n\t-add dir: add dir to the search path (equiv +dir)"
     "\n\t-del dir: remove dir from the search path (equiv -dir)"
     "\n\t-clear: clear the search path"
     "\n\t-path: import the PATH into the search path"},
    {"exec", cmdExec, "exec VAR1 VAR2 ..prog args....[@pri]",
     "Executes, without creating a process, prog with arguments"
     "\n\t\tin an environment that contains only the variables VAR1, VAR2..."},
    {"execpri", cmdExecpri, "execpri prio prog args....", 
     "Executes, without creating a process, prog with arguments with priority "
     "changed to prio"},
    {"fg", cmdFg, "fg prog args...",
     "Creates a process that runs prog in the foreground with arguments"},
    {"fgpri", cmdFgpri, "fgpri prio prog args...",
     "Creates a process that runs prog in the foreground "
     "with arguments with priority changed to prio"},
    {"back", cmdBack, "back progspec",
     "Creates a process that runs prog in the background with arguments"},
    {"backpri", cmdBackpri, "backpri prio prog args...",
     "Creates a process that runs prog in the background with arguments with "
     "the priority changed to prio"},
    {"listjobs", cmdListjobs, "listjobs", 
     "lists background processes (executed with back or backpri)"},
    {"deljobs", cmdDeljobs, "deljobs [-term][-sig]",
     "Deletes background processes from the list"
     "\n\t-term: those terminated"
     "\n\t-sig: those terminated by signal"}
};

int getInput(char *input);

void cmdClear();

void printPrompt()
{
    
    char hostname[64];
    char *username;
    long max_path = pathconf(".", _PC_PATH_MAX);
    char cwd[max_path];
    char *home;

    // Get username
    uid_t effectiveUid = geteuid();
    struct passwd *pw = getpwuid(effectiveUid);

    username = pw->pw_name;
     
    if (username == NULL)
    {
        username = "unknown";
    }

    // Get hostname
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        snprintf(hostname, sizeof(hostname), "unknown_host");
    }

    // Get working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        snprintf(cwd, sizeof(cwd), "unknown_dir");
    }

    // Get $HOME
    home = pw->pw_dir;
    if (home != NULL && strncmp(cwd, home, strlen(home)) == 0)
    {
        char temp[max_path]; // Temp buffer
        snprintf(temp, sizeof(temp), "~%s", cwd + strlen(home));
        strncpy(cwd, temp, sizeof(cwd));
        cwd[sizeof(cwd) - 1] = '\0';
    }

    // Print prompt
    printf(GREEN_BOLD"%s@%s"RST":"BLUE_BOLD"%s"RST"$ ", username, hostname, cwd);
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

void processInput(tLists *L, bool *end, tArgsMain main)
{
    if (!isEmptyListH(L->historic))
    {
        tItemH item = getItemH(lastH(L->historic), L->historic);
        tArgs args;
        args.len = stringCut(item.command, args.array);
        args.main = main;

        selectCommand(args, L, end);
    }
}

void selectCommand(tArgs args, tLists *L, bool *end)
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

        tProgspec pg;
        getProgspec(&args, &pg, 0);
        
        if(doExecuteFg(pg.commands, pg.env, NULL, L->path) == -1)
        {
            pPrintError(args.array[0]);
        }

        if (pg.env != NULL)
            freeEnv(pg.env);
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
   tPosM p; tItemM item;

    while (!isEmptyListM(*list)) {
        p = lastM(*list);

        // For each allocate mem, free()   
        item = getItemM(p, *list);
        switch (item.type)
        {
        case M_MALLOC:
            free(item.address);
            break;
        case M_MMAP:
            munmap(item.address, item.size);
            break;
        case M_SHARED:
            int id;
            // Detach the shared memory segment            
            shmdt(item.address);

            // Retrieve the shared memory segment's
            if ((id = shmget(item.keyDF, 0, 0666)) != -1)
            {
                // Remove the shared memory segment identified
                shmctl(id, IPC_RMID, NULL);
            }
            break;
        default:
            return;
        }

        deleteAtPositionM(p, list);
    }
}

void freeBackgroundList(tListB *list)
{
   tPosB p;

    while (!isEmptyListB(*list)) {
        p = lastB(*list); 
        deleteAtPositionB(p, list);
    }
}

void freePathList(tListP *list)
{
   tPosP p;

    while (!isEmptyListP(*list)) {
        p = lastP(*list); 
        free(getItemP(p, *list));
        deleteAtPositionP(p, list);
    }
}

void freeLists(tLists *L)
{
    freeHistoryList(&L->historic);
    freeFileList(&L->files);
    freeMenList(&L->memory);
    freeBackgroundList(&L->backgroud);
    freePathList(&L->path);
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
    fprintf(stderr, RED_BOLD"Error: %s: %s\n"RST, name, msg);
}

void pPrintError(char *name)
{
    fprintf(stderr, RED_BOLD"Error: %s: %s\n"RST, name, strerror(errno));
}

void pPrintErrorFile(char *name, char *filename)
{
    fprintf(stderr, RED_BOLD"Error: %s: %s: %s\n"RST,
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

bool stringToLong(char *str, long *n)
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

time_t getTime(tArgs args)
{
    time_t now;
    time(&now);

    if (now == -1)
    {
        pPrintError(args.array[0]);
    }

    return now;
}

void printTime(time_t now)
{
    int hours, minutes, seconds;
    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf(BLUE"%02d:%02d:%02d"RST, hours, minutes, seconds);
}

void printDate(time_t now)
{
    int day, month, year;
    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf(BLUE"%02d/%02d/%d"RST, day, month, year);
}

const char* monthToString(int month) {
    // Array of strings with abbreviated month names
    const char* months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    // Check if the month is within the valid range (1-12)
    if (month < 1 || month > 12) {
        return "Err"; // Return an error
    }

    // Return the corresponding month abbreviation
    return months[month - 1]; // Index start at 0, month at 1
}

void printDateString(time_t now)
{
    int day, month;
    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    

    printf(BLUE"%s %d "RST, monthToString(month), day);
    printTime(now);    
}