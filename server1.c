//一台机器上不同端口单向传输字符（信息）
//服务器端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])//argc: 整数,用来统计你运行程序时送给main函数的命令行参数的个数;
//* argv[]: 字符串数组，用来存放指向你的字符串参数的指针数组，每一个元素指向一个参数
//argv[0] 指向程序运行的全路径名,argv[1] 指向Port
{
    int serv_sock, clnt_sock;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);//必须输入完整的信息：可执行文件+Port
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);//socket函数创建套接字
    if (serv_sock < 0) {
        printf("sock() error\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;//地址族：IPv4类型的
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//htons 把unsigned short类型从主机序转换到网络序
    serv_addr.sin_port = htons(atoi(argv[1]));

    int optval = 1;
    // setting address and port reusable
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {//bind()函数指定一个地址，防止IP和Port的重用
        printf("bind() error\n");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {//调用listen()来监听socket
        printf("listen() error\n");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);//接收建立连接的请求

    if (clnt_sock < 0) {
        printf("accept() error\n");
        exit(1);
    }

    char buf[1024];
    printf("connected with ip: %s and port: %d\n",inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, 1024),
           ntohs(clnt_addr.sin_port));
    
    while (1) {//服务器一直处于监听状态，除非遇“＃”结束
        memset(buf, 0, sizeof(buf));
        ssize_t size = read(clnt_sock, buf, sizeof(buf) - 1);//接收来自client的信息
        if (size < 0) {
            printf("read() error\n");
            break;
        } else if(buf[0]=='#') 
         {
	break;
	}
	else{
            printf("client#: %s\n", buf);//输出接收到的信息
        }
    }
    close(clnt_sock);//读写操作后关闭相应的socket描述字
    close(serv_sock);
    return 0;
}
