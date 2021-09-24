#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ifsimbol.h"

// const tik šitam faile arba pvz. kokioje nors funkcijoje ar main
const int STEPSIZE = 200;
// unsigned char **loadfile(unsigned char *filename); galima taip ir tada fukcija gali būti apibrėžta žemiau main

// galima naudoti nustatant array ilgį pvz. int arr[ARRAY_L], bet negalima (const) int array_l = 20; int arr[array_l] 
// nes įvyksta anskčiau nei kodas užloadinamas
#define ARRAY_L 20 // global
// arr[STEPSIZE]; negalima
ar[ARRAY_L]; // galima

unsigned char **loadfile(unsigned char *filename)
{
	FILE *f = fopen(filename, "r");
	if(!f)
	{
		fprintf(stderr, "Cant open %s \n", filename);
		return NULL;
	}
	int arrlen = 0;
	unsigned char **lines = NULL;

	unsigned char buf[1000];
	int i = 0;
	
	while(fgets(buf, 1000, f))
	{
		if(i == arrlen)
		{
			arrlen += STEPSIZE;
			unsigned char **newlines = realloc(lines, arrlen * sizeof(unsigned char *));
			if(!newlines)
			{
				fprintf(stderr, "Cant reaalocate \n");
				exit(1);
			}
			lines = newlines;
		}
		
		unsigned char *tempstr;
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
		unsigned char *str = (unsigned char *)malloc((slen + 1) * sizeof(unsigned char));
		strcpy(str, tempstr);
		lines[i] = str;

		i++;
	}
	fclose(f);
	
	if(i == arrlen)
	{
		unsigned char **newarr = realloc(lines, (arrlen+1) * sizeof(unsigned char *));
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

struct StrLen
{
	int allstrlen; // :sizebit, taip galima nurodyti bit dydį, 1 - reikšmės tarp 0-1, 2 - reikšmės tarp 0-3 ir t.t.
	int realstrlen;
};

struct StrLen realstrlen(unsigned char *str)
{	
	struct StrLen astrl = {0};
	
	while(*str != '\0')
	{
		int ifs = ifsimbol(*str);
		if(ifs == 0)
		// if(*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*'
		// || *str == '(' || *str == ')' || *str == '-' || *str == '{' || *str == '}' || *str == '[' || *str == ']' || *str == ':'
		// || *str == ';' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == '.' || *str == '/' || *str == '?'
		// || *str == '~' || *str == '`' || *str == ',')
		// if(*str != ' ' || *str != '!') cia negerai, nes lyginimas vyksta 1 variablo , o ne pvz. 2, sita condition visada bus true
		// ir else niekada nesuveiks.
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
	unsigned char *begin = "Found word ";
	int lbegin = strlen(begin);
	
	unsigned char *end = " in: ";
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

int compares(unsigned char *str, unsigned char *copystr, unsigned char *findword, unsigned char *findwordcopy, unsigned char *op, int wl, int sl)
{	
	int samelet = 0;
	int strn = 0;
	unsigned char *fw = findword;
	unsigned char *temps = str;
	int stl = 0;
	int wce = 0;

	while(*str != '\0')
	{
		int ifs = ifsimbol(*str);
		if(ifs==0)
		// if(*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*'
		// || *str == '(' || *str == ')' || *str == '-' || *str == '{' || *str == '}' || *str == '[' || *str == ']' || *str == ':'
		// || *str == ';' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == '.' || *str == '/' || *str == '?'
		// || *str == '~' || *str == '`' || *str == ',')
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
				if(tolower(*str) == *fw)
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
			printf("Found word \"%s\" in: \"%s\"\n", findwordcopy, copystr);
		}
	}
	
	return wce;
}

void casefold(unsigned char *cfstr, unsigned long *Upletter, unsigned long *Lowletter, unsigned int realcasef)
{	
	for(int i = 0; cfstr[i] != '\0'; i++)
	{
		int ind;
		int indx = -1;
		unsigned long lowercl;
		
		int ifs = ifsimbol(cfstr[i]);
		if((ifs == 0))
		// if(cfstr[i] == '!' || cfstr[i] == '@' || cfstr[i] == '#' || cfstr[i] == '$' || cfstr[i] == '%' || cfstr[i] == '^' || cfstr[i] == '&' || cfstr[i] == '*'
		// || cfstr[i] == '(' || cfstr[i] == ')' || cfstr[i] == '-' || cfstr[i] == '{' || cfstr[i] == '}' || cfstr[i] == '[' || cfstr[i] == ']' || cfstr[i] == ':'
		// || cfstr[i] == ';' || cfstr[i] == '"' || cfstr[i] == ',' || cfstr[i] == '<' || cfstr[i] == '>' || cfstr[i] == '.' || cfstr[i] == '/' || cfstr[i] == '?'
		// || cfstr[i] == '~' || cfstr[i] == '`' || cfstr[i] == ' ' || cfstr[i] == '\'') // sugaloti kaip sutraukti į funkciją
		{
			if(cfstr[i] != ' ')
			{
				// šarasBatas čia nulūžta
				unsigned char b1 = cfstr[i] >> 5;
				unsigned char b2 = cfstr[i] >> 4;
				unsigned char b3 = cfstr[i] >> 3;
				if((b1 == 6))
				{
					ind = i;
				
					int wrb1 = cfstr[i] - 192;
					int wrb2 = cfstr[i+1] - 128;
					int result = (wrb1 << 6) | wrb2;
					
					for (int j = 0; j < realcasef; j++)
					{
						// galbūt geresnį algoritmą ieškojimui
						if(Upletter[j] == result)
						{
							indx = j;
							break;
						}
					}
					if(indx != -1)
					{
						lowercl = Lowletter[indx];
						unsigned char tempn1 = (char)(lowercl << 2);
						unsigned char tempn2 = (char)(tempn1 >> 2);
						unsigned char tempn3 = tempn2 + 128;
						cfstr[ind] = (char)((24 << 3) | (lowercl >> 6));
						cfstr[ind+1] = (char)tempn3;
					}
				}
				else
				{
					if(cfstr[i] <= 0x005A)
					{
						cfstr[i] += 32;
					}
				}
			}
		}
	}
	// printf("cfstr[i] %s\n", cfstr);
}

int main (int argc, unsigned char *argv[])
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
	
	unsigned char *filename = argv[1];
	int filenize = strlen(filename) - 1;
	
	unsigned char *word = argv[2];
	int wordlen = 0;

	unsigned char *wordcopy = NULL;
	for (int i = 0; word[i] != '\0'; i++)
	{
		wordlen++;
		wordcopy = realloc(wordcopy, wordlen * sizeof(unsigned char));
		wordcopy[i] = word[i];
		
		if(word[i+1] == '\0')
		{
			wordcopy[wordlen] = '\0';
		}
	}
			
	unsigned char **casefolding = loadfile("casefolding.txt");
	unsigned char prefix[]= "0x";

	unsigned long *Upletter = NULL;
	unsigned long *Lowletter = NULL;

	unsigned int arrsize = 0;

	unsigned int casef = 0;
	unsigned int realcasef = 0;

	while(casefolding[casef])
	{
		unsigned long casehex;
		unsigned char casechar1 = casefolding[casef][6];
		unsigned char casechar2 = casefolding[casef][7];
		if((casechar1 == 'F') || (casechar2 == 'F') || (casechar1 == 'S') || (casechar2 == 'S') || (casechar1 == 'T') || (casechar2 == 'T'))
		{
			// printf("Nepalaikomas simbolis\n");
		}
		else
		{
			char uphexstr[8];
			char lowhexstr[8];
			long hexnub;
			
			if(realcasef == arrsize)
			{
				arrsize += STEPSIZE;
				unsigned long *newlup = realloc(Upletter, arrsize * sizeof(unsigned long));
				unsigned long *newllow = realloc(Lowletter, arrsize * sizeof(unsigned long));
				
				if(!newlup || !newllow)
				{
					fprintf(stderr, "Cant reaalocate for Upper or Lower letter array\n");
					exit(1);
				}
				Upletter = newlup;
				Lowletter = newllow;
				
			}
		
			for (int j = 0; prefix[j] != '\0'; j++)
			{
				uphexstr[j] = prefix[j];
				lowhexstr[j] = prefix[j];
			};
			
			int upstrlen = 0;
			for(int j = 0; casefolding[casef][j] != ';'; j++)
			{
				uphexstr[j+2] = casefolding[casef][j];
				upstrlen++;
			}
			uphexstr[upstrlen+2] = '\0';
			casehex = strtol(uphexstr, NULL, 16);
			Upletter[realcasef] = casehex;
			
			int lowstrlen = upstrlen + 5;
			int lowstr = 2;
			
			for(int j = lowstrlen; casefolding[casef][j] != ';'; j++)
			{
				lowhexstr[lowstr] = casefolding[casef][j];
				lowstr++;
			}
			lowhexstr[lowstr] = '\0';
			casehex = strtol(lowhexstr, NULL, 16);
			Lowletter[realcasef] = casehex;

			realcasef++;
		}
		casef++;
	}

	casefold(word, Upletter, Lowletter, realcasef);

	if((filename[filenize] == 't') && (filename[filenize-1] == 'x') && (filename[filenize-2] == 't') && (filename[filenize-3] == '.'))
	{
		unsigned char **words = loadfile(filename);
		
		unsigned char op[2];
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
			unsigned char *wordscopy;
			int wordsl = strlen(words[i]) + 1;
			wordscopy = malloc(wordsl * sizeof(char));
			strcpy(wordscopy, words[i]);

			casefold(words[i], Upletter, Lowletter, realcasef);

			asl = realstrlen(words[i]);
			
			wc = compares(words[i], wordscopy, word, wordcopy, op, wordlen, asl.realstrlen);
			if(wc)
			{
				printsimbol(asl.allstrlen, wordlen);
			}
			notfound += wc;
		}
		
		if(!notfound)
		{
			unsigned char *nfw = "Not found word";
			int nfwlen = strlen(nfw);
			int alnfwlen = nfwlen + wordlen + 2 + 1;
			printf("%s \"%s\"\n", nfw, wordcopy);
			
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