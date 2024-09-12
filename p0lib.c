#include "historyList.h"
/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdbool.h>
*/

void quit(bool *end) 
{
    *end = true;
}

void dateCmd (tArgs args, tListH history)
{
    UNUSED(args); UNUSED(history);
    puts("esta es la data");
}
//Date [-t|-d]

/*
void dateCmd (int numParts, char* parts[]){
    if (numParts > 2)
    {
        printf("\033[1;31mError: date [-t|-d]\033[0m\n"); //Esto en principio saca en rojo el texto
        return;
    } else {
        switch (numParts)
        {
        case 1:
            date();
            time();
            break;

        case 2:
            if (srtcmp(parts[1], "-t"))
            {
                time();
            } else if (srtcmp(parts[1], "-d"))
            {
                date();
            }
            break;
        
        default:
            break;
        }
    }
      
}

void date(){

    int day, month, year;
    time_t now;
    time(&now);
    if (now == -1){
        perror("Error al obtener la hora:");
        return;
    }

    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf("Hoy es %d/%d/%d\n", day, month, year);
}

void time(){

    int hours, minutes, seconds;
    time_t now;
    time(&now);

    if (now == -1){
        perror("Error al obtener la fecha:");
        return;
    }

    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf("Son las: %d:%d:%d\n", hours, minutes, seconds);
}


//Authors [-l|-n]

void authorsCmd (int numParts, char* parts[]){
    if (numParts > 2)
    {
        printf("\033[1;31mError: authors [-l|-n]\033[0m\n");
        return;
    } else {
        switch (numParts)
        {
        case 1:
            autName();
            autLogin();
            break;

        case 2:
            if (srtcmp(parts[1], "-l"))
            {
                autLogin();
            } else if (srtcmp(parts[1], "-n"))
            {
                autName();
            }
            break;
        
        default:
            break;
        }
    }
      
}

void autName(){
    printf("José Manuel Villar \t Alex Borrazás");
}

void autLogin(){
    prntf("jose.villarg@udc.es \t alexandre.bmancebo"); //Ponte el login aquí 
}
//Separo exit de exitCmd por si necesitamos usar solo
//exit en algún punto
void exitCmd(int numParts, bool *finished){
      if (numParts != 1){
        printf("\033[1;31mError: pid\033[0m");
      } else{
        exit(finished);
      }
}
void exit(bool *finished){
    *finished = true;
}

void pidCmd(int numParts, char* parts[]){
      if (numParts != 1){
        printf("\033[1;31mError: pid\033[0m");
    } else {
        int pid;
        pid = getpid();
        printf("%d\n", pid);
    }
}


void ppidCmd(int numParts, char* parts[]){
    if (numParts != 1){
        printf("\033[1;31mError: ppid\033[0m"); 
    } else {
        int ppid;
        ppid = getppid();
        printf("%d\n", ppid);
    }
}
*/