# Process Management Simulation - C
J. Kenneth Wallace, CSCI 485

### Information
A simulation that allows users to create, schedule, and terminate processes.

### Implementation
Each process used has:
1. Process ID (int)
2. Process State (int)
3. Priority (int)
4. Start Time (int)
5. Work Time (int)

The Process ID is a unique identifier for each process. The Process State can be Blocked (-1), Ready (0), or Running (1). Priority and Start Time are used to determine the process run order in certain scheduling algorithms. Lastly, the Work Time is the amount of work that needs to be done for a process to finish.

This simulation has a user interface with six options:
1. Create a New Process
2. Schedule a Process
3. Terminate a Process
4. Process Information
5. Toggle Typewriter Effect
6. Exit Program

When creating a new process, the user must provide the process's information (id, priority, start time, and work time). After that, the program will create the process and automatically put it into the ready state.

When scheduling a process, the user must tell the program which process to schedule, what state that process is in, and which state to move it to. If any information is entered incorrectly the program will return an error message.

The user can terminate a process by providing its process ID.

When the user asks for the process information, the program will list all processes with their information (ID, State, etc.)

Toggle Typewriter Effect simply makes all text immediately print rather than delaying each character by 10ms.

Exit program exits the program entirely.

### Testing
Compilation:
`make`

Testing:
`./PRM` on Linux
`PRM.exe` on Windows
