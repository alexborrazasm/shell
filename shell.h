#include "libp0.h"

void printPrompt();

void readInput(char* input, tArgs *args, tListH *history);

void processInput(tArgs args, tListH history, bool *end);

int stringCut(char* input, char* parts[]);

void freeHistoryList(tListH* list);