/*user space c code to send the Higher and Lower range from user space to the 
 *kernel space and to get back the random number generated in the kernel module 
 *back to user space*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/ioctl.h> 
#include "assignment1.h" /*to include MAGIC NUMBER for IOCTL calls*/
int *r;		/*global int pointers used in ioctl call*/
int *s;         /*ditto*/  

/*send function to send the Lower range to Kernel space*/

int ioctl_send_msg(int file_desc, int *message)
{
	int ret_val;
	ret_val = ioctl(file_desc, READ_RANDOM , message);
	
	if (ret_val < 0) 
	{
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
			
	}
    return 0;
}


/*function to read Random Number generated in Kernel space
 *& write the higher range to the kernel*/

int ioctl_sendrecieve_msg(int file_desc)
{
	int ret_val;
	int *  messag;
	messag = s;
	

	ret_val = ioctl(file_desc, READWRITE_RANDOM, messag);
	if (ret_val < 0) 
	{
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(-1);
	}
	
	
	printf("The random Number Generated is : %d\n",ret_val);

	return 0;
}


int main()
{
   	int ret, fd;
   	int stringToSend[2];// will hold user entered number
	printf("Starting True random Number Generator : \n");
	fd = open("/dev/trng_dev", O_RDWR); // Open with read/write access
	if (fd < 0)
	{
		printf("Failed to open the device...");
		return errno;
	}

	printf("Enter the lower Number :");
   	scanf("%d" , &stringToSend[0]);
	printf("Enter the Higher Number : ");
	scanf("%d", &stringToSend[1]); 
	if(stringToSend[0]<stringToSend[1])
	{	
		r=stringToSend;       //r holds address of lower range
		s=&(stringToSend[1]);// s holds address of higher range  
		ioctl_send_msg(fd,r );
		ioctl_sendrecieve_msg(fd);
	}		

	else
	printf("Enter in correct format Lower Number first");


	return 0;

}
