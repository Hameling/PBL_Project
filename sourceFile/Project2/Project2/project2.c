#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo *previous, *next;
}TAGINFO;

//이것도 괜춘할꺼 같은데?
//typedef struct List {
//	TAGINFO tg;
//	struct List *previous, *next;
//}LIST;


void initList();			//연결리스트 초기화
void inserstList();			//연결리스트와 구조체 입력받아서 저장
void readFile();			//파일을 읽어오는 함수 (tagList, referList, sortList) 함수 내부에서 insertList 호출
void referTagAnalysis();	//레퍼런스 태그 분석 기능
void targetTagAnalysis();	//타겟 태그 분석 기능	taginfo 구조체 활용?
void defineTagetPos();		//타겟 위치 추론

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