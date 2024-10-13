/*****************************************************
*
*
版权声明：本文为CSDN博主「mahuifa」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/qq_43627907/article/details/124028626

*
**********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define ipv6_CLIENT_PORT 5000


int main()
{
    // 创建套接字
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
	int len=0;
	int i=0;
	char buffer[1024];
	/* 定义组播结构体 */
//	struct ipv6_mreq {
//	/* IPv6 multicast address of group */
//	struct in6_addr ipv6mr_multiaddr;
//
//	/* local IPv6 address of interface */
//	int		ipv6mr_ifindex;
//};
//#endif /* __UAPI_DEF_IVP6_MREQ */

	//struct ipv6_mreq group;

    // ADDRESS
    struct sockaddr_in6 localaddr = {AF_INET6, htons(ipv6_CLIENT_PORT)};
    //这个函数转换字符串到网络地址，第一个参数af是地址簇，第二个参数*src是来源地址，第三个参数* dst接收转换后的数据。
   // inet_pton(AF_INET6, "ff02::1", &address.sin6_addr);


/*	//初始化本地端地址
	bzero(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET6;
	inet_pton(AF_INET6, "0.0.0.0" , &localaddr.sin6_addr);
	localaddr.sin_port = htons(CLIENT_PORT);
*/
	bind(fd, (struct sockaddr *)&localaddr, sizeof(localaddr));
	

    //ipv6_mreq结构提供了用于IPv6地址的多播组的信息。
    struct ipv6_mreq group;
    //将接口索引指定为0，则使用默认的多播接口。
    group.ipv6mr_interface = 0;    
    //IPv6组播组的地址。
    inet_pton(AF_INET6, "ff02::1", &group.ipv6mr_multiaddr);  
    //将套接字加入到指定接口上提供的多播组。此选项仅对数据报和原始套接字有效（套接字类>型必须为SOCK_DGRAM或SOCK_RAW）。
    setsockopt(fd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &group, sizeof(group));

    //发送数据
//    char buffer[128];
 //   strcpy(buffer, "");
 //   sendto(fd, "hello world!", sizeof buffer, 0, (struct sockaddr*)&address, sizeof( address));
#if 0
//htonl(INADDR_ANY);
    /*设置组地址*/
	inet_pton(AF_INET6, "ff02::1", &group.ipv6mr_multiaddr);//ipv6mr_multiaddr
	/*本地任意IP*/
	group.ipv6mr_ifindex=0;
	//inet_pton(AF_INET6,  in6addr_any, &group.ipv6mr_ifindex);//ipv6mr_ifindex
	/* eth0 --> 编号 命令：ip ad */
	group.imr_ifindex = if_nametoindex("ens33");
	/*设置client 加入多播组 */
	setsockopt(confd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));
#endif
		

    while (1) {
		memset(buffer,0,sizeof(buffer));
		len = recvfrom(fd, buffer, sizeof(buffer), 0, NULL, 0);
		i++;
		printf("recv packga:%d len:%d data:%s\n",i,len,buffer);
		//write(STDOUT_FILENO, buf, len);
		
	}

	close(fd);
    return 0;
}


