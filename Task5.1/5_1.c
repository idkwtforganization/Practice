#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10

char* get_s(FILE *fl)
{
	int n = 0, k = 0;
	char* s = malloc(M);
	if (!s) 
	{
		free(s);
		return NULL;
	}
	if (fl == NULL)
		fl = stdin;
	while (fgets(s + k, M, fl))
	{
		n = strlen(s);
		if (s[n - 1] != '\n')
		{
			k = k + M - 1;
			s = realloc(s, k + M);
			if (!s) 
			{
				free(s);
				return NULL;
			}
		}
		else
		{
			s[n-1]='\0';
			return s;
		}
	}
	if (n)
		printf("\n");
	free(s);
	return NULL;
}

int main(int argc, char *argv[]){
	if (argc > 2){
		printf("There are more than 1 arguments in terminal!\n");
		return 0;
	}
	FILE *fl = NULL;
	if (argc == 2){
		fl = fopen(argv[1], "r");
		if (fl == NULL){
			printf("There is no such file in directory\n");
			return 0;
		}
	}
	char *s = NULL;
	while (s = get_s(fl)){
		int flag = 0, flag2 = 0, count = 0;
		long int rem = 0, rem2 = 0;
		char *str = NULL, c;
		str = s;
		s[strlen(s)] = ' ';
		char **mas = malloc(sizeof(char*));
		mas[count] = NULL;
		for (int i = 0; i < strlen(s); i++){
			char *cpy = NULL;
			switch (s[i]){
				case ' ': {
					if ((flag == 1) && (flag2 == 0)){
						c = s[i];
						s[i] = '\0';
						if (mas[count] == NULL){
							mas[count] = malloc((s + i + 1 - str) * sizeof(char));
							strcpy(mas[count], str);
						} else {
							rem2 = strlen(mas[count]);
							mas[count] = realloc(mas[count], strlen(mas[count]) + i + s - str + 1);
							strcpy(mas[count] + rem2, str);
						}
						count++;
						mas = realloc(mas, (count + 1) * sizeof(char *));
						mas[count] = NULL;
						s[i] = c;
						flag = 0;
					}
					break;
				}
				case '&': if (cpy == NULL) cpy = "&"; case '|': if (cpy == NULL) cpy = "|"; case '>': if (cpy == NULL) cpy = ">"; case ';': if (cpy == NULL) cpy = ";"; case '<': if (cpy == NULL) cpy = "<"; case '(': if (cpy == NULL) cpy = "("; case ')': if (cpy == NULL) cpy = ")"; {
					if (mas[count] != NULL){
						if (flag == 1){
							c = s[i];
							s[i] = '\0';
							rem2 = strlen(mas[count]);
							mas[count] = realloc(mas[count], strlen(mas[count]) + i + s - str + 1);
							strcpy(mas[count] + rem2, str);
							s[i] = c;
							flag = 0;
						}
						count++;
						mas = realloc(mas, (count + 1) * sizeof(char *));
						mas[count] = NULL;
					}
					if (s[i] == s[i + 1])
						if (cpy == "&")
							cpy = "&&";
						else
							if (cpy == "|")
								cpy = "||";
							else if (cpy == ">")
								cpy = ">>";
					if (flag == 1){
						c = s[i];
						s[i] = '\0';
						if (mas[count] == NULL){
							mas[count] = malloc((s + i + 1 - str) * sizeof(char)); 
							strcpy(mas[count], str);
						} else
							strcpy(mas[count] + strlen(mas[count]), str);
						if (flag2 == 1){
							flag2 = 0;
							rem = 0;
						}
						count++;
						mas = realloc(mas, (count + 1) * sizeof(char *));
						mas[count] = NULL;
						s[i] = c;
						flag = 0;
					}
					i += strlen(cpy) - 1;
					mas[count] = cpy;
					count++;
					mas = realloc(mas, (count + 1) * sizeof(char *));
					mas[count] = NULL;
					break;
				}
				case '"':{
					if (flag == 1){
						c = s[i];
						s[i] = '\0';
						if (mas[count] == NULL){
							rem = s + i - str;
							mas[count] = malloc((s + i + 1 - str) * sizeof(char));
							strcpy(mas[count], str);
						} else {
							rem = rem + s + i - str;
							mas[count] = realloc(mas[count], strlen(mas[count]) + rem);
							strcpy(mas[count] + strlen(mas[count]), str);
						}
						s[i] = c;
						flag = 0;
					}
					if (flag2 == 0)
						if ((i == 0) ||(i > 0) && ((s[i - 1] == ' ') || (s[i - 1] == '>') || (s[i - 1] == '<') || (s[i - 1] == ')') || (s[i - 1] == '(') || (s[i - 1] == ';') || (s[i - 1] == '&') || (s[i - 1] == '|'))){
							flag2 = 1;
							break;
						}
					if (flag2 == 1)
						if ((i == strlen(s) - 1) || (i < strlen(s) - 1) && ((s[i + 1] == ' ') || (s[i + 1] == '>') || (s[i + 1] == '<') || (s[i + 1] == ')') || (s[i + 1] == '(') || (s[i + 1] == ';') || (s[i + 1] == '&') || (s[i + 1] == '|'))) {
							flag2 = 0;
							count++;
							mas = realloc(mas, (count + 1) * sizeof(char *));
							mas[count] = NULL;
							rem = 0;
						}
					break;
				}
				default:
					if (flag == 0){
						str = s + i;
						flag = 1;
					}
			}
		}
		for (int i = 0; i < count; i++){
			printf("%s\n", mas[i]);
			if ((mas[i][0] != '&') && (mas[i][0] != '|') && (mas[i][0] != '>') && (mas[i][0] != ';') && (mas[i][0] != '(') && (mas[i][0] != ')'))
				free(mas[i]);
		}
		free(mas);
		free(s);
	}
	free(s);
	if (fl != NULL)
		fclose(fl);
	return 0;
}
