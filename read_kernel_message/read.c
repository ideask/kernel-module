#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/klog.h>
#include <string.h>
  


int main(void)
{
char *buffer = NULL;
int klog_buf_len = 0;
int count = 0;
size_t len =0;
ssize_t read_count = 0;
int buffer_len = 0; 
char delims[] = "\n";  
char *result = NULL; 

   while(1)
    {
      klog_buf_len = klogctl(10,0,0);
      buffer = (char *)malloc(klog_buf_len + 1);
      if(!buffer)
      {
        perror("malloc\n");
        exit(-1);
      }
      count = klogctl(4,buffer,klog_buf_len);
      if(count < 0)
      {
        perror("klogctl read clean\n");
        exit(-1);
      }
      buffer[count] = '\0';
      buffer_len = strlen(buffer);
      if(buffer_len != 0)
      { 
         result = strtok(buffer,delims);
         while(result != NULL)
         {
            printf("<Start>\n");
            printf("%s\n",result);
            printf("<End>\n");
            result = strtok(NULL, delims);
         }
      }
      free(buffer);
    }
 

  return 0;
}
