 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 
 // hash表默认长度
 #define HASH_TABLE_SIZE 16
 
 // hash表元素链表节点
 typedef struct __node{
     char *key;
     char *value;
     struct __node *next;
 }NODE, *PNODE, **PPNODE;
 
/// hash表结构
 typedef struct __hash_table{
     PPNODE table;
     unsigned total; // 总空间
     unsigned size;  // 已用空间
 }HASH_TABLE, *PHASH_TABLE;
 
 // 创建hash表中元素节点
 PNODE create_node(const char* key, const char *value)
 {
     PNODE pnode = (PNODE)malloc(sizeof(NODE));
     if(NULL == pnode) return NULL;
 
     pnode->next = NULL;
     if (NULL != key)
     {
         unsigned len = strlen(key) + 1;
         pnode->key = (char*)malloc(len);
         memcpy(pnode->key, key, len);
     }
 
     if (NULL != value)
     {
         unsigned len = strlen(value) + 1;
         pnode->value = (char*)malloc(len);
         memcpy(pnode->value, value, len);
     }
     return pnode;
 }
 
 // 创建hash表
 PHASH_TABLE create_hash_table(unsigned size)
 {
     if(size < 1) return NULL;
 
     PPNODE ppNodeTable = (PPNODE)malloc(sizeof(PNODE) * size);
     if (NULL == ppNodeTable) return NULL;
 
     for (unsigned i = 0; i < size; i++)
     {
         ppNodeTable[i] = NULL;
     }
     PHASH_TABLE pHashTable = (PHASH_TABLE)malloc(sizeof(HASH_TABLE));
     if(NULL == pHashTable) return NULL;
     pHashTable->table = ppNodeTable;
     pHashTable->total = size;
     pHashTable->size = 0;
     return pHashTable;
 }
 
 // hash函数 对叠法+除留余数法
 unsigned hash(const char* key, unsigned hash_size)
 {
     unsigned int h=0;
     for( ; *key != '\0'; ++key)
         h = *key + h * 31;
     return h%hash_size;
 }
 
 // 从hash表中获取key对应的value
 const char* get_hash_value(PHASH_TABLE pHashTable, const char* key)
 {
     if(NULL == key) return NULL;
 
     unsigned index = hash(key, pHashTable->total);
     PNODE pnode = pHashTable->table[index];
     if(NULL == pnode) return NULL;
 
     do
     {
         if(strcmp(pnode->key, key) == 0)
         {
             return pnode->value;
         }
 
         pnode = pnode->next;
     }while (pnode != NULL);    
 }
 
 // 插入hash表
 int insert_hash(PHASH_TABLE pHashTable ,const char *key, const char *value)
 {
     if(NULL == key) return 0;
     // 剩余空间不足
     if(pHashTable->size == pHashTable->total) return -1;
 
     if(get_hash_value(pHashTable, key) != NULL) return -1;
 
     
     PNODE pnode = create_node(key, value);
     if(NULL == pnode) return 0;
 
     unsigned index = hash(key,pHashTable->total);
     if(NULL == pHashTable->table[index])
     {
         pHashTable->table[index] = pnode;
         ++ pHashTable->size;
     } else {
         pnode->next = pHashTable->table[index];
         pHashTable->table[index] = pnode;
     }
 
     return index;
 }
 
 // 释放hash表
 void free_hash_table(PHASH_TABLE pHashTable)
 {
     if(NULL == pHashTable || pHashTable->size == 0) return;
 
     PNODE pnode = NULL;
     for (unsigned i = 0; i < pHashTable->total; i++)
     {
         pnode = pHashTable->table[i];
         if(pnode == NULL) continue;
 
         PNODE tmp = NULL;
         while( pnode != NULL)
         {
             tmp = pnode;
             pnode = pnode->next;
             free(tmp->key);
             free(tmp->value);
             free(tmp);
         }
         pHashTable->table[i] = NULL;
         pHashTable->size --;
         if(pHashTable->size == 0) return;
     }
     
     free(pHashTable->table);
     free(pHashTable);
 }
 
 // 打印hash表内容
 void print_hash(PHASH_TABLE pHashTable)
 {
     if(NULL == pHashTable || 0 == pHashTable->size) return;
 
     for (unsigned i = 0; i < pHashTable->total; i++)
     {
         printf("%u ",i);
         PNODE pnode = pHashTable->table[i];
         while (pnode != NULL)
         {
             if(pnode->value != NULL)
                 printf("[%s,%s] ",pnode->key, pnode->value);
             else 
                 printf("[%s,null] ",pnode->key);
             pnode = pnode->next;
         }
         printf("\n");
     }
 }
 
 int main(int argc, char *argv[])
 {
    PHASH_TABLE pHashTable = create_hash_table(HASH_TABLE_SIZE);
    insert_hash(pHashTable, "zs","张三");
    insert_hash(pHashTable, "ls","李四");
    insert_hash(pHashTable, "we","王二");
    insert_hash(pHashTable, "mz","麻子");
    insert_hash(pHashTable, "lb","刘备");
    insert_hash(pHashTable, "gy","关羽");
    insert_hash(pHashTable, "zf","张飞");
    insert_hash(pHashTable, "cc","曹操");
    insert_hash(pHashTable, "cp","曹丕");
    insert_hash(pHashTable, "cz","曹植");
    insert_hash(pHashTable, "cr","曹仁");
 //    const char* value = get_hash_value(pHashTable,"mz");
 //    if(value != NULL)
 //    {
 //        printf("mz->%s\n",value);
 //    } else {
 //        printf("没有找到zs\n");
 //    }
 
     print_hash(pHashTable);
     
     free_hash_table(pHashTable);


	 unsigned int a=10;
	 a=a-11;
	 printf("a=%d\n",a);
	 if(a){
	 	printf("---------------\n");
	 }else{
      printf("----+++++++++++++++++--------\n");
	 }
	 
     return 0;
 }