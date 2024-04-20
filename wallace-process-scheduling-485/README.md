# Process Scheduling Simulation - C
J. Kenneth Wallace, CSCI 485

### Information
A simulation that visualizes four process scheduling algorithms:
1. First-Come First-Serve (FCFS)
	- Order processes by [arrival_time] and execute in order.
2. Shortest Job First (SJF)
	- Order processes by [burst_time]. Initialize a [current_time] to 0.
	- Pick the shortest available job from processes that are ready ([arrival_time] <= [current_time]).
	- Does not stop mid-execution.
3. Round-Robin Scheduling (RRS)
	- Order processes by [arrival_time] and execute in order.
	- Each process can only execute for X amount of time, where X is the time quantum.
	- Default time quantum is 2.
4. Priority Scheduling (PRI)
	- Order processes by [priority]. Initialize a [current_time] to 0.
	- Pick the highest priority from processes that are ready ([arrival_time] <= [current_time]).
	- Does not stop mid-execution.
	- Lower value == Higher priority.

### Implementation
The program lets the user choose one of the four scheduling algorithms listed above.

While running, the system will print out information when any of the following events occur:
1. Process changes state (moving from one state to another).
	- This will print out the Process, what state it was in, and what state it is moving to.
2. Process is completed.
	- This will print out a completion message.
3. All processes are complete.
	- This will print out the average wait time (x) and the turnaround time (y).

The process list is hard-coded (can be changed if code is edited). Here are the default processes:

| ID   | PRIORITY   | ARRIVAL TIME   | BURST TIME   |
| :--- | :--------- | :------------- | :----------- |
| 10   | 2          | 0              | 6            |
| 20   | 4          | 2              | 20           |
| 30   | 6          | 8              | 10           |
| 40   | 8          | 6              | 4            |
| 50   | 0          | 4              | 8            |
| 60   | 1          | 10             | 13           |

Results:

| Algorithm   | Total turn.  | Total wait    | Avg. turn.  | Avg. wait    |
| :---------- | :----------- | :------------ | :---------- | :----------- |
| FCFS        | 183          | 122           | 30.500      | 20.333       |
| SJF         | 134          | 73            | 22.333      | 12.167       |
| RRS (2)     | 247          | 216           | 41.167      | 36.000       |
| RRS (3)     | 249          | 219           | 41.500      | 36.333       |
| RRS (4)     | 237          | 206           | 39.500      | 34.333       |
| PRI         | 182          | 121           | 30.333      | 20.167       |

### Testing
Compilation:
`make`

Testing:
`./PRS` on Linux
`PRS.exe` on Windows
