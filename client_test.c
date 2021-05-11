#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

const char* ADDRESS ="SCHNEIDER*+THUNDERCOMM&=clientAddr";
const char* OPEN ="SCHNEIDER*+THUNDERCOMM&=on";
const char* CLOSE ="SCHNEIDER*+THUNDERCOMM&=off";

int clifd;
struct sockaddr_in seraddr={0};
char buf_str[1024]={0};
char buf_str1[1024]={0};
int tolen=0;

void SendAddress()
{
    int ret = sendto(clifd,ADDRESS,strlen(ADDRESS),0,(struct sockaddr *)&seraddr,tolen);
	if(ret<0)
	{
		perror("sendto failed");
		close(clifd);
	}
	printf("send address success\n");
}

void SendMessage(char* content)
{

    int ret = sendto(clifd,content,strlen(content),0,(struct sockaddr *)&seraddr,tolen);
	if(ret<0)
	{
		perror("sendto failed");
		close(clifd);	
	}
	printf("send message success\n");
}

void ReceiveMessage()
{
   int ret=recvfrom(clifd,buf_str,sizeof(buf_str),0,NULL,NULL);
	if(ret<0)
	{
		perror("recvfrom failed");
		close(clifd);
	}
	printf("recvfrom success\n");
	printf("receive:  %s\n",buf_str);
}

void* tc_message_thread1(void* data)
{
	while (1)
	{
		memset(buf_str,0x00,sizeof(buf_str));
		ReceiveMessage(buf_str);
		printf("message %s\n",buf_str);
		
	}
}
void* tc_message_thread2(void* data)
{
	while (1)
	{
		memset(buf_str1,0x00,sizeof(buf_str1));
		
		gets(buf_str1);
        SendMessage(buf_str1);
	}
}

int main()
{
	pthread_t pt1;
	pthread_t pt2;
	clifd=socket(AF_INET,SOCK_DGRAM,0);
	if(clifd<0)
	{
		perror("socke failed");
		return -1;
	}
	printf("socket success\n");
	seraddr.sin_family=AF_INET;
	seraddr.sin_addr.s_addr=inet_addr("10.0.32.174");
	seraddr.sin_port=htons(8686);
	tolen=sizeof(seraddr);
	SendAddress();
	pthread_create(&pt1,0,tc_message_thread1,NULL);
	pthread_create(&pt2,0,tc_message_thread2,NULL);
	pthread_join(pt1,NULL);
	pthread_join(pt2,NULL);
	return 0;
}

