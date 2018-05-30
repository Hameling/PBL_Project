//��ǻ�Ͱ��а� 3�г� ����� 
//���۱Ⱓ : 2018.4.10~2018.4.13
//���� 3��
#include<stdio.h>
#include<math.h>
#include<string.h>
#pragma warning(disable:4996)

//���ھ߱����� ���� ������ ���� ����
#define MAX_SIZE 5

//Strike ���� Ball�� ������ ��������� ǥ���ϱ� ���� ����ü ����
typedef struct playinfo {
	int strike;
	int ball;
}PLAYINFO;

char* genHiddenNumber();						//������ �̿��Ͽ� ������ ���̸�ŭ�� ���� ���ڿ��� ��ȯ�ϴ� �Լ�
PLAYINFO compNumber(char *hidden_number, char* input);	//������ ������ ���� �Էµ� ���� ���Ͽ� ���� ����� ����ü�� ��ȯ�ϴ� �Լ�
int printScore(PLAYINFO info);					//����ü�� ���޵� ���� ���� ���ھ� ��� �Լ�

int main() {
	char hidden_number[MAX_SIZE + 1];		//���� ���ڿ��� �����ϱ����� ���ڿ�
	char player_input[MAX_SIZE +1];			//����ڷκ����� �Է��� �����ϱ����� ���ڿ�
	strcpy(hidden_number, genHiddenNumber());	
	printf("%s\n", hidden_number);		//�׽�Ʈ�� ��¹�

	while (1) {
		printf("�÷��̾� �Է°� : ");
		if (scanf_s("%s", &player_input, MAX_SIZE + 1) && strlen(player_input) == MAX_SIZE) {
			if (printScore(compNumber(hidden_number, player_input))) break;
		}
		else {
			if (strlen(player_input) != 0) {
				printf("�Է°��� �ʹ� ª���ϴ�. %d���ڷ� �����ּ���.\n\n", MAX_SIZE);
			}
			else {
				printf("�Է°��� %d���ڸ� �ʰ��߽��ϴ�. �ٽ� �Է����ּ���.\n\n", MAX_SIZE);
			}
			while (getchar() != '\n'); continue;
		}
	}
	printf("���α׷��� �����մϴ�.\n");
	return 0;
}

/*
�̸� ������ �ڸ�����ŭ ������ �����Ͽ� ���ڿ��� ��ȯ�ϴ� �Լ�
���ڿ��� ���� ������ ���ڰ� ������� �ٽ� ������ �����Ͽ� ��� �ٸ� ���� ����������
*/
char* genHiddenNumber() {
	char number_field[MAX_SIZE + 1] = "";
	char target[2] = "";
	int pos = 0;						//���� ����� �迭�� ��ġ�� �������ִ� ����

	srand((unsigned int)time(NULL));

	while (strlen(number_field) != MAX_SIZE) {
		itoa(rand() % 9 + 1, target, 10);

		if (strstr(number_field, target) != NULL) continue;
		number_field[pos++] = target[0];
	}
	return number_field;
}

/*
������ ������ ���� �÷��̾ �Է��� ���� ���Ͽ� ���� ����� ����ü�� ���·� ��ȯ�ϴ� �Լ�
*/
PLAYINFO compNumber(char *hidden_number, char *input) {
	PLAYINFO info;						//������ ����� �����ϱ� ���� ����ü
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
���Ӱ���� ���� ����ϴ� �Լ�
���Ӱ���� ���� 1�Ǵ� 0�� ��ȯ�Ͽ� ������ ���Ḧ ����
*/
int printScore(PLAYINFO info) {
	printf("����� : %dS %dB\n\n", info.strike, info.ball);
	if (info.strike == MAX_SIZE) {
		printf("HOME_RUN!!\n");
		return 1;
	}else return 0;
}
