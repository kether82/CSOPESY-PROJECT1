#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const IND_ALGO = 0;
const IND_NUMPROCESS = 1;
const IND_TIMESLICE = 2;
struct process
{
    int pID, arrivalTime, burstTime;
} ;
// Algo, # Processes, Time-Slice

// PID, ArrTime, BurstTime

int main() {
    int params[3];
    // algo:
    // 0 FCFS
    // 1 SJF
    // 2 SRTF
    // 3 RR

    char filename[50];
    FILE *file;

    char ch;
    char buffer[8];
    char firstLine[10];
    int i = 0;
    int j = 0;
    int k = 0;
    struct process processes[100];
    // Get file name
    printf("Enter File Name: ");
    fgets(filename, sizeof(filename), stdin);

    file = fopen(filename, "r");

    if (file){
        // read first line
        fgets(firstLine, 10, file);
        while(firstLine[i] != '/0'){
            ch = firstLine[i];

            if(ch == ' '){
                // squish number
                params[j] = atoi(buffer);
                j++;

                strcpy(buffer,'\0');
                i = 0;
            } else {
                // number not done yet
                buffer[i] = ch;
                i++;
            }
        }
        i = 0;
        j = 0;
        k = 0;
        // read rest of file
        while (!feof(file)){
            ch = fgetc(file);

            if(ch == ' '){
                // squish number
                if(k == 0){
                    processes[j].pID = atoi(buffer);
                }else if (k == 1){
                    processes[j].arrivalTime = atoi(buffer);
                }else if (k == 2){
                    processes[j].burstTime = atoi(buffer);
                }

                k++;

                if(k > 2){
                    k = k % 3;
                    j++;
                }
                
                strcpy(buffer,'\0');
                
                i = 0;
            } else {
                // number not done yet
                buffer[i] = ch;
                i++;
            }
        }
        fclose(file);
    }else{
        printf("%s not found", filename);
    }


    return 0;
}