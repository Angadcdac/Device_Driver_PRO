/*User space file to test file operations module */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEV	"/dev/cdac_dev"
#define BUF_SZ	(10)

int main()
{
	int fd = open(DEV, O_RDWR);
	if (fd < 0)
	{
		perror("Cannot open device file");
		exit(EXIT_FAILURE);
	}
	char buf[BUF_SZ] = "Hello";
	ssize_t nwrite = write(fd, buf, BUF_SZ);
	int nread = read(fd, (void *)buf, BUF_SZ);
	close(fd);
	return (EXIT_SUCCESS);
}
/*******************************************************************************************************/
// #define DEV "/dev/cdac_dev"
/*
 * DEV - Device file path
 *
 * This macro defines the path of the device file in the /dev directory
 * that the user-space application will interact with.
 *
 * The device file (/dev/cdac_dev) represents the character device
 * created by the kernel driver. All system calls like open(), read(),
 * write(), and ioctl() are performed on this file.
 *
 * Example:
 *  fd = open(DEV, O_RDWR);
 *
 * Note:
 *  This device file must be created either manually using mknod
 *  or automatically using device_create() in the driver.
 */
