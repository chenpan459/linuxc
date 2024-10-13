#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
int main(int argc, char const *argv[]){
		 
		//使用ftok函数获取键值
		key_t mykey;
		if((mykey=ftok(".",100))==-1)
		{
		    perror( "fail to ftok");
		    exit(1);
		}
		//通过shmget函数创建或者打开一个共享内存，返回一个共享内存的标识符
		int shmid;
		if((shmid = shmget(mykey,500,IPC_CREAT | 0666))==-1){
		    perror("fail to shmget");
		    exit(1);
		}
		    printf( "shmid = %d\n", shmid);
		    system("ipcs -m");
		    //使用shmat函数映射共享内存的地址
		    //声明一个指针变量指向共享内存，则操作指针变量text，就相当于操作共享内存
		char *text;    
		if((text = shmat(shmid,NULL,0)) == (void *)-1)
		{
		    perror( "fail to shmat");
		    exit(1);
		}
		//通过shmat的返回值对共享内存操作
		strcpy(text,"hello world 12345678");
		//操作完毕后要接触共享内存的映射
		if(shmdt(text) == -1)
		{
		  perror("fail to shmdt");
		  exit(1);
		}
		system("ipcs -m");
		return 0;
}


