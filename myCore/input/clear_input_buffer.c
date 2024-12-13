#include <stdio.h>

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing, just consume the characters
    }
}

int main() {
    int num;
    char str[100];

    printf("Enter a number: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    // 清除输入缓冲区中的剩余数据
    clear_input_buffer();

    printf("Enter a string: ");
    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("Error reading string\n");
        return 1;
    }

    // 移除字符串末尾的换行符
    str[strcspn(str, "\n")] = '\0';

    printf("You entered: %d and %s\n", num, str);

    return 0;
}