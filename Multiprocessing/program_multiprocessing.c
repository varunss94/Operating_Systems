
/* Varun Subramanya*/

#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>


int main(int argc, char *argv[])
{

unsigned int size = atoi(argv[1]);

pid_t pid[size], pid1;

int i = 0, x , status; //x[i], , y[i];
int childNum;
//childNum = i+1;
printf("Parent process %d created\n",getpid());
/* Creating child processes */
for(i=0;i<size;i=i+1) 
{
	pid[i] = fork();
	if(pid[i] < 0) 


	{	
	printf("\n Error");
	exit (1);
	}

	else if (pid[i] == 0) 

	{
	
	childNum = i+1;
	printf(" Child %d Created PID: %d \n",childNum, getpid());
//	for (z = 0; z <= childNum; z++)	

x =getpid();


switch(x%5)
{
	case 0: 
		execlp("./test1","./test1",NULL);
		break;
	case 1: 
		execlp("./test2","./test2",NULL);
		break;
	case 2: 
		execlp("./test3","./test3",NULL);
		break;	
	case 3: 
		execlp("./test4","./test1",NULL);
		break;
	case 4: 
		execlp("./test5","./test5",NULL);
		break;
	
	default: 
		printf("The PID Value is wrong/n");
		break;
}



	//execlp("./test3","./test3",NULL);
sleep (5);

//x[i] = getpid();
	//y[i] = childNum;
	//printf("%d\n", childNum);
exit (0);
	}
	}

/* Child Exit Wait and Removal of PID */
/* Parent Process*/
	
int count = size;
	while (count >0)
	{
	pid1 = wait(NULL);
	sleep(10);	
//pid1= wait(&status);
for (i =0 ; i<size; i++)
{	
if (pid1 = pid[i])
	{
	
	childNum = i+1;	
	printf("\n Process child %d (PID %d) is done", childNum, pid1);
	printf("\n");
	
}
count  --;
}
exit (0);
}

//while (1);
}

//pid1);	

	//printf("%d \n", pid1);

/*//if (pid[size] == x[i])
//if (pid[i] = x[i])
if (pid[i] = x[i])
{
	//printf("\n %d and PID:%d", i, pid[i]); 	
	//i =0 ;	
	childNum = i+1;	
	printf("\n Process child %d (PID %d) is done", childNum, pid[i]);//pid1);
}
	//i--;*/	
	//size--;
//}




