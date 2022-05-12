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
    // fgets(filename, sizeof(filename), stdin);
    // filename[strcspn(filename, "\n")] = 0;
    strcpy(filename,"test.txt");
    puts("\n");
    file = fopen(filename, "r");

    if (file){

        // read first line

        fgets(firstLine, 10, file);
        while(firstLine[i] != 0){
            ch = firstLine[i];

            if(ch == ' ' || ch == '\n'){
                // squish number
                params[j] = atoi(buffer);
                j++;
                memset(buffer, 0, sizeof buffer);
                k = 0;
            } else {
                // number not done yet
                buffer[k] = ch;
                k++;
            }
            i++;
        }
        // printf("%d %d %d", params[0], params[1], params[2]);
        i = 0;
        j = 0;
        k = 0;
        // read rest of file
        while (!feof(file)){
            ch = fgetc(file);
            if(ch == ' ' || ch == '\n'){
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
                
                memset(buffer, 0, sizeof buffer);
                
                i = 0;
            } else {
                // number not done yet
                buffer[i] = ch;
                i++;
            }
        }
        // last one
        if (feof(file)) processes[j].burstTime = atoi(buffer);
        // printf("%d %d %d",i,j,k);
        fclose(file);
    }else{
        printf("%s not found", filename);
    }
    // printf("%d %d %d", params[IND_ALGO], params[IND_NUMPROCESS], params[IND_TIMESLICE]);
    // for(i = 0; i < 3; i++) printf("%d %d %d \n", processes[i].pID, processes[i].arrivalTime, processes[i].burstTime);

    // printf("%d", processes[2].burstTime);
    return 0;
}