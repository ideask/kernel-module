#include <stdio.h>
#include <string.h>
#include <sys/klog.h>
char buffer[1024]={0};
int main()
{
    char* delim="\n";//分隔符字符串
    char* p=0;//第一次调用strtok
    char* end=0;
    int count =1;
    klogctl(2,buffer,1024);
    end = strrchr(buffer,'\n');
    printf("weibu:%s\n",end);
    p=strtok(buffer,delim);
    while(p!=NULL){//当返回值不为NULL时，继续循环
        printf("{%d}:%s\n",count,p);//输出分解的字符串
	printf("<END>\n");
        p=strtok(NULL,delim);//继续调用strtok，分解剩下的字符串
	count++;
    }
    return 0;
}
