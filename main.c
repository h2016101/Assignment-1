/*This is Kernel Module for True random Number Generator, it works based on   
 *combination of get_random_bytes function defined in linux/random.h & 
 *kernel time,the number is generated randomly by picking any one of the 
 *two functions randomly and generating a number in between user defined range.
 *Assignment 1 , Device Drivers ,EEE G547,BITS PILANI
 *Submitted by Sahil Chandna*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/time.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include "assignment1.h"  
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
 
static dev_t first; // variable for major and minor number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls; // variable for the device class
static int firstNumber;//global variable to hold the lower range
static int secondNumber;//global variable to hold the higher range
struct timespec ts;     


/*display system entropy in kernel by reading the file /proc/sys/kernel/random/entropy_avail*/
static void read_file(char *filename)
{
  struct file* fd = NULL;
  size_t bound = 4 ; 
  int entropy =0;
  int conv ,k;
  unsigned char* buf = kmalloc(bound,GFP_USER);
  loff_t offset = 0;
  char __user *locn = (__force char __user *) buf;  
  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);
  fd = filp_open(filename, O_RDONLY, 0);
  if (fd >= 0) {
    printk(KERN_DEBUG);
    vfs_read(fd, locn , 4 ,&offset);
  for (k=0;k<4;k++)
    { conv = (int) *(buf+k);
      conv=conv-48;
      entropy=entropy*10 +conv;
    } 
   printk("entropy is %d", entropy);
    filp_close(fd,NULL);
  }
  set_fs(old_fs);
}

/*ioctl function call mapping 
 *to & from user space to kernel space*/


long device_ioctl(struct file *file,             
                  unsigned int ioctl_num,   
                  unsigned long ioctl_param)	
{		  
	unsigned int rand;		/*random number stored in rand
					 * to access ioctl data*/
	
	int *temp;	
	int ret=0;
	int time_rand;
	unsigned int random;		
	switch(ioctl_num)
	{
		case READ_RANDOM:       /*case 1 : lower range transferred from 
   					 * user space to kernel space */	
           	temp = (int *)ioctl_param;
	
		firstNumber=*temp;	       /*lower range stored in the 
						*global variable firstNumber*/		
		printk(KERN_INFO"%d",firstNumber);
		goto out;
		break;
		
/*in following case Higher Number written to
 * Kernel and Random Number back to user*/

		case READWRITE_RANDOM:	 
		temp = (int *)ioctl_param;
		secondNumber=*temp;   
		printk(KERN_INFO"%d",secondNumber); 
		
	/*******random Number generate code*************/
		get_random_bytes(&random,1);     
	 /*give address of variable which store random number
          * and no of bytes required as parameter*/		
		
		random%=2;     /*select the function 
				*to be used for random no. 0 or 1	       
                                * 0 for get_Random_byes and 1 for time spec */				
		
		if(random==0)	    	
		{
			printk(KERN_INFO"using function%u",random);		
			while(1)      // bound check of random number generated
			{
				get_random_bytes(&rand,4);  
				rand%=secondNumber;	      	
							 			
			if(firstNumber<rand)	       
				{
			            printk(KERN_INFO"random number is %d",rand);
				    ioctl_param = rand; /*random variable 
							stored in ioctl_param*/
				    break ;
	
				}
		
			}
			return ioctl_param;/*return the generated random number
						to user space*/
		}		
	
		
		else if(random==1)
		{	
			printk(KERN_INFO"using function%u",random);	
			while(1) // bound check of random number generated
			{
				ts = current_kernel_time();	
				time_rand=(ts.tv_nsec);
				time_rand%=secondNumber;
			if(firstNumber<time_rand)	       
	
			    {
			       printk(KERN_INFO"random number is %d",time_rand);
			       ioctl_param = time_rand;
                              							
			       break ;
	
		            }
		
			}
			return ioctl_param; /*return the generated random number
					     *to user space*/
	
		}	

		break;

	

	} //end of switch case
    


		goto out;




		out:
		return ret;
}
/******************************************************************************		
******************************************************************************		
******************************************************************************/		
 
/*file open function call*/

 static int my_open(struct inode *i, struct file *f)	 
{
	int ret=0;
	printk(KERN_INFO "trng_dev : open \n");
	goto out;

	out :
	return ret;

}

/******************************************************************************		
 ******************************************************************************		
 ******************************************************************************/		

/*file_operation function of cdev to 
 *map user system calls to kernel functions*/

static struct file_operations fops =
{
 	.owner    = THIS_MODULE,
 	.open     = my_open,
	.unlocked_ioctl = device_ioctl,
	 
};
  
/******************************************************************************		
 ******************************************************************************		
 ******************************************************************************/		




static int __init trng_init(void) 
{
	int a;
	int ret=0;
/*dynamic allocation of device number */
	a=  alloc_chrdev_region(&first, 0, 1, "trngfm"); /*first holds the 
   							  *device number*/
  /* dynamically create device node in dev directory*/

	if ((cls = class_create(THIS_MODULE, "trngfnm")) != NULL)
	    {
		printk(KERN_INFO "class created ");
	    }
	if (device_create(cls, NULL, first, NULL, "trng_dev") != NULL)
	    {
		printk(KERN_INFO "device created ");
	    }

    read_file("/proc/sys/kernel/random/entropy_avail");
  
     
    /* Link fops and cdev to device node */

	cdev_init(&c_dev, &fops);
	if (cdev_add(&c_dev, first, 1) == -1)
	    {
	        device_destroy(cls, first);
	        class_destroy(cls);
	        unregister_chrdev_region(first, 1);
	        return -1;
	    }
	   goto out;

		out :
		return ret;

}
/******************************************************************************		
 *****************************************************************************		
 ******************************************************************************/		
  
static void __exit trng_exit(void) 
{
    cdev_del(&c_dev);
    device_destroy(cls, first);
    class_destroy(cls);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "exiting module\n\n");
}
  
module_init(trng_init);
module_exit(trng_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil");
MODULE_DESCRIPTION("Virtual Char Driver");
