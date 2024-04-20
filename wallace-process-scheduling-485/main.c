/**
 * Process Scheduling main file. See README for more information.
 *
 * @author  J. Kenneth Wallace
 * @version 0.1
 * @since   2024-04-18
 */

#include "cosmetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <stdbool.h>
#define SIZE 100    // Default process list size.

typedef struct {
    int id;
    int state;
    int priority;

    int arrival_time;
    int burst_time;         // Amount of time needed to complete.
    int finish_time;
    int turnaround_time;    // Time from arrival to finish.
    int waiting_time;       // Time not being worked on.

    int work_done;          // Round-robin exclusive. Holds the amount of work that has been done.
} Process;

/* Global Variables */
Process process_list[SIZE];     // Process list that holds all processes.
int num_processes = 0;          // Number of processes, used for process list indexing.
int round_robin_quanta = 2;     // Amount of time allotted to each process 
                                // in the round-robin scheduling algorithm.
float last_avg_wait = -1;       // Last algorithm's average wait time result.
float last_avg_turn = -1;       // Last algorithm's average turnaround result.


//TODO: 
// 1. Add options for quanta in menu
// 2. Add options for scheduling algorithms (replace scheduling option)

/**
 * Initializes the process_list with hard-coded processes.
 */
void init_process_list();

/**
 * Prints out a user interface with six options:
 * 1. View processes, 2. FCFS algorithm, 3. SJF algorithm, 4. RRS algorithm, 5. PRI algorithm, 6. Exit program
 */
void userInterface();

/**
 * Shows current process list.
 * For this program, the processes are hard-coded.
 */
void viewProcesses();

/**
 * Comparison functions used by quicksort to re-order the process list.
 * @param p1 Process 1.
 * @param p2 Process 2.
 * @return int The difference in value (arrival time, burst time, or priority) between the two processes.
 */
int _compare_arrival(const void *p1, const void *p2);
int _compare_burst(const void *p1, const void *p2);
int _compare_priority(const void *p1, const void *p2);

/**
 * Ordering function used to re-order the process list with the help of quick sort.
 * @param order_type Type of ordering to do; 0 == Arrival time, 1 == Burst time, 2 == Priority.
 */
void _psa_order(int order_type);

/* Process Scheduling Algorithms */
/**
 * First-Come First-Serve scheduling algorithm.
 * Order process_list by [arrival_time] and execute in order.
 */
void psa_FCFS();

/**
 * Shortest Job First scheduling algorithm.
 * Order process_list by [burst_time]. Initialize a [current_time] to 0.
 * Pick the shortest available job from processes that are ready ([arrival_time] <= [current_time]).
 * Does not stop mid-execution.
 */
void psa_SJF();

/**
 * Round-Robin scheduling algorithm.
 * Order process_list by [arrival_time] and execute in order.
 * Each process can only execute for X amount of time, where X is the time quantum.
 */
void psa_RRS();

/**
 * Priority scheduling algorithm.
 * Order process_list by [priority]. Initialize a [current_time] to 0.
 * Pick the highest priority from processes that are ready ([arrival_time] <= [current_time]).
 * Does not stop mid-execution.
 * Lower value == Higher priority.
 */
void psa_PRI();

/**
 * Main function that primarily calls userInterface.
 * 
 * @return int Successful run returns 0, otherwise returns 1.
 */
int main() {
    _clearTerminal();
    init_process_list();

    char *intro_text_1    = "PROCESS SCHEDULING SIMULATION     ";
    char *intro_text_2    = "KENNETH WALLACE - CSCI 485        ";
    char *exit_text       = "EXITING PROGRAM ! ! !";
    
    printf("\033[30m"); // black text
    printf("\033[41m"); // red background
    _typewriterPrint(intro_text_1, 3);
    _typewriterPrint(intro_text_2, 2);
    
    printf("\033[0m");  // normal text and background
    _toggleDelay();
    userInterface();

    printf("\033[32m"); // green text
    _typewriterPrint(exit_text, 2);
    printf("\033[0m\n");  // default text
    return 0;
}

void userInterface() {
    int user_option = -1;

    while (user_option != 6) {
        char *option_menu = "PLEASE INPUT A COMMAND:";
        char *option_0    = "> [0] VIEW PROCESSES";
        char *option_1    = "> [1] FIRST-COME FIRST-SERVE ALGORITHM";
        char *option_2    = "> [2] SHORTEST JOB FIRST ALGORITHM";
        char *option_3    = "> [3] ROUND-ROBIN SCHEDULING ALGORITHM";
        char *option_4    = "> [4] PRIORITY ALGORITHM";
        char *option_5    = "> [5] TOGGLE TYPEWRITER EFFECT";
        char *option_6    = "> [6] EXIT PROGRAM";
        char *invalid     = "INVALID COMMAND ! ! !";

        _typewriterPrint(option_menu, 2);
        _typewriterPrint(option_0, 2);
        _typewriterPrint(option_1, 1);
        _typewriterPrint(option_2, 1);
        _typewriterPrint(option_3, 1);
        _typewriterPrint(option_4, 1);
        _typewriterPrint(option_5, 1);
        _typewriterPrint(option_6, 1);
        printf("\n\n  User > ");

        scanf("%d", &user_option);

        _removeLines(11);
        switch (user_option) {
            case 0:
                printf("  User > [0] VIEW PROCESSES");
                viewProcesses();
                break;
            case 1:
                printf("  User > [1] FIRST-COME FIRST-SERVE ALGORITHM");
                psa_FCFS();
                break;
            case 2:
                printf("  User > [2] SHORTEST JOB FIRST ALGORITHM");
                psa_SJF();
                break;
            case 3:
                printf("  User > [3] ROUND-ROBIN SCHEDULING ALGORITHM");
                psa_RRS();
                break;
            case 4:
                printf("  User > [4] PRIORITY ALGORITHM");
                psa_PRI();
                break;
            case 5:
                printf("  User > [5] TOGGLE TYPEWRITER EFFECT");
                _toggleDelay();
                break;
            case 6:
                printf("  User > [6] EXIT PROGRAM");
                return;
            default:
                printf("  User > [%d]", user_option);
                printf("\033[31m"); // red text
                _typewriterPrint(invalid, 2);
                printf("\033[0m");  // default text
        }
    }
}

void init_process_list() {
    int temp_num_processes = 0;

    // Process 1
    Process new_process;
    new_process.id = 10;
    new_process.priority = 2;
    new_process.arrival_time = 0;
    new_process.burst_time = 6;
    process_list[temp_num_processes] = new_process;
    temp_num_processes++;

    // Process 2
    Process new_process_2;
    new_process_2.id = 20;
    new_process_2.priority = 4;
    new_process_2.arrival_time = 2;
    new_process_2.burst_time = 20;
    process_list[temp_num_processes] = new_process_2;
    temp_num_processes++;

    // Process 3
    Process new_process_3;
    new_process_3.id = 30;
    new_process_3.priority = 6;
    new_process_3.arrival_time = 8;
    new_process_3.burst_time = 10;
    process_list[temp_num_processes] = new_process_3;
    temp_num_processes++;

    // Process 4
    Process new_process_4;
    new_process_4.id = 40;
    new_process_4.priority = 8;
    new_process_4.arrival_time = 6;
    new_process_4.burst_time = 4;
    process_list[temp_num_processes] = new_process_4;
    temp_num_processes++;

    // Process 5
    Process new_process_5;
    new_process_5.id = 50;
    new_process_5.priority = 0;
    new_process_5.arrival_time = 4;
    new_process_5.burst_time = 8;
    process_list[temp_num_processes] = new_process_5;
    temp_num_processes++;

    // Process 6
    Process new_process_6;
    new_process_6.id = 60;
    new_process_6.priority = 1;
    new_process_6.arrival_time = 10;
    new_process_6.burst_time = 13;
    process_list[temp_num_processes] = new_process_6;
    temp_num_processes++;

    // Init
    for (int i = 0; i < temp_num_processes; i++) {
        Process * curr = &process_list[i];
        curr->state = 0;
        curr->finish_time = 0;
        curr->turnaround_time = 0;
        curr->waiting_time = 0;
        curr->work_done = 0;
    }
    if (temp_num_processes != num_processes) {
        num_processes = temp_num_processes;
    }
}

void viewProcesses() {
    _psa_order(0);

    /* If you are reading this, I was too lazy to create a separate function for printing borders. */
    // Top Border
    printf("\n\n");
    printf("  ");
    for (int i = 0; i < 76; i++) { printf("="); }
    printf("\n");
    printf("  | ----ID---- | STATE | PRIORITY | ARRIVAL | BURST | FINISH | TURN. | WAIT. |\n");
    printf("  ");
    for (int i = 0; i < 76; i++) { printf("="); }
    printf("\n");

    for (int i = 0; i < num_processes; i++) {
        int p_id = process_list[i].id;
        int p_state = process_list[i].state;
        int p_priority = process_list[i].priority;
        int p_arrive = process_list[i].arrival_time;
        int p_burst = process_list[i].burst_time;
        int p_finish = process_list[i].finish_time;
        int p_turnaround = process_list[i].turnaround_time;
        int p_wait = process_list[i].waiting_time;
        printf("  | %-10d | %-5d | %-8d | %-7d | %-5d | %-6d | %-5d | %-5d |\n", p_id, p_state, p_priority, p_arrive, p_burst, p_finish, p_turnaround, p_wait);
    }

    // Bottom Border
    printf("  ");
    for (int i = 0; i < 76; i++) { printf("="); }

    if (last_avg_wait != -1 && last_avg_turn != -1) {
        printf("\n");
        for (int i = 0; i < 53; i++) { printf(" "); }
        printf("AVERAGE:  %.3f  %.3f", last_avg_turn, last_avg_wait);
    }
}

int _compare_arrival(const void *p1, const void *p2) {
    const Process *process1 = (const Process *) p1;
    const Process *process2 = (const Process *) p2;
    return process1->arrival_time - process2->arrival_time;
}
int _compare_burst(const void *p1, const void *p2) {
    const Process *process1 = (const Process *) p1;
    const Process *process2 = (const Process *) p2;
    return process1->burst_time - process2->burst_time;
}
int _compare_priority(const void *p1, const void *p2) {
    const Process *process1 = (const Process *) p1;
    const Process *process2 = (const Process *) p2;
    return process1->priority - process2->priority;
}

void _psa_order(int order_type) {
    switch (order_type) {
        case 0: // Arrival Time.
            qsort(process_list, num_processes, sizeof(Process), _compare_arrival);
            break;
        case 1: // Burst Time.
            qsort(process_list, num_processes, sizeof(Process), _compare_burst);
            break;
        case 2: // Priority.
            qsort(process_list, num_processes, sizeof(Process), _compare_priority);
            break;
        
        default:
            break;
    }
}

void psa_FCFS() {
    init_process_list();
    _psa_order(0);

    printf("\n\n  Syst > FCFS algorithm running. . .");

    int total_wait = 0;
    int total_turnaround = 0;
    int previous_finish = 0;

    for (int i = 0; i < num_processes; i++) {
        Process * curr = &process_list[i];
        printf("\n\n  Syst > Process %d moving from state %d to state 1.", curr->id, curr->state);
        curr->state = 1;

        curr->finish_time = previous_finish + curr->burst_time;
        curr->waiting_time = previous_finish - curr->arrival_time;
        curr->turnaround_time = curr->finish_time - curr->arrival_time;

        previous_finish = curr->finish_time;
        total_wait += curr->waiting_time;
        total_turnaround += curr->turnaround_time;

        printf("\n  Syst > Process %d complete, set to state -1.", curr->id);
        curr->state = -1;
    }

    last_avg_wait = (float)total_wait / num_processes;
    last_avg_turn = (float)total_turnaround / num_processes;

    printf("\n\n  Syst > FCFS algorithm complete, statistics:\n    - Total turnaround: %d\n    - Total wait: %d\n    - Average turnaround: %.3f\n    - Average wait: %.3f", total_turnaround, total_wait, last_avg_turn, last_avg_wait);
}

void psa_SJF() {
    init_process_list();
    _psa_order(1);

    printf("\n\n  Syst > SJF algorithm running. . .");

    int total_wait = 0;
    int total_turnaround = 0;
    int previous_finish = 0;
    int complete_processes = 0;
    int current_time = 0;

    while (complete_processes != num_processes) {
        for (int i = 0; i < num_processes; i++) {
            Process * curr = &process_list[i];
            if (curr->arrival_time <= current_time && curr->state != -1) {
                printf("\n\n  Syst > Process %d moving from state %d to state 1.", curr->id, curr->state);
                curr->state = 1;

                curr->finish_time = previous_finish + curr->burst_time;
                curr->waiting_time = previous_finish - curr->arrival_time;
                curr->turnaround_time = curr->finish_time - curr->arrival_time;

                previous_finish = curr->finish_time;
                total_wait += curr->waiting_time;
                total_turnaround += curr->turnaround_time;
                current_time += curr->finish_time;

                printf("\n  Syst > Process %d complete, set to state -1.", curr->id);
                curr->state = -1;
                complete_processes++;
                i = num_processes;
            }
        }
    }

    last_avg_wait = (float)total_wait / num_processes;
    last_avg_turn = (float)total_turnaround / num_processes;

    printf("\n\n  Syst > SJF algorithm complete, statistics:\n    - Total turnaround: %d\n    - Total wait: %d\n    - Average turnaround: %.3f\n    - Average wait: %.3f", total_turnaround, total_wait, last_avg_turn, last_avg_wait);
}

void psa_RRS() {
    init_process_list();
    _psa_order(0);

    printf("\n\n  Syst > RRS algorithm running. . .");
    printf("\n  Syst > Please set the time quanta: (Integer, >0)");
    printf("\n\n  User > ");

    scanf("%d", &round_robin_quanta);

    int total_wait = 0;
    int total_turnaround = 0;
    int complete_processes = 0;
    int current_time = 0;

    while (complete_processes != num_processes) {
        for (int i = 0; i < num_processes; i++) {
            Process * curr = &process_list[i];
            if (curr->arrival_time <= current_time && curr->state != -1) {
                printf("\n\n  Syst > Process %d moving from state %d to state 1.", curr->id, curr->state);
                curr->state = 1;
                
                int process_time = 0;   // Time spent in this process.
                int remaining_work = curr->burst_time - curr->work_done;

                // If we finish the process.
                if (remaining_work - round_robin_quanta <= 0) {
                    process_time = round_robin_quanta + (remaining_work - round_robin_quanta);
                    curr->work_done += process_time;
                }
                // If we don't finish the process.
                else {
                    process_time = round_robin_quanta;
                    curr->work_done += process_time;
                }

                // Add wait time to other processes.
                for (int j = 0; j < num_processes; j++) {
                    Process * other = &process_list[j];
                    if (other->arrival_time <= current_time && other->state != -1) {
                        other->waiting_time += process_time;
                    }
                }

                // Check if process is finished.
                if (curr->work_done == curr->burst_time) {
                    curr->finish_time = curr->waiting_time + curr->burst_time;
                    curr->turnaround_time = curr->finish_time - curr->arrival_time;

                    total_wait += curr->waiting_time;
                    total_turnaround += curr->turnaround_time;

                    printf("\n  Syst > Process %d complete, set to state -1.", curr->id);
                    curr->state = -1;
                    complete_processes++;
                }
                else {
                    printf("\n  Syst > Process %d incomplete, set to state 0.", curr->id);
                    curr->state = 0;
                }

                // Iterate.
                current_time += process_time;
            }
        }
    }

    last_avg_wait = (float)total_wait / num_processes;
    last_avg_turn = (float)total_turnaround / num_processes;

    printf("\n\n  Syst > RRS algorithm complete, statistics:\n    - Total turnaround: %d\n    - Total wait: %d\n    - Average turnaround: %.3f\n    - Average wait: %.3f", total_turnaround, total_wait, last_avg_turn, last_avg_wait);
}

void psa_PRI() {
    init_process_list();
    _psa_order(2);

    printf("\n\n  Syst > PRI algorithm running. . .");

    int total_wait = 0;
    int total_turnaround = 0;
    int previous_finish = 0;
    int complete_processes = 0;
    int current_time = 0;

    while (complete_processes != num_processes) {
        for (int i = 0; i < num_processes; i++) {
            Process * curr = &process_list[i];
            if (curr->arrival_time <= current_time && curr->state != -1) {
                printf("\n\n  Syst > Process %d moving from state %d to state 1.", curr->id, curr->state);
                curr->state = 1;

                curr->finish_time = previous_finish + curr->burst_time;
                curr->waiting_time = previous_finish - curr->arrival_time;
                curr->turnaround_time = curr->finish_time - curr->arrival_time;

                previous_finish = curr->finish_time;
                total_wait += curr->waiting_time;
                total_turnaround += curr->turnaround_time;
                current_time += curr->finish_time;

                printf("\n  Syst > Process %d complete, set to state -1.", curr->id);
                curr->state = -1;
                complete_processes++;
                i = num_processes;
            }
        }
    }

    last_avg_wait = (float)total_wait / num_processes;
    last_avg_turn = (float)total_turnaround / num_processes;

    printf("\n\n  Syst > PRI algorithm complete, statistics:\n    - Total turnaround: %d\n    - Total wait: %d\n    - Average turnaround: %.3f\n    - Average wait: %.3f", total_turnaround, total_wait, last_avg_turn, last_avg_wait);
}
