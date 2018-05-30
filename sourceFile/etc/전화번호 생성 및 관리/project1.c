//��ǻ�Ͱ��а� 3�г� ����� 
//���۱Ⱓ : 2018.3.11~2018.3.14
//���� 1��
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#pragma warning (disable:4996)	//scanf ���ȿ��� ������ ���� �����

//C�� bool���� �����Ƿ� ���Ǽ��� ���� �����μ� ����
#define True 1
#define False 0

//��ȭ��ȣ ������ ���� ����ü
typedef struct TelNumberFormat {
	char num_head[4];
	char num_mid[4];
	int num_tail;
}TelNumberFormat;


int fileExist(char *filename);												//file ���翩�� Ȯ���Լ�
char* readLocalNumber(char *LocalName);										//Localnumber �д��Լ�
char* readSubLocalNumber(char *LocalName, char *SubLocalName);				//SubLocalNumber �д��Լ�
void genLastPhoneNumber(TelNumberFormat *target);							//��ȭ��ȣ ������ �ڸ� �����Լ�


int main() {
	TelNumberFormat format = { 0 };											//��ȭ��ȣ ������ ���� ����ü ����
	char LocalNumberAddr[20];												//LocalAddress�� �Է¹ޱ����� ���ڿ� �迭
	char SubLocalNumberAddr[20];											//SubLocalAddress�� �Է¹ޱ� ���� ���ڿ� �迭


	if (fileExist("LocalNumberList.txt") == False
		|| fileExist("SubLocalNumberList.txt") == False) {
		printf("������ �о���µ� �����߽��ϴ�.\n���α׷��� �����մϴ�.\n");	//���� ���� ���ο� ���� ����ó��
		return 1;	//������ �������� ������ ���α׷� ����
	}

	printf("���α׷� ���Ḧ ���Ͻø� '����'�� �Է����ּ���\n\n");
	while (True) {
		printf("���� ��� �ּҸ� �Է����ּ��� : ");
		scanf("%s", &LocalNumberAddr);
		if (strcmp(LocalNumberAddr, "����") == 0) break;					//���� Ű���� �Է����� ���α׷� ��������
		scanf("%s", &SubLocalNumberAddr);
		if (strcmp(SubLocalNumberAddr, "����") == 0) break;					//���� Ű���� �Է����� ���α׷� ��������


		if ((strcmp("CAN'T_FIND_SAME_LETTER.", readLocalNumber(LocalNumberAddr)) == 0)
			|| (strcmp("CAN'T_FIND_SAME_LETTER.", readSubLocalNumber(LocalNumberAddr, SubLocalNumberAddr)) == 0)) {
			printf("�ش� ���ڿ��� �������� �ʽ��ϴ�.\n");				//��ġ�ϴ� ���ڿ��� �������� �������� ���� ����ó��
		}
		else {
			//�� �Լ� ȣ���� ���� ��ȭ��ȣ ����
			strcpy(format.num_head, readLocalNumber(LocalNumberAddr));
			strcpy(format.num_mid, readSubLocalNumber(LocalNumberAddr, SubLocalNumberAddr));
			genLastPhoneNumber(&format);

			//0~9999������ �������� �����Ǳ� ������ 0�̵Ǿ� ���ڸ� ����°��� �������� ���� ������
			printf("������ ��ȭ��ȣ : %s-%s-%04d\n\n", format.num_head, format.num_mid, format.num_tail);
		}
	}
	return 0;
}

/*
Call by value
���� ���翩�� Ȯ���Լ�
*/
int fileExist(char *filename) {
	FILE *file = fopen(filename, "r");
	if (file != NULL) {
		fclose(file); return True;
	}
	else return False;
}

/*
Call by value
LocalNumberList.txt.������ �ҷ��� �� ������ ����� �Լ��� �Ű����� LocalName�� ��ġ�Ͽ� 
������ ���ڿ��� �ִٸ� �� ���ڿ��� �ش��ϴ� ��ȣ�� ��ȯ�ϴ� �Լ�

Ư�� ���ܿ� ���� Ű���带 ��ȯ�ϹǷμ� �����Լ����� ����ó��
*/
char* readLocalNumber(char *LocalName){
	int count = 0;													//������ ���ڿ��� �ִ����� Ȯ���ϱ� ���� flag����
	char* textfield[2][20] = { "","" };								//���Ͽ��� �ҷ��� ���ڿ��� �ӽ÷� �����ϱ����� 2�����迭
	FILE *LocalNumberList = fopen("LocalNumberList.txt", "r");		//LocalNumberList.txt �ҷ�����

	if (LocalNumberList != NULL) {
		while (!feof(LocalNumberList)) {
			fscanf(LocalNumberList, "%s\t%s", &textfield[0], &textfield[1]);
			if (strstr(textfield[1], LocalName) != NULL) {
				count++;
				fclose(LocalNumberList);
				return textfield[0];
			}
		}
		if (count == 0) {
			fclose(LocalNumberList);
			return "CAN'T_FIND_SAME_LETTER.";						//������ ���ڿ��� �������� ������
		}
	}
	else return "";													//�ش� ������ �������� ������
}

/*
Call by value
SubLocalNumberList.txt.������ �ҷ��� �� ������ ����� �Լ��� �Ű����� LocalName�� SubLocalName�� ���� ��ġ�Ͽ�
������ ���ڿ��� �ִٸ� �� ���ڿ��� �ش��ϴ� ��ȣ�� �ӽ÷� �����ϰ� ����� ��ȣ�� �������� �ϳ� ��ȯ�ϴ� �Լ�

Ư�� ���ܿ� ���� Ű���带 ��ȯ�ϹǷμ� �����Լ����� ����ó��
*/
char* readSubLocalNumber(char *LocalName, char *SubLocalName){
	int count = 0;													//������ ���ڿ��� �ִ����� Ȯ���ϱ� ���� flag����
	char textfield[3][20] = { "","","" };							//���Ͽ��� �ҷ��� ���ڿ��� �ӽ÷� �����ϱ����� 2���� �迭
	char numberfield[3][5] = { "","","" };							//������ ���ڿ� �߽߰� �ӽ÷� �ش� ��ȣ���� �����ϱ����� 2���� �迭

	FILE *SubLocalNumberList = fopen("SubLocalNumberList.txt", "r");
	if (SubLocalNumberList != NULL) {
		while (!feof(SubLocalNumberList)) {
			fscanf(SubLocalNumberList, "%s %s %s", &textfield[0], &textfield[1], &textfield[2]);
			if ((strstr(textfield[0], LocalName) != NULL) && (strstr(textfield[1], SubLocalName) != NULL)) {
				strcpy(numberfield[count], textfield[2]);
				count++;
			}
		}
		srand((unsigned int)time(NULL));
		fclose(SubLocalNumberList);
		if (count != 0) {
			return numberfield[rand() % count];
		}
		else {
			return "CAN'T_FIND_SAME_LETTER.";						//������ ���ڿ��� �������� ������
		}
	}
	else return "";													//�ش� ������ ��������������
}
//Call by reference
//�����Լ��� �̿��Ͽ� ��ȭ��ȣ ������ �ڸ��� �������� ����
void genLastPhoneNumber(TelNumberFormat *target) {
	srand((unsigned int)time(NULL));
	target->num_tail = rand() % 10000;								//0~9999�� ��ȣ�� ���Ƿ� �����Ͽ� ��ȭ��ȣ ������ �ڸ��� ����
}