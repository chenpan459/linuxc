#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_SQL_SIZE 1000000 // 允许的最大SQL文件大小

// 回调函数，用于处理查询结果
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for(int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <database> <sql_file>\n", argv[0]);
        return 1;
    }

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    FILE *file;
    long file_size;

    // 打开数据库
    rc = sqlite3_open(argv[1], &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // 读取SQL文件
    file = fopen(argv[2], "rb");
    if (file == NULL) {
        fprintf(stderr, "Can't open SQL file: %s\n", argv[2]);
        sqlite3_close(db);
        return 1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    if (file_size > MAX_SQL_SIZE) {
        fprintf(stderr, "SQL file too large (max %d bytes)\n", MAX_SQL_SIZE);
        fclose(file);
        sqlite3_close(db);
        return 1;
    }

    // 分配内存
    sql = (char*)malloc(file_size + 1);
    if (sql == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        sqlite3_close(db);
        return 1;
    }

    // 读取文件内容
    if (fread(sql, 1, file_size, file) != file_size) {
        fprintf(stderr, "Error reading SQL file\n");
        free(sql);
        fclose(file);
        sqlite3_close(db);
        return 1;
    }

    sql[file_size] = '\0';
    fclose(file);

    // 执行SQL
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "SQL executed successfully\n");
    }

    // 清理
    free(sql);
    sqlite3_close(db);
    return 0;
}
