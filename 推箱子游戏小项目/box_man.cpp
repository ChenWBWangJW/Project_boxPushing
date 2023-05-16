#include<iostream>
#include<graphics.h>	//ͼ�λ�easyx��ͷ�ļ�
#include<conio.h>		//������ȡͷ�ļ�
#include<stdlib.h>		//��׼��ͷ�ļ�
#include<string.h>		//�ַ������ͷ�ļ�
#include<cstdlib>

using namespace std;

#include "box_man.h"
#include "game_db.h"

#define LINE 48
#define COLUMN 48

IMAGE images[ALL];	//easyx�⺯�ṹ���͵���

POS man;	//man position;

int map[LINE][COLUMN] = { 0 };	//map data]

/*
int map[LINE][COLUMN] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	{0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, },
	{0, 1, 4, 1, 0, 2, 1, 0, 2, 1, 0, 0, },
	{0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, },
	{0, 1, 0, 2, 0, 1, 1, 4, 1, 1, 1, 0, },
	{0, 1, 1, 1, 0, 3, 1, 1, 1, 4, 1, 0, },
	{0, 1, 2, 1, 1, 4, 1, 1, 1, 1, 1, 0, },
	{0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, },
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};
*/

/*************************************************
* ��������isGameOver
* ʵ����Ϸ�����ж�
* ���룺��
* �������
* ����ֵ��
	true - ��Ϸ����
* 	false - ��Ϸδ����
**************************************************/
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == BOX_DES) {
				return false;
			}
		}
	}
	return true;
}

void game_Over_Scene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, "����");
	drawtext("��ϲ�㣡������ս�ɹ�������\n���������������ת��һ��", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void game_error_Over_Scene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, "����");
	drawtext("��Ϸ��ͼ�����µ��ˣ�\n����������˳���Ϸ", &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/*************************************************
* ��������ChangeMap
* ʵ�ֵ�ͼ�仯
* ���룺
*	*pos - �˵�ǰλ��
*	prop - �滻��ͼͼƬչʾ
* �������
* ����ֵ����
**************************************************/
void ChangeMap(POS* pos, PROPS prop) {
	map[pos->x][pos->y] = prop;
	putimage(START_X + (pos->y * RATIO), START_Y + (pos->x * RATIO), &images[prop]);
}

/*************************************************
* ��������gameControl
* ʵ����Ϸ�ĸ��������
* ���룺
*	direct - ��ǰ������
* �������
* ����ֵ����
**************************************************/
void gameControl(DIRECTION direct) {
	POS next_pos = man, next_next_pos = man;

	switch (direct) {
	case UP:
		next_pos.x--;
		next_next_pos.x = man.x - 2;
		break;

	case DOWN:
		next_pos.x++;
		next_next_pos.x = man.x + 2;
		break;

	case LEFT:
		next_pos.y--;
		next_next_pos.y = man.y - 2;
		break;

	case RIGHT:
		next_pos.y++;
		next_next_pos.y = man.y + 2;
		break;

	}

	//1.�ж�ǰ��λ���Ƿ�Ϊ�ذ壬true��ǰ����ԭλ�ûָ�Ŀ�ĵ�
/*	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR && map[man.x][man.y] == MAN_IN_DES) {
		ChangeMap(&next_pos, MAN);
		ChangeMap(&man, BOX_DES);
		man = next_pos;
	} */

	/*	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == BOX_DES) {
			ChangeMap(&next_pos, MAN_IN_DES);
			ChangeMap(&man, FLOOR);
			man = next_pos;
		} */

		//1.�ж�ǰ��λ���Ƿ�Ϊ�ذ壬true��ǰ����ԭλ�ûָ��ذ�
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR) {
		ChangeMap(&next_pos, MAN);
		ChangeMap(&man, FLOOR);
		man = next_pos;
	}

	//2.�ж�ǰ��λ���Ƿ�Ϊ���ӣ�true���ж�������һλ��next_next_pos�Ƿ�Ϊ�ذ壬��������ǰ�����ж��Ƿ�Ϊ����Ŀ�ĵأ���վ������λ�ã�ԭλ�ûָ��ذ�
	if (isValid(next_pos) && map[next_pos.x][next_pos.y] == BOX) {

		//2.1����ǰ�ǵذ�
		if (isValid(next_next_pos) && map[next_next_pos.x][next_next_pos.y] == FLOOR) {
			ChangeMap(&next_next_pos, BOX);
			ChangeMap(&next_pos, MAN);
			ChangeMap(&man, FLOOR);
			man = next_pos;
		}

		//2.1����ǰ����Ŀ�ĵ�
		else if (isValid(next_next_pos) && map[next_next_pos.x][next_next_pos.y] == BOX_DES) {
			ChangeMap(&next_next_pos, BOX);
			ChangeMap(&next_pos, MAN);
			ChangeMap(&man, FLOOR);
			man = next_pos;
		}
	}
}

/*************************************************
* ��������login
* ʵ���û���¼
* ���룺
*	userinfo - �ṹ���ͽṹ���ַ����
* �����
*	��
* ����ֵ��
*	ret - true��¼�ɹ�
*	ret = false��¼ʧ��
* ˵������¼ʧ�ܴ�������5�Σ������˳�
* 	    ��¼�ɹ��󣬽�����Ϸ
**************************************************/
bool login(userinfo &user) {
	int times = 0;
	bool ret = false;

	do
	{
		cout << "�û���:";
		cin >> user.username;

		cout << "����:";
		cin >> user.password;

		//����bool��trueΪ��֤�ɹ���falseΪ��֤ʧ��
		ret = fetch_user_info(user);
		
		if (times > MAX_RETRY_TIMES) {
			cout << "�������������࣬�����˳�" << endl;
			break;
		}
		if (ret == false) {
			times++;
			cout << "�û������������,����������" << endl;
			printf("ʣ��%d�λ���\n", ((MAX_RETRY_TIMES - 1) - times) + 1);
		}
	} while (!ret);

	return ret;
}

void init_game_graphic(IMAGE &bg_img) {
	

	//��̨��Ϸ
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);

	//���ñ���
	loadimage(&bg_img, _T("picture/blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);	//_T()Ϊ�ַ�ת��
	putimage(0, 0, &bg_img);

	//����ͼƬ(ǽ0�� �ذ�1�� ����Ŀ�ĵ�2�� С��3�� ����4�� ��������Ŀ��5)
	loadimage(&images[WALL], _T("picture/wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("picture/floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("picture/des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("picture/man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("picture/box.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("picture/box.bmp"), RATIO, RATIO, true);
	//	loadimage(&images[MAN_IN_DES], _T("man_in_des.bmp"), RATIO, RATIO, true);
}

void main(void)
{
	//0.�û������֤
	userinfo user;
	levelinfo level;
	IMAGE bg_img;
	bool ret = false;

	if (!login(user)) {
		cout << "��֤ʧ�ܣ������˳�" << endl;
		exit(-1);
	}
	else {
		cout << "��֤�ɹ�." << endl;
		::system, ("pause");
	}
	
	//��Ϸ��ѭ��
	bool quit = false;

	do {
		//�����û����ڵĹؿ�id��ȡ�ؿ�����
		ret = fetch_level_info(level, user.level_id);
		if (ret)
		{
			cout << "��ͼչ����......" << endl;
			::system("pause");
		}
		else {
			cout << "��ȡ�ؿ�����ʧ��" << endl;
			game_error_Over_Scene(&bg_img);
			break;
		}

		init_game_graphic(bg_img);

		//�����ݿ��ͼ����ת����map��
		ret = transfrom_map_db2array(level, map);
		if (!ret)
		{
			cout << "��ͼת������" << endl;
			exit(-1);
		}

		for (int i = 0; i < level.map_row; i++) {
			for (int j = 0; j < level.map_col; j++) {
				if (map[i][j] == MAN) {
					man.x = i;
					man.y = j;
				}
				putimage(START_X + (j * RATIO), START_Y + (i * RATIO), &images[map[i][j]]);
			}
		}

		do {
			if (_kbhit()) {
				char ch = _getch();

				if (ch == KEY_UP) {
					gameControl(UP);

				}
				else if (ch == KEY_DOWN) {
					gameControl(DOWN);

				}
				else if (ch == KEY_LEFT) {
					gameControl(LEFT);

				}
				else if (ch == KEY_RIGHT) {
					gameControl(RIGHT);

				}
				else if (ch == KEY_QUIT) {
					quit = true;
				}

				if (isGameOver()) {

					if(level.next_level_id == 0)
					{
						quit = true;
						game_error_Over_Scene(&bg_img);
						::system("pause");
						break;
					}
					else 
					{
						user.level_id += 1;
						update_user_level(user, user.level_id);
						game_Over_Scene(&bg_img);
						::system("pause");
						break;
					}
				}
			}
			Sleep(100);
		} while (quit == false);	//!quit
	} while (quit == false);

	user.level_id = 1;
	update_user_level(user, user.level_id);

	exit(-1);
}