//컴퓨터공학과 3학년 백승한 
//제작기간 : 2018.3.11~2018.3.14
//과제 1번
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#pragma warning (disable:4996)	//scanf 보안오류 억제를 위한 예약어

//C언어에 bool형이 없으므로 편의성을 위해 예약어로서 선언
#define True 1
#define False 0

//전화번호 저장을 위한 구조체
typedef struct TelNumberFormat {
	char num_head[4];
	char num_mid[4];
	int num_tail;
}TelNumberFormat;


int fileExist(char *filename);												//file 존재여부 확인함수
char* readLocalNumber(char *LocalName);										//Localnumber 읽는함수
char* readSubLocalNumber(char *LocalName, char *SubLocalName);				//SubLocalNumber 읽는함수
void genLastPhoneNumber(TelNumberFormat *target);							//전화번호 마지막 자리 생성함수


int main() {
	TelNumberFormat format = { 0 };											//전화번호 저장을 위한 구조체 선언
	char LocalNumberAddr[20];												//LocalAddress를 입력받기위한 문자열 배열
	char SubLocalNumberAddr[20];											//SubLocalAddress를 입력받기 위한 문자열 배열


	if (fileExist("LocalNumberList.txt") == False
		|| fileExist("SubLocalNumberList.txt") == False) {
		printf("파일을 읽어오는데 실패했습니다.\n프로그램을 종료합니다.\n");	//파일 존재 여부에 따른 예외처리
		return 1;	//파일이 존재하지 않을때 프로그램 종료
	}

	printf("프로그램 종료를 원하시면 '종료'를 입력해주세요\n\n");
	while (True) {
		printf("서비스 희망 주소를 입력해주세요 : ");
		scanf("%s", &LocalNumberAddr);
		if (strcmp(LocalNumberAddr, "종료") == 0) break;					//종료 키워드 입력으로 프로그램 종료유도
		scanf("%s", &SubLocalNumberAddr);
		if (strcmp(SubLocalNumberAddr, "종료") == 0) break;					//종료 키워드 입력으로 프로그램 종료유도


		if ((strcmp("CAN'T_FIND_SAME_LETTER.", readLocalNumber(LocalNumberAddr)) == 0)
			|| (strcmp("CAN'T_FIND_SAME_LETTER.", readSubLocalNumber(LocalNumberAddr, SubLocalNumberAddr)) == 0)) {
			printf("해당 문자열이 존재하지 않습니다.\n");				//일치하는 문자열이 존재하지 않을때에 대한 예외처리
		}
		else {
			//각 함수 호출을 통한 전화번호 생성
			strcpy(format.num_head, readLocalNumber(LocalNumberAddr));
			strcpy(format.num_mid, readSubLocalNumber(LocalNumberAddr, SubLocalNumberAddr));
			genLastPhoneNumber(&format);

			//0~9999까지의 랜덤값이 생성되기 때문에 0이되어 빈자리 생기는것을 막기위한 형식 지정자
			printf("생성된 전화번호 : %s-%s-%04d\n\n", format.num_head, format.num_mid, format.num_tail);
		}
	}
	return 0;
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
	else return False;
}

/*
Call by value
LocalNumberList.txt.파일을 불러온 후 파일의 내용과 함수의 매개변수 LocalName을 대치하여 
동일한 문자열이 있다면 그 문자열에 해당하는 번호를 반환하는 함수

특정 예외에 대한 키워드를 반환하므로서 메인함수에서 예외처리
*/
char* readLocalNumber(char *LocalName){
	int count = 0;													//동일한 문자열이 있는지를 확인하기 위한 flag변수
	char* textfield[2][20] = { "","" };								//파일에서 불러온 문자열을 임시로 저장하기위한 2차원배열
	FILE *LocalNumberList = fopen("LocalNumberList.txt", "r");		//LocalNumberList.txt 불러오기

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
			return "CAN'T_FIND_SAME_LETTER.";						//동일한 문자열이 존재하지 않을때
		}
	}
	else return "";													//해당 파일이 존재하지 않을때
}

/*
Call by value
SubLocalNumberList.txt.파일을 불러온 후 파일의 내용과 함수의 매개변수 LocalName과 SubLocalName을 각각 대치하여
동일한 문자열이 있다면 그 문자열에 해당하는 번호를 임시로 저장하고 저장된 번호를 무작위로 하나 반환하는 함수

특정 예외에 대한 키워드를 반환하므로서 메인함수에서 예외처리
*/
char* readSubLocalNumber(char *LocalName, char *SubLocalName){
	int count = 0;													//동일한 문자열이 있는지를 확인하기 위한 flag변수
	char textfield[3][20] = { "","","" };							//파일에서 불러온 문자열을 임시로 저장하기위한 2차원 배열
	char numberfield[3][5] = { "","","" };							//동일한 문자열 발견시 임시로 해당 번호들을 저장하기위한 2차원 배열

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
			return "CAN'T_FIND_SAME_LETTER.";						//동일한 문자열이 존재하지 않을때
		}
	}
	else return "";													//해당 파일이 존재하지않을때
}
//Call by reference
//랜덤함수를 이용하여 전화번호 마지막 자리를 무작위로 생성
void genLastPhoneNumber(TelNumberFormat *target) {
	srand((unsigned int)time(NULL));
	target->num_tail = rand() % 10000;								//0~9999의 번호를 임의로 생성하여 전화번호 마지막 자리에 저장
}