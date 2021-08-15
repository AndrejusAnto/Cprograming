#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const tik šitam faile arba pvz. kokioje nors funkcijoje ar main
const int STEPSIZE = 200;
char **loadfile(char *filename);

// galima naudoti nustatant array ilgį pvz. int arr[ARRAY_L], bet negalima (const) int array_l = 20; int arr[array_l] 
// nes įvyksta anskčiau nei kodas užloadinamas
#define ARRAY_L 20 // global
// arr[STEPSIZE]; negalima
ar[ARRAY_L]; // galima

struct StrLen
{
	int allstrlen; // :sizebit, taip galima nurodyti bit dydį, 1 - reikšmės tarp 0-1, 2 - reikšmės tarp 0-3 ir t.t.
	int realstrlen;
};

struct StrLen realstrlen(char *str)
{	
	struct StrLen astrl = {0};
	
	while(*str != '\0')
	{
		if(*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*'
		|| *str == '(' || *str == ')' || *str == '-' || *str == '{' || *str == '}' || *str == '[' || *str == ']' || *str == ':'
		|| *str == ';' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == '.' || *str == '/' || *str == '?'
		|| *str == '~' || *str == '`' || *str == ',')
		// if(*str != ' ' || *str != '!') cia negerai, nes lyginimas vyksta 1 variablo , o ne pvz. 2, sita condition visada bus true
		// ir else niekada nesuveiks.
		{
		}
		else
		{
			astrl.realstrlen++;			
		}
		astrl.allstrlen++;
		str++;
	}
	
	return astrl;
}

void printsimbol(int number, int wordl)
{
	char *begin = "Found word ";
	int lbegin = strlen(begin);
	
	char *end = " in: ";
	int lend = strlen(end);
	
	int alllength = lbegin + (wordl + 2) + lend + (number + 2);
	
	for (int i = 0; i < (alllength + 1); i++)
	{
		if(i != (alllength))
		{
			printf("-");
		}
		else
		{
			printf("\n");
		}
	}
}

int compares(char *str, char *findword, char *op, int wl, int sl)
{

	int samelet = 0;
	int strn = 0;
	char *fw = findword;
	char *temps = str;
	int stl = 0;
	int wce = 0;

	while(*str != '\0')
	{
		if(*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*'
		|| *str == '(' || *str == ')' || *str == '-' || *str == '{' || *str == '}' || *str == '[' || *str == ']' || *str == ':'
		|| *str == ';' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == '.' || *str == '/' || *str == '?'
		|| *str == '~' || *str == '`' || *str == ',')
		{
			
		}
		else
		{
			stl++;
			if(*str == ' ')
			{
				if((op[0] == 'e'))
				{
					if((samelet == wl)&&(samelet==strn))
					{
						wce++;
					}
				}
				samelet = 0;
				strn = 0;
				fw = findword;
			}
			else
			{
				if(*str == *fw)
				{
					samelet++;
					strn++;
					fw++;
					
					if(samelet == wl)
					{
						if( (op[0] == 'i') || ((op[0] == 'e') && (stl == sl)&&(samelet==strn)) )
						{
							wce++;
						}
					}
				}
				else
				{
					samelet = 0;
					fw = findword;
					strn++;
				}
			}				
		}	
		str++;
	}
	
	if(wce != 0)
	{
		for (int i = 0; i < wce; i++)
		{
			printf("Found word \"%s\" in: \"%s\"\n", findword, temps);
		}
	}
	
	return wce;
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
	
	char *filename = argv[1];
	int filenize = strlen(filename) - 1;

	if((filename[filenize] == 't') && (filename[filenize-1] == 'x') && (filename[filenize-2] == 't') && (filename[filenize-3] == '.'))
	{
		char **words = loadfile(argv[1]);
		int wordlen = 0;
		char *word = argv[2];
		
		for (int i = 0; word[i] != '\0'; i++)
		{
			wordlen++;
		}
		
		char op[2];
		int ops = 0;
		
		printf("E(xactly) or I(ncluded)? ");
		while(!ops)
		{
			fgets(op, 2, stdin);
			if(op[0] != '\n')
			{
				switch(op[0])
				{	
					case 'E':
					case 'e':
						op[0] = 'e';
						ops = 1;
						break;
					case 'I':
					case 'i':
						op[0] = 'i';
						ops = 1;
						break;
					default:
						printf("No, No, No, E(e) arba I(i) \n");
				}
			}
		}
		
		int notfound = 0;
		
		for (int i = 0; words[i] != NULL; i++)
		{
			int lnb = 0;
			int spnb = 0;
			int wc = 0;
			struct StrLen asl;
			
			asl = realstrlen(words[i]);
			
			wc = compares(words[i], word, op, wordlen, asl.realstrlen);
			if(wc)
			{
				printsimbol(asl.allstrlen, wordlen);
			}
			notfound += wc;
		}
		
		if(!notfound)
		{
			char *nfw = "Not found word";
			int nfwlen = strlen(nfw);
			int alnfwlen = nfwlen + wordlen + 2 + 1;
			printf("%s \"%s\"\n", nfw, word);
			
			for (int i = 0; i < (alnfwlen + 1); i++)
			{
				if(i != alnfwlen)
				{
					printf("-");
				}
				else
				{
					printf("\n");
				}
			}
		}
	}
	else
	{
		printf("No, no, no, blogas failas\n");
	}
	
	printf("Baigta");
	return 0;
}

char **loadfile(char *filename)
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
		
		char *tempstr;
		int ifspace = 0;
		for(int j = 0; buf[j] == ' '; j++)
		{
			tempstr = buf + (j+1);
			ifspace++;
		}
		
		if(ifspace == 0)
		{
			// buf = tempstr; negalima you can't assign arrays. You'd have to copy
			tempstr = buf;
		}
		
		for(int k = 0; tempstr[k] != '\0'; k++)
		{
			if(tempstr[k] == '\n')
			{
				tempstr[k] = '\0';
			}
		}
		
		int slen = strlen(tempstr);	
		char *str = (char *)malloc((slen + 1) * sizeof(char));
		strcpy(str, tempstr);
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