# Assignment-1
<<<<<<< HEAD
#download the user.c file , main.c file Makefile & assignment1.h file
#now open the command line from the working directory and type make all
 this should generate .ko files
# use command sudo insmod main.ko
# now use command sudo chmod 777 /dev/trng_dev
# compile the c code using gcc user.c
# now execute the code using ./a.out
# enter the lower range and press enter
# enter the higher range and press enter
# the random number will be generated
 

=======
This is a true random number generator which generates a random number (POSITIVE ONLY) between the range provided by the user. It works using combination of two functions get_random_bytes provided in linux/random.h and timespec which read kernel time in nanoseconds and provide a random number . A totally random number generating process will first select one of the function out of these two and then the function provide random number to user space. 


The input range starts from 0, in case negative number are required change the type of rand variable from uint to int in main.c .

Procedure :

> Download the main.c,user.c, assignment1.h & Makefile
>in the command line opened from the directory containing these files, type make all
> this should generate .ko files
> push the module to kernel using command sudo insmod main.ko
> type gcc user.c in the command line, it produces executable a.out
> to gain access to device file trng_dev type command
sudo chmod 777 /dev/trng_dev
>now execute the code using ./a.out
if everything works fine command line will ask you to enter the lower number first and then higher number 
press enter after each step and you will see random number generated .
>to remove the module write the command sudo rmmod main.ko
 


you can view the kernel messages also using dmesg after ./a.out (optional) this will show you random number generated and which amongst the two function is used to do so.
>>>>>>> 6d0dcd3dc56ca3fd2d5e067de3705e500b7c0041
