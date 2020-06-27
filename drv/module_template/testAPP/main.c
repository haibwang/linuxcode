#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    int i=0;
    int size=0;
    char buff[1024];
    char *buf = "hello world,this is char device driver test\n";

    
    fd = open("/dev/char_dev",O_RDWR);
    if(fd < 0)
    {
        printf("open failed\n");
        return -1;
    }

    size = write(fd, buf, strlen(buf));
    if(size<0)
    {
        printf("write failed\n");
        if(close(fd) <0)
        {
            printf("close failed\n");
        }
    }

    size = read(fd, buff, sizeof(buff));
    printf("read size = %d\n",size);
    while(size--)
    {
        printf("%c",buff[i++]);
    }

    sleep(1);
    if(close(fd) <0)
    {
        printf("close failed\n");
    }
    
    return 0;
}
