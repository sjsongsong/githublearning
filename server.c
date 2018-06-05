// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    char message[100];      

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)//socket函数创建套接字
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))//setsockopt函数对套接字进行设置（防止地址和端口的重用）
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    //指定一个socket的一端"ip+port"
    address.sin_family = AF_INET;//地址族：IPv4类型的
    address.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY表示的是一个服务器上所有的网卡;指定IP地址，htonl 把unsigned long类型从主机序转换到网络序
    address.sin_port = htons( PORT );//htons 把unsigned short类型从主机序转换到网络序
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)//bind函数给一个套接字指定一个地址
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)//作为server调用listen来监听socket
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)//接受client发送来的连接请求
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);//接收hello信息
    send(new_socket , hello , strlen(hello) , 0 );//发送消息
    printf("%s\n",buffer );//输出hello信息
    valread = read( new_socket , buffer, 1024);//接收对话信息
    printf("%s\n",buffer );//输出对话信息
    printf("Please input the message: ");//输入对话信息
    scanf("%s",&message);
    send(new_socket , message , strlen(message) , 0 );//发送消息
    printf("Message is sent successfully\n");
    return 0;
}
