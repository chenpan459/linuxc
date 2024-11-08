#!/bin/bash

# 定义密钥、IV、加密文件和解密后的文件
KEY="62616e6779616e78696e616e31323334"
IV="62793132333435366279313233343536"
ENCRYPTED_FILE="$1"
DECRYPTED_FILE="$1.enc"

# 使用gmssl进行解密
#gmssl sm4-cbc -d -in $ENCRYPTED_FILE -out $DECRYPTED_FILE -K $KEY -iv $IV
gmssl sm4 -cbc  -encrypt -key  $KEY  -iv  $IV -in $ENCRYPTED_FILE -out  $DECRYPTED_FILE

# 检查gmssl命令的返回值
if [ $? -eq 0 ]; then
    echo "Decryption successful."
else
    echo "Decryption failed."
fi




