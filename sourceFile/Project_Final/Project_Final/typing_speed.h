#pragma once
#ifndef TYPING_SPEED_H
#define TYPING_SPEED_H

#include<wchar.h>

#include<chrono>
#define MAX 512 //문자열 길이를 정의

using namespace std;
using namespace chrono;

static system_clock::time_point start;
static system_clock::time_point stop;
static duration<double> total_time;

double english_gettext(duration<double> total_time, char text[20]);
double korea_gettext(duration<double> total_time, char text[20]);
int KoreanBreak(wchar_t *str, wchar_t *buffer, unsigned int nSize);

#endif