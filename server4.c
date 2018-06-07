//两台机器的间传输文件（包括文本文档、压缩文件、图片等）
//服务器端
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
  
#define    MAXLINE        1024  
  
int main(int argc,char **argv)   
{  
    struct sockaddr_in     serv_addr;  
    struct sockaddr_in     clie_addr;  
    char                   buf[MAXLINE];  
    int                    sock_id;  
    int                    link_id;  
    int                    recv_len;  
    int                    write_leng;  
    int                    clie_addr_len;  
    FILE                   *fp;  
  
    if (argc != 3)//保证输入信息的完整性：端口号+文件名 
    {  
        printf("usage :%s portnum filename\n", argv[0]);  
        exit(0);
    }  
//创建套接字
    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {  
        perror("Create socket failed\n");  
        exit(0);  
    }  
//制定一个套接字的"ip+port"
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;////地址族：IPv4类型的  
    serv_addr.sin_port = htons(atoi(argv[1]));  //制定port,htons 把unsigned short类型从主机序转换到网络序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //INADDR_ANY表示的是一个服务器上所有的网卡;指定IP地址，htonl 把unsigned long类型从主机序转换到网络序
  //通过bind（）函数设置套接字，防止IP和port的重用
    if (bind(sock_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
    {  
        perror("Bind socket failed\n");  
        exit(0);  
    }  
//作为服务器，监听套接字
    if (listen(sock_id, 10)==-1) 
    {  
        perror("Listen socket failed！\n");  
        exit(0);  
    }  
//服务器接收，打开文件（若没有，则建立文件）
        //”a+“ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
        if ((fp = fopen(argv[2], "a+")) == NULL) 
        {  
            perror("Open file failed！\n");  
            exit(0);  
        }  
        clie_addr_len = sizeof(clie_addr); 
//接受建立连接的请求
        link_id = accept(sock_id, (struct sockaddr *)&clie_addr, &clie_addr_len);  
        if (-1 == link_id) {  
            perror("Accept socket failed\n");  
            exit(0);  
        }
//接受文件，写入文件  
        bzero(buf, MAXLINE);  
        while (recv_len = recv(link_id, buf, MAXLINE, 0))//接受
        {  
            if(recv_len < 0) 
            {  
                printf("Recieve Data From Server Failed!\n");  
                break;  
            }  
            write_leng = fwrite(buf, sizeof(char), recv_len, fp);//写入
            if (write_leng < recv_len) 
            {  
                printf("Write file failed\n");  
                break;  
            }  
            bzero(buf,MAXLINE);//置为０
        }  
        printf("\nFinish Recieve\n");  
        fclose(fp);  
        close(link_id);//读写操作后关闭相应的socket描述字
    	close(sock_id);   
    return 0;  
}  
