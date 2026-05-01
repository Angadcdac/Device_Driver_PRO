/* User space file to test userspace / kernelspace data exchange module */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#define DEV	"/dev/cdac_dev"

int main()
{
	int fd = open(DEV, O_RDWR);
	if (fd<0)
	{
		perror("Cannot open device file");
		exit(EXIT_FAILURE);
	}

	char buf[] = "Hello from user space!\n";
	ssize_t nwrite = write(fd, buf, strlen(buf));
	
	char *buf1 = (char *)calloc(23, sizeof(char));
	int nread = read(fd, (void *)buf1, 23);
	write(STDOUT_FILENO, buf1, strlen(buf1));//print data to the terminal
	
	free(buf1);
	close(fd);
	return (EXIT_SUCCESS);
}
/*******************************************************************************************************/
/*
   #define DEV "/dev/cdac_dev"
 * DEV - Device file path
 *
 * This macro defines the path of the character device file
 * created by the kernel driver in the /dev directory.
 *
 * The user application will use this path to open the device
 * and perform operations like read(), write(), and ioctl().
 * 
 // ssize_t nwrite = write(fd, buf, strlen(buf));#define DEV
 * write() - Send data from user space to kernel driver
 *
 * fd      : File descriptor of the opened device file
 * buf     : Pointer to user buffer containing data to send
 * strlen(buf) : Number of bytes to write (excluding '\0')
 *
 * This system call transfers data from user space to kernel space.
 * It internally triggers the driver's my_write() function.
 *
 * Return value:
 *  nwrite > 0  → number of bytes successfully written
 *  nwrite < 0  → error occurred
 */
/*******************************************************************************************************/
//	int nread = read(fd, (void *)buf1, 23);
/*
 * read() - Receive data from kernel driver to user space
 *
 * fd      : File descriptor of the opened device file
 * buf1    : Pointer to user buffer where data will be stored
 * 23      : Maximum number of bytes to read
 *
 * This system call requests data from the kernel driver.
 * It internally triggers the driver's my_read() function,
 * which copies data from kernel space to user space.
 *
 * Return value:
 *  nread > 0  → number of bytes successfully read
 *  nread = 0  → End Of File (no more data)
 *  nread < 0  → error occurred
 */
/*******************************************************************************************************/
//	write(STDOUT_FILENO, buf1, strlen(buf1));
/*
 * write() - Print data to standard output (terminal)
 *
 * STDOUT_FILENO : File descriptor for standard output (usually the terminal)
 * buf1          : Pointer to buffer containing data to display
 * strlen(buf1)  : Number of bytes to write (length of string)
 *
 * This system call writes data from the user buffer (buf1)
 * directly to the terminal (screen).
 *
 * Note:
 *  strlen(buf1) is used assuming buf1 is a null-terminated string.
 *
 * Return value:
 *  > 0  → number of bytes successfully written
 *  < 0  → error occurred
 *//*******************************************************************************************************/

