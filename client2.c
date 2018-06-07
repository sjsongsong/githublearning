//两台机器间单向传输信息
//客户端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])//argc: 整数,用来统计你运行程序时送给main函数的命令行参数的个数;
//* argv[ ]: 字符串数组，用来存放指向你的字符串参数的指针数组，每一个元素指向一个参数.
//argv[0] 指向程序运行的全路径名,argv[1] 指向IP,argv[2] 指向Port
{
    int clnt_sock;//一个客户机的socket描述符
    struct sockaddr_in serv_addr;//地址协议族
    int str_len;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);//必须输入完整的信息：可执行文件+IP+Port
        exit(1);
    }

    clnt_sock = socket(AF_INET, SOCK_STREAM, 0);//socket函数创建套接字
    if (clnt_sock < 0) {
        printf("sock() error\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//地址族：IPv4类型的
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//htons 把unsigned short类型从主机序转换到网络序
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)//client通过connect函数发送建立连接的请求
     {   printf("connect() error\n");
        exit(1);
    }

    char buf[1024];
    printf("please input # to end!!\n");//规定结束输入信息的标志
    while (1) {//未结束输入，一直可以输入信息，传输给server
        memset(buf, 0, sizeof(buf));
        printf("client enter: ");
        fflush(stdout);//清空stdout的缓冲区
        ssize_t size = read(0, buf, sizeof(buf) - 1);//读出输入在屏幕上的内容
        if (size < 0) {
            printf("read() error\n");
            break;
        } else if(buf[0]=='#')//遇“＃”结束
	{
	  buf[size - 1] = '\0';
          write(clnt_sock, buf, strlen(buf));
           break;
        }
	else {
            buf[size - 1] = '\0';
        }
        write(clnt_sock, buf, strlen(buf));//发送给server
    }
    close(clnt_sock);//完成了读写操作就要关闭相应的socket描述字

    return 0;
}

