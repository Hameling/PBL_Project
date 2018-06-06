#include"function.h"



int fileExist(const char *filename) {
	FILE *file;
	if ((file = fopen(filename, "r")) == NULL) {
		printf("������ �дµ� ������ �߻��߽��ϴ�.");
		return -1;
	}
	fclose(file);
	return 1;
}
char* readTitle() {
	FILE *t_file;
	char title[10][10];
	int count = 0;
	char select[10];

	t_file = fopen(TITLE_FILE, "r");

	while (!feof(t_file)) {
		fscanf(t_file, "%s", title[count]);
		count++;
	}
	fclose(t_file);
	printMenu(title, count);


	printf("�޴��� �������ּ���.\n>>");
	scanf("%s", select);
	for (int i = 0; i < count; i++) {
		if (!strcmp(title[i], select)) return select;
	}
	return NULL;
}


void printMenu(char title[][10], int count) {
	printf("=====================================\n");
	printf("�޴�\n");
	for (int i = 0; i < count; i++) {
		if (!strcmp(title[i], "kor")){
			printf("%d. �ѱ�\n", i + 1);
		}
		else if (!strcmp(title[i], "eng")) {
			printf("%d. ����\n", i + 1);
		}
		else {
			printf("%d. %s\n", i + 1, title[i]);
		}
	}
	printf("=====================================\n");
}


void readCustom();