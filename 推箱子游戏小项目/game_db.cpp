#include "game_db.h"
#include<mysql.h>
#include<stdio.h>

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_USER_PASSWD "chenweibin980928."
#define DB_NAME "BOX_MAN"
#define DB_PORT 3306

#define SIZE_OF_SQL 256

static int debug = 1;
static bool connect_db(MYSQL &mysql);


/************************************
* ��������fetch_user_info
* ���ܣ�ͨ�����ݿ���֤�û����,��ȡ�û���Ϣ
* ���룺
*	user - �û���Ϣ�ṹ��
* ����ֵ��
*	bool - trueΪ��֤�ɹ���falseΪ��֤ʧ��
*************************************/
bool fetch_user_info(userinfo& user)
{
	MYSQL mysql;					//mysql������ٷ��ṹ��
	MYSQL_RES* mysql_result;		//��ѯ�����
	MYSQL_ROW mysql_row;			//������е�һ��

	char sql[SIZE_OF_SQL];
	bool ret = false;
	
	//1.�������ݿ�
	if (connect_db(mysql) == false)
	{
		return false;
	}

	//2.�����û����������ѯ��ȡ�û���Ϣ��id�� level_id��
	//���ɱ���� ������ ����format�ĸ�ʽ��ʽ��Ϊ�ַ�����Ȼ���ٽ��俽����str��
	//c_str()��C++�ַ�������ת��ΪC�����ַ������ú�������һ��ָ���Կ��ַ���β���ַ������ָ�룬���ַ����������C++�ַ��������е��ַ�����
	snprintf(sql, SIZE_OF_SQL, "select id, level_id from users where username = '%s' and password = md5('%s');", user.username.c_str(), user.password.c_str());
	ret = mysql_query(&mysql, sql);		//��ѯ�ɹ�����ֵΪ0������Ϊ��0

	if (ret)
	{
		printf("�û����ݲ�ѯ����: %s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("�û����ݲ�ѯ�ɹ�......\n");
	}

	//3.��ȡ��ѯ���
	mysql_result = mysql_store_result(&mysql);
	if (mysql_result == NULL)
	{						//Ԥ����ָ�뱨��
		printf("�û������������, reason: %s\n", mysql_error(&mysql));
		mysql_free_result(mysql_result);			//�ͷŽ����
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("�û���������ɹ�......\n");
	}

	mysql_row = mysql_fetch_row(mysql_result);		//��ȡ������е�һ��
	if (mysql_row == NULL)
	{
		printf("�û����ݲ���ȡ���������, reason: %s\n", mysql_error(&mysql));
		mysql_free_result(mysql_result);			//�ͷŽ����
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("�û����ݻ�ȡ�ɹ�......\n");
	}

	//4.���ؽ��
	/*
	atoi()��һ��C��׼�⺯�������ڽ��ַ���ת��������
	����ַ����а����������ַ�����atoi()������ֹͣת�����������Ѿ�ת���Ĳ���
	���⣬��ʹ��atoi()����ʱ����Ҫȷ��Ҫת�����ַ�����ʾ������������int���͵ķ�Χ������ᵼ�½������ȷ
	*/
	user.id = atoi(mysql_row[0]);
	user.level_id = atoi(mysql_row[1]);
	if(debug) printf("�û��� �� %d, ��ͼ�� �� %d\n", user.id, user.level_id);

	//�رս����
	mysql_free_result(mysql_result);			//�ͷŽ����
	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
}


/************************************
* ��������connetc_db
* ���ܣ���׼���������ݿ�
* ���룺
*	��mysql���������
* ����ֵ��
* 	bool - trueΪ���ӳɹ���falseΪ����ʧ��
*************************************/
bool connect_db(MYSQL &mysql) {
	//1.��ʼ�����ݿ���
	mysql_init(&mysql);

	//2.�����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	//3.�������ݿ�
	if (mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0) != NULL) {
		if (debug) printf("���ݿ����ӳɹ�......\n");
	}
	else {
		printf("���ݿ����Ӵ���:%s\n", mysql_error(&mysql));
		return false;
	}


	return true;
}


/************************************
* ��������fetch_level_info
* ���ܣ�ͨ�����ݿ��ȡ�ؿ���Ϣ
* ���룺
*	level - �ؿ���Ϣ�ṹ��
*	level_id - ���û������ؿ�id
* ����ֵ��
*	bool - trueΪ��ȡ�ɹ���falseΪ��ȡʧ��
*************************************/
bool fetch_level_info(levelinfo &level, int level_id) {
	MYSQL mysql;					//mysql������ٷ��ṹ��
	MYSQL_ROW mysql_row;			//������е�һ��
	MYSQL_RES* mysql_result;		//��ѯ�����
	char sql[SIZE_OF_SQL];
	bool ret = false;

	//1.�������ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}

	//2.���ݹؿ�id��ѯ��ȡ�ؿ���Ϣ
	//���ɱ���� ������ ����format�ĸ�ʽ��ʽ��Ϊ�ַ�����Ȼ���ٽ��俽����str��
	snprintf(sql, SIZE_OF_SQL, "select name, map_row, map_column, map_data, next_level_id from levels where id = '%d';", level_id);
	ret = mysql_query(&mysql, sql);		//��ѯ�ɹ�����ֵΪ0������Ϊ��0
	if (ret)
	{
		printf("��ͼ���ݲ�ѯ����, reason: %s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("��ͼ���ݲ�ѯ�ɹ�......\n");
	}

	//3.��ȡ��ѯ���
	mysql_result = mysql_store_result(&mysql);
	if(mysql_result == NULL)		//Ԥ����ָ�뱨��
	{
		printf("��ͼ�����������, reason: %s\n", mysql_error(&mysql));
		mysql_free_result(mysql_result);			//�ͷŽ����
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("��ͼ��������ɹ�......\n");
	}

	mysql_row = mysql_fetch_row(mysql_result);		//��ȡ������е�һ��
	if (mysql_row == NULL)
	{
		printf("��ͼ���ݻ�ȡʧ��, reason: %s\n", mysql_error(&mysql));
		mysql_free_result(mysql_result);			//�ͷŽ����
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("��ͼ���ݻ�ȡ�ɹ�......\n");
	}

	//4.���ؽ��
	level.id = level_id;
	level.name = mysql_row[0];
	level.map_row = atoi(mysql_row[1]);
	level.map_col = atoi(mysql_row[2]);
	level.map_data = mysql_row[3];
	level.next_level_id = atoi(mysql_row[4]);
	if (debug) printf("��ͼ������׼����������ͼ���� %s\n", level.name.c_str());

	//�رս����
	mysql_free_result(mysql_result);			//�ͷŽ����

	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
}


/************************************
* ��������transfrom_map_db2array
* ���ܣ������ݿ��еĵ�ͼ����ת��Ϊ����
* ���룺
*	level - �ؿ���Ϣ�ṹ��
*	map - ��ά����
* ����ֵ��
*	true - ת���ɹ�
*	false - ת��ʧ��
*************************************/
bool transfrom_map_db2array(levelinfo& level, int map[LINE][COLUMN])
{
	if (level.map_row > LINE || level.map_col > COLUMN)
	{
		printf("��ͼ���ݴ��󣬵�ͼ����������������Χ\n");
		return false;
	}

	if (level.map_data.length() < 1)
	{
		printf("��ͼ���ݴ�������������\n");
		return false;
	}

	int start = 0, end = 0;
	int row = 0, column = 0;
	
	do {
		/*
		.find()��һ���ַ������͵ĳ�Ա�������������ַ����в���ָ�����Ӵ�
		.find()��������һ����������Ҫ���ҵ��Ӵ�������ҵ��˸��Ӵ����򷵻ظ��Ӵ���ԭ�ַ����е���ʼλ�ã����򷵻�string::npos����ʾδ�ҵ�
		*/
		end = level.map_data.find("|", start);

		if (end < 0)
		{
			end = level.map_data.length();
			break;
		}

		if (start == end) break;

		/*
		��һ���ַ������͵ĳ�Ա���������ڻ�ȡ�ַ������Ӵ�
		.substr()��������������������һ���������Ӵ�����ʼλ�ã��ڶ����������Ӵ��ĳ���
		���ֻ����һ�����������ʾ��ȡ����ʼλ�õ��ַ���ĩβ���Ӵ�
		��Ҫ����ͷ�ļ�<string.h>
		*/
		string line = level.map_data.substr(start, end - start);

		/*
		����ÿһ��
		strtok_s��һ���ַ��������������ڽ�һ���ַ����ָ�ɶ���Ӵ�
		str��Ҫ�ָ���ַ�����delim�Ƿָ�����context��һ��ָ��ָ���ָ�룬���ڱ���ָ��״̬
		��Ҫ����ͷ�ļ�<string.h>
		//0,0,0,0,0,0,0,0,0,0,0,0|0,1,0,1,1,1,1,1,1,1,0,0|0,1,4,1,0,2,1,0,2,1,0,0........
		*/
		char *next_token = NULL;
		char* item = strtok_s((char *)line.c_str(), ",", &next_token);

		column = 0;

		while (item && column < level.map_col)
		{
			map[row][column] = atoi(item);
			column++;
			item = strtok_s(NULL, ",", &next_token);
		}

		if(column < level.map_col)
		{
			printf("��ͼ���ݴ��󣬵�ͼ��������\n");
			return false;
		}

		row++;

		if (row > level.map_row) break;

		start = end + 1;
	} while (true);

	if (row < level.map_row)
	{
		printf("��ͼ���������趨�� %d��need��%d��\n", row, level.map_row);
		return false;
	}
}

/************************************
* ��������update_user_level
* ���ܣ������û��ؿ���Ϣ
* ���룺
*	user - �û���Ϣ�ṹ��
*	next_level_id - ��һ�ؿ�ID
* ����ֵ��
*	true - ���³ɹ�
*	false - ����ʧ��
*************************************/
bool update_user_level(userinfo& user, int next_level_id)
{
	//1.�������ݿ�
	MYSQL mysql;
	char sql[SIZE_OF_SQL];

	if (!(connect_db(mysql)))
	{
		printf("���ݿ�����ʧ��\n");
		return false;
	}

	//2.�����û��ؿ���Ϣ
	snprintf(sql, SIZE_OF_SQL, "update users set level_id = %d where id = %d;", next_level_id, user.id);
	if (mysql_query(&mysql, sql))
	{
		printf("���ݿ����ʧ�ܣ�reason: %s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	else
	{
		if (debug) printf("�û��ؿ���Ϣ���³ɹ�......\n");
	}

	//3.�ر����ݿ�
	mysql_close(&mysql);
	return true;
}
