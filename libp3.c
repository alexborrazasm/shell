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
        printError(args.array[0], "Not acept arguments");
}

void doGetuid()
{
    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();
    struct passwd *realPw = getpwuid(realUid);
    struct passwd *effectivePw = getpwuid(effectiveUid);

    printf("Credencial real: "GREEN"%d"RST", ("BLUE"%s"RST")\n",
           realUid, realPw ? realPw->pw_name : "unknown");
    printf("Credencial efective: "GREEN"%d"RST", ("BLUE"%s"RST")\n",
           effectiveUid, effectivePw ? effectivePw->pw_name : "unknown");
}

/******************************************************************************/
// setuid [-l] id
void cmdSetuid(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
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
void cmdChangevar(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// subsvar [-a|-e] v1 v2 val
void cmdSubsvar(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
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
        doFork(&L->backgroud);
    else
        printError(args.array[0], "Not acept arguments");
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
// exec progspec
void cmdExec(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// execpri prio progspec
void cmdExecpri(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// fg progspec
void cmdFg(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// fgpri prio progspec
void cmdFgpri(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// back progspec
void cmdBack(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// backpri prio progspec
void cmdBackpri(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
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

/*
help

int BuscarVariable (char * var, char *e[])  //busca una variable en el entorno que se le pasa como parÃ¡metro
{                                           //devuelve la posicion de la variable en el entorno, -1 si no existe
  int pos=0;
  char aux[MAXVAR];
  
  strcpy (aux,var);
  strcat (aux,"=");
  
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else 
      pos++;
  errno=ENOENT;   //no hay tal variable
  return(-1);
}


int CambiarVariable(char * var, char * valor, char *e[]) //cambia una variable en el entorno que se le pasa como parÃ¡metro
{                                                        //lo hace directamente, no usa putenv
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

char * Ejecutable (char *s)
{
        static char path[MAXNAME];
        struct stat st;
        char *p;
   
        if (s==NULL || (p=SearchListFirst())==NULL)
                return s;
        if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;        //is an absolute pathname
        
        strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);
        if (lstat(path,&st)!=-1)
                return path;
        while ((p=SearchListNext())!=NULL){
            strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);
            if (lstat(path,&st)!=-1)
                   return path;
        }
        return s;
}

int Execpve(char *tr[], char **NewEnv, int * pprio)
{
    char *p;               //NewEnv contains the address of the new environment
                           //pprio the address of the new priority
                           //NULL indicates no change in environment and/or priority
    if (tr[0]==NULL || (p=Ejecutable(tr[0]))==NULL){
        errno=EFAULT;
        return-1;
        }
    if (pprio !=NULL  && setpriority(PRIO_PROCESS,getpid(),*pprio)==-1 && errno){
                        printf ("Imposible cambiar prioridad: %s\n",strerror(errno));
                        return -1;
        }

        if (NewEnv==NULL)
                return execv(p,tr);
        else
                return execve (p, tr, NewEnv);
}

//las siguientes funciones nos permiten obtener el nombre de una senal a partir
del nÃºmero y viceversa 
static struct SEN sigstrnum[]={   
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
//senales que no hay en todas partes
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
	};    //fin array sigstrnum 


int ValorSenal(char * sen)  //devuelve el numero de senial a partir del nombre
{ 
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (!strcmp(sen, sigstrnum[i].nombre))
		return sigstrnum[i].senal;
  return -1;
}


char *NombreSenal(int sen)  //devuelve el nombre senal a partir de la senal
{			// para sitios donde no hay sig2str
 int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;
 return ("SIGUNKNOWN");
}

*/