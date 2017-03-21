#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<sys/socket.h>  
#include<sys/types.h>  
#include<unistd.h>  
#include<netinet/in.h>
#include <arpa/inet.h>

#define PORT 6666  

int main(int argc,char **argv)  
{  
    int sockfd;  
    int err,n;  
    struct sockaddr_in addr_ser;  
    const char *sendline = "hello baby"; 
      
    sockfd=socket(AF_INET,SOCK_STREAM,0);  
    if(sockfd==-1)  
    {  
        printf("socket error\n");  
        return -1;  
    }  
      
    bzero(&addr_ser,sizeof(addr_ser));  
    addr_ser.sin_family=AF_INET;  
    inet_aton( "127.0.0.1",&addr_ser.sin_addr );
    addr_ser.sin_port=htons(PORT);  
    err=connect(sockfd,(struct sockaddr *)&addr_ser,sizeof(addr_ser));  
    if(err==-1)  
    {  
        printf("connect error\n");  
        return -1;  
    }  
      
    printf("connect with server...\n");  

    send(sockfd,sendline,strlen(sendline),0);
    
    close( sockfd );
      
    printf("already close...\n");  
 
    return 0;  
}
