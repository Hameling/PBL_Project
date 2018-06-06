#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<thread>
#include"function.h"
using namespace std;



int main() {
	
	char title[10];
	strcpy(title, readTitle());

	printf("%s",title);

	return 0;
}