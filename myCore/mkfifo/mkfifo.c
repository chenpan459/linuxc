#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>

int   fd;

void rm_fifo()
{

     //关闭文件、删除管道文件
     close(fd);
     unlink("text.pipe");
   //  exit(-1);
}

void mkfifo_file()
{

	mkfifo("text.pipe" ,  0666);

}

int open_fifo()
{
   int fd=0;

   fd=open( "text.pipe", O_RDWR);
   
   return fd;
}

int close_fifo(int fd)
{
   int ret=0;

   ret=close(fd);
   
   return ret;
}


 

 

void   main()

{

      int i=0;
	  int fd;

    //  signal( SIGINT , end);

      

	 while(1){

	   //1.创建管道
	   mkfifo_file();
	   //2.打开文件
	   fd=open_fifo();
	   printf("i=%d   fd=%d\n",i,fd);
	   if(fd<0){
	   	    perror("open error");
          break;
	   }
	   while(1);
	   //3.关闭
	 //  close_fifo(fd);
	   //4.删除管道
	//   rm_fifo();

	    i++;

	 }

     //循环写入数据
/*
     while(1)
     {

           write(fd, & i , 4);
           sleep(1);
           i++;
     }*/

}

