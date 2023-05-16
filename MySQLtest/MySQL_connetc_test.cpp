#include <stdio.h>
#include <mysql.h>

int main(void) {
	MYSQL mysql;	//数据库句柄
	MYSQL_RES *res;	//这个结构代表返回行的一个查询结果集
	MYSQL_ROW row;	//一个行数据的类型安全(type-safe)的表示，表示数据行的列

	//初始化mysql句柄
	mysql_init(&mysql);

	//设置字符集
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	//连接数据库
	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "chenweibin980928.", "BOX_MAN", 3306, NULL, 0)) {
		printf("connect success!\n");
	}
	else {
		printf("connect failed:%s\n", mysql_error(&mysql));
		exit(-1);
	}

	//执行SQL语句查询数据库
	int ret = mysql_query(&mysql, "select * from users");
	printf("ret = %d\n", ret);

	//获取结果集
	res = mysql_store_result(&mysql);

	if (res == NULL) {
		printf("mysql_store_result failed:%s\n", mysql_error(&mysql));
		exit(-1);
	}

	//给ROW赋值，判断ROW是否为空，不为空就打印数据
	while (row = mysql_fetch_row(res)) {
		printf("%s\t%s\t%s\t%s\t\n", row[0], row[1], row[2], row[3]);
	}

	//释放结果集
	mysql_free_result(res);

	//关闭数据库
	mysql_close(&mysql);

	system("pause");
	return 0;

}