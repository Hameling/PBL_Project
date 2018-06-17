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
		if (key == 8) { // 백스페이스를 사용했을 때 내용
			len--;
			count--;
			printf("%c", key);
		}
		else if (key == 13) // 엔터키를 사용할을 때 내용
		{
			stop = system_clock::now();// 엔터키를 누름과 동시에 시간을잼
			count = 0;
			total_time = stop - start; // 마지막에 시간을잰거에서 시작할 때 시간을 잰거를 뺌
			printf("\n");
			break;
		}
		else if (key != '\0') { // 평소에 타이핑을 할 때 내용
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); } //타이핑을 받음과 동시에 시간을잼
			ary[count] = key;
			count++;
			len++;
		}
	}

	int textlen = strlen(text); // 주어진 단어의 길이를 가져온뒤 

	for (i = 0; i < textlen; i++) //...! 
	{
		if (ary[i] != text[i]) { 
			len = 0;
			break;
		}
		if (textlen - 1 == i) {
			if(textlen==len) correct++;
			else len = 0;// 길이가 같고 생긴것도 같을경우 correct를 하나 증가시킴
		}
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
	wchar_t w_text[100] = L"";
	size_t cn;
	wchar_t buffer[4096];
	wchar_t buffer_[4096];
	double arr;
	int i;
	int cnt = 0;
	while (1)
	{
		key = getKey();
		if (key == 8) { // 백스페이스를 사용했을 때
			if (count == 0) {
				for (i = 0; str[i] != NULL; i++) {// str 초기화 만약 count가 0일 때
					str[i] = NULL;
				}
			}
			if (count != 0) count--;
			printf("%c", key);
		}
		else if (key == 13) //엔터를 사용했을 때
		{
			stop = system_clock::now(); // stop에 시간을 저장
			str[count] = NULL;// 맨 끝자리에 쓰레기 값이 들어가서 마지막값을 지움
			count = 0;
			total_time = stop - start; // 걸린시간을 구함
			printf("%\n");
			break;
		}
		else if (key != '\0')
		{
			printf("%c", key);
			if (count == 0) { start = system_clock::now(); } // 타이핑을 시작할 때 시간을 재기 시작함
			str[count] = key; // str이라는 문자열에 key값을 저장
			count++;
		}

	}
	setlocale(LC_ALL, "Korean"); // 한글 유니코드
	mbstowcs_s(&cn, w_str, 100, str, 100); // w_str에 str값을 복사

	int strlen_str = strlen(str); // srt의 길이
	int strlen_text = strlen(text); // text의 길이
	if (strlen_str != strlen_text) return 0; // 둘이 비교후 다를경우 함수를 종료


	KoreanBreak(w_str, buffer, sizeof buffer); // 한글을 초성 중성 종성으로 나눔
	setlocale(LC_ALL, "Korean");
	for (i = 0; (int)buffer[i] != NULL; i++)
	{
		cnt++; // 몇글잔지 카운팅
	}
	setlocale(LC_ALL, "Korean");

	if (strlen_str != strlen_text) cnt = 0; 

	setlocale(LC_ALL, "Korean");
	mbstowcs_s(&cn, w_text, 100, text, 100);

	int textlen = KoreanBreak(w_text, buffer_, sizeof buffer_); // text의 값을 한글을 분해한 뒤 기리이를 구함


	for (i = 0; i < textlen; i++) //...! 
	{
		if (str[i] != text[i]) {
			cnt = 0;
			break;
		}
		if (textlen - 1 == i) {
			if (textlen == cnt) {
				correct++;
				break;
			}
			else {
				cnt = 0;// 길이가 같고 생긴것도 같을경우 correct를 하나 증가시킴
				break;
			}
		}
	}

	arr = 60 / total_time.count() * (double)cnt;

	for (i = 0; buffer[i] != NULL; i++) {// buffer 초기화
		buffer[i] = NULL;
	}
	for (i = 0; buffer_[i] != NULL; i++) {// buffer_ 초기화
		buffer_[i] = NULL;
	}
	return arr;
}



int KoreanBreak(wchar_t *str, wchar_t *buffer, unsigned int nSize) // 초성 중성 종성 분리하는 함수
{
	//초성 
	wchar_t wcHead[] = { L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', // 초성
		L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
		L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
		L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
		L'ㅌ', L'ㅍ', L'ㅎ' };

	//중성 
	wchar_t   wcMid[] = { L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', // 중성
		L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
		L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
		L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
		L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };

	//종성 
	wchar_t wcTail[] = { L' ', L'ㄱ', L'ㄲ', L'ㄳ ', // 종성
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

			buffer[pos] = wcHead[(*str - 0xAC00) / (21 * 28)];  // 초성
			buffer[pos + 1] = wcMid[(*str - 0xAC00) % (21 * 28) / 28]; // 중성
			buffer[pos + 2] = wcTail[(*str - 0xAC00) % 28]; // 종성
			if ((*str - 0xAC00) % (21 * 28) / 28 == 9 || (*str - 0xAC00) % (21 * 28) / 28 == 10 || (*str - 0xAC00) % (21 * 28) / 28 == 11 || (*str - 0xAC00) % (21 * 28) / 28 == 14 || (*str - 0xAC00) % (21 * 28) / 28 == 15 || (*str - 0xAC00) % (21 * 28) / 28 == 16 || (*str - 0xAC00) % (21 * 28) / 28 == 19)
			{
				buffer[pos + 2] = '0';
				pos = pos + 1;
			}// ㅘ나 ㅚ 같은것을 예외처리
			if ((*str - 0xAC00) % 28 == 0) pos = pos - 1;
			if ((*str - 0xAC00) % 28 == 3 || (*str - 0xAC00) % 28 == 5 || (*str - 0xAC00) % 28 == 6 || (*str - 0xAC00) % 28 == 9 || (*str - 0xAC00) % 28 == 10 || (*str - 0xAC00) % 28 == 11 || (*str - 0xAC00) % 28 == 12 || (*str - 0xAC00) % 28 == 13 || (*str - 0xAC00) % 28 == 14 || (*str - 0xAC00) % 28 == 15 || (*str - 0xAC00) % 28 == 18) {
				buffer[pos + 3] = '0';
				pos = pos + 1;
			}//ㄳ ㅄ 같은것을 예외처리
			pos += 3;
		}

		++str;
	}

	buffer[pos] = '\0';

	return pos;
}



char getKey() // 
{
	if (kbhit()) return getch();   // kbhit()이용해 입력값이 있는지 확인  입력값이 getch()로 char를 리턴해줌
	return '\0'; // 입력값이 없으면 널 문자 리턴
}


void korea_mode() // 한글 모드
{
	int i = 0;
	double avg = 0;
	while (i!=5)//조건 넣어야함
	{
		avg += korea_gettext();
		i++;
	}

	printf("%d", correct);

	if (correct == 0) printf("최종 분당 타자수는 : 0 입니다.\n");
	else printf("최종 분당 타자수는 : %f 입니다.\n", avg / correct);
}

void english_mode() // 영어 모드
{
	int i = 0;
	double avg = 0;
	while (i!=5)//조건 넣어야함
	{
		avg = avg + english_gettext();
		i++;
	}
	if (correct == 0) printf("최종 분당 타자수는 : 0 입니다.\n");
	printf("최종 분당 타자수는 : %f 입니다.\n", avg / correct);
}