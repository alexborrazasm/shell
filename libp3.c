#include "libp3.h"

#define ACTIVE 

extern char **environ;  // Declaration of environ

void printEnv(char *envp[], bool environ);

struct SIN 
{
    char *name;
    int num;
};

static struct SIN sigstrnum[] = 
{   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
};

int getNumSin(char *sin) 
{ 
  for (int i = 0; sigstrnum[i].name != NULL; i++)
  	if (!strcmp(sin, sigstrnum[i].name))
		return sigstrnum[i].num;
  return -1;
}

char *getNameSin(int sin)
{
  for (int i = 0; sigstrnum[i].name != NULL; i++)
  	if (sin == sigstrnum[i].num)
		return sigstrnum[i].name;
 return ("SIGUNKNOWN");
}

/******************************************************************************/
// getuid
void doGetuid();

void cmdGetuid(tArgs args, tLists *L)
{
    UNUSED(L);

    if(args.len == 1)
        doGetuid();
    else
        printError(args.array[0], "Not accept arguments");
}

void doGetuid()
{
    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();
    struct passwd *passwd = getpwuid(realUid);

    printf("Credencial real: "YELLOW"%d"RST", ("BLUE"%s"RST")\n",
           realUid, passwd ? passwd->pw_name : "unknown");

    passwd = getpwuid(effectiveUid);
           
    printf("Credencial efective: "YELLOW"%d"RST", ("BLUE"%s"RST")\n",
           effectiveUid, passwd ? passwd->pw_name : "unknown");
}

/******************************************************************************/
// setuid [-l] id
void setuidLogin(tArgs args);

void setuidInt(int n);

void cmdSetuid(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1:
        doGetuid();
        break;
    case 2:
        if (args.array[1][0] != '-') // -l
        {
            int n;
            if (stringToInt(args.array[1], &n))
            {
                setuidInt(n);
            }
        }
        else
            doGetuid();        
        break;
    case 3:
        if (args.array[1][0] == '-' && args.array[1][1] == 'l') // -l login
            setuidLogin(args);
        else
            printError(args.array[0], "Invalid argument");
        break;
    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void setuidLogin(tArgs args)
{
    char *login = args.array[2]; struct passwd *pw = getpwnam(login);
    
    if (pw == NULL)
    {
        printError(args.array[0], "User not found");
        return;
    }

    setuidInt(pw->pw_uid);
}

void setuidInt(int n)
{
    // Try to change UID
    if (seteuid((uid_t)n) == -1) 
        pPrintError("Can not change uid");
}

/******************************************************************************/
// showvar v1 v2 ...
void printVar(tArgs args, char* var);

void cmdShowvar(tArgs args, tLists *L)
{
    UNUSED(L);

    if(args.len == 1) // Print environ
    {
        printEnv(args.main.envp, false);
    }
    else
    {
        for(int i = 1; i < args.len; i++)
        {
            printVar(args, args.array[i]);
        }
    }
}

void printVar(tArgs args, char* var)
{
    char **envp = args.main.envp, *value = NULL, *foundEnvp = NULL;
    char *foundEnviron = NULL, **entryAddressArg3 = NULL;
    char **entryAddressEnviron = NULL;   
    size_t varLen = strlen(var);

    for(char **env = envp; *env != NULL; env++) // search on envp (arg3)
    {
        if(strncmp(*env, var, varLen) == 0 && (*env)[varLen] == '=')
        {
            foundEnvp = *env + varLen + 1; // value addr
            entryAddressArg3 = env;        // save addr of pointer
            break;
        }
    }

    for(char **env = environ; *env != NULL; env++) // search on environ 
    {
        if(strncmp(*env, var, varLen) == 0 && (*env)[varLen] == '=')
        {
            foundEnviron = *env + varLen + 1; // value addr
            entryAddressEnviron = env;        // save addr of pointer
            break;
        }
    }

    value = getenv(var);

    // Print values if found
    if(foundEnvp != NULL)
        printf("with arg3 main: %s="BLUE"%s"RST"("GREEN"%p"RST") @"GREEN"%p\n"RST,
               var, foundEnvp, foundEnvp, entryAddressArg3);

    if(foundEnviron != NULL)
        printf("  with environ: %s="BLUE"%s"RST"("GREEN"%p"RST") @"GREEN"%p\n"RST,
               var, foundEnviron, foundEnviron, entryAddressEnviron);

    if(value != NULL)
        printf("    with getenv: "BLUE"%s"RST"("GREEN"%p"RST")\n", value, value);
}

/******************************************************************************/
// changevar [-a|-e|-p] var val
int doChangeVar(char *var, char *value, char *env[], tListM *L);

int searchVar(char *var, char *env[]);

int doPutEnv(char *var, char *value);

void cmdChangevar(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len == 4) 
    {
        if(args.array[1][0] == '-')
        {
            if(args.array[1][1] == 'a') // main arg3
            {
                char **envp = args.main.envp;
                if(doChangeVar(args.array[2], args.array[3], envp, 
                   &L->memory) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
            else if(args.array[1][1] == 'e') // environ
            {
                if(doChangeVar(args.array[2], args.array[3], environ, 
                   &L->memory) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
            else if(args.array[1][1] == 'p') // putenv
            {
                if(doPutEnv(args.array[2], args.array[3]) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
        }
    }

    // Error   
    printError(args.array[0], "Use changevar [-a|-e|-p] var val");
}


int searchVar(char *var, char *env[])  
{
    int pos=0;
    char *aux = malloc(sizeof(var) + 2); // +2 for '\0' and '='
  
    strcpy(aux, var);
    strcat(aux, "=");
  
    while(env[pos] != NULL)
    {
        if (!strncmp(env[pos], aux, strlen(aux)))
        {
            free(aux); return (pos);
        }
        else 
            pos++;
    }
    free(aux);
    errno = ENOENT;   // Var not found
    return -1;
}

int doChangeVar(char *var, char *value, char *env[], tListM *L) 
{
    int pos; char *aux;
   
    if((pos = searchVar(var, env)) == -1)
        return -1;
 
    if((aux = (char *)malloc(strlen(var)+strlen(value)+2)) == NULL)
	    return -1;

    // NOTE: Do not free envVar because environment directly uses this pointer   
    // add to memoryList to free at end
    tItemM item; time_t now; time(&now);
    item.address = aux;
    item.date = now;
    item.type = M_MALLOC;
    item.size = sizeof(aux);

    insertItemM(item, MNULL, L);

    // Change env
    strcpy(aux, var);
    strcat(aux, "=");
    strcat(aux, value);

    env[pos] = aux;
    return pos;
}

int doPutEnv(char *var, char *value)
{
    size_t length = strlen(var) + strlen(value) + 2; // +2 for '=' '\0'
    char *envVar = malloc(length);
    
    if (!envVar)
        return -1;
    
    snprintf(envVar, length, "%s=%s", var, value); // Build var=value

    // Change var
    if (putenv(envVar) != 0) 
    {
        free(envVar); return -1;
    }

    return 0; // AL RIGHT
    // NOTE: Do not free envVar because putenv directly uses this pointer
}

/******************************************************************************/
// subsvar [-a|-e] v1 v2 val
int doSubsVar(char *v1, char *v2, char *value, char *env[], tListM *L);

void cmdSubsvar(tArgs args, tLists *L)
{
    UNUSED(L);

    if (args.len == 5) 
    {
        char** array = args.array;
        if(args.array[1][0] == '-')
        {
            if(args.array[1][1] == 'a') // main arg3
            {
                char **envp = args.main.envp;
                if(doSubsVar(array[2], array[3], array[4], envp, 
                   &L->memory) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
            else if(args.array[1][1] == 'e') // environ
            {
                if(doSubsVar(array[2], array[3], array[4], environ, 
                   &L->memory) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
        }
    }

    // Error   
    printError(args.array[0], "Subsvar [-a|-e] v1 v2 val");
}

int doSubsVar(char *v1, char *v2, char *value, char *env[], tListM *L)
{
    int pos; char *aux;
   
    if((pos = searchVar(v1, env)) == -1)
        return -1;
 
    if((aux = (char *)malloc(strlen(v2) + strlen(value) + 2)) == NULL)
	    return -1;

    // NOTE: Do not free envVar because environment directly uses this pointer   
    // add to memoryList to free at end
    tItemM item; time_t now; time(&now);
    item.address = aux;
    item.date = now;
    item.type = M_MALLOC;
    item.size = sizeof(aux);

    insertItemM(item, MNULL, L);

    // Change env
    strcpy(aux, v2);
    strcat(aux, "=");
    strcat(aux, value);

    env[pos] = aux;
    return pos;
}

/******************************************************************************/
// environ [-environ|-addr]
void doEnvironAddr(tArgs args);

void cmdEnviron(tArgs args, tLists *L)
{
    UNUSED(L);

    switch (args.len)
    {
    case 1: // third argument of main
        printEnv(args.main.envp, false);
        break;
    case 2: // -environ or -addr
        if (strcmp(args.array[1], "-addr") == 0)
            doEnvironAddr(args);
        else if (strcmp(args.array[1], "-environ") == 0)
            printEnv(environ, true);
        else
            printError(args.array[0], "Invalid argument");
        break;
    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }
}

void printEnv(char *envp[], bool environ) 
{
    int i = 0;

    for (char **env = envp; *env != NULL; env++) {
        printf(GREEN"%p"RST"-> %s ["YELLOW"%d"RST"]=("GREEN"%p"RST") "BLUE"%s\n"
               RST, env, (environ)? "environ":"main arg3", i, *env, *env);
        i++;
    }
}

void doEnvironAddr(tArgs args)
{
    char **envp = args.main.envp;

    printf("environ:   "GREEN"%p"RST" (stored in "GREEN"%p"RST")\n",
           environ, (void*)&environ);
    printf("main arg3: "GREEN"%p"RST" (stored in "GREEN"%p"RST")\n",
           envp, (void*)&envp);
}

/******************************************************************************/
// fork
void doFork(tListB *L);

void cmdFork(tArgs args, tLists *L)
{
    if(args.len == 1)
        doFork(&L->background);
    else
        printError(args.array[0], "Not accept arguments");
}

void doFork(tListB *L)
{
	pid_t pid;
	
	if ((pid = fork()) == 0)
    {
        freeBackgroundList(L); // Remove all items of process list
		printf("execute process "BLUE"%d\n"RST, getpid());
	}
	else if (pid!=-1)
		waitpid(pid, NULL, 0);
}

/******************************************************************************/
// search [-add dir -del dir -clear -path]
void printPathList(tListP L);

void searchClean(tListP *L);

void searchPath(tArgs args, tListP *L);

void searchAdd(tArgs args, tListP *L);

void searchDel(tArgs args, tListP *L);

void cmdSearch(tArgs args, tLists *L)
{
   switch (args.len)
   {
    case 1: // Print path list
        printPathList(L->path);
        break;
    case 2: // -clear or -path
        if(strcmp(args.array[1], "-clear") == 0)
            searchClean(&L->path);
        else if(strcmp(args.array[1], "-path") == 0)
            searchPath(args, &L->path);
        else
            printError(args.array[0], "Invalid argument");
        break;
    case 3: // -add dir or -del dir
        if(strcmp(args.array[1], "-add") == 0)
            searchAdd(args, &L->path);
        else if(strcmp(args.array[1], "-del") == 0)
            searchDel(args, &L->path);
        else
            printError(args.array[0], "Invalid argument");
        break;
    default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }    
}

void printPathList(tListP L)
{
    tItemP item;

    for (tPosP p = firstP(L); p != PNULL; p = nextP(p, L))
    {
        item = getItemP(p, L);
        printf(BLUE"%s\n"RST, item);
    }
}

void searchClean(tListP *L)
{
    freePathList(L);
}

void searchPath(tArgs args, tListP *L)
{
    const char *pathEnv = getenv("PATH");
    if (!pathEnv) 
    {
        printError(args.array[0], "Can not find PATH");
        return;
    }

    char *pathCopy = strdup(pathEnv);
    char *token = strtok(pathCopy, ":"); // split path ':'
    int i = 0;

    while (token != NULL) 
    {
        insertItemP(strdup(token), PNULL, L);
        token = strtok(NULL, ":");
        i++;
    }

    printf("Imported "YELLOW"%d"RST" directories into the "BLUE"search path\n"
           RST, i);

    free(pathCopy);
}

void searchAdd(tArgs args, tListP *L)
{
    tItemP path = strdup(args.array[2]);

    insertItemP(path, PNULL, L);
}

void searchDel(tArgs args, tListP *L)
{
    char* path = args.array[2];
    tPosP p = findItemP(path, *L);

    if (p != PNULL)
    {
        free(getItemP(p, *L));
        deleteAtPositionP(p, L);
    }
}

/******************************************************************************/
// Aux functions to execute ... 

char *executable(char *s, tListP L)
{       
    static char path[MAX_PATH]; struct stat st; tPosP pos = firstP(L);

    if (s == NULL || pos == PNULL)
        return s;
    
    if (s[0]=='/' || !strncmp(s,"./",2) || !strncmp(s,"../",3))
        return s;        // is an absolute pathname

    while(pos != PNULL)
    {
        strncpy(path, getItemP(pos, L), MAX_PATH - 1); 
        strncat (path,"/", MAX_PATH - 1); 
        strncat(path, s, MAX_PATH - 1);
        
        if(lstat(path, &st) != -1)
            return path;
        pos = nextP(pos, L);
    }

    return s;
}

/*  NewEnv contains the address of the new environment
    pprio the address of the new priority
    NULL indicates no change in environment and/or priority */
int doExecpve(char **args, char **newEnv, int *pprio, tListP L)
{   
    char *p; 
                                        
    if(args[0] == NULL || (p = executable(args[0], L)) == NULL)
    {
        errno = EFAULT;
        return -1;
    }

    if(pprio != NULL && setpriority(PRIO_PROCESS, getpid(), *pprio) == -1 
        && errno)
    {
        return -1;
    }

    if(newEnv == NULL)
        return execv(p, args);
    else
        return execve(p, args, newEnv);
}

int doExecuteFg(char **args, char **newEnv, int *pprio, tListP L)
{   
    pid_t pid = fork(); // Create a child process

    if (pid == -1) { return -1; }

    if (pid == 0) // Child execute 
    {   
        if(doExecpve(args, newEnv, pprio, L) == -1 )
        { 
            pPrintError(args[0]);
            exit(255); 
        }
    } 
    // Father wait
    int status;
    if (waitpid(pid, &status, 0) == -1) { return -1; }
    
    return 0; // All right
}

int doExecuteBg(tArgs args, char **command, char **newEnv, int *pprio, tLists *L)
{
    pid_t pid = fork(); // Create a child process

    if (pid == -1) { return -1; }

    if (pid == 0) // Child execute 
    {   
        if(doExecpve(command, newEnv, pprio, L->path) == -1 )
        { 
            pPrintError(command[0]);
            exit(255); 
        }
    }
    // Father
    
    tItemB item;
    item.pid = pid;
    item.time = getTime(args);
    item.status = S_ACTIVE;
    item.sin = 0;

    char buffer[MAX_BUFFER_INPUT] = "";

    for (int i = 0; command[i] != NULL; i++)
    {
        if (i > 0) 
            strcat(buffer, " "); // Add ' '
        strcat(buffer, command[i]);
    }

    strcpy(item.commandLine, buffer);

    insertItemB(item, BNULL, &L->background);    

    return 0; // All right
}

void freeEnv(char **envp)
{
    if (envp) 
    {
        for (int i = 0; envp[i] != NULL; i++) 
        {
            free(envp[i]);
        }
        free(envp);
    }
}

int copyEnvVar(char ***envp, char *name, char *value, int i)
{
    if (!name) { return -1; } 

    // +2 for '=' and '\0'
    char *newEnvVar = malloc(strlen(name) + strlen(value) + 2);

    if (!newEnvVar) { return -1; }

    sprintf(newEnvVar, "%s=%s", name, value);  // "name=value"

    // +1 for new variable, +1 for NULL terminator
    *envp = realloc(*envp, (i + 2) * sizeof(char *));

    if (!*envp)
    {
        free(newEnvVar);
        return -1;
    }

    (*envp)[i] = newEnvVar;
    (*envp)[i + 1] = NULL;
   
    return 0;
}

int addEnvVar(char ***envp, char *new, int i)
{
    if (!new) { return -1; }

    char *newEnvVar = malloc(strlen(new) + 1);  // +1 for the null terminator

    if (!newEnvVar) return -1;

    strcpy(newEnvVar, new);

    // +1 for new variable, +1 for NULL terminator
    *envp = realloc(*envp, (i + 2) * sizeof(char *));

    if (!*envp)
    {
        free(newEnvVar);
        return -1;
    }

    (*envp)[i] = newEnvVar;
    (*envp)[i + 1] = NULL;

    return 0;
}

// s = start
int findEnvEnd(char ***envipNew, char *args[], int s) 
{    
    int i = 0;
    while (args[i] != NULL)
    { 
        if (strchr(args[i+s], '='))
        {
            if (addEnvVar(envipNew, args[i+s], i))
            {
                pPrintError("New env error");
                return -1;
            }
        }
        else
        {
            char * value = getenv(args[i+s]);
            if (value)
            {
                if (copyEnvVar(envipNew, args[i+s], value, i))
                {
                    pPrintError("New env error");
                    return -1;
                }
            } 
            else {
                break;
            }
        }
        i++;   
    }
    return i+s;
}


// Remember free pg.env
void getProgspec(tArgs *args, tProgspec *pg, int start)
{
    char **env = NULL;
    int endVars = findEnvEnd(&env, args->array, start);

    if (endVars == -1) { freeEnv(env); }
    
    if (endVars == start) // No new env
    {
        pg->env = NULL;
        pg->commands = args->array + start;
        pg->len = args->len - start;
    }
    else // New env
    {
        pg->env = env;
        pg->commands = args->array + endVars;
        pg->len = args->len - endVars;
    }
}

/******************************************************************************/
// exec progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdExec(tArgs args, tLists *L)
{
    tProgspec pg; // free?
    
    getProgspec(&args, &pg, 1);
    
    if (doExecpve(pg.commands, pg.env, NULL, L->path) == -1)
    {
        pPrintError(args.array[0]);
    }
}

/******************************************************************************/
// execpri prio progspec 
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdExecpri(tArgs args, tLists *L)
{   
    if (args.len > 1)
    {
        tProgspec pg; // free?
        int prio;

        if (!stringToInt(args.array[1], &prio)) 
        {
            printError(args.array[0], "Invalid prio");
            return;
        }

        getProgspec(&args, &pg, 2);
            
        if(doExecpve(pg.commands, pg.env, &prio, L->path) == -1)
        {
            pPrintError(args.array[0]);
        }
    }
    else
    {
        printError(args.array[0], "Invalid argument");
    } 
}

/******************************************************************************/
// fg progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdFg(tArgs args, tLists *L)
{
    tProgspec pg;
    getProgspec(&args, &pg, 1);
        
    if(doExecuteFg(pg.commands, pg.env, NULL, L->path) == -1)
    {
        pPrintError(args.array[0]);
    }

    if (pg.env != NULL)
        freeEnv(pg.env);
}

/******************************************************************************/
// fgpri prio progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdFgpri(tArgs args, tLists *L)
{
    if (args.len > 1)
    {
        tProgspec pg; int prio;

        if (!stringToInt(args.array[1], &prio)) 
        {
            printError(args.array[0], "Invalid prio");
            return;
        }    

        getProgspec(&args, &pg, 2);
        
        if(doExecuteFg(pg.commands, pg.env, &prio, L->path) == -1)
        {
            pPrintError(args.array[0]);
        }

        if (pg.env != NULL)
            freeEnv(pg.env);
    }
    else
    {
        printError(args.array[0], "Invalid argument");
    }
}

/******************************************************************************/
// back progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdBack(tArgs args, tLists *L)
{
    tProgspec pg;

    getProgspec(&args, &pg, 1);
 
    if(doExecuteBg(args, pg.commands, pg.env, NULL, L) == -1)
    {
        pPrintError(args.array[0]);
    }

    if (pg.env != NULL)
        freeEnv(pg.env);

}

/******************************************************************************/
// backpri prio progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdBackpri(tArgs args, tLists *L)
{
    if (args.len > 1)
    {
        tProgspec pg; int prio;

        if (!stringToInt(args.array[1], &prio)) 
        {
            printError(args.array[0], "Invalid prio");
            return;
        }    

        getProgspec(&args, &pg, 2);
        
        if(doExecuteBg(args, pg.commands, pg.env, &prio, L) == -1)
        {
            pPrintError(args.array[0]);
        }

        if (pg.env != NULL)
            freeEnv(pg.env);
    }
    else
    {
        printError(args.array[0], "Invalid argument");
    }
}

/******************************************************************************/
// listjobs
const char* getStateName(int state);

void updateItemjobs(tItemB *item, tPosB p, tListB* bL);

void printJob(tItemB item);

void cmdListjobs(tArgs args, tLists *L)
{
    UNUSED(args);

    tListB bL = L->background; tItemB item; 

    for (tPosB p = firstB(bL); p != PNULL; p = nextB(p, bL))
    {
        item = getItemB(p, bL);

        updateItemjobs(&item, p, &bL);

        printJob(item);
    }
}

const char* getStateName(int state)
{
    if (state & S_ACTIVE)   return "ACTIVE";
    if (state & S_FINISHED) return "FINISHED";
    if (state & S_SIGNALED) return "SIGNALED";
    if (state & S_STOPPED)  return "STOPPED";
    return "UNKNOWN";
}

void updateItemjobs(tItemB *item, tPosB p, tListB* bL)
{
    if (item->status & S_ACTIVE_STOPED) // child has not finished
    {
        int status; int pid = item->pid;
        pid_t result = waitpid(pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        
        if (result > 0) // State has changed
        {   
            if (WIFEXITED(status)) // finish
            { 
                item->status = S_FINISHED;
                item->sin = WEXITSTATUS(status); 
            }
            else if (WIFSIGNALED(status)) // signaled
            { 
                item->status = S_SIGNALED;
                item->sin = WTERMSIG(status); 
            }
            else if (WIFSTOPPED(status)) // stopped
            {   
                item->status = S_STOPPED;
                item->sin = WSTOPSIG(status);
            }
            else if (WIFCONTINUED(status)) // continued -> active
            {
                item->status = S_ACTIVE;
                item->sin = 0;
            }

            updateItemB(*item, p, bL);
        }
    }
}

void printJob(tItemB item)
{
    struct passwd *pw = getpwuid(getuid());

    // PID USER PRIORITY
    printf(YELLOW"%7d"MAGENTA" %11s"RST" p=%2d ", item.pid, pw->pw_name, 
           getpriority(PRIO_PROCESS, item.pid));
    
    // DATE CLOCK
    printDate(item.time); printf(" "); printTime(item.time);
    
    // STATUS
    printf(" %s ", getStateName(item.status));

    if (item.status & S_SIGNALED)
        printf("(%s)", getNameSin(item.sin));
    else
        printf("(%03d)", item.sin);
    
    printf(" %s\n", item.commandLine);
}

/******************************************************************************/
// deljobs -term|-sig
#define O_TERM (1 << 0)  // 0001 (1)
#define O_SIG  (1 << 1)  // 0010 (2)

void doDelJobs(tListB *bL, byte opt);

void cmdDeljobs(tArgs args, tLists *L)
{
    switch (args.len)
    {
    case 1:  // do listjobs
        cmdListjobs(args, L);
        break;
    case 2: // -term or -sig
        if (strcmp(args.array[1], "-term") == 0) // do -term
            doDelJobs(&L->background, O_TERM);            
        else if (strcmp(args.array[1], "-sig") == 0) // do -sig
            doDelJobs(&L->background, O_SIG);            
        else
            printError(args.array[0], "Invalid argument");
        break;
   default:
        printError(args.array[0], "Invalid num of arguments");
        break;
    }    
}

void doDelJobs(tListB *bL, byte opt)
{
    tItemB item; tPosB p = firstB(*bL);

   
    while (p != PNULL)
    {
        item = getItemB(p, *bL);

        updateItemjobs(&item, p, bL);

        if (opt & O_TERM && item.status == S_FINISHED)
        {
            tPosB aux = nextB(p, *bL);
            deleteAtPositionB(p, bL);
            p = aux;                 
            continue;                
        }

        if (opt & O_SIG && item.status == S_SIGNALED)
        {
            tPosB aux = nextB(p, *bL);
            deleteAtPositionB(p, bL); 
            p = aux;                  
            continue;                 
        }

        p = nextB(p, *bL);

        printJob(item);
    } 
}

/******************************************************************************/