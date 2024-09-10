#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

//Date [-t|-d]

void dateCmd (int numParts, char* parts[]){
    if (numParts > 2)
    {
        printf("Error: date [-t|-d]\n");
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
                date()
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
        printf("Error: authors [-l|-n]\n");
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
                autName()
            }
            break;
        
        default:
            break;
        }
    }
      
}

void autName(){
    printf("José Manuel Villar \t Alex Borrazás")
}

void autLogin(){
    prntf("jose.villarg@udc.es \t") //Ponte el login aquí 
}

void exitCmd(bool finished){
    finished = true;
}