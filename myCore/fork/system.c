#include <stdio.h>
#include <stdlib.h>

int main() {
    int result;

    // 执行一个成功的命令
	result = system("./sh1.sh");
	printf("sh1 return =%d\n",result);
	
	result = system("./sh2.sh");
	printf("sh2 return =%d\n",result);
	
	result = system("./sh3.sh");
	printf("sh3 return =%d\n",result);
	
	result = system("./sh4.sh");
	printf("sh4 return =%d\n",result);
   

    return 0;
}