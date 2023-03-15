#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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

void my_shell_command(char **mas_of_strings){
	if (strcmp(mas_of_strings[0], "cd") == 0){
		if ((mas_of_strings[1] != NULL) && (mas_of_strings[2] == NULL)){
			if (chdir(mas_of_strings[1]) == -1)
				printf("There is no such directory!\n");
		} else if (mas_of_strings[1] == NULL)
			chdir(getenv("HOME"));
		else
			printf("There are more than 1 arguments for cd!\n");
	} else {
		if (fork() == 0){
			execvp(mas_of_strings[0], mas_of_strings);
			printf("there is no such file!\n");
			exit(1);
		}
		wait(NULL);
	}
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
		int flag_found_word = 0, flag_found_space = 0, count = 0;
		long int rem = 0, rem2 = 0;
		char *str = NULL, c;
		str = s;
		s[strlen(s)] = ' ';
		char **mas_of_strings = malloc(sizeof(char*));
		mas_of_strings[count] = NULL;
		for (int i = 0; i < strlen(s); i++){
			char *cpy = NULL;
			switch (s[i]){
				case ' ': {
					if ((flag_found_word == 1) && (flag_found_space == 0)){
						c = s[i];
						s[i] = '\0';
						if (mas_of_strings[count] == NULL){
							mas_of_strings[count] = malloc((s + i + 1 - str) * sizeof(char));
							strcpy(mas_of_strings[count], str);
						} else {
							rem2 = strlen(mas_of_strings[count]);
							mas_of_strings[count] = realloc(mas_of_strings[count], strlen(mas_of_strings[count]) + i + s - str + 1);
							strcpy(mas_of_strings[count] + rem2, str);
						}
						count++;
						mas_of_strings = realloc(mas_of_strings, (count + 1) * sizeof(char *));
						mas_of_strings[count] = NULL;
						s[i] = c;
						flag_found_word = 0;
					}
					break;
				}
				case '&': if (cpy == NULL) cpy = "&"; case '|': if (cpy == NULL) cpy = "|";
				case '>': if (cpy == NULL) cpy = ">"; case ';': if (cpy == NULL) cpy = ";";
				case '<': if (cpy == NULL) cpy = "<"; case '(': if (cpy == NULL) cpy = "(";
				case ')': if (cpy == NULL) cpy = ")"; {
					if (mas_of_strings[count] != NULL){
						if (flag_found_word == 1){
							c = s[i];
							s[i] = '\0';
							rem2 = strlen(mas_of_strings[count]);
							mas_of_strings[count] = realloc(mas_of_strings[count], strlen(mas_of_strings[count]) + i + s - str + 1);
							strcpy(mas_of_strings[count] + rem2, str);
							s[i] = c;
							flag_found_word = 0;
						}
						count++;
						mas_of_strings = realloc(mas_of_strings, (count + 1) * sizeof(char *));
						mas_of_strings[count] = NULL;
					}
					if (s[i] == s[i + 1])
						if (cpy == "&")
							cpy = "&&";
						else
							if (cpy == "|")
								cpy = "||";
							else if (cpy == ">")
								cpy = ">>";
					if (flag_found_word == 1){
						c = s[i];
						s[i] = '\0';
						if (mas_of_strings[count] == NULL){
							mas_of_strings[count] = malloc((s + i + 1 - str) * sizeof(char)); 
							strcpy(mas_of_strings[count], str);
						} else
							strcpy(mas_of_strings[count] + strlen(mas_of_strings[count]), str);
						if (flag_found_space == 1){
							flag_found_space = 0;
							rem = 0;
						}
						count++;
						mas_of_strings = realloc(mas_of_strings, (count + 1) * sizeof(char *));
						mas_of_strings[count] = NULL;
						s[i] = c;
						flag_found_word = 0;
					}
					i += strlen(cpy) - 1;
					mas_of_strings[count] = cpy;
					count++;
					mas_of_strings = realloc(mas_of_strings, (count + 1) * sizeof(char *));
					mas_of_strings[count] = NULL;
					break;
				}
				case '"':{
					if (flag_found_word == 1){
						c = s[i];
						s[i] = '\0';
						if (mas_of_strings[count] == NULL){
							rem = s + i - str;
							mas_of_strings[count] = malloc((s + i + 1 - str) * sizeof(char));
							strcpy(mas_of_strings[count], str);
						} else {
							rem = rem + s + i - str;
							mas_of_strings[count] = realloc(mas_of_strings[count], strlen(mas_of_strings[count]) + rem);
							strcpy(mas_of_strings[count] + strlen(mas_of_strings[count]), str);
						}
						s[i] = c;
						flag_found_word = 0;
					}
					if (flag_found_space == 0)
						if ((i == 0) ||(i > 0) && ((s[i - 1] == ' ') || (s[i - 1] == '>') || (s[i - 1] == '<')
						   || (s[i - 1] == ')') || (s[i - 1] == '(') || (s[i - 1] == ';') || (s[i - 1] == '&') || (s[i - 1] == '|'))){
							flag_found_space = 1;
							break;
						}
					if (flag_found_space == 1)
						if ((i == strlen(s) - 1) || (i < strlen(s) - 1) && ((s[i + 1] == ' ') || (s[i + 1] == '>') || (s[i + 1] == '<')
							|| (s[i + 1] == ')') || (s[i + 1] == '(') || (s[i + 1] == ';') || (s[i + 1] == '&') || (s[i + 1] == '|'))) {
							flag_found_space = 0;
							count++;
							mas_of_strings = realloc(mas_of_strings, (count + 1) * sizeof(char *));
							mas_of_strings[count] = NULL;
							rem = 0;
						}
					break;
				}
				default:
					if (flag_found_word == 0){
						str = s + i;
						flag_found_word = 1;
					}
			}
		}
		//for (int i = 0; i < count; i++)
		//	printf("%s\n", mas_of_strings[i]);
		my_shell_command(mas_of_strings);
		for (int i = 0; i < count; i++){
			if ((mas_of_strings[i][0] != '&') && (mas_of_strings[i][0] != '|') && (mas_of_strings[i][0] != '>') &&
				(mas_of_strings[i][0] != ';') && (mas_of_strings[i][0] != '(') && (mas_of_strings[i][0] != ')') && (mas_of_strings[i][0] != '<'))
					free(mas_of_strings[i]);
		}
		free(mas_of_strings);
		free(s);
	}
	free(s);
	if (fl != NULL)
		fclose(fl);
	return 0;
}
