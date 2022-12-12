#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
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

char *** separation(char **mas_of_strings, int count, int count_of_mas, int *change_in, int *change_out){
	char ***mas_of_mas = malloc(count_of_mas * sizeof(char **)), **tmp_mas = malloc(sizeof(char *));
	int tmp_count = 0, flag = 1, number_of_mas = 1;
	for (int i = 0; i < count; i++){
		if (strcmp(mas_of_strings[i], "<") == 0){
			flag = 0;
			int file = open(mas_of_strings[i + 1], O_RDONLY, 0644);
			i++;
			*change_in = 1;
			dup2(file, 0);
			close(file);
		} else if (strcmp(mas_of_strings[i], ">") == 0){
			flag = 0;
			int file = open(mas_of_strings[i + 1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
			i++;
			*change_out = 1;
			dup2(file, 1);
			close(file);
		} else if (strcmp(mas_of_strings[i], ">>") == 0){
			flag = 0;
			int file = open(mas_of_strings[i + 1], O_RDWR|O_CREAT|O_APPEND, 0644);
			i++;
			*change_out = 1;
			dup2(file, 1);
			close(file);
		} else {
			tmp_count++;
			tmp_mas = realloc(tmp_mas, (tmp_count + 1) * sizeof(char *));
			if (mas_of_strings[i] == "|"){
				number_of_mas = number_of_mas + 1;
				flag = 1;
				tmp_mas[tmp_count - 1] = NULL;
				mas_of_mas[number_of_mas - 2] = tmp_mas;
				tmp_mas = malloc(sizeof(char *));
				tmp_count = 0;
			} else if (flag == 1){
				tmp_mas[tmp_count - 1] = malloc(strlen(mas_of_strings[i]) + 1);
				for (int j = 0; j < strlen(mas_of_strings[i]); j++)
					tmp_mas[tmp_count - 1][j] = mas_of_strings[i][j];
				tmp_mas[tmp_count - 1][strlen(mas_of_strings[i])] = '\0';
			}
		}	
	}
	tmp_mas[tmp_count] = NULL;
	mas_of_mas[number_of_mas - 1] = tmp_mas;
	tmp_mas = NULL;
	return mas_of_mas;
}

int conveyor(char *** mas_of_mas, int count_of_mas, int conveyor_type, int save_in, int save_out, int change_in, int change_out)
{
	int fd[2], i = 0;
	int tmp = dup(0), status = 0, result = 0;
	if (conveyor_type == 1){
		printf("CONVEER\n");
		if (change_in == 0){
			printf("VVOD NE IZMENEN\n");
			int file = open("/dev/null", O_RDWR, 0644);
			dup2(file, 0);
			close(file);
		}
		if (change_out == 0){
			printf("VIIVOD NE IZMENEN\n");
			int file = open("/dev/null", O_RDWR, 0644);
			dup2(file, 1);
			close(file);
		}
	}
	for(i = 0; i < count_of_mas; i++)
	{
		pipe(fd);
		switch(fork()){
			case -1: {
				printf("The process can not be created!\n");
				close(fd[0]);
				close(fd[1]);
				break;
			}
			case 0: {
				if(i + 1 != count_of_mas) dup2(fd[1], 1);
					close(fd[0]);
					close(fd[1]);
					execvp(mas_of_mas[i][0], mas_of_mas[i]);
					printf("Error!\n");
					exit(1);
			}
			default: {
				dup2(fd[0], 0);
				close(fd[0]);
				close(fd[1]);
			}
		}
	}
	dup2(tmp, 0);
	close(tmp);
	while(wait(&status) != -1)
	{
		if (status != 0)
			result = 1;
	}
	return result;
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
	char *s = NULL, ***mas_of_mas = NULL, **mas_of_signs = NULL;
	int save_in = dup(0), save_out = dup(1), change_in = 0, change_out = 0, go = 0, status = -1, count_of_signs = 0;
	while ((s != NULL) || (s = get_s(fl))){
		int flag_found_word = 0, flag_found_space = 0, count = 0, count_of_mas = 1, conveyor_type = 0, flag_skip = 0, flag_exit = 0;
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
				case '&': if (cpy == NULL) cpy = "&"; case '|': {if (cpy == NULL) cpy = "|"; count_of_mas++;}
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
						if (cpy == "&"){
							cpy = "&&";
							flag_skip = 1;
							count_of_signs++;
							if (mas_of_signs == NULL)
								mas_of_signs = malloc(sizeof(char *));
							else
								mas_of_signs = realloc(mas_of_signs, count_of_signs * sizeof(char *));
							mas_of_signs[count_of_signs - 1] = "&&";
						}
						else
							if (cpy == "|"){
								cpy = "||";
								flag_skip = 1;
								count_of_signs++;
								if (mas_of_signs == NULL)
									mas_of_signs = malloc(sizeof(char *));
								else
									mas_of_signs = realloc(mas_of_signs, count_of_signs * sizeof(char *));
								mas_of_signs[count_of_signs - 1] = "||";
							}
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
			if ((s[i] == ';') || ((cpy != NULL) && ((strlen(cpy) == 2) && ((cpy[1] == '|') || (cpy[1] == '&'))))){
				flag_skip = 1;
				if (s[i] == ';'){
					count_of_signs++;
					if (mas_of_signs == NULL)
						mas_of_signs = malloc(sizeof(char *));
					else
						mas_of_signs = realloc(mas_of_signs, count_of_signs * sizeof(char *));
					mas_of_signs[count_of_signs - 1] = ";";
				} else
					count_of_mas -= 1;
				char * raznica = str + i + 1;
				if (strlen(s) < (raznica + 2 - s)){
					printf("Meme\n");
					s = NULL;
					status = 0;
				}
				else
					s = raznica;
				count--;
				break;
			}
		}

		/*for (int i = 0; i < count; i++){
			if ((mas_of_strings[i][0] != '&') && (mas_of_strings[i][0] != '|') && (mas_of_strings[i][0] != '>') &&
				(mas_of_strings[i][0] != ';') && (mas_of_strings[i][0] != '(') && (mas_of_strings[i][0] != ')') && (mas_of_strings[i][0] != '<'))
					printf("%s ", mas_of_strings[i]);
		}
		printf("\n");*/
		if (strcmp(mas_of_strings[count - 1], "&") == 0){
			count_of_mas -= 1;
			mas_of_strings[count - 1] = NULL;
			count -= 1;
			conveyor_type = 1;
		}
		
		mas_of_mas = separation(mas_of_strings, count, count_of_mas, &change_in, &change_out);
		
		if (count_of_signs)
			if (((strcmp(mas_of_signs[go], "||") == 0) && (status == 0)) || ((strcmp(mas_of_signs[go], "&&") == 0) && (status == 1))){
				flag_exit = 1;
				while((go != count_of_signs - 1) && (strcmp(mas_of_signs[go], ";") != 0))
					go++;
				if(strstr(s, ";") != NULL){
					s = strstr(s, ";");
					if (strlen(s) > 1)
						s++;
				} else {
					for (int i = 0; i < count_of_signs; i++)
						mas_of_signs[i] = NULL;
					count_of_signs = 0;
					s = NULL;
				}
			} else {
				if (go != count_of_signs - 1)
					go++;
			}
		if (!flag_exit){
			status = conveyor(mas_of_mas, count_of_mas, conveyor_type, save_in, save_out, change_in, change_out);
			//printf("Status: %d\n", status);
		}
				
		dup2(save_in,  0);
		dup2(save_out, 1);
		
		/*for (int i = 0; i < count; i++){
			if ((mas_of_strings[i][0] != '&') && (mas_of_strings[i][0] != '|') && (mas_of_strings[i][0] != '>') &&
				(mas_of_strings[i][0] != ';') && (mas_of_strings[i][0] != '(') && (mas_of_strings[i][0] != ')') && (mas_of_strings[i][0] != '<'))
					free(mas_of_strings[i]);
		}
		free(mas_of_strings);
		for (int i = 0; i < count_of_mas; i++)
			free(mas_of_mas[i]);
		free(mas_of_mas);*/ /////////////
		
		/*for (int i = 0; i < count_of_mas; i++){
			int j = 0;
			while(mas_of_mas[i][j] != NULL){
				free(mas_of_mas[i][j]);
				j++;
			}
			free(mas_of_mas[i]);
		}
		free(mas_of_mas);*/
		//free(s);/////////////
		/*for (int i = 0; i < count_of_signs; i++)
			printf("%s ", mas_of_signs[i]);
		printf("\n");*/
		if (!flag_skip){
			for (int i = 0; i < count_of_signs; i++)
				mas_of_signs[i] = NULL;
			s = NULL;
			count_of_signs = 0;
		}
	}
	//free(s);//////////////
	if (status == -1)
		status = 0;
	if (fl != NULL)
		fclose(fl);
	printf("FINAL STATUS: %d\n", status);
	return 0;
}
