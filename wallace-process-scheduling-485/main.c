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

struct Process {
    int id;
    int state;
    int priority;
    int start_time;
    int work_time;
};

/* Global Variables */
struct Process process_list[SIZE];  // Process list that holds all processes.
struct Process exec_list[SIZE];     // Order of execution list used by scheduling algorithms.
int num_processes = 0;          // Number of processes, used for process list indexing.
int remaining_processes = 0;    // Number of processes remaining in the exec_list.
int round_robin_quanta = 2;     // Amount of time allotted to each process 
                                // in the round-robin scheduling algorithm.

//TODO: 
// 1. Add options for quanta in menu
// 2. Add options for scheduling algorithms (replace scheduling option)

/**
 * Prints out a user interface with four options:
 * 1. Create a New Process, 2. Schedule a Process, 3. Terminate a Process, 4. Process Information
 */
void userInterface();

/**
 * Creates a new process and adds it to the list of processes.
 * User must manually enter the process's information:
 * ID, Priority, Start Time, and Work Time. State is automatically set to Ready.
 */
void createProcess();

/**
 * Schedules a process.
 * User must specify which process to schedule, what state it is in, and what state to move it to.
 * If any invalid information is provided, prints an error message.
 */
void scheduleProcess();

/**
 * scheduleProcess helper that ensures valid scheduling inputs.
 * 
 * @param p_index Process index within the process_list.
 * @param p_id Process ID.
 * @param p_curr_state Process's current state.
 * @param p_next_state State the user wants to move the process to.
 * @return bool T/F depending on if the inputs are valid and the state movement is valid.
 */
bool _validateSchedule(int p_index, int p_id, int p_curr_state, int p_next_state);

/**
 * Terminates a process, removing it from the list of processes.
 */
void terminateProcess();

/**
 * terminateProcess helper that adjusts the process list.
 * 
 * @param p_index Index of the process to remove from the process_list.
 */
void _removeProcess(int p_index);

/**
 * Prints a list of all processes with related information (ID, State, etc.)
 */
void printProcesses();

/* Process Scheduling Algorithms */
/**
 * First-Come First-Serve scheduling algorithm.
 * Creates an execution list based on which processes were added first (start_time).
 */
void psa_FCFS();

/**
 * Shortest Job First scheduling algorithm.
 * Creates an execution list based on the work_time value going from least to greatest.
 */
void psa_SJF();

/**
 * Round-Robin scheduling algorithm.
 * 
 */
void psa_RRS();

/**
 * Priority scheduling algorithm.
 * Creates an execution list based on the priority value going from least to greatest.
 */
void psa_PRI();

/**
 * Main function that primarily calls userInterface.
 * 
 * @return int Successful run returns 0, otherwise returns 1.
 */
int main() {
    _clearTerminal();

    char *intro_text_1    = "PROCESS MANAGEMENT SIMULATION     ";
    char *intro_text_2    = "KENNETH WALLACE - CSCI 485        ";
    char *exit_text       = "EXITING PROGRAM ! ! !";
    
    printf("\033[30m"); // black text
    printf("\033[41m"); // red background
    _typewriterPrint(intro_text_1, 3);
    _typewriterPrint(intro_text_2, 2);
    
    printf("\033[0m");  // normal text and background
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
        char *option_1    = "> [1] CREATE A NEW PROCESS";
        char *option_2    = "> [2] SCHEDULE A PROCESS";
        char *option_3    = "> [3] TERMINATE A PROCESS";
        char *option_4    = "> [4] PRINT PROCESS INFORMATION";
        char *option_5    = "> [5] TOGGLE TYPEWRITER EFFECT";
        char *option_6    = "> [6] EXIT PROGRAM";
        char *invalid     = "INVALID COMMAND ! ! !";

        _typewriterPrint(option_menu, 2);
        _typewriterPrint(option_1, 2);
        _typewriterPrint(option_2, 1);
        _typewriterPrint(option_3, 1);
        _typewriterPrint(option_4, 1);
        _typewriterPrint(option_5, 1);
        _typewriterPrint(option_6, 1);
        printf("\n\n  User > ");

        scanf("%d", &user_option);

        _removeLines(10);
        switch (user_option) {
            case 1:
                printf("  User > [1] CREATE A NEW PROCESS");
                createProcess();
                break;
            case 2:
                printf("  User > [2] SCHEDULE A PROCESS");
                scheduleProcess();
                break;
            case 3:
                printf("  User > [3] TERMINATE A PROCESS");
                terminateProcess();
                break;
            case 4:
                printf("  User > [4] PRINT PROCESS INFORMATION");
                printProcesses();
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

void createProcess() {
    int p_id;
    int p_priority;
    int p_start_time;
    int p_work_time;

    char *create_1  = "PLEASE ENTER NEW PROCESS ID (INTEGER, >= 0):";
    char *create_2  = "PLEASE ENTER NEW PROCESS PRIORITY:";
    char *create_3  = "PLEASE ENTER NEW PROCESS START TIME (> 0):";
    char *create_4  = "PLEASE ENTER NEW PROCESS WORK TIME (> 0):";
    char *error_1   = "PROCESS ID MUST BE (>= 0) ! ! !";
    char *error_2   = "PROCESS WITH THAT ID ALREADY EXISTS ! ! !";
    char *error_3   = "START TIME AND WORK TIME MUST BE (> 0) ! ! !";
    char *error_4   = "PROCESS LIST IS FULL ! ! !";
    char *error_5   = "PROCESS ID IS LARGER THAN INTEGER LIMIT ! ! !";
    
    _typewriterPrint(create_1, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_id);

    _removeLines(4);
    printf("  User > NEW PROCESS ID: %d", p_id);

    _typewriterPrint(create_2, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_priority);

    _removeLines(4);
    printf("  User > NEW PROCESS PRIORITY: %d", p_priority);

    _typewriterPrint(create_3, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_start_time);

    _removeLines(4);
    printf("  User > NEW PROCESS START TIME: %d", p_start_time);

    _typewriterPrint(create_4, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_work_time);

    _removeLines(4);
    printf("  User > NEW PROCESS WORK TIME: %d", p_work_time);

    // Check if process id is valid (>= 0) (< 2147483647).
    if (p_id < 0) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_1, 2);
        printf("\033[0m");  // default text
        return;
    }
    else if (p_id >= 2147483647) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_5, 2);
        printf("\033[0m");  // default text
        return;
    }
    

    // Check if process exists.
    for (int i = 0; i < num_processes; i++) {
        if (process_list[i].id == p_id) {
            printf("\033[31m"); // red text
            _typewriterPrint(error_2, 2);
            printf("\033[0m");  // default text
            return;
        }
    }

    // Check if Start Time and Work Time are valid (> 0).
    if (p_start_time <= 0 || p_work_time <= 0) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_3, 2);
        printf("\033[0m");  // default text
        return;
    }

    struct Process new_process;
    new_process.id = p_id;
    new_process.state = 0;
    new_process.priority = p_priority;
    new_process.start_time = p_start_time;
    new_process.work_time = p_work_time;

    // Check if process list is full.
    if (num_processes >= SIZE) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_4, 2);
        printf("\033[0m");  // default text
        return;
    }

    process_list[num_processes] = new_process;
    num_processes++;

    printf("\n\n  Syst > Sucessfully created process %d with priority %d, start time %d, and work time %d.", p_id, p_priority, p_start_time, p_work_time);
}

void scheduleProcess() {
    int p_id;
    int p_curr_state;
    int p_next_state;
    int p_index = -1;

    char *schedule_1    = "PLEASE ENTER PROCESS ID:";
    char *schedule_2    = "PLEASE ENTER CURRENT STATE: (Blocked = -1, Ready = 0, Running = 1)";
    char *schedule_3    = "PLEASE ENTER STATE TO SCHEDULE TO: (Blocked = -1, Ready = 0, Running = 1)";
    char *error_1       = "PROCESS NOT FOUND ! ! !";
    char *error_2       = "SCHEDULE INPUT NOT VALID ! ! !";
    
    _typewriterPrint(schedule_1, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_id);

    _removeLines(4);
    printf("  User > PROCESS ID: %d", p_id);

    _typewriterPrint(schedule_2, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_curr_state);

    _removeLines(4);
    printf("  User > MOVE FROM STATE: %d", p_curr_state);
    
    _typewriterPrint(schedule_3, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_next_state);

    _removeLines(4);
    printf("  User > TO NEW STATE: %d", p_next_state);

    // Check if process exists.
    for (int i = 0; i < num_processes; i++) {
        if (process_list[i].id == p_id) {
            p_index = i;
            break;
        }
    }
    if (p_index == -1) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_1, 2);
        printf("\033[0m");  // default text
        return;
    }

    // Validate user input.
    bool valid = _validateSchedule(p_index, p_id, p_curr_state, p_next_state);
    if (!valid) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_2, 2);
        printf("\033[0m");  // default text
        return;
    }

    process_list[p_index].state = p_next_state;
    printf("\n\n  Syst > Sucessfully moved process %d from state %d to state %d.", p_id, p_curr_state, p_next_state);
}

bool _validateSchedule(int p_index, int p_id, int p_curr_state, int p_next_state) {
    // Check if state inputs are valid
    if (p_curr_state > 1 || p_curr_state < -1) {
        return false;
    }
    else if (p_next_state > 1 || p_next_state < -1) {
        return false;
    }
    
    // Check if current state matches
    if (process_list[p_index].state != p_curr_state) {
        return false;
    }

    // Check if valid next state
    switch (p_curr_state) {
        case -1:    // Blocked.
            switch (p_next_state) {
                case -1:
                    return false;
                case 0:
                    return true;
                case 1:
                    return false;
            }
        case 0:     // Ready.
            switch (p_next_state) {
                case -1:
                    return false;
                case 0:
                    return false;
                case 1:
                    return true;
            }
        case 1:     // Running.
            switch (p_next_state) {
                case -1:
                    return true;
                case 0:
                    return true;
                case 1:
                    return false;
            }
    }
}

void terminateProcess() {
    int p_id;
    int p_index = -1;

    char *remove_1  = "PLEASE ENTER THE ID OF THE PROCESS TO TERMINATE:";
    char *error_1   = "PROCESS NOT FOUND ! ! !";
    
    _typewriterPrint(remove_1, 2);
    printf("\n\n  User > ");
    scanf("%d", &p_id);

    _removeLines(4);
    printf("  User > TERMINATE PROCESS: %d", p_id);

    // Check if process exists.
    for (int i = 0; i < num_processes; i++) {
        if (process_list[i].id == p_id) {
            p_index = i;
            break;
        }
    }
    if (p_index == -1) {
        printf("\033[31m"); // red text
        _typewriterPrint(error_1, 2);
        printf("\033[0m");  // default text
        return;
    }

    _removeProcess(p_index);

    printf("\n\n  Syst > Successfully terminated process %d.", p_id);
}

void _removeProcess(int p_index) {
    for (int i = p_index; i < num_processes - 1; i++) {
        process_list[i] = process_list[i + 1];
    }

    num_processes--;
}

void printProcesses() {
    /* If you are reading this, I was too lazy to create a separate function for printing borders. */
    // Top Border
    printf("\n\n");
    printf("  ");
    for (int i = 0; i < 48; i++) { printf("="); }
    printf("\n");
    printf("  | ----ID---- | STATE | PRIORITY | START | WORK |\n");
    printf("  ");
    for (int i = 0; i < 48; i++) { printf("="); }
    printf("\n");

    for (int i = 0; i < num_processes; i++) {
        int p_id = process_list[i].id;
        int p_state = process_list[i].state;
        int p_priority = process_list[i].priority;
        int p_start = process_list[i].start_time;
        int p_work = process_list[i].work_time;
        printf("  | %-10d | %-5d | %-8d | %-5d | %-4d |\n", p_id, p_state, p_priority, p_start, p_work);
    }

    // Bottom Border
    printf("  ");
    for (int i = 0; i < 48; i++) { printf("="); }
}

