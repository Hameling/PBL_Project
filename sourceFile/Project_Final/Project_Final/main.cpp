#include<signal.h>
#include<time.h>
#include<thread>
#include"function.h"
using namespace std;



int main() {

	char title[20];

	strcpy(title, readTitle());

	printf("%s\n",title);

	readCustom(title);

	return 0;
}