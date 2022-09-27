#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define M 10

char* get_s(void){
	int n, k = 0;
	char* s = malloc(M);
	if (!s){
		free(s);
		return NULL;
	}
	while (fgets(s + k, M, stdin)){
		n = strlen(s);
		if(s[n - 1] != '\n'){
			k = k + M - 1;
			s = realloc(s, k + M);
			if (!s){
				free(s);
				return NULL;
			}
		}
		else {
			s[n - 1] = '\0';
			return s;
		}
	}
	free(s);
	return NULL;
}

char** sort(char** a, int i){
	short flag = 0;
	char* c;
	for (int j = 0; j < (i - 1); j++)
	{
		flag = 0;
		for (int l = 0; l < (i - 1 - j); l++)
		{
			if (strcmp(a[l], a[l + 1]) > 0)
			{
				c = a[l + 1];
				a[l + 1] = a[l];
				a[l] = c;
				flag = 1;
			}
		}
		if (flag == 0) return a;
	}
	return a;
}

void out(char** a, int i){
	for (int j = 0; j < i; j++){
		printf("%s\n", a[j]);
  		free(a[j]);
  	}
    free(a);
}

int main(){

	char** mas;
	int i = 0, k = 1;
	mas = (char**)malloc(sizeof(char*));
	
  	while(1){
		char* s = get_s();
		if (s == NULL){
			free(s);
			break;
		}
		mas[i] = s;
		i = i + 1;
		mas = realloc(mas, sizeof(char*) * k);	
		k = k + 1;
	}
	
	mas = sort(mas, i);
	
  	out(mas, i);
  	
	return 0;
	
}
