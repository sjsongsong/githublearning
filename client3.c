//两台机器之间传输消息（对话）
//客户端
#include <stdlib.h>
#include <netinet/in.h>
#include<stdio.h> //printf
#include<string.h> //strcpy
#include<sys/socket.h>
#include<netdb.h> //hostent
#include<arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    char message[100];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)//socket函数创建套接字
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;//地址族：IPv4类型的
    serv_addr.sin_port = htons(PORT);///htons 把unsigned short类型从主机序转换到网络序
    serv_addr.sin_addr.s_addr = inet_addr("192.168.126.132");//指定IP地址
      
    // Convert IPv4 and IPv6 addresses from text to binary form 地址转换
    if(inet_pton(AF_INET, "192.168.126.132", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//client通过connect函数发送建立连接的请求
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );//发送hello信息
    valread = read( sock , buffer, 1024);//接受hello信息
    printf("%s\n",buffer );
    printf("Please input the message: ");//输入对话信息
    scanf("%s",&message);
    send(sock,message,strlen(message),0);//发送对话信息
    printf("Message is sent successfully!\n");
    valread = read( sock , buffer, 1024);//接受消息
    printf("%s\n",buffer );
    close(sock);
    close(valread);
    return 0;
}
