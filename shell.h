#include "libp0.h"

void printPrompt();

void readInput(char* input, tArgs *args, tListH *history);

void processInput(tArgs args, tListH history, bool *end);

void freeHistoryList(tListH* list);