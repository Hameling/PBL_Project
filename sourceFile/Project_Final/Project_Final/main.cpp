#include<thread>

#include"function.h"
#include"TeamProject.h"

#pragma warning(disable:4996)
using namespace std;

//�Լ��� ����� ���� ��������
char user_name[20];
int ready_to_game = 0;


//Ÿ��Ʋ ���� ���� ���޿�
char title[5][20];
int title_count = 0;

//target ���� ���� ���޿�
char target_name[20];
Node *data_set = NULL;
Node_h *data_list = NULL;
int data_size = 0;

int main() {
	
	//����� �̸��� �Է¹ް� �������̽��� ���
	printf("����� �̸��� �Է����ּ���  : ");
	scanf("%s", &user_name);
	system("cls");
	printInterface();

	//�ش� ���� �д� �κ�
	readTitle();
	strcpy(target_name,printMenu(title, title_count));
	readTarget(target_name);

	//��µǾ��� �޴��� ����
	clearMenu(title_count);

	//������ ���� �� ���� �������
	thread input(userInput);
	thread play(gamePlay);

	if (ready_to_game == 1) {
		input.join();
		play.join();
	}
	gameover();
	return 0;
}