//��ǻ�Ͱ��а� 3�г� ����� 
//���۱Ⱓ : 2018.3.11~2018.3.19
//���� 2��
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#pragma warning (disable:4996)	//scanf ���ȿ��� ���ø� ���� �����

//C�� bool���� �����Ƿ� ���Ǽ��� ���� �����μ� ����
#define True 1
#define False 0

//��ȭ��ȣ ������ ���� ����ü
typedef struct TelNumberFormat {
	char num_head[4];
	char num_mid[4];
	int num_tail;
}TelNumberFormat;

//���Ḯ��Ʈ ����� ���� ��弱��
typedef struct Node {
	struct Node *next;
	char PhoneNumber[20];
}Node;

//���Ḯ��Ʈ ���� ����
typedef struct List {
Node *first, *end;
}List;

//���Ḯ��Ʈ �����Լ�
List* initList();									//List �ʱ�ȭ �Լ�
void addList(List *list, char *data);				//List ��� �߰��Լ�
void delList(List *list);							//List ��� �����Լ�

//���� ��ɰ� ���õ� �Լ�
void clearFormat(TelNumberFormat *format);															//TelNumberFormat ����ü �ʱ�ȭ �Լ�
int fileExist(char *filename);																		//file ���翩�� Ȯ���Լ�
int readLocalNumber(TelNumberFormat *format, char *LocalName);										//Localnumber �д��Լ�
int readSubLocalNumber(TelNumberFormat *format, char *LocalName, char *SubLocalName);				//SubLocalNumber �д��Լ�
void genLastPhoneNumber(TelNumberFormat *target);													//��ȭ��ȣ ������ �ڸ� �����Լ�
int compareTelNumber(TelNumberFormat format);														//��ȭ��ȣ ���翩�� Ȯ���Լ�
void saveTelNumber(TelNumberFormat format);															//���Ͽ� ��ȭ��ȣ�� �����ϴ� �Լ�


int main() {
	TelNumberFormat format = { "" };										//��ȭ��ȣ ������ ���� ����ü ����
	char LocalNumberAddr[20];												//LocalAddress�� �Է¹ޱ����� ���ڿ� �迭
	char SubLocalNumberAddr[20];											//SubLocalAddress�� �Է¹ޱ� ���� ���ڿ� �迭
	char input[20];															//�����ȣ ���ÿ��� Ȯ�κ���
	char PrivateNumber[10];													//�Է¹��� �����ȣ�� �����ϴ� ����
	int flag = 0;															//�ݺ��� Ż���� ���� Flag ����
	

	if(fileExist("LocalNumberList.txt") == False
		|| fileExist("SubLocalNumberList.txt") == False
		|| fileExist("ExistingPhoneNumber.txt") == False){
		printf("������ �о���µ� �����߽��ϴ�.\n���α׷��� �����մϴ�.\n");	//���� ���� ���ο� ���� ����ó��
		return 1;	//������ �������� ������ ���α׷� ����
	}


	printf("���α׷� ���Ḧ ���Ͻø� '����'�� �Է����ּ���\n\n");
	while (True) {
		flag = 0;
		printf("���� ��� �ּҸ� �Է����ּ��� : ");
		scanf("%s", &LocalNumberAddr);
		if (strcmp(LocalNumberAddr, "����") == 0) break;					//���� Ű���� �Է����� ���α׷� ��������
		scanf("%s", &SubLocalNumberAddr);
		if (strcmp(SubLocalNumberAddr, "����") == 0) break;					//���� Ű���� �Է����� ���α׷� ��������

		//��ġ�ϴ� ���ڿ��� ������� TelNumberFormat�� �ʱ�ȭ�ϰ� �ٽ� �Է¹���
		if (readLocalNumber(&format, LocalNumberAddr) == False ||
			readSubLocalNumber(&format, LocalNumberAddr, SubLocalNumberAddr) == False) {
			printf("-�Է��Ͻ� �ּҿ� ��ġ�ϴ� ������ ã���� �����ϴ�\n");	
			clearFormat(&format);
		}

		//��ġ�ϴ� ���ڿ��� �ִ°�� ���ι�ȣ ��뿩�ο� ��� ���ι�ȣ�� �Է¹޾� ���� ��ȭ��ȣ�� ����ϰ� ����
		else {	
			while (flag != 1) {
				printf("���ι�ȣ�� ����Ͻðڽ��ϱ�? (yes/no) : ");
				scanf("%s", input);
				//���ι�ȣ ��뿩��
				if (strcmp(input, "yes") != 0 && strcmp(input, "no") != 0) {
					printf("-�߸� �Է��ϼ̽��ϴ�. yes �Ǵ� no�� �Է����ּ���\n");
				}
				else if(strcmp(input,"yes") == 0){
					while (flag != 1) {
						printf("����ϴ� ���ι�ȣ�� 4�ڸ��� �Է����ּ��� : ");
						scanf("%s", PrivateNumber);
						//��� ���ι�ȣ
						if (strlen(PrivateNumber) != 4) {
							printf("-�߸��Է��ϼ̽��ϴ�.\n");
						}
						else {
							format.num_tail = atoi(PrivateNumber);
							//�ش� ��ȣ�� ���翩��
							if (compareTelNumber(format) == 1) {
								flag = 1;
								printf("�ش��ȣ�� �����Ǿ����ϴ�.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
							}
							else {
								printf("-�ش� ��ȣ�� �����մϴ�.\n"); 
								while (flag != 1) {
									printf("�ٽ� �Է��Ͻðڽ��ϱ� ? (yes / no) : ");
									scanf("%s", input);
									if (strcmp(input, "yes") != 0 && strcmp(input, "no") != 0) {
										printf("-�߸� �Է��ϼ̽��ϴ�. yes �Ǵ� no�� �Է����ּ���\n");
									}
									else if(strcmp(input, "yes") == 0) break;
									else{
										flag = 1;
										genLastPhoneNumber(&format);
										printf("���ο��ȣ�� �����Ǿ����ϴ�.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
									}
								}
								
							}
						}
					}
				}
				else {
					genLastPhoneNumber(&format);
					printf("���ο��ȣ�� �����Ǿ����ϴ�.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
					flag = 1;
				}
			}
		}
		printf("\n");
		if (flag == 1) saveTelNumber(format);
		clearFormat(&format);
	}
	
	return 0;
}

/*
Call by value
List�� �����ϰ� �ش� List�� �ʱ�ȭ���� ��ȯ���ִ� �Լ�
*/
List* initList() {
	List *list;
	list = (List*)malloc(sizeof(List));
	list->first = NULL;
	list->end = NULL;
	return list;
}

/*
Call by reference
���ο� ��带 �����Ͽ� �Ű������� data���� �����ϰ�
�Ű����� ���� list�� ��尡 ���ٸ� ù��忡
�Ű����� ���� list�� ��尡 �ִٸ� ������ ����� �ڿ� �����ϴ� �Լ�
*/
void addList(List *list, char * data) {
	Node *newnode = (Node*)malloc(sizeof(Node));
	strcpy(newnode->PhoneNumber, data);
	newnode->next = NULL;

	if (list->first == NULL) {
		list->first = newnode;
		list->end = newnode;
	}
	else {
		list->end->next = newnode;
		list->end = newnode;
	}
}

/*
Call by reference
�Ű������� ������ list�� ��带 �����ϰ� �Ҵ�Ǿ� �ִ� �����͸� ��ȯ�ϴ� �Լ�
*/
void delList(List *list) {
	Node *oldnode = list->first;
	Node *tmp;
	if (list->first == NULL) return;
	else {
		while (oldnode->next != NULL) {
			tmp = oldnode->next;
			free(oldnode);
			oldnode = tmp;
		}
	}
}

/*
Call by reference
TelNumberFormat ������ �ʱ�ȭ
*/
void clearFormat(TelNumberFormat *format) {
	strcpy(format->num_head, "");
	strcpy(format->num_mid, "");
	format->num_tail = 0;
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
	else  return False;
}

/*
Call by reference
LocalNumberList.txt.������ �ҷ��� �� ������ ����� �Լ��� �Ű����� LocalName�� ���Ͽ�
������ ���ڿ��� �ִٸ� �� ���ڿ��� �ش��ϴ� ��ȣ�� ��ȯ�ϴ� �Լ�
Ư�� ���ܿ� ���� ���� ��ȯ�ϹǷμ� if���� ���� ����ó�� ����
*/
int readLocalNumber(TelNumberFormat *format, char *LocalName) {
	int count = 0;													//������ ���ڿ��� �ִ����� Ȯ���ϱ� ���� flag����
	char* textfield[2][20] = { "","" };								//���Ͽ��� �ҷ��� ���ڿ��� �ӽ÷� �����ϱ����� 2�����迭
	FILE *LocalNumberList = fopen("LocalNumberList.txt", "r");		//LocalNumberList.txt �ҷ�����

	if (LocalNumberList != NULL) {
		while (!feof(LocalNumberList)) {
			fscanf(LocalNumberList, "%s\t%s", &textfield[0], &textfield[1]);
			if (strstr(textfield[1], LocalName) != NULL) {
				count++;
				fclose(LocalNumberList);
				strcpy(format->num_head, textfield[0]);
				return True;
			}
		}
		if (count == 0) {
			fclose(LocalNumberList);
			return False;							//������ ���ڿ��� �������� ������
		}
	}
	else return -1;									//�ش� ������ �������� ������
}

/*
Call by reference
SubLocalNumberList.txt.������ �ҷ��� �� ������ ����� �Լ��� �Ű����� LocalName�� SubLocalName�� ���� ���Ͽ�
������ ���ڿ��� �ִٸ� �� ���ڿ��� �ش��ϴ� ��ȣ�� �ӽ÷� �����ϰ� ����� ��ȣ�� �������� �ϳ� ��ȯ�ϴ� �Լ�
Ư�� ���ܿ� ���� ���� ��ȯ�ϹǷμ� if���� ���� ����ó�� ����
*/
int readSubLocalNumber(TelNumberFormat *format, char *LocalName, char *SubLocalName) {
	int count = 0;													//������ ���ڿ��� �ִ����� Ȯ���ϱ� ���� flag����
	char textfield[3][20] = { "","","" };							//���Ͽ��� �ҷ��� ���ڿ��� �ӽ÷� �����ϱ����� 2���� �迭
	char numberfield[3][5] = { "","","" };							//������ ���ڿ� �߽߰� �ӽ÷� �ش� ��ȣ���� �����ϱ����� 2���� �迭
	FILE *SubLocalNumberList = fopen("SubLocalNumberList.txt", "r");//SubLocalNumberList.txt �ҷ�����

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
			strcpy(format->num_mid, numberfield[rand() % count]);
			return True;
		}
		else return False;							//������ ���ڿ��� �������� ������
	}
	else return -1;									//�ش� ������ ��������������
}

/*
Call by reference
�����Լ��� �̿��Ͽ� ��ȭ��ȣ ������ �ڸ��� �������� ����
*/
void genLastPhoneNumber(TelNumberFormat *target) {
	srand((unsigned int)time(NULL));
	target->num_tail = rand() % 10000;								//0~9999�� ��ȣ�� ���Ƿ� �����Ͽ� ��ȭ��ȣ ������ �ڸ��� ����
}

/*
Call by value
ExistingPhoneNumber.txt ������ �ҷ����� ������ ����� �Էµ� ��ȭ��ȣ�� ���Ͽ� �ش� ��ȭ��ȣ�� ���翩�θ� �Ǹ��ϴ� �Լ�
Ư�� ���ܿ� ���� ���� ��ȯ�ϹǷμ� if���� ���� ����ó�� ����
*/
int compareTelNumber(TelNumberFormat format) {
	FILE *readExistPhoneNumberList = fopen("ExistingPhoneNumber.txt", "r");	//ExistingPhoneNumber.txt �ҷ�����
	List *numberlist = initList();												//List ������ �����ϰ� List�� �ʱ�ȭ
	Node *tmp = &numberlist->first;												//List�� �����͸� �о�������� temp����
	char textfield[20];															//���Ͽ��� �о�� ���ڿ��� �ӽ÷� �����ϱ����� ���ڿ�

	if (readExistPhoneNumberList != NULL) {
		while (!feof(readExistPhoneNumberList)){
			fscanf(readExistPhoneNumberList, "%s", &textfield);
			addList(numberlist, textfield);
		}
	}
	else return -1;					//�ش� ������ �������� ������

	while (tmp->next != NULL) {
		sprintf(textfield, "%s-%s-%04d", format.num_head, format.num_mid, format.num_tail);
		if (strcmp(tmp->next->PhoneNumber,textfield) == 0) {
			delList(numberlist);
			free(numberlist);
			return False;			//������ ��ȣ�� �����Ҷ� 
		}
		tmp = tmp->next;
	}
	delList(numberlist);
	free(numberlist);
	return True;

}

/*
���� ��µ� ��ȭ��ȣ�� ExistingPhoneNumber.txt ���Ͽ� �����ϴ� �Լ�
*/
void saveTelNumber(TelNumberFormat format) {
	FILE *readExistPhoneNumberList = fopen("ExistingPhoneNumber.txt", "a+");	//ExistingPhoneNumber.txt �ҷ�����
	fprintf(readExistPhoneNumberList, "\n%s-%s-%04d", format.num_head, format.num_mid, format.num_tail);
	fclose(readExistPhoneNumberList);
}