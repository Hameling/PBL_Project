//컴퓨터공학과 3학년 백승한 
//제작기간 : 2018.4.10~2018.4.13
//과제 3번
#include<stdio.h>
#include<math.h>
#include<string.h>
#pragma warning(disable:4996)

//숫자야구에서 사용될 숫자의 길이 정의
#define MAX_SIZE 5

//Strike 수와 Ball의 갯수를 명시적으로 표현하기 위해 구조체 선언
typedef struct playinfo {
	int strike;
	int ball;
}PLAYINFO;

char* genHiddenNumber();						//난수를 이용하여 정해진 길이만큼의 난수 문자열을 반환하는 함수
PLAYINFO compNumber(char *hidden_number, char* input);	//난수로 생성된 값과 입력된 값을 비교하여 게임 결과를 구조체로 반환하는 함수
int printScore(PLAYINFO info);					//구조체로 전달된 값에 대한 스코어 출력 함수

int main() {
	char hidden_number[MAX_SIZE + 1];		//난수 문자열을 저장하기위한 문자열
	char player_input[MAX_SIZE +1];			//사용자로부터의 입력을 저장하기위한 문자열
	strcpy(hidden_number, genHiddenNumber());	
	printf("%s\n", hidden_number);		//테스트용 출력문

	while (1) {
		printf("플레이어 입력값 : ");
		if (scanf_s("%s", &player_input, MAX_SIZE + 1) && strlen(player_input) == MAX_SIZE) {
			if (printScore(compNumber(hidden_number, player_input))) break;
		}
		else {
			if (strlen(player_input) != 0) {
				printf("입력값이 너무 짧습니다. %d글자로 맞춰주세요.\n\n", MAX_SIZE);
			}
			else {
				printf("입력값이 %d글자를 초과했습니다. 다시 입력해주세요.\n\n", MAX_SIZE);
			}
			while (getchar() != '\n'); continue;
		}
	}
	printf("프로그램을 종료합니다.\n");
	return 0;
}

/*
미리 정해진 자릿수만큼 난수를 생성하여 문자열로 반환하는 함수
문자열에 내에 동일한 숫자가 있을경우 다시 난수를 생성하여 모두 다른 값을 가지도록함
*/
char* genHiddenNumber() {
	char number_field[MAX_SIZE + 1] = "";
	char target[2] = "";
	int pos = 0;						//값이 저장될 배열의 위치를 지정해주는 변수

	srand((unsigned int)time(NULL));

	while (strlen(number_field) != MAX_SIZE) {
		itoa(rand() % 9 + 1, target, 10);

		if (strstr(number_field, target) != NULL) continue;
		number_field[pos++] = target[0];
	}
	return number_field;
}

/*
난수로 생성된 값과 플레이어가 입력한 값을 비교하여 게임 결과를 구조체의 형태로 반환하는 함수
*/
PLAYINFO compNumber(char *hidden_number, char *input) {
	PLAYINFO info;						//게임의 결과를 저장하기 위한 구조체
	int i,j;

	info.strike = 0; info.ball = 0;
	for (i = 0; i < MAX_SIZE; i++) {
		if (hidden_number[i] == input[i]) info.strike++;
		for (j = 0; j < MAX_SIZE; j++) 	
			if (hidden_number[i] == input[j] && i != j) info.ball++;
	}
	return info;
}

/*
게임결과에 값을 출력하는 함수
게임결과에 따라 1또는 0을 반환하여 게임의 종료를 유도
*/
int printScore(PLAYINFO info) {
	printf("결과값 : %dS %dB\n\n", info.strike, info.ball);
	if (info.strike == MAX_SIZE) {
		printf("HOME_RUN!!\n");
		return 1;
	}else return 0;
}
