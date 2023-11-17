
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>    
#include <stdlib.h>    
#include <unistd.h>  

void quicksort(int [10],int,int);
void main()
{
	int pid, ppid, a[10], size, i;

	printf("\nPARENT: My process-id is %d.", getpid());
	
	printf("\n\nEnter the number of elements in the array: ");
	scanf("%d", &size);
	printf("Enter %d elements: ", size);
  	for(i=0;i<size;i++)
    	scanf("%d",&a[i]);
	
	printf("\nForking now!\n");

	pid = fork();			//Here, the child process is created and both child & parent run 
							//simultaneously. fork() returns 0 to child process and pid of 								//child to parent process. So, if int pid here is 0, it means we 								//are in child process and if any +ve no., then in parent process 								//where the +ve no. is the child's pid.
	if(pid==0)
	{
		printf("\nCHILD: My process-id is %d.", getpid());
		printf("\nCHILD: My parent's process-id is %d.", getppid());
		quicksort(a, 0, size-1);
		
		printf("\n Sorting elements using Quick Sort: ");
 		for(i=0;i<size;i++)
    		printf(" %d",a[i]);
		
		printf("\nCHILD: I am dying now");
		printf("\n-------------------------------------------------------");
		//	ORPHAN
		printf("\nCHILD: I am sleeping now.");
		sleep(10);
		printf("\n-------------------------------------------------------\n\n");
		system("ps -elf | grep a.out");

		/*  ZOMBIE
		printf("\nCHILD: I am dying now");
		printf("\n-------------------------------------------------------\n\n");
		system("ps -elf | grep a.out");
		exit(0);*/
	}
	else if (pid>0)
	{
		system("wait");
		printf("\nPARENT: I am back. ");
		printf("\nPARENT: My process-id is %d.", getpid());	
		printf("\nPARENT: My child's process-id is %d.", pid);
		quicksort(a, 0, size-1);
		
		printf("\nSorted elements: ");
 		for(i=0;i<size;i++)
    		printf(" %d",a[i]);
		
		printf("\nPARENT: I am dying now.");
		printf("\n-------------------------------------------------------");
		//  ORPHAN
		system("ps -elf | grep a.out");
		exit(0);

		//ZOMBIE
		printf("\nPARENT: I am sleeping now.");
		sleep(10);
		printf("\n-------------------------------------------------------\n");
		system("ps -elf | grep a.out");
	}
}

void quicksort(int a[10],int first,int last)
{
    int pivot,j,temp,i;

    if(first<last)
	{
		pivot=first;
        i=first;
        j=last;

        while(i<j){
            while(a[i]<=a[pivot]&&i<last)
                i++;
            while(a[j]>a[pivot])
                j--;
            if(i<j){
				temp=a[i];
                a[i]=a[j];
                a[j]=temp;
             }
         }

         temp=a[pivot];
         a[pivot]=a[j];
         a[j]=temp;
         quicksort(a,first,j-1);
         quicksort(a,j+1,last);
    }
}


/*
====================================OUTPUT====================================
(base) kasturi@kasturi-virtual-machine:~$ gedit Assign2.c
(base) kasturi@kasturi-virtual-machine:~$ gcc Assign2.c
(base) kasturi@kasturi-virtual-machine:~$ ./a.out

PARENT: My process-id is 3740.

Enter the number of elements in the array: 4
Enter 4 elements: 6 3 0 2

Forking now!

CHILD: My process-id is 3741.
CHILD: My parent's process-id is 3740.
 Sorting elements using Quick Sort:  0 2 3 6
CHILD: I am dying now
-------------------------------------------------------

PARENT: I am back. 
PARENT: My process-id is 3740.
PARENT: My child's process-id is 3741.
Sorted elements:  0 2 3 6
PARENT: I am dying now.
0 T kasturi     3097    2344  0  80   0 -   693 do_sig 17:51 pts/0    00:00:00 ./a.out
0 S kasturi     3740    2344  0  80   0 -   693 do_wai 18:20 pts/0    00:00:00 ./a.out
1 S kasturi     3741    3740  0  80   0 -   693 hrtime 18:20 pts/0    00:00:00 ./a.out
0 S kasturi     3743    3740  0  80   0 -   722 do_wai 18:20 pts/0    00:00:00 sh -c ps -elf | grep a.out
0 S kasturi     3745    3743  0  80   0 -  4433 pipe_r 18:20 pts/0    00:00:00 grep a.out
-------------------------------------------------------(base) kasturi@kasturi-virtual-machine:~$ CHILD: I am sleeping now.
-------------------------------------------------------

0 T kasturi     3097    2344  0  80   0 -   693 do_sig 17:51 pts/0    00:00:00 ./a.out
1 S kasturi     3741    1582  0  80   0 -   693 do_wai 18:20 pts/0    00:00:00 ./a.out
0 S kasturi     3746    3741  0  80   0 -   722 do_wai 18:20 pts/0    00:00:00 sh -c ps -elf | grep a.out
0 R kasturi     3748    3746  0  80   0 -  4433 -      18:20 pts/0    00:00:00 grep a.out

CHILD: I am dying now
-------------------------------------------------------

0 T kasturi     3097    2344  0  80   0 -   693 do_sig 17:51 pts/0    00:00:00 ./a.out
1 S kasturi     3741    1582  0  80   0 -   693 do_wai 18:20 pts/0    00:00:00 ./a.out
0 S kasturi     3749    3741  0  80   0 -   722 do_wai 18:20 pts/0    00:00:00 sh -c ps -elf | grep a.out
0 R kasturi     3751    3749  0  80   0 -  4433 -      18:20 pts/0    00:00:00 grep a.out
