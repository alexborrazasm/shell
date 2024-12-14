#ifndef LIBP3_H
#define LIBP3_H

#include "types.h"
#include "shell.h"

// getuid
void cmdGetuid(tArgs args, tLists *L);

// setuid [-l] id
void cmdSetuid(tArgs args, tLists *L);

// showvar v1 v2 ...
void cmdShowvar(tArgs args, tLists *L);

// changevar [-a|-e|-p] var val
void cmdChangevar(tArgs args, tLists *L);

// subsvar [-a|-e] v1 v2 val
void cmdSubsvar(tArgs args, tLists *L);

// environ [-environ|-addr] 
void cmdEnviron(tArgs args, tLists *L);

// fork
void cmdFork(tArgs args, tLists *L);

// search [-add dir -del dir -clear -path]
void cmdSearch(tArgs args, tLists *L);

// exec progspec
void cmdExec(tArgs args, tLists *L);

// execpri prio progspec
void cmdExecpri(tArgs args, tLists *L);

// fg progspec
void cmdFg(tArgs args, tLists *L);

// fgpri prio progspec
void cmdFgpri(tArgs args, tLists *L);

// back progspec
void cmdBack(tArgs args, tLists *L);

// backpri prio progspec
void cmdBackpri(tArgs args, tLists *L);

// listjobs
void cmdListjobs(tArgs args, tLists *L);

// deljobs -term|-sig 
void cmdDeljobs(tArgs args, tLists *L);

void getProgspec(tArgs *args, tProgspec *pg, int start);

int doExecuteFg(char **args, char **newEnv, int *pprio, tListP L);

void freeEnv(char **envp);

void searchPath(tListP *L);

#endif // LIBP3_H