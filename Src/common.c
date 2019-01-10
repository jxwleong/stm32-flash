#include "common.h"

void stringCopy(char *destination, char *source){
	int i = 0;
	while(source[i] != '\0'){
		destination[i] = source[i];
		i++;
	}
	source[i] = '\0';
}
