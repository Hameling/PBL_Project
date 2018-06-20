#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include "locale.h"
#include<wchar.h>
#include<chrono>
#include<iostream>
#include<string>
#pragma comment(lib, "winmm.lib")
#pragma warning (disable :4996)


using namespace std;
using namespace chrono;


double english_gettext();
double korea_gettext();
int KoreanBreak(wchar_t *str, wchar_t *buffer, unsigned int nSize);
char getKey();
void korea_mode();
void english_mode();



#define MAX 512 //���ڿ� ���̸� ����

char text[20] = { "abcde" }; // ���߿��Ḯ��Ʈ�� ����
int correct = 0;


int main() {
	int num = 0;
	puts("�д�Ÿ�ڼ� ��� ���α׷��Դϴ�.");
	printf("�ѱ۸��� 1�� ������� 2���� �����ּ��� : ");
	scanf("%d", &num);
	if (num == 1) korea_mode();
	else if (num == 2) english_mode();
	else puts("��ȣ�� �߸� �Է��ϼ̽��ϴ� . ���α׷��� �ٽ� ��������ּ���");
	system("pause");
	return 0;
}

double english_gettext()
{
	system_clock::time_point start;
	system_clock::time_point stop;
	duration<double> total_time;
	unsigned int time;
	char key;
	int count = 0;
	double arr = 0;
	int len = 0;
	char ary[20];
	int i = 0;


	while (1)
	{
		key = getKey();
		if (key == 8) { // �齺���̽��� ������� �� ����
			len--;
			count--;
			printf("%c", key);
		}
		else if (key == 13) // ����Ű�� ������� �� ����
		{
			stop = system_clock::now();// ����Ű�� ������ ���ÿ� �ð�����
			count = 0;
			total_time = stop - start; // �������� �ð�����ſ��� ������ �� �ð��� ��Ÿ� ��
			printf("\n");
			break;
		}
		else if (key != '\0') { // ��ҿ� Ÿ������ �� �� ����
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); } //Ÿ������ ������ ���ÿ� �ð�����
			ary[count] = key;
			count++;
			len++;
		}
	}

	int textlen = strlen(text); // �־��� �ܾ��� ���̸� �����µ� 

	for (i = 0; i < textlen; i++) //...! 
	{
		if (ary[i] != text[i]) { 
			len = 0;
			break;
		}
		if (textlen - 1 == i) {
			if(textlen==len) correct++;
			else len = 0;// ���̰� ���� ����͵� ������� correct�� �ϳ� ������Ŵ
		}
	}
	printf("\n%d\n", correct);
	arr = 60 / total_time.count() * len; //�ð����� �Է��� Ÿ�ڼ��� ����
	return arr;//�д� Ÿ�ڼ�

}

double korea_gettext()
{
	system_clock::time_point start;
	system_clock::time_point stop;
	duration<double> total_time;
	int count = 0;
	char key;
	wchar_t str[100];
	wchar_t w_str[100] = L"";
	size_t cn;
	wchar_t buffer[4096];
	double arr;
	int i;
	int cnt = 0;
	int strlen_ = 0;
	while (1)
	{
		key = getKey();
		if (key == 8) {
			count--;
		}
		else if (key == 13)
		{
			stop = system_clock::now();
			count = 0;
			total_time = stop - start;
			printf("�ϼ� �ؽ�Ʈ : %s\n", str);
			break;
		}
		else if (key != '\0')
		{
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); }
			str[count] = key;
			strlen_++;
			count++;
		}

	}
	int strlen_text = strlen(text);
	for (i = strlen_text; i < strlen_; i++)
	{
		str[i] = NULL;
	}
	
	KoreanBreak(str, buffer, sizeof buffer);
	setlocale(LC_ALL, "Korean");
	for (i = 0; (int)buffer[i] != NULL; i++)
	{
		cnt++;
	}
	printf("��� �ð� : %f\n", total_time.count());
	printf("Ÿ�ڼ� : %d\n", cnt);

	arr = 60 / total_time.count() * cnt;

	printf("�д�Ÿ�ڼ� : %f", arr);
	return arr;
}


int KoreanBreak(wchar_t *str, wchar_t *buffer, unsigned int nSize) // �ʼ� �߼� ���� �и��ϴ� �Լ�
{
	//�ʼ� 
	wchar_t wcHead[] = { L'��', L'��', L'��', L'��', // �ʼ�
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��' };

	//�߼� 
	wchar_t   wcMid[] = { L'��', L'��', L'��', L'��', // �߼�
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��', L'��' };

	//���� 
	wchar_t wcTail[] = { L' ', L'��', L'��', L'�� ', // ����
		L'��', L'�� ', L'�� ', L'��',
		L'��', L'��', L'�� ', L'�� ',
		L'�� ', L'�� ', L'��', L'��',
		L'��', L'��', L'�� ', L'��',
		L'��', L'��', L'��', L'��',
		L'��', L'��', L'��', L'��' };

	unsigned int pos = 0;

	while (*str != '\0')
	{
		if (*str < 256)
		{
			if (pos + 2 >= nSize - 1)
				break;

			buffer[pos] = *str;
			++pos;
		}
		else
		{
			if (pos + 4 >= nSize - 1)
				break;

			buffer[pos] = wcHead[(*str - 0xAC00) / (21 * 28)];  // �ʼ�
			buffer[pos + 1] = wcMid[(*str - 0xAC00) % (21 * 28) / 28]; // �߼�
			buffer[pos + 2] = wcTail[(*str - 0xAC00) % 28]; // ����
			if ((*str - 0xAC00) % (21 * 28) / 28 == 9 || (*str - 0xAC00) % (21 * 28) / 28 == 10 || (*str - 0xAC00) % (21 * 28) / 28 == 11 || (*str - 0xAC00) % (21 * 28) / 28 == 14 || (*str - 0xAC00) % (21 * 28) / 28 == 15 || (*str - 0xAC00) % (21 * 28) / 28 == 16 || (*str - 0xAC00) % (21 * 28) / 28 == 19)
			{
				buffer[pos + 2] = '0';
				pos = pos + 1;
			}// �ȳ� �� �������� ����ó��
			if ((*str - 0xAC00) % 28 == 0) pos = pos - 1;
			if ((*str - 0xAC00) % 28 == 3 || (*str - 0xAC00) % 28 == 5 || (*str - 0xAC00) % 28 == 6 || (*str - 0xAC00) % 28 == 9 || (*str - 0xAC00) % 28 == 10 || (*str - 0xAC00) % 28 == 11 || (*str - 0xAC00) % 28 == 12 || (*str - 0xAC00) % 28 == 13 || (*str - 0xAC00) % 28 == 14 || (*str - 0xAC00) % 28 == 15 || (*str - 0xAC00) % 28 == 18) {
				buffer[pos + 3] = '0';
				pos = pos + 1;
			}//�� �� �������� ����ó��
			pos += 3;
		}

		++str;
	}

	buffer[pos] = '\0';

	return pos;
}



char getKey() // 
{
	if (kbhit()) return getch();   // kbhit()�̿��� �Է°��� �ִ��� Ȯ��  �Է°��� getch()�� char�� ��������
	return '\0'; // �Է°��� ������ �� ���� ����
}


void korea_mode() // �ѱ� ���
{
	double avg = 0;
	while (1)//���� �־����
	{
		avg += korea_gettext();
	}
	printf("���� �д� Ÿ�ڼ��� : %d �Դϴ�.\n", avg / correct);
}

void english_mode() // ���� ���
{
	int i = 0;
	double avg = 0;
	while (i!=5)//���� �־����
	{
		avg = avg + english_gettext();
		i++;
	}
	printf("���� �д� Ÿ�ڼ��� : %f �Դϴ�.\n", avg / correct);
}