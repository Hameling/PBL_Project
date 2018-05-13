#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tag_ID_position.h"
#define TD 0
#pragma warning(disable:4996)

//RFID 태그 정보를 저장하기 위한 구조체
typedef struct taginfo {
	char id[27];
	float rssi;
	double identifiedTime;
	struct taginfo *next;
}TAGINFO;

typedef struct List {
	 TAGINFO *first, *end;
}LIST;

//RSSI 값 순으로 정렬시키기 위한 구조체
typedef struct sortedList {
	char id[27];
	int pos[2];
	float rssi;
	struct sortedList *previous, *next;
}SORTEDLIST;


//TAGINFO *data_set[61];		//이걸 전역변수로 할지 아니면 헤더에 넣을지 고민각 아니면

void initList(TAGINFO *list);									//연결리스트 초기화
void inserstList(TAGINFO **list, TAGINFO *input, int p);		//연결리스트와 구조체 입력받아서 저장
void readFile(TAGINFO *list[], SORTEDLIST *sorted);				//파일을 읽어오는 함수 (tagList, referList, sortList) 함수 내부에서 insertList 호출
void referTagAnalysis(TAGINFO *list[]);							//레퍼런스 태그 분석 기능
void targetTagAnalysis();								//타겟 태그 분석 기능	taginfo 구조체 활용?
void defineTagetPos();									//타겟 위치 추론
void printInfo();										//태그내용 출력

int main() {
	char input;
	TAGINFO *data_set[61] = { NULL };
	SORTEDLIST *s_list = NULL;


	readFile(data_set, s_list);
	
	referTagAnalysis(data_set);

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
	printf("프로그램이 정상 종료되었습니다.\n");
	return 0;
}

void initList(TAGINFO *list) {
	strcpy(list->id, "");
	list->rssi = 0.0;
	list->identifiedTime = 0.0;
	list->next = NULL;
}

void inserstList(TAGINFO **list, TAGINFO *input, int p) {
	//printf("%p\n", input);
	if (list[p] == NULL)
		list[p]= input;
	else {
		list[p]->next = input;
		list[p] = input;
	}
	//printf("%p\n", list[p]);
}


void readFile(TAGINFO *list[], SORTEDLIST *sorted) {

	TAGINFO *temp_data;
	char readData[150];
	char *pos, *pos2;

	char tag_id[27];
	char rssi[5] = "";
	char time[13];

	int i;
	int count = 1;

	FILE *RFID_DATA = fopen("RFID_Data.txt", "r");

	
	if (RFID_DATA != NULL) {
		while (!feof(RFID_DATA)) {

			temp_data = (TAGINFO*)malloc(sizeof(TAGINFO));
			initList(temp_data);

			fgets(readData, 150, RFID_DATA);
			
			//printf("%s\n", readData);

			//Tag_ID
			pos = strstr(readData, "tag_id=") + 7;
			strncpy(temp_data->id, pos, 26);
			temp_data->id[26] = '\0';

			//rssi 값 매번 초기화 필요
			pos = strstr(readData, "rssi=") + 5;
			pos2 = strrchr(readData, ',');
			strncpy(rssi, pos, pos2 - pos);
			temp_data->rssi = atof(rssi);

			//시간
			pos = strstr(readData, "T") + 1;
			strncpy(time, pos, 12);
			time[12] = '\0';

			//시간
			pos = strtok(time, ":");
			temp_data->identifiedTime += atof(pos) * 3600 * 1000;
			//분
			pos = strtok(NULL, ":");
			temp_data->identifiedTime += atof(pos) * 60 * 1000;
			//초+밀리초
			pos = strtok(NULL, ":");
			temp_data->identifiedTime += atof(pos) * 1000;

			//printf("%s\n%f\n%f\n", temp_data->id, temp_data->rssi, temp_data->identifiedTime);

			if (strcmp(targetID, temp_data->id) == 0) {
				//printf("Target Tag입니다\n");
				inserstList(list, temp_data, 0);
			}
			else {
				i = 0;
				while ((strcmp(referenceIDs[i++], temp_data->id) != 0)) {}
				//printf("%d번째 Reference Tag입니다\n", i);
				inserstList(list, temp_data, i);
			}
			//printf("%p\n", list[i]);
			//printf("%s\n%f\n%f\n", list[i]->id, list[i]->rssi, list[i]->identifiedTime);
			strcpy(rssi, "    ");
			//테스트용
			//printf("%d번째 실행\n", count++);
		}
		
	}

	fclose(RFID_DATA);
}

void referTagAnalysis(TAGINFO *list[]) {
	int i;
	/*for (i = 0; i < 61; i++) {
		
	}*/
	if (list[TD] != NULL) {
		while (list[TD] != NULL) {
			printf("%s\n%f\n%f\n", list[TD]->id, list[TD]->rssi, list[TD]->identifiedTime);
			list[TD]= list[TD]->next;
		}
	}
}