#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const IND_ALGO = 0;
const IND_NUMPROCESS = 1;
const IND_TIMESLICE = 2;
const SIZE_PROCESSES = 100;

struct process
{
    int pID, arrivalTime, burstTime;
    int waitTime;
    int startTime[50];
    int endTime[50];
    int cycle;
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
    struct process processes[SIZE_PROCESSES];
    // Get file name
    printf("Enter File Name: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    // strcpy(filename,"test4.txt");
    // puts("\n");
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
    // printf("%d %d %d\n", params[IND_ALGO], params[IND_NUMPROCESS], params[IND_TIMESLICE]);
    // for(i = 0; i < 3; i++) printf("%d %d %d \n", processes[i].pID, processes[i].arrivalTime, processes[i].burstTime);

    // printf("%d", processes[2].burstTime);

    // algorithm portion
    if(params[IND_ALGO] == 0){
        // First come first serve
        puts("First Come First Serve:");
        fcfs(processes, params[IND_NUMPROCESS]);
    } else if (params[IND_ALGO] == 1){
        // Shortest Job First
        puts("Shortest Job First:");
        sjf(processes, params[IND_NUMPROCESS]);
    } else if (params[IND_ALGO] == 2){
        // Shortest Remaining Time First
        puts("Shortest Remaining Time First:");
        srtf(processes, params[IND_NUMPROCESS]);
    } else if (params[IND_ALGO] == 3){
        // Round Robin
        puts("Round Robin:");
        roundRobin(processes, params[IND_TIMESLICE], params[IND_NUMPROCESS]);
    }
    return 0;
}

void swap(int *a, int *b) 
{ 
int temp = *a; 
*a = *b; 
*b = temp; 
}  

// bubble sort function
void bubbleSort(int array[], int n, int array2[]) 
{ 
int i, j; 
for (i = 0; i < n-1; i++)       
for (j = 0; j < n-i-1; j++) if (array[j] > array[j+1])
{
	swap(&array[j], &array[j+1]); 
	swap(&array2[j], &array2[j+1]);
} 

}

fcfs(struct process processes[], int n)
{
	int timeWaiting = 0;
	int totalWT= 0;
	double avgTime = 0;
	int arrTime[n];
	int sortedIndex[n];
	int i = 0;
	
	for(i = 0; i < n; i++)
	{
		//Get the arrival times of each process and set the values for the sortedIndex array
		arrTime[i] = processes[i].arrivalTime;
		sortedIndex[i] = i;
	}
	
	//sort the arrival times along with the indexes
	bubbleSort(arrTime, n, sortedIndex);
	
	for(i = 0; i < n; i++)
	{
		//setting the values for the first case
		if(i == 0)
			{
				processes[sortedIndex[i]].waitTime = 0;
				processes[sortedIndex[i]].startTime[0] = 0;
				processes[sortedIndex[i]].endTime[0] = processes[sortedIndex[i]].burstTime;
				processes[sortedIndex[i]].cycle = 0;
			}
		//setting values for the rest of the cases
		else
		{
			timeWaiting += processes[sortedIndex[i-1]].burstTime;
			processes[sortedIndex[i]].waitTime = timeWaiting - processes[sortedIndex[i]].arrivalTime;
			processes[sortedIndex[i]].startTime[0] = timeWaiting;
			processes[sortedIndex[i]].endTime[0] = timeWaiting + processes[sortedIndex[i]].burstTime;
			processes[sortedIndex[i]].cycle = 0;
			
		}
	}
	
	outputFormat(processes, n);
    outputAvgWaitTime(processes, n);

	// for(i = 0; i < n; i++)
	// 	totalWT += processes[i].waitTime;
	// avgTime = totalWT * 1.0 / n ;
	
	// printf("Average Waiting Time: <%.2f>", avgTime);
	
}

void bubbleSort2(int array[], int n, int array2[], int array3[]) 
{ 
int i, j; 
for (i = 0; i < n-1; i++)       
for (j = 0; j < n-i-1; j++) if (array[j] > array[j+1])
{
	swap(&array[j], &array[j+1]); 
	swap(&array2[j], &array2[j+1]);
	swap(&array3[j], &array3[j+1]);
} 

}

sjf(struct process processes[], int n)
{		
	int timeWaiting = 0;
	int totalWT= 0;
	double avgTime = 0;
	
	int arrTime[n];
	int sortedIndex[n];
	int burstTime[n];
	int smallest = 999999;
	int indexSmallest;
	int prevIndex;
	int i = 0;
	int j = 0;
	
	int counter = 0;
	int timeCheck = 0;
	int flag[n];
	
	for(i = 0; i < n; i++)
	{
		//Get the arrival times of each process and set the values for the sortedIndex array
		arrTime[i] = processes[i].arrivalTime;
		sortedIndex[i] = i;
		burstTime[i] = processes[i].burstTime;
		flag[i] = 0;
	}
	//sort first by burst times
	bubbleSort2(burstTime, n, sortedIndex, arrTime);
	//sort the arrival times along with the indexes
	bubbleSort2(arrTime, n, sortedIndex, burstTime);
	
	for(i = 0; i < n; i++)
	{
		printf("index: %d ||Burst %d||Arrival %d\n", sortedIndex[i], burstTime[i], arrTime[i]);
	}
	
	for(i = 0; i < n; i++)
 	   {
        //setting the values for the first case
      	  if(i == 0)
       	    {
                processes[sortedIndex[i]].waitTime = 0;
                processes[sortedIndex[i]].startTime[0] = 0;
                processes[sortedIndex[i]].endTime[0] = processes[sortedIndex[i]].burstTime;
                processes[sortedIndex[i]].cycle = 0;
                flag[sortedIndex[i]] = 1;
                prevIndex = sortedIndex[i];
                counter += processes[sortedIndex[i]].burstTime;
            }
        //setting values for the rest of the cases
      	  else
     	   {
            for(j = 0; j < n; j++)
            {
            	//we check if the arrival time of the current process is smaller than the previous and check the flag(if it has already been used)
            	if((processes[sortedIndex[j]].arrivalTime <=  (counter)) 
				&& (flag[sortedIndex[j]] != 1))
				{
					//get the smallest burst time and the index
					if(processes[sortedIndex[j]].burstTime < smallest)
					{
						smallest = processes[sortedIndex[j]].burstTime;
						indexSmallest = j;
					}
				}
			}
			//once the smallest burst time has been found, we use the index to get the process and calculate from there
            timeWaiting += processes[prevIndex].burstTime;
            processes[sortedIndex[indexSmallest]].waitTime = timeWaiting - processes[sortedIndex[indexSmallest]].arrivalTime;
            processes[sortedIndex[indexSmallest]].startTime[0] = timeWaiting;
            processes[sortedIndex[indexSmallest]].endTime[0] = timeWaiting + processes[sortedIndex[indexSmallest]].burstTime;
            processes[sortedIndex[indexSmallest]].cycle = 0;
            flag[sortedIndex[indexSmallest]] = 1;
            smallest = 999999;
            prevIndex = sortedIndex[indexSmallest];
            counter += processes[sortedIndex[indexSmallest]].burstTime;
       	 }
       	 
  	  }
  	
	outputFormat(processes, n);
    outputAvgWaitTime(processes, n);
	// for(i = 0; i < n; i++)
	// 	totalWT += processes[i].waitTime;
	// avgTime = totalWT * 1.0 / n ;
	
	// printf("Average Waiting Time: <%.2f>", avgTime);

}

srtf(struct process processes[], int n){
    // search for shortest remaining time
    // execute with 1 step
    // loop till end
    int i = 0;
    int t = 0;
    int shortest = 0;
    int min = 9999999;
    int flag = 0;
    int remBurst[SIZE_PROCESSES];
    int endTime = 0;
    int countDone = 0;
    int prev = 0;
    double wtAvg = 0;
    int wtAccu = 0;
    for(i = 0; i < n; i++){
        remBurst[i] = processes[i].burstTime;
    }
    while(countDone != n){
        // printf("%d\n", countDone);
        for(i = 0; i < n; i++){

            // find the shortest active process
            if(processes[i].arrivalTime <= t && remBurst[i] < min && remBurst[i] > 0){
                min = remBurst[i];
                shortest = i;
                flag = 1;
                processes[i].startTime[processes[i].cycle] = t;
                if(prev != i){
                    processes[prev].endTime[processes[prev].cycle] = t;
                    if(remBurst[prev] > 0){
                        processes[prev].cycle++;
                    }
                }
            }

        }
        // if we cant found one, increment the timer
        if(!flag){
            t++;
        } else {
            
            remBurst[shortest]--;
            min--;
            prev = shortest;
            // process is done
            if(remBurst[shortest] == 0){
                min = 9999999;
                flag = 0;
                countDone++;
                endTime = t + 1;
                processes[shortest].waitTime = (endTime - processes[shortest].burstTime - processes[shortest].arrivalTime);
                if(countDone == n){
                    processes[shortest].endTime[processes[shortest].cycle] = endTime;
                }
            }

            t++;
        }
        // outputFormat(processes, n);
    }
    // for(i = 0; i < n; i++) wtAccu += processes[i].waitTime;
    // wtAvg = wtAccu * 1.0 / n ;

    outputFormat(processes, n);
    outputAvgWaitTime(processes, n);
    // printf("Average Waiting Time: <%.2f>", wtAvg);
}
roundRobin(struct process processes[], int timeSlice, int n){
    // Apply time slice to all tasks in order of arrival time
    // check if arrived
    //      if arrived compute
    // else skip

    int remBurst[SIZE_PROCESSES];
    int wtAccu = 0;
    double wtAvg = 0;
    int t = 0;
    int i = 0;
    int j = 0;
    int minTime = 999999;
    for (i = 0; i < n; i++){
        remBurst[i] = processes[i].burstTime;
    }

    while(1){
        int flag = 1;

        for(i = 0; i < n; i++){

            
            if(t >= processes[i].arrivalTime){

                if(remBurst[i] > 0){
                    flag = 0;
                    if(remBurst[i] > timeSlice){

                        processes[i].startTime[processes[i].cycle] = t;
                        t += timeSlice;
                        processes[i].endTime[processes[i].cycle] = t;
                        remBurst[i] -= timeSlice;
                        processes[i].cycle++;

                    }else{
                        processes[i].startTime[processes[i].cycle] = t;
                        t += remBurst[i];
                        processes[i].endTime[processes[i].cycle] = t;
                        remBurst[i] = 0;
                        int accu = 0;
                        
                        for(j = 1; j <= processes[i].cycle; j++){
                            accu += processes[i].startTime[j] - processes[i].endTime[j-1];
                        }
                        processes[i].waitTime = accu + (processes[i].startTime[0] - processes[i].arrivalTime);
                        
                    }
                }
            }
            
            // get minimum time for anything to get going
            if(minTime > processes[i].arrivalTime) minTime = processes[i].arrivalTime;
        }
        if(minTime > t){
            t = minTime;
            flag = 0;
        }
        if(flag) break;
    }

    // for(i = 0; i < n; i++) wtAccu += processes[i].waitTime;
    // wtAvg = wtAccu * 1.0 / n ;

    outputFormat(processes, n);
    outputAvgWaitTime(processes, n);
    // printf("Average Waiting Time: <%.2f>", wtAvg);
}

outputFormat(struct process processes[], int n){
    int i = 0;
    int j = 0;
    for(i = 0; i < n; i++){
        printf("P[%d] " , processes[i].pID);
        for(j = 0; j <= processes[i].cycle; j++){
            printf("Start time: <%d> End Time: <%d> | ", processes[i].startTime[j], processes[i].endTime[j]);
        }
        printf("Waiting Time: <%d>\n", processes[i].waitTime);
    }
}

outputAvgWaitTime(struct process processes[], int n){
    int wtAccu = 0;
    double wtAvg = 0;
    int i = 0;
    for(i = 0; i < n; i++) wtAccu += processes[i].waitTime;
    wtAvg = wtAccu * 1.0 / n ;
    printf("Average Waiting Time: <%.2f>", wtAvg);
}
