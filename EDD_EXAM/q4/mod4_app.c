

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

#define DEV     "/dev/pradeep_dev"

int main()
{
        int fd = open(DEV, O_RDWR);
        if (fd<0)
        {
                perror("Error in open device file");
                exit(EXIT_FAILURE);
        }

        char buffer[] = "I am sending Hello World\n";

        ssize_t nwrite = write(fd, buffer, strlen(buffer));

        char *buffer1 = (char *)calloc((strlen(buffer)+1), sizeof(char));
        int nread = read(fd, (void *)buffer1, 5);
        close(fd);

        return (EXIT_SUCCESS);
}

