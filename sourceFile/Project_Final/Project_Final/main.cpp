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

//Ÿ��Ʋ ���� ���� ���޿�
char title[5][20];
int title_count = 0;

//target ���� ���� ���޿�
Node *data_set = NULL;
Node_h *data_list = NULL;
int data_size = 0;

int main() {

	printf("����� �̸��� �Է����ּ���  : ");
	scanf("%s", &user_name);
	system("cls");
	printInterface();

	//�ش� ���� �д� �κ�
	readTitle();
	strcpy(target_name,printMenu(title, title_count));
	//printf("%s\n", target_name);
	readTarget(target_name);

	//������ ��� Ȯ��
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