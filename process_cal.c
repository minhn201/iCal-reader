#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE_LEN 132
#define MAX_EVENTS 500

#define WEEK 7

// If input digit is 18:30, then resulting hr and min should be
// 18 and 30 respectively
void tmtok(int digit, int* hr, int* min){
        *hr = (int)(digit*.0001);
        *min = digit*.01 - (*hr)*100;
}

// Check if the given hour is after 12:00 AM, and print the value in a different
// format.
// E.g. Input: hr = 13
//             min = 30
// Print output: " 1:30 PM"
void formatted_time(int hr, int min){
    int hour[24] = {12,1,2,3,4,5,6,7,8,9,10,11,12,1,2,3,4,5,6,7,8,9,10,11};
    if(hr >= hour[12]){
        printf("%2d:%02d PM", hour[hr], min);
    }else{
        printf("%2d:%02d AM", hour[hr], min);
    }
}

// Print the content of the event
void print_summary(char location[132], char summary[132], int shr, int smin, int ehr, int emin){
    formatted_time(shr,smin);
    printf(" to ");
    formatted_time(ehr,emin);
    printf(": %s {{%s}}\n", summary, location);
}

// Take an string date input and create a readable string as output
// E.g. Input: 20220101 => Output: January 1st, 2022 (Sat)
void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t    full_time;
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)",
        localtime(&full_time));
}

//Produce an output string that respresent the date num_days after the input
//string.
//E.g. Input:  20220101
//     Output: 20220108 (+ nums_days = 7)
void dt_increment(char *after, const char *before, int const num_days)
{
    struct tm temp_time, *p_temp_time;
    time_t    full_time;
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2dT%2d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday, &temp_time.tm_hour,
        &temp_time.tm_min, &temp_time.tm_sec);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
    strftime(after, 16, "%Y%m%dT%H%M%S", localtime(&full_time));
    strncpy(after + 16, before + 16, MAX_LINE_LEN - 16);
    after[MAX_LINE_LEN - 1] = '\0';
}

//Structures
struct Dates{
    char event[132];
    char location[132];
    char date[132], formatted_date[132];
    int date_digit, rdate, fc;
    int stime_digit, etime_digit;
    int Stime[2], Etime[2];
};

//Compare if the date are different.
//Used when call quicksort()
int date_cmp(const void *a, const void *b)
{
    struct Dates *ia = (struct Dates *)a;
    struct Dates *ib = (struct Dates *)b;
    return (int)(ia->date_digit - ib->date_digit);
}

//MAIN
int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i;



    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--start=", 8) == 0) {
            sscanf(argv[i], "--start=%d/%d/%d", &from_y, &from_m, &from_d);
        } else if (strncmp(argv[i], "--end=", 6) == 0) {
            sscanf(argv[i], "--end=%d/%d/%d", &to_y, &to_m, &to_d);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr,
            "usage: %s --start=yyyy/mm/dd --end=yyyy/mm/dd --file=icsfile\n",
            argv[0]);
        exit(1);
    }

    /* Starting calling your own code from this point. */

    //Additional variables
    int from_digit = (from_y*10000)+(from_m*100)+from_d;
    int to_digit = (to_y*10000)+(to_m*100)+to_d;

    int counter, a, first, current_date_digit;
    int Scounter = 0;

    char StrList[500][132];
    char line[132], current_date[132];

    struct Dates eventlist[500];

    FILE* fp = fopen(filename, "r");
            if(fp == NULL){
                printf("Cannot open");
            }

   //Scanning the file
   while (fgets(line,sizeof(line),fp)){
        int i = 0;
        while(line[i] != '\n'){
            StrList[counter][i] = line[i];
            i++;
        }
        counter++;
    }
    fclose(fp);

   //Collecting data
    for(int i = 0; i < counter; i++){
        if(strcmp("BEGIN:VCALENDAR", StrList[i]) == 0){
            int j = i;
            while(strcmp("END:VCALENDAR", StrList[j]) != 0){
                    if(strncmp("DTSTART:",StrList[j], 8) == 0){
                        sscanf(StrList[j], "DTSTART:%dT%d", &eventlist[Scounter].date_digit, &eventlist[Scounter].stime_digit);
                        sscanf(StrList[j], "DTSTART:%8sT", eventlist[Scounter].date);
                    }

                   if(strncmp("DTEND:", StrList[j], 6) == 0){
                        sscanf(StrList[j], "DTEND:%8dT%d", &a, &eventlist[Scounter].etime_digit);
                    }

                    if(strncmp("RRULE:", StrList[j], 6) == 0){
                       sscanf(StrList[j], "RRULE:%*[^U]UNTIL=%dT", &eventlist[Scounter].rdate);
                       eventlist[Scounter].fc = 2;
                    }

                    if(strncmp("LOCATION:",StrList[j], 9) == 0){
                        sscanf(StrList[j], "LOCATION:%[^;]", eventlist[Scounter].location);
                    }

                    if(strncmp("SUMMARY:",StrList[j], 8) == 0){
                        sscanf(StrList[j], "SUMMARY:%[^;]", eventlist[Scounter].event);
                        Scounter++;
                    }
                    j++;
            }
        }
    }


    //Creating multiple events if the pointed event is to be repeated

   for(int i = 0; i < Scounter; i++){
        if(eventlist[i].fc == 2){
            strcpy(current_date,eventlist[i].date);
            current_date_digit = atoi(current_date);
            for(int j = 0; j < MAX_EVENTS; j++){
                if((current_date_digit + WEEK) > eventlist[i].rdate){
                    break;
                }else{
                    dt_increment(eventlist[Scounter].date,current_date, WEEK);
                    strcpy(current_date,eventlist[Scounter].date);
                    current_date_digit = atoi(current_date);
                    eventlist[Scounter].date_digit = current_date_digit;

                    strcpy(eventlist[Scounter].location, eventlist[i].location);
                    strcpy(eventlist[Scounter].event, eventlist[i].event);

                    eventlist[Scounter].stime_digit = eventlist[i].stime_digit;
                    eventlist[Scounter].etime_digit = eventlist[i].etime_digit;

                    Scounter++;
                }
            }
        }
   }

   //Sorting events first according to date and then time
    qsort(eventlist, Scounter, sizeof(struct Dates), date_cmp);

    for(int i = 0; i < Scounter ;i++){
        for(int j = 0; j < Scounter - i ;j++){
            if(eventlist[j].date_digit == eventlist[j+1].date_digit){
                if(eventlist[j].etime_digit > eventlist[j+1].stime_digit){
                    struct Dates temp;
                    temp = eventlist[j+1];
                    eventlist[j+1] = eventlist[j];
                    eventlist[j] = temp;
                }
            }
        }
    }

    //Making formatted chars, hours and minutes.
    for(int i = 0; i < Scounter; i++){
        dt_format(eventlist[i].formatted_date, eventlist[i].date, MAX_LINE_LEN);
        tmtok(eventlist[i].stime_digit, &eventlist[i].Stime[0], &eventlist[i].Stime[1]);
        tmtok(eventlist[i].etime_digit, &eventlist[i].Etime[0], &eventlist[i].Etime[1]);
    }

    //Printing events
    for(int i = 0; i < Scounter; i++){
        if(eventlist[i].date_digit >= from_digit){
            first = eventlist[i].date_digit;
            break;
        }
    }

    for(int i = 0; i < Scounter; i++){
        if(eventlist[i].date_digit >= from_digit){
            if(eventlist[i].date_digit <= to_digit){
                if(eventlist[i].date_digit != eventlist[i-1].date_digit){
                    if(eventlist[i].date_digit == first){
                            printf("%s\n", eventlist[i].formatted_date);
                        }else{
                            printf("\n%s\n", eventlist[i].formatted_date);
                        }
                    int Clength = strlen(eventlist[i].formatted_date);
                    for(int j = 0; j < Clength; j++){
                        printf("-");
                    }
                    printf("\n");
                    print_summary(eventlist[i].location, eventlist[i].event, eventlist[i].Stime[0], eventlist[i].Stime[1], eventlist[i].Etime[0], eventlist[i].Etime[1]);
                }else{ //Printing only event if there are other events of the same date
                    print_summary(eventlist[i].location, eventlist[i].event, eventlist[i].Stime[0], eventlist[i].Stime[1], eventlist[i].Etime[0], eventlist[i].Etime[1]);
                }
            }
        }
    }

    exit(0);
}
