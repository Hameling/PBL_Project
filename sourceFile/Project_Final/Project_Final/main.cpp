#include<signal.h>
#include<time.h>
#include<thread>

#include"function.h"
#include<Windows.h>
#pragma warning(disable:4996)
#pragma comment(lib, "winmm.lib")
using namespace std;

char user_name[20];
int ready_to_game = 0;

int main() {
	
	
	
	char title[3][20] = { "kor","eng","C_language" };
	char target_name[20];
	printf("사용자 이름을 입력해주세요  : ");
	scanf("%s", &user_name);
	system("cls");
	printInterface();

	//해당 파일 읽는 부분
	strcpy(target_name,printMenu(title, 3));
	//printf("%s\n", target_name);
	
	clearMenu(3);

	thread input(userInput);
	thread play(gamePlay);

	//gamePlay();
	if (ready_to_game == 1) {
		input.join();
		play.join();
	}

	/*char title[20];

	strcpy(title, readTitle());

	printf("%s\n",title);

	readCustom(title);*/

	return 0;
}