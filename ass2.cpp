#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

//struct for events in priority queue
struct node
{
	int event_type;  
	double event_time;  
	double service_time;
	struct node *link;
	node(int type,double et,double st)
	{
		link=NULL;
		event_type=type;
		event_time=et;
		service_time=st;
	}
};

//for jobs
struct job
{
	double arrival_time;
	double service_time;
	double waiting_time;
	struct job *link;
	job(double atime,double stime)
	{
		arrival_time=atime;
		service_time=stime;
		waiting_time=0;
		link=NULL;
	}
};

//for processors
struct processor
{
	int id;
	double efficiency;
	double finish_time;
	double served_time;
	bool is_idle;
	int jobs_served;
	processor()
	{
		is_idle=true;
		jobs_served=0;
		served_time=0;
	}
};

//array of processors with max size 20
processor processors[20];
int num_processors;  //global variable to store number of processors

//implementation of priority queue
class PQueue
{
    private:
        node *front;
    public:
        PQueue()
        {
            front = NULL;
        }
       
       	//insertion in priority queue
        void insert(int type, double et,double st )
        {
            node *tmp, *q;
            tmp = new node(type,et,st);
            if (front == NULL || et < front->event_time)
            {
                tmp->link = front;
                front = tmp;
            }
            else
            {
                q = front;
                while (q->link != NULL && q->link->event_time <= et)
                    q=q->link;
                tmp->link = q->link;
                q->link = tmp;
            }
        }

        //getting event with most priority
        node *get()
        {
        	if(front!=NULL)
        	{
        		node *tmp=front;
        		front=front->link;
        		return tmp;
        	}
        	return front;
        }

        //check if priority queue is empty
        bool empty()
        {
        	return front==NULL;
        }

};

//implementation of queue using linked list
class Queue{
	private:
		job *back,*front;
	public:
		Queue()
		{
			back=front=NULL;
		}
		//insert job at back of queue
		void insert(double atime,double stime)
		{
			if(front==NULL)
			{
				front=new job(atime,stime);
				back=front;
			}
			else
			{
				back->link=new job(atime,stime);
				back=back->link;
			}
		}

		//get a job from front of queue
		job *get()
		{
			if(front!=NULL)
			{
				job *tmp=front;
				front=front->link;
				return tmp;
			}
			return front;
		}

		//check if queue is empty
		bool empty()
		{
			return front==NULL;
		}

		//returns size of queue
		int size()
		{
			if(front==NULL)
				return 0;
			int cnt=1;
			job *tmp=front;
			while(tmp!=back)
			{
				cnt++;
				tmp=tmp->link;
			}
			return cnt;
		}
};

//used for sorting processors on the basis of availability and efficiency
bool sortingFun(processor a,processor b)
{
	if(a.is_idle==b.is_idle)
		return a.efficiency<=b.efficiency;
	else
		return a.is_idle>b.is_idle;
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	char filename[100];

	//read filename and no. of processors
	cout<<"Enter filename: ";
	scanf("%s",filename);
	fp=fopen(filename,"r");
	fscanf(fp,"%d",&num_processors);

	//if invalid no. of processors, exit the program with message
	if(num_processors<=0 || num_processors>20)
	{
		cout<<"No. of processors either too less or too high!"<<endl;
		return 0;
	}

	//read efficiency of processors
	for(int i=0;i<num_processors;i++)
	{
		fscanf(fp,"%lf",&processors[i].efficiency);
		processors[i].id=i;
	}

	//sorting processors on the basis of efficiency
	sort(processors,processors+num_processors,sortingFun);

	double atime,stime;  //for reading arrival time and service time of jobs
	PQueue pq; //Priority queue for events
	Queue q; //Queue for storing job arrival

	fscanf(fp,"%lf %lf",&atime,&stime);  //read arrival time and service time for first job

	int TYPE_ARRIVAL=num_processors,num_jobs=0,max_queue_size=0;
	double sum_queue_length=0,total_waiting_time=0,last_time=atime;

	//insert arrival of first job as event in priority queue
	pq.insert(TYPE_ARRIVAL,atime,stime);
	double curr_time=atime,start_time=atime; //for current time and start time of simulation
 
	while(!pq.empty())
	{
		//get event with most priority
		node *tmp=pq.get();
		curr_time=tmp->event_time;
		
		//job arrives
		if(tmp->event_type==TYPE_ARRIVAL)
		{
			num_jobs++;  //incrementing #jobs
			sum_queue_length+=(curr_time-last_time)*q.size();
			last_time=curr_time;
			q.insert(tmp->event_time,tmp->service_time);
			if(fscanf(fp,"%lf %lf",&atime,&stime)!=EOF)
			{
				pq.insert(TYPE_ARRIVAL,atime,stime);
			}
		}
		//job checkouts
		else
		{
			for(int i=0;i<num_processors;i++)
			{
				if(processors[i].id==tmp->event_type)
				{
					processors[i].is_idle=true;
					processors[i].jobs_served++;
					processors[i].finish_time=tmp->event_time;
					break;
				}
			}
			//sort(processors,processors+num_processors,sortingFun);
		}

		//allotting best available processor to job in front of queue
		if(!q.empty())
		{
			sum_queue_length+=(curr_time-last_time)*q.size();
			last_time=curr_time;
			sort(processors,processors+num_processors,sortingFun);
			max_queue_size=max(max_queue_size,q.size());
			for(int i=0;i<num_processors;i++)
				if(processors[i].is_idle)
				{
					job *temp=q.get();
					processors[i].is_idle=false;
					total_waiting_time+=curr_time-temp->arrival_time;
					processors[i].served_time+=processors[i].efficiency*temp->service_time;
					double tm=curr_time+(processors[i].efficiency*temp->service_time);
					pq.insert(processors[i].id,tm,-1);
					break;
				}
			
		}
	}

	sort(processors,processors+num_processors,sortingFun);  //for printing in ascending order of efficiency
	//printing the stats
	cout<<"No. of jobs served: "<<num_jobs<<endl;
	cout<<"Time at which last job was served: "<<curr_time<<endl ;
	cout<<"Greatest length reached by queue: "<<max_queue_size<<endl;
	cout<<"Average queue length: "<<sum_queue_length/(curr_time-start_time)<<endl;
	cout<<"Average waiting time for jobs: "<<total_waiting_time/num_jobs<<endl;

	cout<<"processor#	"<<"efficiency	"<<"jobs served   "<<"Idle time"<<endl;
	for(int i=0;i<num_processors;i++)
		cout<<processors[i].id<<"\t\t"<<processors[i].efficiency<<"\t\t"<<processors[i].jobs_served<<"\t\t   "
		<<(curr_time-start_time)-processors[i].served_time<<endl;
}
