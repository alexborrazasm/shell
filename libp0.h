#include "historicList.h"

void cmdExit(bool *end);//Exit/Quit/Bye

void cmdHistoric(tArgs args, tListH historic);

void cmdDate (tArgs args, tListH historic);  //Date [-t|-d]

void cmdPid(tArgs args, tListH historic);
void cmdPpid(tArgs args, tListH historic);

void cmdAuthors (tArgs args, tListH historic);

void cmdChdir (tArgs args, tListH historic);

