#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int tcp_socket;
int new_socket;

int  init_network()
{
	//1.初始化网络
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	
	//2.绑定服务器  
	//设置链接的服务器地址信息 
	struct sockaddr_in  addr;  
	addr.sin_family   = AF_INET; //IPV4 协议  
	addr.sin_port     = htons(8888); //端口
	addr.sin_addr.s_addr = inet_addr("0.0.0.0"); //服务器的IP 地址
	
	//解决端口复用
	int on=1;
	setsockopt(tcp_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	//on=1;
	//setsockopt(tcp_socket,SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
	
	
	//2.绑定服务器地址信息
	int ret=bind(tcp_socket,(struct sockaddr *)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("");
		return 0;
	}
	else
	{
		printf("绑定成功\n");
	}
	
	
	//3.设置服务器为监听模式 
	ret=listen(tcp_socket,5);
	if(ret < 0)
	{
		perror("");
		return 0;
	}
	else
	{
		printf("设置监听成功\n");
	}
	
	

	
}

int main()
{
	//1.初始化网络 
	init_network();
	
	while(1)
	{
		//4.接收客户端的链接请求 
		printf("等待服务器链接进来\n");
		new_socket=accept(tcp_socket,NULL,NULL);
		 if(new_socket < 0)
		 {
			perror("");
			return  0;
		 }
		 else
		 {
			 printf("链接成功！！\n");
		 }
		//2.接收网络数据 
		char buf[4096]={0}; 
		read(new_socket,buf,4096); 
		
		
		//3.调用 xtts_offline_sample  转换文字  
		 
		char  cmd[4096*2]={0}; 
		
		sprintf(cmd,"./xtts_offline_sample  %s",buf);
		
		system(cmd);
		
		
		//4.播放语音 
		 system("mplayer tts_sample.wav");
		
		
		//5.删除音频文件  
		system("rm  tts_sample.wav");
		//break;
		
		
	}
	
	

}