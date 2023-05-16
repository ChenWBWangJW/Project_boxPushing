#pragma once
#ifndef _GAME_DB_H
#define	_GAME_DB_H

#include<string>

using namespace std;

#define LINE 48
#define COLUMN 48

//�û���Ϣ
typedef struct _userinfo {

	int id;		//�û�id
	string username;	//�û���
	string password;	//�û�����
	int level_id;	//�û����ڹؿ�id

}userinfo;

typedef struct _levelinfo {

	int id;		//�ؿ�id
	string name;	//�ؿ�����
	int map_row;	//��ͼ������
	int map_col;	//��ͼ������
	string map_data;	//�ؿ�����
	int next_level_id;	//��һ��id

}levelinfo;

bool fetch_user_info(userinfo &user);
bool login(userinfo &user);
bool fetch_level_info(levelinfo &level, int level_id);
bool transfrom_map_db2array(levelinfo &level, int map[LINE][COLUMN]);
bool update_user_level(userinfo &user, int next_level_id);

#endif // !_GAME_DB_H