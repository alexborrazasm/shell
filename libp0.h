#include "types.h"

int stringCut(char* input, char* parts[]);

void cmdExit(tArgs args, bool *end);//Exit/Quit/Bye

void cmdHistoric(tArgs args, tLists L);

void cmdDate (tArgs args, tLists L);  //Date [-t|-d]

void cmdPid(tArgs args, tLists L);

void cmdPpid(tArgs args, tLists L);

void cmdAuthors (tArgs args, tLists L);

void cmdChdir (tArgs args, tLists L);

void cmdInfosys (tArgs args, tLists L);

