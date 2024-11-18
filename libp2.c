#include "libp2.h"

/******************************************************************************/
// allocate
void printMenList(tListM L, byte type);

void allocateMalloc(tArgs args, tListM *L);

void allocateMMap(tArgs args, tListM *L);

void allocateCreateShared(tArgs args, tListM *L);

void allocateShared(tArgs args, tListM *L);

long getSize(char *size);

// Free me
char* auxPrintMenList(tItemM item);

void printHead(byte type);

void* mapFile(char* file, int protection, tArgs args, tListM *L);

void cmdAllocate(tArgs args, tLists *L)
{
   switch (args.len)
   {
   case 1: // call print list 
      printMenList(L->memory, M_ALL); 
      break;
   case 2: // check args length = 2, print item list if = args[1]
      if (strcmp(args.array[1], "-malloc") == 0)
         printMenList(L->memory, M_MALLOC);
      else if (strcmp(args.array[1], "-mmap") == 0)
         printMenList(L->memory, M_MMAP);
      else if (strcmp(args.array[1], "-createshared") == 0)
         printMenList(L->memory, M_SHARED);
      else if (strcmp(args.array[1], "-shared") == 0)
         printMenList(L->memory, M_SHARED);
      else
         printError(args.array[0], "Invalid argument");
      break;
   case 3: // check args length = 3, malloc shared
      if (strcmp(args.array[1], "-malloc") == 0)
         allocateMalloc(args, &L->memory);
      else if (strcmp(args.array[1], "-shared") == 0)
         allocateShared(args, &L->memory);
      else if (strcmp(args.array[1], "-mmap") == 0)
         allocateMMap(args, &L->memory);
      else
         printError(args.array[0], "Invalid argument");
      break;
   case 4: // check args length = 4, nmap createshared
      if (strcmp(args.array[1], "-mmap") == 0)
         allocateMMap(args, &L->memory);
      else if (strcmp(args.array[1], "-createshared") == 0)
         allocateCreateShared(args, &L->memory);
      else
         printError(args.array[0], "Invalid argument");
      break;
   default:
      printError(args.array[0], "Invalid num of arguments");
      break;
   }
}

long getSize(char *size) // Input size to int
{
   long result;

   if (!stringToLong(size, &result)) { return -1; }
   
   if (result <= 0) { return -1; }

   return result;
}

char* auxPrintMenList(tItemM item)
{
   char* str = malloc(512 * sizeof(char));

   switch (item.type)
   {
   case M_MALLOC:
      strcpy(str, "malloc");
      return str;
   case M_MMAP:
      sprintf(str, "%s (df %d)", item.name, item.keyDF);
      return str;
   case M_SHARED:
      sprintf(str, "shared (key %d)", item.keyDF);
      return str;
   default:
      strcpy(str, "error");
      return str;
   }
}

void printHead(byte type)
{
   switch (type)
   {
   case M_ALL:
      printf("******List of blocks allocated for process "BLUE"%d\n"RST,
             getpid());
      break;
   case M_MALLOC:
      printf("******List of malloc allocated blocks for process "BLUE"%d\n"RST,
             getpid());
      break;
   case M_MMAP:
      printf("******List of mmap allocated blocks for the process "BLUE"%d\n"RST,
             getpid());
      break;
   case M_SHARED:
      printf("******List of blocks allocated shared for the process "BLUE"%d\n"RST,
            getpid());
      break;
   default:
      printError("printHead", "Head table error");
      break;
   }
}

void printMenList(tListM L, byte type)
{
   tPosM p = firstM(L); tItemM item; char* msg;

   printHead(type);

   for (; p != MNULL; p = nextM(p, L))
   {
      item = getItemM(p, L);
      if (item.type & type)
      {
         printf(GREEN"%20p %10ld ", item.address, item.size);
         printDateString(item.date);
         msg = auxPrintMenList(item);
         printf(" %s\n", msg);
         free(msg);
      }
   }
}

void* mapFile(char* file, int protection, tArgs args, tListM *L)
{
   int df, map=MAP_PRIVATE, mode=O_RDONLY;
   struct stat s;
   void *p;

   if (protection&PROT_WRITE)
      mode = O_RDWR;
   if (stat(file, &s) == -1 || (df = open(file, mode)) == -1)
      return NULL;
   if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
      return NULL;

   tItemM item;
   item.address = p;
   strcpy(item.name, file);
   item.type = M_MMAP;
   item.date = getTime(args);
   item.keyDF = df;
   item.size = s.st_size;

   insertItemM(item, MNULL, L);
   
   return p;
}

void allocateMalloc(tArgs args, tListM *L)
{
   long size = getSize(args.array[2]);
   void* block = NULL;
   tItemM item;

   if (size == -1) // Invalid size
   {
      printError(args.array[0], "Invalid size");
      return;
   }
   
   block = malloc(size);

   if (block == NULL) // Check malloc
   {
      printError(args.array[0], "Can't allocate memory");
      return;
   }

   // Prepare item to insert
   item.address = block;
   item.type= M_MALLOC;
   item.date = getTime(args);
   item.keyDF = -1;
   item.size = size;

   // Insert then
   insertItemM(item, MNULL, L);

   printf("Allocated "BLUE"%ld bytes"RST" at "GREEN"%p\n"RST, size, block);
}

void allocateMMap(tArgs args, tListM *L)
{
   char *perm; void *p; int protection = 0;

   if ((perm=args.array[3])!=NULL && strlen(perm)<4) 
   {
      if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
      if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
      if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
   }

   if ((p = mapFile(args.array[2], protection, args, L)) == NULL)
      pPrintError(args.array[0]);
   else
      printf("file "BLUE"%s"RST" mapped to "GREEN"%p\n"RST, args.array[2], p);
}

void allocateCreateShared(tArgs args, tListM *L)
{
   key_t cl;
   size_t tam;
   void *p;

   cl=(key_t)  strtoul(tr[0],NULL,10);
   tam=(size_t) strtoul(tr[1],NULL,10);
   if (tam==0) {
   printf ("No se asignan bloques de 0 bytes\n");
   return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL)
      printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   else
      printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}
/*
void * ObtenerMemoriaShmget (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     // tam distito de 0 indica crear
        flags=flags | IPC_CREAT | IPC_EXCL; // cuando no es crear pasamos de tamano 0
    if (clave==IPC_PRIVATE)  //no nos vale
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); // si no es crear, necesitamos el tamano, que es s.shm_segsz
 // Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave);
    return (p);
}
*/

void allocateShared(tArgs args, tListM *L)
{
   UNUSED(L); UNUSED(args);
}

/*
void do_AllocateShared (char *tr[])
{
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL) {
      ImprimirListaShared(&L);
      return;
   }

   cl=(key_t)  strtoul(tr[0],NULL,10);

   if ((p=ObtenerMemoriaShmget(cl,0))!=NULL)
      printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
   else
      printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
} 
*/

/******************************************************************************/
// deallocate
void deallocateMalloc(tArgs args, tListM *L);

void deallocateMMap(tArgs args, tListM *L);

void deallocateShared(tArgs args, tListM *L);

void deallocateDelKey(tArgs args, tListM *L);

void deallocateAddr(tArgs args, tListM *L);

void* stringToVoidPointer(char* str);

void cmdDeallocate(tArgs args, tLists *L)
{
   switch (args.len)
      {
      case 1: // call print list 
         printMenList(L->memory, M_ALL); 
         break;
      case 2: // check args length = 2, print item list if = args[1]
         if (args.array[1][0] == '-')
         {
            if (strcmp(args.array[1], "-malloc") == 0)
               printMenList(L->memory, M_MALLOC);
            else if (strcmp(args.array[1], "-mmap") == 0)
               printMenList(L->memory, M_MMAP);
            else if (strcmp(args.array[1], "-shared") == 0)
               printMenList(L->memory, M_SHARED);
            else if (strcmp(args.array[1], "-delkey") == 0)
               printMenList(L->memory, M_SHARED);
            else
               printError(args.array[0], "Invalid argument");
         }
         else // deallocate addr
            deallocateAddr(args, &L->memory);
         break;
      case 3: // check args length = 3
         if (strcmp(args.array[1], "-malloc") == 0)
            deallocateMalloc(args, &L->memory);
         else if (strcmp(args.array[1], "-mmap") == 0)
            deallocateMMap(args, &L->memory);
         else if (strcmp(args.array[1], "-shared") == 0)
            deallocateShared(args, &L->memory);
         else if (strcmp(args.array[1], "-delkey") == 0)
            deallocateDelKey(args, &L->memory);
         else
            printError(args.array[0], "Invalid argument");
         break;
      default:
         printError(args.array[0], "Invalid num of arguments");
         break;
      }
}

void deallocateMalloc(tArgs args, tListM *L)
{  
   long size = getSize(args.array[2]);

   if (size == -1) 
   { 
      printError(args.array[0], "Error size");
      return; 
   }
   
   tPosM p = findMalloc(size, *L);

   if (p != MNULL)
   {
      tItemM item = getItemM(p, *L);
      
      free(item.address);
      deleteAtPositionM(p, L);
   }
   else
   {
      printf("There is no block of size "BLUE"%ld"RST" allocated with malloc\n",
             size);
   }
}

void deallocateMMap(tArgs args, tListM *L)
{
   tPosM p = findMMap(args.array[2], *L);

   if (p != MNULL)
   {
      tItemM item = getItemM(p, *L);
      
      // Unmap the file
      if (munmap(item.address, item.size) == -1) 
      {
         pPrintError(args.array[0]);
         return;
      }
      deleteAtPositionM(p, L);
   }
   else
   {
      printf("File "BLUE"%s"RST" was not mapped\n", args.array[2]);
   }
}

void deallocateShared(tArgs args, tListM *L)
{
   UNUSED(args); UNUSED(L);
}

void deallocateDelKey(tArgs args, tListM *L)
{
   UNUSED(args); UNUSED(L);
}
/* 
void do_DeallocateDelkey (char *args[])
{
   key_t clave;
   int id;
   char *key=args[0];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}
 */

void* stringToVoidPointer(char* str) 
{   
   // Convert the string to an unsigned long using base 16 (hexadecimal)
   unsigned long address = strtoul(str, NULL, 16);  
   
   return (void*)address;  // Cast the result to void*
}

void deallocateAddr(tArgs args, tListM *L)
{
   void* ptr = stringToVoidPointer(args.array[1]);
   tPosM p = findByAddress(ptr, *L);

   if (p != MNULL)
   {
      tItemM item = getItemM(p, *L);

      switch (item.type)
      {
      case M_MALLOC:
         free(item.address);
         break;
      case M_MMAP:
         // Unmap the file
         if (munmap(item.address, item.size) == -1) 
         {
            pPrintError(args.array[0]);
            return;
         }
         break;
      case M_SHARED:
         puts("TO DO");
         break;
      default:
         printError(args.array[0], "Unknown block type");
         return;
      }
      deleteAtPositionM(p, L);
   }
   else
   {
      printf("Address "GREEN"%p"RST" not allocated with malloc, shared or mmap\n",
      ptr);
   }
}

/******************************************************************************/
// menfill
void cmdMemfill(tArgs args, tLists *L)
{
   UNUSED(args);
   UNUSED(L);
}
/******************************************************************************/
// mendump
void cmdMemdump(tArgs args, tLists *L)
{
   UNUSED(args);
   UNUSED(L);
}
/******************************************************************************/
// memory [-blocks|-funcs|-vars|-all|-pmap]
void memoryBlocks(tLists L);

void memoryFuncs();

void memoryVars();

void memoryAll(tLists L);

void memoryPmap();

void cmdMemory(tArgs args, tLists *L)
{
   switch (args.len)
   {
   case 1: // call -all 
      memoryAll(*L);
      break;
   case 2: // check args
      if (strcmp(args.array[1], "-blocks") == 0)
         memoryBlocks(*L);
      else if (strcmp(args.array[1], "-funcs") == 0)
         memoryFuncs();
      else if (strcmp(args.array[1], "-vars") == 0)
         memoryVars();
      else if (strcmp(args.array[1], "-all") == 0)
         memoryAll(*L);
      else if (strcmp(args.array[1], "-pmap") == 0)
         memoryPmap();
      else
         printError(args.array[0], "Invalid argument");
      break;

   default:
      printError(args.array[0], "Invalid num of arguments");
      break;
   }
}

void memoryBlocks(tLists L)
{
   printMenList(L.memory, M_ALL);
}

void memoryFuncs()
{
   // program functions
   void (*pFunc1)() = pPrintError;
   void (*pFunc2)() = processInput;
   void (*pFunc3)(tLists *) = freeLists;

   // library functions
   void (*libFunc1)(int) = exit;
   void *(*libFunc2)(size_t) = malloc;
   int (*libFunc3)(const char *restrict, ...) = printf;

   // Print
   printf("Program functions" GREEN "%20p" RST "," GREEN "%20p" RST "," GREEN
          "%20p" RST "\n", (void *)pFunc1, (void *)pFunc2, (void *)pFunc3);
   printf("Library functions" GREEN "%20p" RST "," GREEN "%20p" RST "," GREEN
          "%20p" RST "\n", (void *)libFunc1, (void *)libFunc2, (void *)libFunc3);
}

// Declaration of external variables (defined in another file)
extern int ext_var1, ext_var2, ext_var3;
extern int ext_init_var1, ext_init_var2, ext_init_var3;

// Global static variables (accessible only within this file)
static int static_var1, static_var2, static_var3;
static int static_init_var1 = 10, static_init_var2 = 20, static_init_var3 = 30;

void memoryVars()
{
   // Automatic (local) variables
   int auto_var1 = 1, auto_var2 = 2, auto_var3 = 3;

   // Print
   printf("Local variables"GREEN"%22p" RST "," GREEN "%20p" RST "," GREEN "%20p"
          RST "\n", &auto_var1, &auto_var2, &auto_var3);
   printf("Global external" GREEN "%22p" RST "," GREEN "%20p" RST "," GREEN 
          "%20p" RST "\n", &ext_init_var1, &ext_init_var2, &ext_init_var3);
   printf("Global external(N.I)" GREEN "%17p" RST "," GREEN "%20p" RST "," GREEN
          "%20p" RST "\n", &ext_var1, &ext_var2, &ext_var3);
   printf("Global static" GREEN "%24p" RST "," GREEN "%20p" RST "," GREEN "%20p"
          RST "\n", &static_init_var1, &static_init_var2, &static_init_var3);
   printf("Global static(N.I)" GREEN "%19p" RST "," GREEN "%20p" RST "," GREEN 
          "%20p" RST "\n", &static_var1, &static_var2, &static_var3);
}

void memoryAll(tLists L)
{
   memoryVars();
   memoryFuncs();
   memoryBlocks(L);
}

void memoryPmap()
{
   pid_t pid;
   char elpid[32];
   char *argv[4] = {"pmap", elpid, NULL};

   sprintf(elpid, "%d", (int)getpid());

   if ((pid = fork()) == -1)
   {
      pPrintError("Can't create process");
      return;
   }

   if (pid == 0)
   {
      if (execvp(argv[0], argv) == -1) // try pmap
         pPrintError("Cannot execute pmap (linux, solaris)");

      argv[0] = "procstat";
      argv[1] = "vm";
      argv[2] = elpid;
      argv[3] = NULL;
      if (execvp(argv[0], argv) == -1) // not pmap, try procstat FreeBSD
         pPrintError("Cannot execute procstat (FreeBSD)");

      argv[0] = "procmap", argv[1] = elpid;
      argv[2] = NULL;
      if (execvp(argv[0], argv) == -1) // try procmap OpenBSD
         pPrintError("Cannot execute procmap (OpenBSD)");

      argv[0] = "vmmap";
      argv[1] = "-interleave";
      argv[2] = elpid;
      argv[3] = NULL;
      if (execvp(argv[0], argv) == -1) // Try vmmap Mac-OS
         pPrintError("Cannot execute vmmap (Mac-OS)");
      exit(1);
   }
   waitpid(pid, NULL, 0);
}

/******************************************************************************/
// readfile
void cmdReadfile(tArgs args, tLists *L)
{
   switch (args.len)
   {
   case 3:
      //LeerFichero();
      break;
   case 4:
      //   
   
   default:
      printError(args.array[0],"");//TODO
      break;
   }
   UNUSED(L);
}


void Cmd_ReadFile (tArgs args)
{
   void *p;
   size_t cont=-1;  // si no pasamos tamano se lee entero
   ssize_t n;
   
   p=cadtop(args.array[2]);  // convertimos de cadena a puntero
   if (args.array[3]!=NULL)
   cont=(size_t) atoll(args.array[3]);

   if ((n=LeerFichero(args.array[1],p,cont))==-1)
   pPrintError(args.array[0]);
   else
   printf ("leidos %lld bytes de %s en %p\n",(long long) n,args.array[1],p);
}


ssize_t LeerFichero (char *f, void *p, size_t cont)
{
   struct stat s;
   ssize_t  n;
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
   return -1;
   if (cont==-1)   // si pasamos -1 como bytes a leer lo leemos entero
   cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
   aux=errno;
   close(df);
   errno=aux;
   return -1;
   }
   close (df);
   return n;
}




/******************************************************************************/
// writefile
void cmdWritefile(tArgs args, tLists *L)
{
   UNUSED(args);
   UNUSED(L);
}
/******************************************************************************/
// read
void cmdRead(tArgs args, tLists *L)
{
   UNUSED(args);
   UNUSED(L);
}
/******************************************************************************/
// write
void cmdWrite(tArgs args, tLists *L)
{
   UNUSED(args);
   UNUSED(L);
}
/******************************************************************************/
// recurse
void rec(int n);

void cmdRecurse(tArgs args, tLists *L)
{
   UNUSED(L);

   switch (args.len)
   {
   case 1:
      printError(args.array[0], "Not argument");
      break;

   case 2:
      int n;

      if (stringToInt(args.array[1], &n))
         rec(n);
      else
         printError(args.array[0], "Invalid argument");
      break;

   default:
      printError(args.array[0], "Invalid num of arguments");
      break;
   }
}

void rec(int n)
{
   char autoArray[SIZE_REC];
   static char staticArray[SIZE_REC];

   printf("param:%3d(" GREEN "%p" RST ") array (" GREEN "%p" RST "),"
          " arr static (" GREEN "%p" RST ")\n",
          n, &n, autoArray, staticArray);

   if (n > 0)
      rec(n - 1);
}

/*
void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
      arr[i]=byte;
}

*/