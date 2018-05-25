/******************
 Name: CPSC 3220
 Name: Scheduling Project
 Due: November 28, 2017

 Compile: gcc schedule.c
 Run: ./a.out
********************/
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

/* struct represention one process/task of work */
typedef struct _process {
   int process_id;
   int arrival_time;
   int process_length;
   int process_loaded;
   int time_remaining;
   int time_waiting;
   int completion_time;
   int response_time;
} process;

/* Global variable are poor, but fast to code up */
process process_load[MAX]; /* array to hold work load */
process work_queue[MAX];  /* array representing work in progress */
process on_cpu;  /* cpu hold the task running on the cpu */

int number_of_processes;  /* number of process to simulate during the simulation (total workload)*/
int scheduling_policy; /* 0=FIFO, 1=SJF, 2=RR */
int premption_policy;  /* 0 is off 1 is on (scheduling policy will overrule this setting) */
int time_quantium;  /* used in preemption */
int master_clock;  /* clock start at 0 */
int processes_left;  /* processes currently loaded in the work_queue */

/* otuput section for reporting purposes */
void print_report() {
 	int i, avg;

   avg = 0;
   if (scheduling_policy == 0) { printf("Scheduling Policy: FIFO\n"); }
   else if (scheduling_policy == 1) { printf("Scheduling Policy: SJF\n"); }
   else if (scheduling_policy == 2) { printf("Scheduling Policy: RR\n"); }

   if (premption_policy == 0) { printf("Premption: OFF\n"); }
   else if (premption_policy == 1) { printf("Premption: ON\n"); }

   printf("Time Quantium: %d\n", time_quantium);
   printf("Number of Processes: %d\n\n", number_of_processes);

   for (i=0; i<number_of_processes; i++) {
  	printf("Process ID: %d\n", process_load[i].process_id);
  	printf("   Arrival Time: %d\n", process_load[i].arrival_time);
  	printf("   Process Length: %d\n", process_load[i].process_length);
  	printf("   Completion Time: %d\n", process_load[i].completion_time);
  	printf("   Response Time: %d\n\n", process_load[i].response_time);
  	avg += process_load[i].response_time;
   }

   avg /= number_of_processes;
   printf(" Avg Response Time: %d\n\n", avg);
}

/* debuging output section */
void print_machine_state() {
	int i;

   printf("\n\n TOTAL WORK LOAD \n");
   for (i=0; i<number_of_processes; i++) {
  	printf("Process ID: %d\n", process_load[i].process_id);
  	printf("   Arrival Time: %d\n", process_load[i].arrival_time);
  	printf("   Process Length: %d\n", process_load[i].process_length);
  	printf("   Process Loaded: %d\n", process_load[i].process_loaded);
  	printf("   Time Remaining: %d\n", process_load[i].time_remaining);
  	printf("   Time Waiting: %d\n", process_load[i].time_waiting);
  	printf("   Completion Time: %d\n", process_load[i].completion_time);
  	printf("   Response Time: %d\n\n", process_load[i].response_time);
   }

   printf("\n\n WORK QUEUE \n");
   for (i=0; i<processes_left; i++) {
  	printf("Process ID: %d\n", work_queue[i].process_id);
  	printf("   Arrival Time: %d\n", work_queue[i].arrival_time);
  	printf("   Process Length: %d\n", work_queue[i].process_length);
  	printf("   Process Loaded: %d\n", work_queue[i].process_loaded);
  	printf("   Time Remaining: %d\n", work_queue[i].time_remaining);
  	printf("   Time Waiting: %d\n", work_queue[i].time_waiting);
  	printf("   Completion Time: %d\n", process_load[i].completion_time);
  	printf("   Response Time: %d\n\n", process_load[i].response_time);
   }

   printf("\n\n");
}

/* Stop Running process and put it back on the work queue */
/* If task is done then compute response time and leave it out of work queue */
void preempt_process() {
	process process_id;
for(int i = 0; i < number_of_processes; i++){
	if (on_cpu.time_remaining == 0) {
     	process_load[i].completion_time = process_load[i].process_length + process_load[i].time_waiting;
     	process_load[i].response_time = process_load[i].completion_time - process_load[i].arrival_time;
	} else {
      	work_queue[number_of_processes].process_id = on_cpu.process_id;
	}
}
}

/* Run one process and up all the waiting time for all process in the work_queue */
/* Watch out for process that have less than time quantium work left to do */
void run_process() {

  	process process_id;
  	//process_load[0].process_id;
  	for (int i = 0; i < number_of_processes; i++) {
    	process_load[i].time_waiting = process_load[i-1].completion_time;// + time_quantium;
  	}

}

/* Decide which process will be loaded onto the cpu to run next based on Scheduling Policy */
void load_process() {
   /* process process_id;
	for(int i = 0; i < number_of_processes; i++){
  	if(scheduling_policy == 0){
   	on_cpu.process_id = work_queue[i].process_id;
  	}
 	}*/

for(int i = 0; i < number_of_processes; i++){
  	if(scheduling_policy == 0){
   	on_cpu.process_loaded = work_queue[i].process_id;
  	}
  	if(scheduling_policy == 1){
   	if(time_quantium += 10)//for loop incrementing time quantum
   	on_cpu.process_loaded = work_queue[i].time_remaining;
 
	}
  }
}

/* Pull Process from load queue and put it on the work queue */
void new_process() {
	if(processes_left == 0){
  	processes_left = number_of_processes;
	}else {
  	processes_left--;
	}
	for (int i = 0; i<processes_left; i++) {
 	work_queue[i].process_id = process_load[i].process_id;

 	//processes_left = number_of_processes;
	}
}

void load_task_simulation_data() {
 	FILE *fp;
 	int i;
 	char filler[100];  // used to read to end of line in the config files -- like a NULL read

   for (i=0; i<MAX; i++) {
  	process_load[i].process_id = -1;
  	work_queue[i].process_id = -1;
   }

   fp = fopen("process_load.txt","r");
   fscanf(fp,"%d",&scheduling_policy);
   fgets(filler, 100, fp);

   fscanf(fp,"%d",&premption_policy);
   fgets(filler, 100, fp);

   fscanf(fp,"%d",&time_quantium);
   fgets(filler, 100, fp);

   fscanf(fp,"%d",&number_of_processes);
   fgets(filler, 100, fp);

   for (i=0; i<number_of_processes; i++) {
  	fscanf(fp,"%d",&(process_load[i].process_id));
  	fgets(filler, 100, fp);

  	fscanf(fp,"%d",&(process_load[i].arrival_time));
  	fgets(filler, 100, fp);

  	fscanf(fp,"%d",&(process_load[i].process_length));
  	fgets(filler, 100, fp);

  	process_load[i].process_loaded = 0;
  	process_load[i].time_remaining = process_load[i].process_length;
  	process_load[i].time_waiting = 0;
  	process_load[i].completion_time = -1;
  	process_load[i].response_time = -1;
   }
   fclose(fp);

   master_clock = 0;
   if (scheduling_policy == 0) {  // FIFO requires Premption to be turned OFF
  	premption_policy = 0;
   } else if (scheduling_policy == 2) {  // RR requires Premption to be turned ON
  	premption_policy = 1;
   }
   if (premption_policy < 0 || premption_policy > 1) { premption_policy = 1; }  // Catch bad input
   processes_left = 0;
}

int main() {
 	int i;

   load_task_simulation_data();  // Get input
   new_process();           	// Load at least 1+ processes onto work queue
   while (processes_left) {
  	load_process();   // Move work from work queue to cpu
  	run_process();	// run ONE process (add wait times to process in work_queue)
  	new_process();	// Load any newly arriving processes
  	preempt_process(); // Kick CPU process back to work queue (or remove if complete)
   }
   print_report();  // Output stats of run
}


