/* Assignment No. 7: Inter process communication in Linux using following.
a. Pipes: Full duplex communication between parent and child processes. Parent process writes a pathname of a file (the contents of the file are desired) on one pipe to be read by child process and child process writes the contents of the file on second pipe to be read by parent process and displays on standard output.
*/

#include<stdio.h>  
#include<stdlib.h>
#include<unistd.h> 	//for STDOUT_FILENO
#include<string.h>
#include<sys/types.h> 	//for all types
#include<sys/stat.h>  	//for open
#include<fcntl.h>
#define maxline 30

void client(int,int);
void server(int,int);

int main()
{
  
	int pipe1[2],pipe2[2]; //file descriptors
	int status;
	pid_t pid;

	pipe(pipe1); 		//for client to server. creating pipe for client
	printf("\n\n File descriptors of pipe1 are : %d %d\n\n",pipe1[0],pipe1[1]);

	pipe(pipe2); 		// from server to client creating pipe for server
	printf("\n\n File descriptors of pipe2 are : %d %d\n\n",pipe2[0],pipe2[1]);

	if((pid=fork())==0)
	{
	close(pipe1[1]); //close write fd of client
  	close(pipe2[0]); //close read fd of server
  
  	server(pipe1[0],pipe2[1]); //call client function 
  	exit(0);
    	}
 	else
 	{
  	sleep(5);   
  	close(pipe1[0]); //close read fd of client 
  	close(pipe2[1]); //close write fd of server
  	client(pipe1[1],pipe2[0]);//wait(&status);
  	exit(0);
 	}
}//main ends




//client function
void client(int writefd,int readfd)
{
 	int len,n;
 	char buff[maxline];
 	printf("\n\nEnter pathname to get file from server \n");
 	fgets(buff,maxline,stdin);
 	len = strlen(buff);
 	if(buff[len-1]=='\n')
      	len--;
 	write(writefd,buff,len);
 	while((n=read(readfd,buff,maxline))>0)
 	{
  	write(STDOUT_FILENO,buff,n);
  	sleep(2);
 	}
}//client ends


//server function
void server(int readfd,int writefd)
{
 	int fd;
 	int len,n;
 	char buff[maxline+1];
    
 	if((n=read(readfd,buff,maxline))==0)
 	{
  	printf("\n\nNo pathname: ");
  	exit(0);
 	}

 	buff[n]='\0';
 	fd = open(buff,O_RDONLY);
 	if(fd < 0)
 	{
  	strcpy(buff,"FILE DOES NOT EXISTS \n\n");
  	len=strlen(buff);  
  	write(writefd,buff,len);  
 	}

 	while((n=read(fd,buff,maxline))> 0)
 	{
	write(writefd,buff,n);
 	}
 	close(fd);
}//server ends

/*

(base) kasturi@kasturi-virtual-machine:~$ gcc ipca.c
(base) kasturi@kasturi-virtual-machine:~$  ./a.out


 File descriptors of pipe1 are : 3 4



 File descriptors of pipe2 are : 5 6



Enter pathname to get file from server 
crm.c
/* Chinese Remainder Theorem */

#include <stdio.h>
 
// returns x where (a * x) % b == 1

/*
int mul_inv(int a, int b)
{
	int b0 = b, t, q;
	int x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}
 
int chinese_remainder(int *n, int *a, int len)
{
	int p, i, prod = 1, sum = 0;
 
	for (i = 0; i < len; i++) prod *= n[i];
 
	for (i = 0; i < len; i++) {
		p = prod / n[i];
		sum += a[i] * mul_inv(p, n[i]) * p;
	}
 
	return sum % prod;
}
 
int main(void)
{
	int n[] = { 5, 7, 11 };
	int a[] = { 1, 6, 8 };
 
	printf("%d\n", chinese_remainder(n, a, sizeof(n)/sizeof(n[0])));
	return 0;
}


