#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	char buf[60]; 
	FILE* fp_r; 
	FILE* fp_w;

	fp_r = fopen("read.txt", "r");
	fp_w = fopen("write.txt", "w");

	if(!fp_r){
		perror("Cannot open file!");
		exit(EXIT_FAILURE);
	}

	if(!fp_w){
		perror("Cannot open file!");
		exit(EXIT_FAILURE);		
	}

	while(fgets(buf, 60, fp_r) != NULL){
		fputs(buf, fp_w);
	}

	fclose(fp_r);
	fclose(fp_w);

	return 0;

}