#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STEPSIZE = 200;
char ** loadfile(char *filename);

void findfirstletter(char *str, char *findword, char *arr[100], int *nb)
{	
	int howmany = 0;

	while (*str != '\0')
	{
		if(*str == *findword)
		{
			arr[howmany] = str;
			howmany++;
		}
		str++;
		
	}
	*nb = howmany;
	
}

int comparestr(char *str, char *findword, char *op)
{
	int wordlen = 0;
	for (int i = 0; findword[i] != '\0'; i++)
	{
		wordlen++;
		
	}
	
	int sameletters = 0;
	while (*findword != '\0')
	{
		if(*findword == *str)
		{
			findword++;
			str++;
			sameletters++;
		}
		else
		{
			break;
		}
	}
	
	
	if((wordlen ==  sameletters))
	{
		if(op[0] == 'i')
		{
			return 0;
		}
		else if(op[0] == 'e')
		{
			if((*str == ' ') || (*str == '\0'))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{	
			return 1;
		}
	}
	// nežianu, kuris variantas geriau, pirmas trumpesnis ir be kodo kartojimosi, bet sunkiau suprantamas
	// antrasis su kodo kartojimusi, bet lengviau suprantamas
	// if(op[0] == 'i')
	// {
		// if((wordlen ==  sameletters))
		// {
			// return 0;
		// }
		// else
		// {	
			// return 1;
		// }
	// }
	
	// if(op[0] == 'e')
	// {
		// if( ((wordlen ==  sameletters)) && ((*str == ' ') || (*str == '\0')))
		// {
			// return 0;
		// }
		// else
		// {	
			// return 1;
		// }
	// }
}

int main (int argc, char *argv[])
{
	if(argc == 1)
	{
		fprintf(stderr, "Supply file name and word to find \n");
		exit(1);
	}
	else if(argc == 2)
	{
		printf("Supply file name or word to find \n");
		exit(1);
	}
	
	char **words = loadfile(argv[1]);
	char *word = argv[2];
	char op[2];
	int ifgoodop = 0;
	
	printf("E(xactly) or I(ncluded)? ");
	while(!ifgoodop)
	{
		fgets(op, 2, stdin);
		if(op[0] != '\n')
		{
			switch(op[0])
			{	
				case 'E':
				case 'e':
					op[0] = 'e';
					ifgoodop = 1;
					break;
				case 'I':
				case 'i':
					op[0] = 'i';
					ifgoodop = 1;
					break;
				default:
					printf("No, No, No, E(e) arba I(i) ");
			}
		}
	}
	
	
	for (int i = 0; words[i] != NULL; i++)
	{
		// padaryti su malloc
		char *allletters[100];
		int lnb = 0;
		
		findfirstletter(words[i], word, allletters, &lnb);
		
		for (int j = 0; j < lnb; j++)
		{
			int iffound = comparestr(allletters[j], word, op);
			if(iffound == 0)
			{
				printf("Found word \"%s\" in %s\n", word, words[i]);
			}
		}		
	}
	
	printf("Baigta");
	return 0;
}

char ** loadfile(char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f)
	{
		fprintf(stderr, "Cant open %s \n", filename);
		return NULL;
	}
	int arrlen = 0;
	char **lines = NULL;

	char buf[1000];
	int i = 0;
	while(fgets(buf, 1000, f))
	{
		if(i == arrlen)
		{
			arrlen += STEPSIZE;
			char **newlines = realloc(lines, arrlen * sizeof(char *));
			if(!newlines)
			{
				fprintf(stderr, "Cant reaalocate \n");
				exit(1);
			}
			lines = newlines;
		}
		
		for(int j = 0; buf[j] != '\0'; j++)
		{
			if(buf[j] == '\n')
			{
				buf[j] = '\0';
			}
		}
		
		int slen = strlen(buf);
		char *str = (char *)malloc((slen + 1) * sizeof(char));
		strcpy(str, buf);
		lines[i] = str;

		i++;
	}
	fclose(f);
	
	if(i == arrlen)
	{
		char **newarr = realloc(lines, (arrlen+1) * sizeof(char *));
		if(!newarr)
		{
			printf("Blogai");
			exit(1);
		}
		lines = newarr;
	}

	lines[i] = NULL;
	return lines;
}