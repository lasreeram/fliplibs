#include <stdio.h>
union abc{ 
	int i; 
	char c[sizeof(int)]; 
	};
int main(){
	abc x; 
	x.i = 1; 
	if(x.c[0] == 1) 
		printf("little-endian\n"); 
	else 
		printf("big-endian\n"); 
}
