#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define M 10

char c;

char* get_s(void)
{
	int n, k = 0;
	char* s = malloc(M);
	if (!s) 
	{
		free(s);
		return NULL;
	}
	while (fgets(s + k, M, stdin))
	{
		n = strlen(s);
		if (s[n - 1] != '\n')
		{
			k = k + M - 1;
			s = realloc(s, k + M);
			c = getchar();
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
	free(s);
	return NULL;
}

char** sort(char** a, int i)
{
	short flag = 0;
	char* c;
	for (int j = 0; j < (i - 1); j++)
	{
		flag = 1;
		for (int l = 0; l < (i - 1); l++)
		{
			if ((strlen(a[l]) > strlen(a[l + 1])) || ((strcmp(a[l], a[l + 1]) > 0) && (strlen(a[l]) == strlen(a[l + 1])))) 
			{
				c = a[l + 1];
				a[l + 1] = a[l];
				a[l] = c;
				flag = 0;
			}
		}
		if (flag == 1) return a;
	}
	return a;
}

void out(char** a, int i)
{
	for (int j = 0; j < i; j++)
	{
		printf("%s\n", a[j]);
		free(a[j]);
	}
	free(a);
}

int main()
{
	int i = 0, k = 1;
	char** stringsArr;
	stringsArr = (char**)malloc(sizeof(char*));
	while (1)
	{
		char* s = get_s();
		if (s == NULL) 
		{
			free(s);
			break;
		}
		stringsArr[i] = s;
		k++;
		stringsArr = realloc(stringsArr, k * sizeof(char*));
		i++;
		if (c == EOF){
			free(s);
			break;
		}
	}
	stringsArr = sort(stringsArr, i);
	
	printf("\n");
	
	out(stringsArr, i);
	return 0;
}
