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



#define MAX 512 //문자열 길이를 정의

char text[20] = { "abcd" }; // 이중연결리스트로 변경
int correct = 0;


int main() {
	int num = 0;
	puts("분당타자수 계산 프로그램입니다.");
	printf("한글모드는 1번 영어모드는 2번을 눌러주세요 : ");
	scanf("%d", &num);
	if (num == 1) korea_mode();
	else if (num == 2) english_mode();
	else puts("번호를 잘못 입력하셨습니다 . 프로그램을 다시 실행시켜주세요");
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
		if (key == 8) {
			len--;
			count--;
			printf("%c", key);
		}
		else if (key == 13)
		{
			stop = system_clock::now();
			count = 0;
			total_time = stop - start;
			printf("\n");
			break;
		}
		else if (key != '\0') {
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); }
			ary[count] = key;
			count++;
			len++;
		}
	}

	int textlen = strlen(text);

	for (i = 0; i < textlen; i++)
	{
		if (ary[i] != text[i]) {
			len = 0;
			break;
		}
		if (textlen -1 == i) correct++;
	}
	arr = 60 / total_time.count() * len; //시간에서 입력한 타자수를 곱함
	return arr;//분당 타자수

}

double korea_gettext()
{
	system_clock::time_point start;
	system_clock::time_point stop;
	duration<double> total_time;
	int count = 0;
	char key;
	char str[100];
	wchar_t w_str[100] = L"";
	size_t cn;
	wchar_t buffer[4096];
	double arr;
	int i;
	int cnt = 0;
	while (1)
	{
		key = getKey();
		if (key != '\0')
		{
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); }
			str[count] = key;
			count++;
		}
		if (key == 13)
		{
			stop = system_clock::now();
			count = 0;
			total_time = stop - start;
			printf("완성 텍스트 : %s\n", str);
			break;
		}
	}
	setlocale(LC_ALL, "Korean");
	mbstowcs_s(&cn, w_str, 100, str, 100);
	wprintf(L"%s\n", w_str);


	KoreanBreak(w_str, buffer, sizeof buffer);
	setlocale(LC_ALL, "Korean");
	for (i = 0; (int)buffer[i] != NULL; i++)
	{
		cnt++;
	}
	cnt--;
	printf("경과 시간 : %f\n", total_time.count());
	printf("타자수 : %d\n", cnt);

	arr = 60 / total_time.count() * cnt;

	printf("분당타자수 : %f", arr);
	return arr;
}


int KoreanBreak(wchar_t *str, wchar_t *buffer, unsigned int nSize)
{
	//초성 
	wchar_t wcHead[] = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
		L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
		L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
		L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
		L'ㅌ', L'ㅍ', L'ㅎ' };

	//중성 
	wchar_t   wcMid[] = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ',
		L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
		L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
		L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
		L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };

	//종성 
	wchar_t wcTail[] = { L' ', L'ㄱ', L'ㄲ', L'ㄳ ',
		L'ㄴ', L'ㄵ ', L'ㄶ ', L'ㄷ',
		L'ㄹ', L'ㄺ', L'ㄻ ', L'ㄼ ',
		L'ㄽ ', L'ㄾ ', L'ㄿ', L'ㅀ',
		L'ㅁ', L'ㅂ', L'ㅄ ', L'ㅅ',
		L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
		L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };

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

			buffer[pos] = wcHead[(*str - 0xAC00) / (21 * 28)];
			buffer[pos + 1] = wcMid[(*str - 0xAC00) % (21 * 28) / 28];
			buffer[pos + 2] = wcTail[(*str - 0xAC00) % 28];
			if ((*str - 0xAC00) % (21 * 28) / 28 == 9 || (*str - 0xAC00) % (21 * 28) / 28 == 10 || (*str - 0xAC00) % (21 * 28) / 28 == 11 || (*str - 0xAC00) % (21 * 28) / 28 == 14 || (*str - 0xAC00) % (21 * 28) / 28 == 15 || (*str - 0xAC00) % (21 * 28) / 28 == 16 || (*str - 0xAC00) % (21 * 28) / 28 == 19)
			{
				buffer[pos + 2] = '0';
				pos = pos + 1;
			}
			if ((*str - 0xAC00) % 28 == 0) pos = pos - 1;
			if ((*str - 0xAC00) % 28 == 3 || (*str - 0xAC00) % 28 == 5 || (*str - 0xAC00) % 28 == 6 || (*str - 0xAC00) % 28 == 9 || (*str - 0xAC00) % 28 == 10 || (*str - 0xAC00) % 28 == 11 || (*str - 0xAC00) % 28 == 12 || (*str - 0xAC00) % 28 == 13 || (*str - 0xAC00) % 28 == 14 || (*str - 0xAC00) % 28 == 15 || (*str - 0xAC00) % 28 == 18) {
				buffer[pos + 3] = '0';
				pos = pos + 1;
			}
			pos += 3;
		}

		++str;
	}

	buffer[pos] = '\0';

	return pos;
}



char getKey()
{
	if (kbhit()) // kbhit()이용해 입력값이 있는지 확인 
	{
		return getch();     // 입력값이 getch()로 char를 리턴해줌
	}
	return '\0'; // 입력값이 없으면 널 문자 리턴
}


void korea_mode()
{
	double avg = 0;
	while (1)//조건 넣어야함
	{
		avg += korea_gettext();
	}
	printf("최종 분당 타자수는 : %d 입니다.\n", avg / correct);
}

void english_mode()
{
	int i = 0;
	double avg = 0;
	while (i!=5)//조건 넣어야함
	{
		avg = avg + english_gettext();
		i++;
	}
	printf("최종 분당 타자수는 : %f 입니다.\n", avg / correct);
}