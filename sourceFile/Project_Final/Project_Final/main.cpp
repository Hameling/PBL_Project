#include<thread>

#include"function.h"
#include"TeamProject.h"

#pragma warning(disable:4996)
using namespace std;

//함수간 통신을 위한 전역변수
char user_name[20];
int ready_to_game = 0;


//타이틀 파일 정보 전달용
char title[5][20];
int title_count = 0;

//target 파일 정보 전달용
char target_name[20];
Node *data_set = NULL;
Node_h *data_list = NULL;
int data_size = 0;

int main() {
	
	//사용자 이름을 입력받고 인터페이스를 출력
	printf("사용자 이름을 입력해주세요  : ");
	scanf("%s", &user_name);
	system("cls");
	printInterface();

	//해당 파일 읽는 부분
	readTitle();
	strcpy(target_name,printMenu(title, title_count));
	readTarget(target_name);

	//출력되었던 메뉴를 삭제
	clearMenu(title_count);

	//쓰레드 선언 및 게임 진행시작
	thread input(userInput);
	thread play(gamePlay);

	if (ready_to_game == 1) {
		input.join();
		play.join();
	}
	gameover();
	return 0;
}