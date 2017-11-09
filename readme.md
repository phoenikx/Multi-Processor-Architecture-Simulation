Problem Statement-
write a program which models the operation of a multi-processor architecture using Discrete Event simulation.

We have been given N processors each with a different efficiency.
This is measured by a time multiplier ei which differs for each processor pi and works as follows:

If processor pi with efficiency ei serves job j with service time tj the actual time taken to serve the
job is: ei * tj .
For example, if processor 5 has an efficiency of 1.2 and job 7 has a service time of 5.0 then processor
5 takes a time of 5*1.2=6.0 to serve job 7.


INPUT--->
There is an file which contains the following information.
1. The number of processors.
2. For each processor: the efficiency of the processor.
3. Arrival records consisting of the arrival time and service time of each job.

CONSTRAINTS-->
1. There is a single queue of jobs.
2. Each job will be served by the processor with the best available efficiency (the smallest
value) from the available idle processors.
3. The processors are all initially idle.
4. Jobs must be served in the order in which they arrive.
5. It is not allowed to read and store all data in the beginning,i.e process the data discretely.

GOALS-->
Simulate this process using discrete even simulation and generate the following-
1. The number of jobs served.
2. The time at which the last job completed service.
3. The greatest length reached by the queue.
4. The average length of the queue.
5. The average time spent by a job in the queue. (If a job is served immediately, the queue time is 0.0).
6. For each processor:
	a. The number of jobs it served
	b. The time it spent idle.

APPROACH-->
1. Start by declaring an array of processors and a variable to indicate the number of
processors. Each processor element should contain members for the processor’s priority, job finish time,
idle (or busy) flag - and other variables for keeping the statistics to be printed at the end of the
program. 

2.The main() should start by asking for a filename, then read from the file the number of processors and the
priorities which are stored in the processor array. main() should then read the job arrival events
from the file and process them.

3.To help process the job arrival and processor events, we implement a priority queue. The
elements of the priority queue should contain thee members: the event type, event time and the
service time (for jobs). The element at the top of the priority queue has the smallest event time.
The event type can be a job arriving or a job completing.

4.We also implement a FIFO queue for storing the jobs that arrive when all the processors are busy. The FIFO elements should store the job’s arrival time and the service time.

PSEUDO CODE-->
Read 1st jobArrival event from file and add it to the priority queue-
While event priority queue not empty:
	Get next event from event priority queue
	If Event = jobArrival
		Add job to job FIFO queue
		Read next jobArrival event from file
		If not EOF add Event to event priority queue
	Else // must be a jobComplete event
		Set processor[Event] to idle and sort it
	End if
	If job FIFO not empty and idle processor available:
		Get Next job from FIFO
		Find fastest idle processor 
		set processor’s idle flag to busy...
		calculate processor’s finish time and do its stats...
		add jobComplete event to priority queue
	End if
end while

Print stats