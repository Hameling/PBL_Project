#include<signal.h>
#include<time.h>
#include<thread>

#include"function.h"
#include"TeamProject.h"
#include<Windows.h>

#pragma warning(disable:4996)
using namespace std;

char user_name[20];
int ready_to_game = 0;
char target_name[20];

//타이틀 파일 정보 전달용
char title[5][20];
int title_count = 0;

//target 파일 정보 전달용
Node *data_set = NULL;
Node_h *data_list = NULL;
int data_size = 0;

int main() {

	printf("사용자 이름을 입력해주세요  : ");
	scanf("%s", &user_name);
	system("cls");
	printInterface();

	//해당 파일 읽는 부분
	readTitle();
	strcpy(target_name,printMenu(title, title_count));
	//printf("%s\n", target_name);
	readTarget(target_name);

	//데이터 출력 확인
	/*while (data_list->head->prev != NULL) { data_list->head = data_list->head->prev; }
	while (data_list->head != NULL) {
		printf("Data  : %s\n", data_list->head->data);
		data_list->head = data_list->head->next;
	}*/

	clearMenu(title_count);

	thread input(userInput);
	thread play(gamePlay);

	//gamePlay();
	if (ready_to_game == 1) {
		input.join();
		play.join();
	}
	return 0;
}