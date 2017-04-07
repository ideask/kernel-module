#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/klog.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define DEV_NAME "/dev/module_a"  


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
  int fd = 0;
  int ret = 0;
  fd = open(DEV_NAME, O_RDWR);
  if(fd < 0)
  {
     perror("Open \"DEV_NAME\" Failed!\n");
     exit(1);
  }
 
//  while(1)  //屏蔽这里为了避免B和C模块printk的信息被清空
//  { 
      klog_buf_len = klogctl(10,0,0);//读取klogctl的buff缓冲区大小
      buffer = (char *)malloc(klog_buf_len + 1);//补上字符串结束符'\0'
      if(!buffer)
      {
        perror("malloc\n");
        exit(-1);
      }
      count = klogctl(4,buffer,klog_buf_len);//读取buff后清空buff，避免读取重复内容
      if(count < 0)
      {
        perror("klogctl read clean\n");
        exit(-1);
      }
      buffer[count] = '\0';//补上字符串结束符号
      buffer_len = strlen(buffer);
      if(buffer_len != 0)
      { 
         result = strtok(buffer,delims);//遇到'\n'换行符时分割字符串，实现每行传送
         while(result != NULL)
         {
//            printf("<Start>\n");
//            printf("%s\n",result);
//            printf("<End>\n");
           ret = write(fd,result,strlen(result));//行写入
           if(!ret)
           {
             perror("write \"DEV_NAME\"error\n");
             exit(1);
           }
            result = strtok(NULL, delims);//继续分割字符串，直到遇到'\0'字符串结束符为止
         }
      }
      free(buffer);//释放缓冲区
//  }  //屏蔽这里为了避免B和C模块printk的信息被清空

    close(fd);
    return 0;
}
