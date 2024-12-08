#include "libp3.h"

extern char **environ;  // Declaration of environ

void printEnv(char *envp[], bool environ);

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
int doChangeVar(char *var, char *value, char *env[]);

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
                if(doChangeVar(args.array[2], args.array[3], envp) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
            else if(args.array[1][1] == 'e') // environ
            {
                if(doChangeVar(args.array[2], args.array[3], environ) == -1)
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

int doChangeVar(char *var, char *value, char *env[]) 
{
    int pos; char *aux;
   
    if((pos = searchVar(var, env)) == -1)
        return -1;
 
    if((aux = (char *)malloc(strlen(var)+strlen(value)+2)) == NULL)
	    return -1;

    strcpy(aux, var);
    strcat(aux, "=");
    strcat(aux, value);

    env[pos] = aux;
    return pos;

    // NOTE: Do not free envVar because environment directly uses this pointer
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
int doSubsVar(char *v1, char *v2, char *value, char *env[]);

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
                if(doSubsVar(array[2], array[3], array[4], envp) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
            else if(args.array[1][1] == 'e') // environ
            {
                if(doSubsVar(array[2], array[3], array[4], environ) == -1)
                    pPrintError(args.array[0]);
                return; // all right
            }
        }
    }

    // Error   
    printError(args.array[0], "Subsvar [-a|-e] v1 v2 val");
}

int doSubsVar(char *v1, char *v2, char *value, char *env[])
{
    int pos; char *aux;
   
    if((pos = searchVar(v1, env)) == -1)
        return -1;
 
    if((aux = (char *)malloc(strlen(v2) + strlen(value) + 2)) == NULL)
	    return -1;

    strcpy(aux, v2);
    strcat(aux, "=");
    strcat(aux, value);

    env[pos] = aux;
    return pos;

    // NOTE: Do not free envVar because environment directly uses this pointer   
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
            exit(EXIT_FAILURE); 
        }
    } 
    else 
    {   // Father wait
        int status;
        if (waitpid(pid, &status, 0) == -1) { return -1; }
    }

    return 0; // All right
}

int doExecuteBg(char **args, char **newEnv, int *pprio, tListP L)
{
    pid_t pid = fork(); // Create a child process

    if (pid == -1) { return -1; }

    if (pid == 0) // Child execute 
    {   
        if (setsid() == -1) {
            pPrintError(args[0]);
            exit(EXIT_FAILURE);
        }

        if(doExecpve(args, newEnv, pprio, L) == -1 )
        { 
            pPrintError(args[0]);
            exit(EXIT_FAILURE); 
        }
    } 

    return 0; // All right
}

// i = start
int findEnvEnd(char *args[], int i) 
{    

    while (args[i] != NULL && strchr(args[i], '=')) { i++; }

    return i;
}

int addEnvVar(char ***envp, char *new, int i)
{
    if (!new) return -1;

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

// Remember free pg.env
void getProgspec(tArgs *args, tProgspec *pg, int start)
{
    int endVars = findEnvEnd(args->array, start);

    if (endVars == start)
    {
        pg->env = NULL;
        pg->commands = args->array + start;
        pg->len = args->len - start;
    }
    else
    {
        char **env = NULL;
        int envCount = endVars - start;

        for (int i = 0; i < envCount; i++) 
        {
            if (addEnvVar(&env, args->array[start + i], i) == -1) 
            {
                pPrintError(args->array[0]);
                if (env) { freeEnv(env); }
                return;
            }
        }
    
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

/******************************************************************************/
// back progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdBack(tArgs args, tLists *L)
{
    tProgspec pg; // free?  

    getProgspec(&args, &pg, 1);
        
    if(doExecuteFg(pg.commands, pg.env, NULL, L->path) == -1)
    {
        pPrintError(args.array[0]);
    }
}

/******************************************************************************/
// backpri prio progspec
// progspec = [VAR1 VAR2 VAR3 ....] executablefile [arg1 arg2......]
void cmdBackpri(tArgs args, tLists *L)
{
    tProgspec pg; int prio; // free?

    if (!stringToInt(args.array[1], &prio)) 
    {
        printError(args.array[0], "Invalid prio");
        return;
    }    

    getProgspec(&args, &pg, 2);
        
    if(doExecuteBg(pg.commands, pg.env, &prio, L->path) == -1)
    {
        pPrintError(args.array[0]);
    }
}

/******************************************************************************/
// listjobs
void cmdListjobs(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// deljobs -term|-sig 
void cmdDeljobs(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/