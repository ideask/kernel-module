#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/klog.h>
char buffer[1024]={0};
int main (int argc,char *argv[])
{
   char filenamep[] = "kernel.txt";
   int fd =-1;
   fd = open(filenamep,O_WRONLY|O_CREAT,0777);
   if(fd < 0)
    return -1;
   while(1)
   {
     klogctl(2,buffer,1024);
     write(fd,buffer,strlen(buffer));
   }
   close(fd);
}
