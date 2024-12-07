#include "libp3.h"
/******************************************************************************/
// getuid
void cmdGetuid(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// setuid [-l] id
void cmdSetuid(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// showvar v1 v2 ...
void cmdShowvar(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
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
void cmdEnviron(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
}

/******************************************************************************/
// fork
void doFork(tListB *L);

void cmdFork(tArgs args, tLists *L)
{
    if(args.len == 1)
        doFork(&L->backgroud);
    else
        printError(args.array[0], "Fork not acept arguments");
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
void cmdSearch(tArgs args, tLists *L)
{
    UNUSED(args); UNUSED(L);
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