#include "historyList.h"
#include <time.h>
#include "p0lib.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdbool.h>


//Date [-t|-d]

void dateCmd (tArgs args, tListH history){
    UNUSED(history);
    if (args.len > 2)
    {
        
        printf("\033[1;31mError: date [-t|-d]\033[0m\n"); //Esto en principio saca en rojo el texto
        return;
    } else {
        switch (args.len)
        {
        case 1:
            date();
            timeC();
            break;

        case 2:
            if (strcmp(args.array[1], "-t") == 0)
            {
                timeC();
            } else if (strcmp(args.array[1], "-d") == 0)
            {
                date();
            } else {
                printf("\033[1;31mError: date [-t|-d]\033[0m\n");
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
    }

    struct tm *local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;

    printf("Hoy es %d/%d/%d\n", day, month, year);
}

void timeC(){   //Lo nombro así para que no choque con
                //la función time
    int hours, minutes, seconds;
    time_t now;
    time(&now);

    if (now == -1){
        perror("Error al obtener la fecha:");
    }

    struct tm *local = localtime(&now);

    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    printf("Son las: %d:%d:%d\n", hours, minutes, seconds);
}


//Authors [-l|-n]

void authorsCmd (tArgs args, tListH history){
    UNUSED(history);
    if (args.len > 2)
    {
        printf("\033[1;31mError: authors [-l|-n]\033[0m\n");
        return;
    } else {
        switch (args.len)
        {
        case 1:
            autName();
            autLogin();
            break;

        case 2:
            if (strcmp(args.array[1], "-l") == 0)
            {
                autLogin();
            } else if (strcmp(args.array[1], "-n") == 0)
            {
                autName();
            } else {
                printf("\033[1;31mError: authors [-l|-n]\033[0m\n");
            }
            break;
        
        default:
            break;
        }
    }
      
}

void autName(){
    printf("José Manuel Villar \t Alex Borrazás\n");
}

void autLogin(){
    printf("jose.villarg@udc.es \t alexandre.bmancebo\n"); //Ponte el login aquí 
}

//Pid

void pidCmd(tArgs args, tListH history){
    UNUSED(history);
    if (args.len != 1){
        printf("\033[1;31mError: pid\033[0m\n");
    } else {
        int pid;
        pid = getpid();
        printf("%d\n", pid);
    }
}

//Ppid

void ppidCmd(tArgs args, tListH history){
    UNUSED(history);
    if (args.len != 1){
        printf("\033[1;31mError: ppid\033[0m\n"); 
    } else {
        int ppid;
        ppid = getppid();
        printf("%d\n", ppid);
    }
}

//Quit, Bye, Exit

void exitCmd(bool *end) {
    *end = true;
}
