#include <stdio.h>
#include <string.h>
#include <sys/klog.h>
char buffer[512]={0};
char buffer1=NULL;
char buffer2=NULL;
/*
char *pinjie(char *str1,char *str2)//拼接字符串函数
{
    char *result = malloc(strlen(str1)+strlen(str2)+1);
    if(result == NULL)
       exit(1);
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char *get_string (char *buffer)
{
    char* delim="\n";//分隔符字符串
    char* p=0;//第一次调用strtok
    char* end=0;
    int count =1;
    end = strrchr(buffer,'\n');
    p=strtok(buffer,delim);
    while(p!=NULL){//当返回值不为NULL时，继续循环
	if(strcmp(end,p) == 0)
	{
           printf("{%d}:%s\n",count,p);//输出分解的字符串
	}
	printf("<END>\n");
        p=strtok(NULL,delim);//继续调用strtok，分解剩下的字符串
	count++;
    }
 
}
*/
int main()
{
    char* delim="\n";//分隔符字符串
    char* p=0;//第一次调用strtok
    char* end=0;
    int count =1;
    klogctl(2,buffer,512);
    end = strrchr(buffer,'\n');
    if(end != NULL)
      printf("weibu:%s\n",end);
    p=strtok(buffer,delim);
    while(p!=NULL){//当返回值不为NULL时，继续循环
	if(strncmp(end,p,strlen(end)) == 0)
	{
           printf("{%d}:%s\n",count,p);//输出分解的字符串
	   printf("the end length is :%d\n",strlen(end));
	}
	printf("<END>\n");
        p=strtok(NULL,delim);//继续调用strtok，分解剩下的字符串
	count++;
    }
    return 0;
}
