/* User space file to test file operations module */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#define DEV	"/dev/my_char_dev"
#define BUF_SZ	(10)

int main()
{
	int fd = open(DEV, O_RDWR);
	if (fd<0)
	{
		perror("Cannot open device file");
		exit(EXIT_FAILURE);
	}

	char buf[BUF_SZ];
	printf("Enter the string to be written into %s:: ", DEV);
	scanf("%[^\n]s", buf);
	ssize_t nwrite = write(fd, buf, BUF_SZ);

	int nread = read(fd, (void *)buf, BUF_SZ);
	printf("\nRead from %s is \"%s\"", DEV, buf);
	close(fd);

	return (EXIT_SUCCESS);
}
