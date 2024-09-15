#ifndef LIBP0_H
#define LIBP0_H

#include "types.h"  // For tLists and tArgs
#include "historicList.h"

void cmdExit(tArgs args, bool *end);//Exit/Quit/Bye

void cmdHistoric(tArgs args, tLists L);

void cmdDate (tArgs args, tLists L);  //Date [-t|-d]

void cmdPid(tArgs args, tLists L);

void cmdPpid(tArgs args, tLists L);

void cmdAuthors (tArgs args, tLists L);

void cmdChdir (tArgs args, tLists L);

void cmdInfosys (tArgs args, tLists L);
#endif
