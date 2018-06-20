#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include<wchar.h>

#include<chrono>

#include "locale.h"
#include "typing_speed.h"

#pragma comment(lib, "winmm.lib")
#pragma warning (disable :4996)



//char text[20] = { "안녕" }; // 이중연결리스트로 변경


double english_gettext(duration<double> total_time, char text[20], char target[20])
{	
	double arr;

	if (strcmp(target, text) == 0) {
		arr = 60 / total_time.count() *  (double)strlen(text); //시간에서 입력한 타자수를 곱함
		return arr;//분당 타자수
	}
	else {
		return 0.0;
	}

}

double korea_gettext(duration<double> total_time, char text[20], char target[20])
{

	char str[20] = "";
	wchar_t w_str[20] = L"";
	wchar_t w_text[20] = L"";
	size_t cn;
	wchar_t buffer[100];
	double arr;
	int i;
	int cnt = 0;
	
	setlocale(LC_ALL, "Korean"); // 한글 유니코드
	mbstowcs_s(&cn, w_str, 20, text, 20); // w_str에 str값을 복사

	//int strlen_str = strlen(str); // srt의 길이
	//int strlen_text = strlen(text); // text의 길이
	//if (strlen_str != strlen_text) return 0; // 둘이 비교후 다를경우 함수를 종료


	KoreanBreak(w_str, buffer, sizeof buffer); // 한글을 초성 중성 종성으로 나눔
	setlocale(LC_ALL, "Korean");
	for (i = 0; (int)buffer[i] != NULL; i++)
	{
		cnt++; // 몇글잔지 카운팅
	}
	setlocale(LC_ALL, "Korean");

	/*if (strlen_str != strlen_text) cnt = 0;

	setlocale(LC_ALL, "Korean");
	mbstowcs_s(&cn, w_text, 100, text, 100);
*/
	for (i = 0; buffer[i] != NULL; i++) {// buffer 초기화
		buffer[i] = NULL;
	}
	
	if (strcmp(target, text) == 0) {
		arr = 60 / total_time.count() * (double)cnt; //시간에서 입력한 타자수를 곱함
		return arr;//분당 타자수
	}
	else {
		return 0.0;
	}
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

	avg = korea_gettext();

	printf("최종 분당 타자수는 : %f 입니다.\n", avg);
}

void english_mode() // 영어 모드
{
	int i = 0;
	double avg = 0;
	avg = english_gettext();
	printf("최종 분당 타자수는 : %f 입니다.\n", avg);
}