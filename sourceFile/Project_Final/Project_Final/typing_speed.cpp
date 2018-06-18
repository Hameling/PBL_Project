#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

#include "locale.h"
#include "typing_speed.h"

#pragma comment(lib, "winmm.lib")
#pragma warning (disable :4996)


double english_gettext(duration<double> total_time, char text[20])
{	
	double arr;

	arr = 60 / total_time.count() *  (double)strlen(text); //�ð����� �Է��� Ÿ�ڼ��� ����
	return arr;//�д� Ÿ�ڼ�
}

double korea_gettext(duration<double> total_time, char text[20])
{

	char str[20] = "";
	wchar_t w_str[20] = L"";
	wchar_t w_text[20] = L"";
	size_t cn;
	wchar_t buffer[100];
	double arr;
	int i;
	int cnt = 0;
	
	setlocale(LC_ALL, "Korean"); // �ѱ� �����ڵ�
	mbstowcs_s(&cn, w_str, 20, text, 20); // w_str�� str���� ����

	KoreanBreak(w_str, buffer, sizeof buffer); // �ѱ��� �ʼ� �߼� �������� ����
	setlocale(LC_ALL, "Korean");
	for (i = 0; (int)buffer[i] != NULL; i++)
	{
		cnt++; // ������� ī����
	}
	setlocale(LC_ALL, "Korean");

	for (i = 0; buffer[i] != NULL; i++) {// buffer �ʱ�ȭ
		buffer[i] = NULL;
	}
	
	arr = 60 / total_time.count() * (double)cnt; //�ð����� �Է��� Ÿ�ڼ��� ����
	return arr;//�д� Ÿ�ڼ�
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

