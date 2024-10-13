#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <errno.h>  

#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <linux/sockios.h>

 
#define ETH_P_NDN	0x8899
int main (int argc, char **argv)
{

	int sockfd = socket(AF_PACKET , SOCK_RAW, htons(ETH_P_NDN)) ;
 
	struct sockaddr_ll myaddr ;
	myaddr.sll_family = AF_PACKET ;
	myaddr.sll_ifindex = if_nametoindex("ens33") ;
	myaddr.sll_halen = htons(6) ;
	uint8_t macDstAddr[6] ;
	macDstAddr[0] = 0xFF ;
	macDstAddr[1] = 0xFF ;
	macDstAddr[2] = 0xFF ;
	macDstAddr[3] = 0xFF ;
	macDstAddr[4] = 0xFF ;
	macDstAddr[5] = 0xFF ;
	memcpy(myaddr.sll_addr, macDstAddr , 6) ;
	
	bind(sockfd, (struct sockaddr*)&myaddr , sizeof(myaddr)) ;
 
	int dataLen = 5 +10;
	char data[10] ;
	memcpy(data,"hello1234567890",15) ;
 
	int frameLen = 6+6+2+dataLen+20 ;
	uint8_t frame[3000] ;
	memset(frame,0x77,sizeof(frame));
	memcpy(frame , macDstAddr,6) ;
	memset(frame+6 , 0x66 , 6) ;
	*((uint16_t*)(frame+12)) = htons(ETH_P_NDN) ;
 
	int writeLen = write(sockfd,frame,frameLen) ;
	close(sockfd) ;



/*
	char *device="ens33"; //teh0是网卡设备名
	unsigned char macaddr[ETH_ALEN]; //ETH_ALEN（6）是MAC地址长度
	int s=socket(AF_INET,SOCK_DGRAM,0);//建立套接口
	struct ifreq req;
    int err;
	int i=0;
    strcpy(req.ifr_name,device);//将设备名作为输入参数传入
    err=ioctl(s,SIOCGIFHWADDR,&req);//执行取MAC地址操作
    close(s);

    if(err!= -1){
		memcpy(macaddr,req.ifr_hwaddr.sa_data,ETH_ALEN);//取输出的MAC地址　　
       for(i=0; i<ETH_ALEN; i++)
	   	  printf("%0x:",macaddr[i]);
	}
	printf("\n");
*/
	return 0;
}


