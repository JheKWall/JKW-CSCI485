# Process Scheduling Simulation - C
J. Kenneth Wallace, CSCI 485

### Information
A simulation that visualizes four process scheduling algorithms:
1. First-Come First-Serve (FCFS)
2. Shortest Job First (SJF)
3. Round-Robin Scheduling (RRS)
4. Priority Scheduling (PRI)

### Implementation
The program lets the user choose one of the four scheduling algorithms listed above.

While running, the system will print out information when any of the following events occur:
1. Process changes state (moving from one state to another).
	- This will print out the Process, what state it was in, and what state it is moving to.
2. Process is completed.
	- This will print out a completion message.
3. All processes are complete.
	- This will print out the average wait time (x) and the turnaround time (y).

### Testing
Compilation:
`make`

Testing:
`./PRS` on Linux
`PRS.exe` on Windows
