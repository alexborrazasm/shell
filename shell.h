#ifndef SHELL_H
#define SHELL_H

#include "types.h"
#include "historicList.h"
#include "filesList.h"
#include "memoryList.h"
#include "backgroundList.h"
#include "pathList.h"
#include "libp0.h"
#include "libp1.h"
#include "libp2.h"
#include "libp3.h"

void printPrompt();

bool readInput(tLists *L);

void processInput(tLists *L, bool *end, tArgsMain main);

void selectCommand(tArgs args, tLists *L, bool *end);

void freeLists(tLists *L);

int stringCut(char *input, char *parts[]);

void printError(char *name, char *msg);

void pPrintError(char *name);

void pPrintErrorFile(char *name, char *filename);

int getCommandsLen();

void getCommands(tCommand *pointer);

void initFList(tListF *L);

bool stringToInt(char *str, int *n);

bool stringToLong(char *str, long *n);

time_t getTime(tArgs args);

// 00:00
void printTime(time_t now);

// dd/mm/yyyy
void printDate(time_t now);

// Nov 31 00:00
void printDateString(time_t now);

void freeBackgroundList(tListB *list);

void freePathList(tListP *list);

#endif // SHELL_H