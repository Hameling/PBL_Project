#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tag_ID_position.h"
#pragma warning(disable:4996)

//RFID �±� ������ �����ϱ� ���� ����ü
typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo *next;
	//struct taginfo *previous, *next;
}TAGINFO;

//�̰͵� �����Ҳ� ������? �Ʒ��� ���� �뾵��
//typedef struct List {
//	TAGINFO tg;
//	struct List *previous, *next;
//}LIST;

//RSSI �� ������ ���Ľ�Ű�� ���� ����ü
typedef struct sortList {
	char id[27];
	int pos[2];
	float rssi;
	struct sortList *previous, *next;
}SORTLIST;

void initList();			//���Ḯ��Ʈ �ʱ�ȭ
void inserstList();			//���Ḯ��Ʈ�� ����ü �Է¹޾Ƽ� ����
void readFile();			//������ �о���� �Լ� (tagList, referList, sortList) �Լ� ���ο��� insertList ȣ��
void referTagAnalysis();	//���۷��� �±� �м� ���
void targetTagAnalysis();	//Ÿ�� �±� �м� ���	taginfo ����ü Ȱ��?
void defineTagetPos();		//Ÿ�� ��ġ �߷�
void printInfo();			//�±׳��� ���

int main() {
	char input;

	printf("=============================================================\n");
	printf("   RFID Tag Information Analysis based Localization System\n");
	printf("=============================================================\n");
	printf("  A. Reference Tag Analysis\n");
	printf("  B. Target Tag Analysis\n");
	printf("  C. Estimation of Target Position\n");
	printf("  D. Quit\n");

	while (1) {
		printf(">>");
		scanf("%c", &input);

		if (input == 'A') {
			printf("  A. Reference Tag Analysis\n");
		}
		else if (input == 'B') {
			printf("  B. Target Tag Analysis\n");
		}
		else if (input == 'C') {
			printf("  C. Estimation of Target Position\n");
		}
		else if (input == 'D') {
			printf("  D. Quit\n");
			break;
		}
		else {
			printf("unknown Message\n");
			continue;
		}
	}


	return 0;
}