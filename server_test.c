#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include <arpa/inet.h>


char buf[1024]={0};

struct sockaddr_in clientaddr={0};
int addrlen=sizeof(clientaddr);

// typedef struct sockaddr sockaddress;
// typedef struct sockaddr_in  sockaddress_in;
//创建UDP实现服务器和客户端的通信

void* ReceiveMessage(int* serfd)
{

    int ret = recvfrom(*serfd,buf,sizeof(buf),0, (struct sockaddr *)&clientaddr, &addrlen);
	printf("sizeof(buf) is %d\n",sizeof(buf));
	if(ret<0)
	{
		perror("recvfrom failed in connect step");
		close(*serfd);
		return -1;
	}
	// printf("IP=%s,port=%u\n",inet_ntoa(clientaddr->sin_addr),ntohs(clientaddr->sin_port));
	printf("recvfrom success\n");
	printf("receive:  %s\n",buf); 
}

void* SendMessage(int* serfd)
{

	
    memset(buf,0,sizeof(buf));
	gets(buf);
	int ret = sendto(*serfd,buf,sizeof(buf),0, (struct sockaddr*)&clientaddr, addrlen);
	if(ret<0)
	{
		perror("sendto failed");
		close(*serfd);
		return -1;
	}
	printf("sendto success\n");
}


void* thread_receive(void* data)
{ 
	
	while(1)
	{
			ReceiveMessage(data);
	}
}

void* thread_send(void* data)
{
	while(1)
	{
			SendMessage(data);
	}
}


int main()
{
//创建socket连接
	int serfd=0;
	serfd=socket(AF_INET,SOCK_DGRAM,0);
	if(serfd<0)
	{
		perror("socke failed");
		return -1;
	}
	printf("socket success\n");


//绑定IP地址和端口信息
	int ret=0;
	struct sockaddr_in seraddr={0};
	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=inet_addr("10.0.32.174");
	seraddr.sin_port=htons(8686);
	ret=bind(serfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	if(ret<0)
	{
		perror("bind failed");
		close(serfd);
		return -1;
	}
	printf("bind success\n");
   pthread_t pt1;
   pthread_t pt2;


  pthread_create(&pt1,0,thread_receive,&serfd);
  pthread_create(&pt2,0,thread_send,&serfd);
  pthread_join(pt1,NULL);
   pthread_join(pt2,NULL);

	close(serfd);
	return 0;
}

