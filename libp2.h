#ifndef LIBP2_H
#define LIBP2_H

#include "types.h"

void cmdAllocate(tArgs args, tLists *L);

void cmdDeallocate(tArgs args, tLists *L);

void cmdMemfill(tArgs args, tLists *L);

void cmdMemdump(tArgs args, tLists *L);

void cmdMemory(tArgs args, tLists *L);

void cmdReadfile(tArgs args, tLists *L);

void cmdWritefile(tArgs args, tLists *L);

void cmdRead(tArgs args, tLists *L);

void cmdWrite(tArgs args, tLists *L);

void cmdRecurse(tArgs args, tLists *L);

#endif // LIBP2_H