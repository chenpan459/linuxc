/**************************************************************
*
*
*————————————————
版权声明：本文为CSDN博主「mahuifa」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_43627907/article/details/124028626
*
*
***************************************************/





#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>



#define ipv6_SERVER_PORT 6060
#define ipv6_CLIENT_PORT 5000

//#define MAXLINE 4096
//#define CLIENT_PORT 9000
#define ipv6_GROUP "ff02::1"



int main()
{
    int len=0;
	char buffer[1024]="1234567890";
     //创建套接字
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);

    //绑定本地网络信息
    struct sockaddr_in6 address = {AF_INET6, htons(ipv6_SERVER_PORT)};
	struct sockaddr_in6 clientaddr;

	//address.sin6_addr, "2000::100");
	inet_pton(AF_INET6, "2000::100" , &address.sin6_addr);
	 
    bind(fd, (struct sockaddr*)&address, sizeof address);

    //ipv6_mreq结构提供了用于IPv6地址的多播组的信息。
    struct ipv6_mreq group;
    //将接口索引指定为0，则使用默认的多播接口。
    group.ipv6mr_interface = 0;    
    //IPv6组播组的地址。
    inet_pton(AF_INET6, "ff02::1", &group.ipv6mr_multiaddr);  
    //将套接字加入到指定接口上提供的多播组。此选项仅对数据报和原始套接字有效（套接字类>型必须为SOCK_DGRAM或SOCK_RAW）。
    setsockopt(fd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &group, sizeof(group));

    printf("开始发送数据包！\n");
    //接收数据
 //   char buffer[128];
 //   read(fd, buffer, sizeof(buffer));    


	/*构造 client 地址 IP+端口 */
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sin6_family = AF_INET6; /* IPv6 */
	inet_pton(AF_INET6, ipv6_GROUP, &clientaddr.sin6_addr);
	clientaddr.sin6_port = htons(ipv6_CLIENT_PORT);


	while (1) {
		
		len=sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr_in6 *)&clientaddr, sizeof(clientaddr));
		printf("sendto len=%d\n", len);

	    sleep(1);
	}

	
    return 0;

}








