//컴퓨터공학과 3학년 백승한 
//제작기간 : 2018.3.11~2018.3.19
//과제 2번
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#pragma warning (disable:4996)	//scanf 보안오류 무시를 위한 예약어

//C언어에 bool형이 없으므로 편의성을 위해 예약어로서 선언
#define True 1
#define False 0

//전화번호 저장을 위한 구조체
typedef struct TelNumberFormat {
	char num_head[4];
	char num_mid[4];
	int num_tail;
}TelNumberFormat;

//연결리스트 사용을 위한 노드선언
typedef struct Node {
	struct Node *next;
	char PhoneNumber[20];
}Node;

//연결리스트 구조 선언
typedef struct List {
Node *first, *end;
}List;

//연결리스트 관련함수
List* initList();									//List 초기화 함수
void addList(List *list, char *data);				//List 노드 추가함수
void delList(List *list);							//List 노드 제거함수

//내부 기능과 관련된 함수
void clearFormat(TelNumberFormat *format);															//TelNumberFormat 구조체 초기화 함수
int fileExist(char *filename);																		//file 존재여부 확인함수
int readLocalNumber(TelNumberFormat *format, char *LocalName);										//Localnumber 읽는함수
int readSubLocalNumber(TelNumberFormat *format, char *LocalName, char *SubLocalName);				//SubLocalNumber 읽는함수
void genLastPhoneNumber(TelNumberFormat *target);													//전화번호 마지막 자리 생성함수
int compareTelNumber(TelNumberFormat format);														//전화번호 존재여부 확인함수
void saveTelNumber(TelNumberFormat format);															//파일에 전화번호를 저장하는 함수


int main() {
	TelNumberFormat format = { "" };										//전화번호 저장을 위한 구조체 선언
	char LocalNumberAddr[20];												//LocalAddress를 입력받기위한 문자열 배열
	char SubLocalNumberAddr[20];											//SubLocalAddress를 입력받기 위한 문자열 배열
	char input[20];															//희망번호 선택여부 확인변수
	char PrivateNumber[10];													//입력받은 희망번호를 저장하는 변수
	int flag = 0;															//반복문 탈출을 위한 Flag 변수
	

	if(fileExist("LocalNumberList.txt") == False
		|| fileExist("SubLocalNumberList.txt") == False
		|| fileExist("ExistingPhoneNumber.txt") == False){
		printf("파일을 읽어오는데 실패했습니다.\n프로그램을 종료합니다.\n");	//파일 존재 여부에 따른 예외처리
		return 1;	//파일이 존재하지 않을때 프로그램 종료
	}


	printf("프로그램 종료를 원하시면 '종료'를 입력해주세요\n\n");
	while (True) {
		flag = 0;
		printf("서비스 희망 주소를 입력해주세요 : ");
		scanf("%s", &LocalNumberAddr);
		if (strcmp(LocalNumberAddr, "종료") == 0) break;					//종료 키워드 입력으로 프로그램 종료유도
		scanf("%s", &SubLocalNumberAddr);
		if (strcmp(SubLocalNumberAddr, "종료") == 0) break;					//종료 키워드 입력으로 프로그램 종료유도

		//일치하는 문자열이 없을경우 TelNumberFormat을 초기화하고 다시 입력받음
		if (readLocalNumber(&format, LocalNumberAddr) == False ||
			readSubLocalNumber(&format, LocalNumberAddr, SubLocalNumberAddr) == False) {
			printf("-입력하신 주소와 일치하는 지역을 찾을수 없습니다\n");	
			clearFormat(&format);
		}

		//일치하는 문자열이 있는경우 개인번호 사용여부와 희망 개인번호를 입력받아 최종 전화번호를 출력하고 저장
		else {	
			while (flag != 1) {
				printf("개인번호를 사용하시겠습니까? (yes/no) : ");
				scanf("%s", input);
				//개인번호 사용여부
				if (strcmp(input, "yes") != 0 && strcmp(input, "no") != 0) {
					printf("-잘못 입력하셨습니다. yes 또는 no를 입력해주세요\n");
				}
				else if(strcmp(input,"yes") == 0){
					while (flag != 1) {
						printf("희망하는 개인번호를 4자리를 입력해주세요 : ");
						scanf("%s", PrivateNumber);
						//희망 개인번호
						if (strlen(PrivateNumber) != 4) {
							printf("-잘못입력하셨습니다.\n");
						}
						else {
							format.num_tail = atoi(PrivateNumber);
							//해당 번호의 존재여부
							if (compareTelNumber(format) == 1) {
								flag = 1;
								printf("해당번호가 생성되었습니다.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
							}
							else {
								printf("-해당 번호가 존재합니다.\n"); 
								while (flag != 1) {
									printf("다시 입력하시겠습니까 ? (yes / no) : ");
									scanf("%s", input);
									if (strcmp(input, "yes") != 0 && strcmp(input, "no") != 0) {
										printf("-잘못 입력하셨습니다. yes 또는 no를 입력해주세요\n");
									}
									else if(strcmp(input, "yes") == 0) break;
									else{
										flag = 1;
										genLastPhoneNumber(&format);
										printf("새로운번호가 생성되었습니다.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
									}
								}
								
							}
						}
					}
				}
				else {
					genLastPhoneNumber(&format);
					printf("새로운번호가 생성되었습니다.\n->%s-%s-%04d\n", format.num_head, format.num_mid, format.num_tail);
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
List를 생성하고 해당 List를 초기화한후 반환해주는 함수
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
새로운 노드를 생성하여 매개변수의 data값을 저장하고
매개변수 값의 list에 노드가 없다면 첫노드에
매개변수 값의 list에 노드가 있다면 마지막 노드의 뒤에 연결하는 함수
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
매개변수로 생성된 list의 노드를 제거하고 할당되어 있던 데이터를 반환하는 함수
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
TelNumberFormat 변수를 초기화
*/
void clearFormat(TelNumberFormat *format) {
	strcpy(format->num_head, "");
	strcpy(format->num_mid, "");
	format->num_tail = 0;
}

/*
Call by value
파일 존재여부 확인함수
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
LocalNumberList.txt.파일을 불러온 후 파일의 내용과 함수의 매개변수 LocalName을 비교하여
동일한 문자열이 있다면 그 문자열에 해당하는 번호를 반환하는 함수
특정 예외에 대한 값을 반환하므로서 if문을 통해 예외처리 가능
*/
int readLocalNumber(TelNumberFormat *format, char *LocalName) {
	int count = 0;													//동일한 문자열이 있는지를 확인하기 위한 flag변수
	char* textfield[2][20] = { "","" };								//파일에서 불러온 문자열을 임시로 저장하기위한 2차원배열
	FILE *LocalNumberList = fopen("LocalNumberList.txt", "r");		//LocalNumberList.txt 불러오기

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
			return False;							//동일한 문자열이 존재하지 않을때
		}
	}
	else return -1;									//해당 파일이 존재하지 않을때
}

/*
Call by reference
SubLocalNumberList.txt.파일을 불러온 후 파일의 내용과 함수의 매개변수 LocalName과 SubLocalName을 각각 비교하여
동일한 문자열이 있다면 그 문자열에 해당하는 번호를 임시로 저장하고 저장된 번호를 무작위로 하나 반환하는 함수
특정 예외에 대한 값을 반환하므로서 if문을 통해 예외처리 가능
*/
int readSubLocalNumber(TelNumberFormat *format, char *LocalName, char *SubLocalName) {
	int count = 0;													//동일한 문자열이 있는지를 확인하기 위한 flag변수
	char textfield[3][20] = { "","","" };							//파일에서 불러온 문자열을 임시로 저장하기위한 2차원 배열
	char numberfield[3][5] = { "","","" };							//동일한 문자열 발견시 임시로 해당 번호들을 저장하기위한 2차원 배열
	FILE *SubLocalNumberList = fopen("SubLocalNumberList.txt", "r");//SubLocalNumberList.txt 불러오기

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
		else return False;							//동일한 문자열이 존재하지 않을때
	}
	else return -1;									//해당 파일이 존재하지않을때
}

/*
Call by reference
랜덤함수를 이용하여 전화번호 마지막 자리를 무작위로 생성
*/
void genLastPhoneNumber(TelNumberFormat *target) {
	srand((unsigned int)time(NULL));
	target->num_tail = rand() % 10000;								//0~9999의 번호를 임의로 생성하여 전화번호 마지막 자리에 저장
}

/*
Call by value
ExistingPhoneNumber.txt 파일을 불러온후 파일의 내용과 입력된 전화번호를 비교하여 해당 전화번호의 존재여부를 판명하는 함수
특정 예외에 대한 값을 반환하므로서 if문을 통해 예외처리 가능
*/
int compareTelNumber(TelNumberFormat format) {
	FILE *readExistPhoneNumberList = fopen("ExistingPhoneNumber.txt", "r");	//ExistingPhoneNumber.txt 불러오기
	List *numberlist = initList();												//List 변수를 선언하고 List를 초기화
	Node *tmp = &numberlist->first;												//List의 데이터를 읽어오기위한 temp변수
	char textfield[20];															//파일에서 읽어온 문자열을 임시로 저장하기위한 문자열

	if (readExistPhoneNumberList != NULL) {
		while (!feof(readExistPhoneNumberList)){
			fscanf(readExistPhoneNumberList, "%s", &textfield);
			addList(numberlist, textfield);
		}
	}
	else return -1;					//해당 파일이 존재하지 않을때

	while (tmp->next != NULL) {
		sprintf(textfield, "%s-%s-%04d", format.num_head, format.num_mid, format.num_tail);
		if (strcmp(tmp->next->PhoneNumber,textfield) == 0) {
			delList(numberlist);
			free(numberlist);
			return False;			//동일한 번호가 존재할때 
		}
		tmp = tmp->next;
	}
	delList(numberlist);
	free(numberlist);
	return True;

}

/*
최종 출력된 전화번호를 ExistingPhoneNumber.txt 파일에 저장하는 함수
*/
void saveTelNumber(TelNumberFormat format) {
	FILE *readExistPhoneNumberList = fopen("ExistingPhoneNumber.txt", "a+");	//ExistingPhoneNumber.txt 불러오기
	fprintf(readExistPhoneNumberList, "\n%s-%s-%04d", format.num_head, format.num_mid, format.num_tail);
	fclose(readExistPhoneNumberList);
}