/* ECE3220 Lab 9 template
 * Author: Luis Rivera
 *
 * This program shows how to map the ports of the TS7250 board so they can be used
 * in an application (User Space). */

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

    fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
	    return(-1);  // failed open
	}

	// We need to map Address 0x80840000 (beginning of a memory page)
	// An entire memory page is mapped. We need to point to the beginning of the page.
	BasePtr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
    if(BasePtr == MAP_FAILED){
    	printf("\n Unable to map memory space \n");
    	return(-2);
    }  // failed mmap

	// To access other registers in the page, we need to offset the base pointer to reach the
	// corresponding addresses. Those can be found in the board's manual.
	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
    PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

    *PBDDR |= 0x80;			// configures port B7 as output (Green LED)
	*PBDDR &= 0xFFFFFFFE;	// configures port B0 as input (first push button). You could use: &= ~0x01

	// The program will turn on the green LED, sleep for a while, then off, sleep, then on again, then off.
	// You could use loops, if you wanted/needed.
	//*PBDR &= 0x80; //turns off green before starting
	//*PBDR &= 0x40; //turns off yellow before starting

    *PBDR |= 0x80;	// ON: write a 1 to port B0. Mask all other bits.
	sleep(1);	// How can you sleep for less than a second?
    *PBDR &= ~0x80;	// OFF: write a 0 to port B0. Mask all other bits.
    sleep(1);
    *PBDR |= 0x80;
	sleep(2);
    *PBDR &= ~0x80;

    *PBDR |= 0x80; //green
    sleep(1);
    *PBDR &= ~0x80;

    *PBDDR |= 0x40; // makes B6 output
    *PBDR |= 0x40; //yellow
    sleep(1);
    *PBDR &= ~0x40;

    *PBDDR |= 0x20; //makes B5 output
    *PBDR |= 0x20; //red
    sleep(1);
    *PBDR &= ~0x20;

    *PBDDR |= 0xE0; //makes B7, B6, & B5 all outputs
    *PBDR |= 0xE0; //all
    sleep(1);
    *PBDR &= ~0xE0;
	// If you wanted to read the status of Port B0, how could you do it?

    close(fd);	// close the special file
    return 0;
}