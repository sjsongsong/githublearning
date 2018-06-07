//两台机器间传输文件（包括文档、压缩包、图片、视频、音乐及大型文件等）
//客户端
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
  
#define    MAXLINE        1024  
   
int main(int argc,char **argv)  //argc: 整数,用来统计你运行程序时送给main函数的命令行参数的个数;
//* argv[ ]: 字符串数组，用来存放指向你的字符串参数的指针数组，每一个元素指向一个参数
//argv[0] 指向程序运行的全路径名,argv[1] 指向IP,argv[2] 指向Port,argv[3] 指向文件名
{  
    struct sockaddr_in     serv_addr;  
    char                   buf[MAXLINE];  
    int                    sock_id;  
    int                    read_len;  
    int                    send_len;  
    FILE                   *fp;  
    int                    i_ret;  
     
    if (argc != 4) 
    {  
        printf("usage :%s ipaddr portnum filename\n", argv[0]);//必须输入完整的信息：可执行文件+IP+Port
        exit(0); 
    }  
      
    if ((fp = fopen(argv[3],"r")) == NULL)//以只读方式打开文件
    {  
        perror("Open file failed\n");  
        exit(0);  
    }  
      
//创建套接字
    if ((sock_id = socket(AF_INET,SOCK_STREAM,0)) < 0) {  
        perror("Create socket failed\n");  
        exit(0);  
    }
//指定一个socket的一端"ip+port"  
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;//地址族：IPv4类型的  
    serv_addr.sin_port = htons(atoi(argv[2]));//htons 把unsigned short类型从主机序转换到网络序
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); 
////作为客户机，通过connect()函数发送建立连接的请求
    i_ret = connect(sock_id, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));  
    if (-1 == i_ret) 
    {  
        printf("Connect socket failed！\n");  
       return -1;  
    }  
//读出文件内容，发送文件内容 
    bzero(buf, MAXLINE);  
    while ((read_len = fread(buf, sizeof(char), MAXLINE, fp)) >0 ) //读文件
    {  
        send_len = send(sock_id, buf, read_len, 0);//发送给服务器
        if ( send_len < 0 ) 
        {  
            perror("Send file failed！\n");  
            exit(0);  
        }  
        bzero(buf, MAXLINE);  
    }  
  
    fclose(fp);//关闭文件 
    close(sock_id);//读写操作后关闭相应的socket描述字  
    printf("Send Finish！！\n");  
    return 0;  
}  
