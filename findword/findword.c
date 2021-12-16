#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// import'uojas failas, kuris turi funkciją, kuri patikrina ar ženklas yra tam tikras simbolis
#include "ifsimbol.h"

// #include <locale.h> dėl šito kažkodėl dažnai scirpt'tas nulužta

// https://www.py4u.net/discuss/86119
#include <windows.h>
// #define MAX_INPUT_LENGTH 255

// In the Windows API (with some exceptions discussed in the following paragraphs), 
// the maximum length for a path is MAX_PATH, which is defined as 260 characters
#define MAX_FILE_PATH 260

#define MAX_WORD_LENGTH 50
#define STRBUFFERSIZE 1024
// nekeičiamas skaičius, kuris nauodojamas keičiant nustatytos atminties dydį  

#define true 1
#define false 0

const int STEPSIZE = 200;

/* pagal https://www.youtube.com/watch?v=5wzmEKjNqiU, funkcija, kuri užload'ina tekstinį failą ir jo turinį
perkopijuoja į atmintį
*/
unsigned char **loadfile(unsigned char *filename)
{
	// atidarom failą (.txt)
	FILE *f = fopen(filename, "r");
	
	// patikrinam ar failas buvo atidarytas, jei ne atitinkamos info atvaizdavimas
	if(!f)
	{
		printf("Cant open %s \n", filename);
		exit(1);
		// return NULL;
	}
	// masyvymo kūrimo kintamasis, kuris keičiamas per STEPSIZE
	int arrlen = 0;
	
	// čia inicijuojamas pointer'is, kuris saugos pointer'ių masyvą t.y. kuris saugos tekstinio (char) tipo informaciją
	unsigned char **lines = NULL;

	/* laikina atminties vieta, kuri bus naudojama laikinai saugoti duomenis, perkopijuous iš failo
	ir kurios dydis 1000 vienetų (unsigned char) */ 
	unsigned char buf[STRBUFFERSIZE];
	
	// kintamasis, naudojamas kaip indeksas
	int i = 0;
	
	/* su while ciklu čia naudojam fgets, kuri perkopijuoja duomenis iš failo, atskirtus naujos eilutės ženklu ir tada
	duomenys perkeliami į sukurtą pointer'ių masyvą 'lines' */ 
	while(fgets(buf, STRBUFFERSIZE, f))
	{
		// patikrinam ar i(indeksas) yra lygu arrlen
		if(i == arrlen)
		{
			// padidinam arrlen dydžiu STEPSIZE t.y kai arrlen yra 0, gaunam 200, kai 200, tada 400 ir t.t.
			arrlen += STEPSIZE;
			
			// ir su realloc padidinam pointer'ių masyvo dydį, su malloc taip negalima 
			unsigned char **newlines = realloc(lines, arrlen * sizeof(unsigned char *));
			
			// patikrinam ar pointer'ių masyvo dydinimas gavosi, jei ne atitinkamos info atvaizdavimas
			if(!newlines)
			{
				fprintf(stderr, "Cant reaalocate \n");
				exit(1);
			}
			// ir priskiriam "newlines" pointer'ių masyvo pointer'į "lines" pointerių masyvo pointer'iui
			lines = newlines;
		}
		// MANO KODAS
		/* inicijuojam kintamąjį, kuris bus naudojamas panaikinti tarpą (space) string'o pradžioje t.y.
		tikrinam ar ženklas " ", jei taip, tada perkeliam pointer'į per vieną atminties vietą ir jei ženklas
		nebe " ", padidinam "ifspace", taip turėsim kaip ir žymeklį, kad " " buvo ir kad reikia pakeisti pointer'io 
		atminties adresą*/
		unsigned char *tempstr;
		int ifspace = 0;
		for(int j = 0; buf[j] == ' '; j++)
		{
			tempstr = buf + (j+1);
			ifspace++;
		}
		// tada naują pointer'io atminties adresą priskiriam tempstr, kur jau nebėra " "
		if(ifspace == 0)
		{
			// buf = tempstr; negalima you can't assign arrays. You'd have to copy
			tempstr = buf;
		}
		
		// "for" ciklo pagalba pakeičiam naujos eilutės ženklu "\n" į '\0', kuris C reiškia string'o pabaigą
		for(int k = 0; tempstr[k] != '\0'; k++)
		{
			if(tempstr[k] == '\n')
			{
				tempstr[k] = '\0';
			}
		}
		// MANO KODAS

		// c funkcija "strlen" nustatom tempstr ženklų skaičių
		int slen = strlen(tempstr);	
		
		// rezevuojam atmintį string'ui iš failo, (slen + 1), nes reikia papildomos vietos '\0' ženklui
		unsigned char *str = (unsigned char *)malloc((slen + 1) * sizeof(unsigned char));
		
		// su c funkcija "strcpy" nukopijuojam iš tempstr į str
		strcpy(str, tempstr);
		
		// ir tada priskiriam str sukurtam char tipo pointer'ių masyvui, naudodami i kaip indeksą
		lines[i] = str;
		// printf("lines[i] %s\n", lines[i]);

		// padidinam i per 1
		i++;
	}
	// būtinai uždarom failą
	fclose(f);
	
	/* kaip string'ų atveju, kad galima naudoti '\0' kaip "for" ciklo nutraukimo ženklą, taip ir su arrays, 
	jei gaunam, kad indeksas yra lygus arrlen, galima padidinti per 1 ir jį nustatyti, kaip NULL ir tada
	"for (int i = 0; array[i] != NULL; i++)"
	*/ 
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

// c duomenų struktūra saugoti įvarius duomenų tipus ir pan. Šiuo atveju tai yra string'o ženklų skaičius
struct StrLen
{
	// visų string'o ženklų skaičius 
	int allstrlen; // :sizebit, taip galima nurodyti bit dydį, 1 - reikšmės tarp 0-1, 2 - reikšmės tarp 0-3 ir t.t.
	
	// string'o tik raidžių ženklų skaičius 
	int realstrlen;
};

// funkcija string'o ženklų skaičių
struct StrLen strleninfo(unsigned char *str)
{	
	// inicijuojam struct'ą
	struct StrLen astrl = {0};
	
	// su while ciklu ir naudodami "dereferencing a pointer == *str" gaunam string'o ženklą
	while(*str != '\0')
	{
		// patikrinam ar ženklas yra simbolis
		int ifs = ifsimbol(*str);

		// jei nėra simbolis
		if(ifs == 0)
		{
			// padidinam realstrlen per 1
			astrl.realstrlen++;
		}

		// padidinam astrl.allstrlen per 1
		astrl.allstrlen++;

		/* ir pastummiam pointer'io atminties vietą per 1 vieną (šiuo atveju char) t.y. jei string'as 
		"namas", tai jei buvo pradžia ties "n", dabar jau yra ties "a" */
		str++;
	}
	
	// grąžinam astrl struct'ą tolimensiam naudojimui
	return astrl;
}

/* funcija, kuri atprint'tina "_"
Found word "žodis" in: "žodis arba žodžių seka"
-------------------------------*/
void printunder(int alllen)
{
	/* "for" cikle turim "alllen + 1", nes jei atprint'inamos info ilgis pvz. 13, tai "for" ciklas bus
	0 - 12 t.y. 13 elementų, bet dar reikia vienos vietos "\n", kad būtų atprint'a nauja eilutė:
	Found word "žodis" in: "žodis arba žodžių seka"
	-------------------------------
	kitas elementas
	
	kitap būtų: 
	Found word "žodis" in: "žodis arba žodžių seka"
	-------------------------------kitas elementas
	*/
	for (int i = 0; i < (alllen + 1); i++)
	{
		/* kol ne alllen skaičus, print'inam "_" t.y. 0-12=13, bet reikia dar vienos vietos, tai turi būti 14,
		 taigi c array logika yra kad 13 atitinka 14 elementą ir todėl "i != alllen"  
		*/
		if(i != alllen)
		{
			printf("-");
		}

		// kitaip nauja eilutė
		else
		{
			printf("\n");
		}
	}
}

// funkcija, kuri atprint'itina info ar žodis rastas ar ne
void iffoundword(int allstrlen, int wordl, int wordcount, unsigned char *wordcopy, unsigned char *strcopy, int notfound, int nsl)
{
	//jei žodis rastas vienam string'e, tada atprint'inama, kad rasta
	if(wordcount > 0)
	{
		const unsigned char *begin = "Found word ";
		// surandam begin elementų skaičių
		int lbegin = strlen(begin);

		const unsigned char *end = " in: ";
		// surandam end elementų skaičių
		int lend = strlen(end);
		
		/* gaunam pilną skaičių:
		(wordl + 2)- ieškomas žodis yra kabutėse, todėl "+2"
		(allstrlen + 2) - string'as taip kabutėse, todėl irgi "+2"
		*/

		int alllength = lbegin + (wordl + 2) + lend + (allstrlen + 2) - nsl;

		for (int i = 0; i < wordcount; i++)
		{
			/* kiek kartų rastas žodis viename string'e, tiek ir atprint'inama
			wordcopy - nes, kitoje funkcijoje yra didžiosios raidės paverčiamos
			mažosiomis, todėl reikia originalios versijos kopijos, su strcopy
			tas pats
			*/
			printf("%s\"%s\"%s\"%s\"\n", begin, wordcopy, end, strcopy);
			printunder(alllength);
		}
	}

	//kitaip, tada atprint'inama, kad nerasta
	else
	{
		// notfound nurodo, kad žodis nerastas
		if(!notfound)
		{
			const unsigned char *nfw = "Not found word";
			// surandam nfw skaičių
			int nfwlen = strlen(nfw);
			
			// surandam bendrą skaičių, "+2" dėl kabučių 
			int alllength = nfwlen + wordl + 2;
			
			// ir atprint'inam, kad žodis nerastas
			printf("%s \"%s\"\n", nfw, wordcopy);
			printunder(alllength);
		}
	}

}

int comparestr(unsigned char *str, unsigned char *findword, unsigned char *op, int wl, int sl)
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
	
	return wce;
}

unsigned long *Upletter = NULL;
unsigned long *Lowletter = NULL;

unsigned int casef = 0;
unsigned int realcasef = 0;

void loadcaseffile(void)
{
	unsigned char **casefolding = loadfile("casefolding.txt");
	unsigned char prefix[]= "0x";
	unsigned int arrsize = 0;
	
	unsigned long *newlup = NULL;
	unsigned long *newllow = NULL;
	while(casefolding[casef])
	{
		unsigned long casehex;
		unsigned char casechar1 = casefolding[casef][6];
		unsigned char casechar2 = casefolding[casef][7];
		
		if((casechar1 == 'F') || (casechar2 == 'F') || (casechar1 == 'S') || (casechar2 == 'S') || (casechar1 == 'T') || (casechar2 == 'T'))
		{
			// printf("Nepalaikomas simbolis\n");
			// continue; kažkodėl negerai
		}
		else
		{
			char uphexstr[8];
			char lowhexstr[8];
			
			if(realcasef == arrsize)
			{
				arrsize += STEPSIZE;
				newlup = realloc(Upletter, arrsize * sizeof(unsigned long));
				newllow = realloc(Lowletter, arrsize * sizeof(unsigned long));
				
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
	free(newlup);
	free(newllow);
}

void casefold(unsigned char *cfstr, int *nsl)
{
	int nslcount = 0;
	for(int i = 0; cfstr[i] != '\0'; i++)
	{
		int ind;
		int indx = -1;
		unsigned long lowercl;
		
		int ifs = ifsimbol(cfstr[i]);
		if(ifs == 0)
		{
			if(cfstr[i] != ' ')
			{
				if(cfstr[i] <= 0x005A)
				{
					cfstr[i] += 32;
				}
				else
				{
					unsigned char b1 = cfstr[i] >> 5;
					unsigned char b2 = cfstr[i] >> 4;
					unsigned char b3 = cfstr[i] >> 3;
					// gal pabaigti su kitas utf kodavimais?
					if(b1 == 6)
					{
						nslcount++;
						ind = i;
					
						unsigned char wrb1temp = (cfstr[i] << 3);
    					unsigned char wrb1 = (wrb1temp >> 3);
						unsigned char wrb2temp = (cfstr[i+1] << 2);
    					unsigned char wrb2 = (wrb2temp >> 2);

						unsigned long result = (unsigned long)(wrb1 << 6) | wrb2;

						int pr = 0;
						int pb = realcasef;
						int vid = (pr + pb) / 2;
						int tempv = 0;
						// šitą algoritmą sugalvojau pats, tik ieškodmas geresnio varianto, pamačiau,
						// tai vadinama binary search
						while(1)
						{
							if(tempv == Upletter[vid])
							{
								break;
							}
							
							if(result == Upletter[vid])
							{
								indx = vid;
								break;
							}
							else if(result > Upletter[vid])
							{
								pr = vid;
								tempv = Upletter[vid];
							}
							else
							{
								pb = vid;
								tempv = Upletter[vid];
							}

							vid = (pr + pb)/2;
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
				}
			}
		}
	}

	*nsl = nslcount;
}

int main (int argc, unsigned char *argv[])
{	
	unsigned char **words;
	unsigned char filename[MAX_FILE_PATH];
	unsigned char word[MAX_WORD_LENGTH];
	unsigned char *tempfn;
	int fsize = 0;
	int endidx = 0;

	if (argc < 4)
	{
		if(argc != 3)
		{
			wchar_t wfname[MAX_FILE_PATH];
			wchar_t wword[MAX_WORD_LENGTH];
	
			unsigned long read;
			void *con = GetStdHandle(STD_INPUT_HANDLE);
			int size;
			unsigned char fileok = 0;
			unsigned char *msg = "Negerai, patikrinkit ar teisingai nurodėte failo pavadinimą su .txt galūne\n";
			
			if(argc == 1)
			{
				printf("Galima ir taip: findword.exe (C:\\Users\\Admin\\Desktop\\)failas.txt ieškomasžodis\n");
				printf("Dabar galite nurodyti failo pavadinimą (jei tame pačiame aplanke (folder'yje))\n");
				printf("arba failo vietą pvz.: C:\\Users\\Admin\\Desktop\\failas.txt\n");
				printf("(galite, laikydami ant failo, užtempti ant command prompt):\n");

				// panašu, kad dėl bug'o? neina su scanf priimti UTF8 ženklų, kaip lietuviškos raidės, todėl
				// padaryta pagal:
				// https://www.py4u.net/discuss/86119
				// SetConsoleOutputCP(CP_UTF8);
				// SetConsoleCP(CP_UTF8);
				
				while(!fileok)
				{
					ReadConsoleW(con, wfname, MAX_FILE_PATH, &read, NULL);
					size = WideCharToMultiByte(CP_UTF8, 0, wfname, read, filename, sizeof(filename), NULL, NULL);
					filename[size-2] = '\0';
					fsize = strlen(filename);
					endidx = fsize - 1;
					
					if(fsize >= 5)
					{
						int ifdot = 0;
						// for (int i = 0; i < fsize; i++)
						for (int i = 0; ((filename[i] != '\0') && (filename[i+1] != '.')); i++)
						{
							ifdot = i;
							// if(filename[i] == '.')
							// {
							// 	ifdot = 1;
							// }
						}

						// if(ifdot)
						if(ifdot != endidx)
						{
							fileok = ((filename[endidx-2] == 't') && 
									  (filename[endidx-1] == 'x') && 
									  (filename[endidx] == 't')
								  	   ) ? 1 : 0;

							if(!fileok)
							{
								// "Negerai, patikrinkit ar teisingai nurodėte failo pavadinimą su .txt galūne\n"
								printf("%s", msg);
							}
						}
						else
						{
							// "Negerai, patikrinkit ar teisingai nurodėte failo pavadinimą su .txt galūne\n")
							printf("%s", msg);
						}
					}
					else
					{
						printf("Neteisingas failas, turi būti bent viena raidė pavadinime su .txt galūne\n");
					}
				}
				words = loadfile(filename);
				
				printf("ir ieškomą žodį:\n");
				ReadConsoleW(con, wword, MAX_WORD_LENGTH, &read, NULL);
				size = WideCharToMultiByte(CP_UTF8, 0, wword, read, word, sizeof(word), NULL, NULL);
				word[size-2] = '\0';
						words = loadfile(filename);
			}
			else
			{
				fsize = strlen(argv[1]);
				endidx = fsize - 1;

				int ifdot = -2;
				int iffile = 0;
				for (int i = 0; i != (ifdot + 1); i++)
				{
					if(i == endidx)
					{
						ifdot = 0;
						goto jump;
					}
					else
					{
						if(argv[1][i] == '.')
						{
							ifdot = i;
						}
					}
				}
				jump:
				if(ifdot != 0)
				{
					int sk = endidx - ifdot;
					fileok = ((argv[1][endidx-2] == 't') && 
							  (argv[1][endidx-1] == 'x') && 
							  (argv[1][endidx] == 't')
						  	   ) ? 1 : 0;
					
					if((sk == 3) && (fileok))
					{
						tempfn = argv[1];
						int i = 0;
						while(*tempfn)
						{
							filename[i] = *tempfn;
							tempfn++;
							i++;
						}
						filename[i] = '\0';
						words = loadfile(filename);

						printf("Reikia ieškomo žodžio:\n");		
						
						// wchar_t wfname[MAX_FILE_PATH];
						// unsigned long read;
						// void *con = GetStdHandle(STD_INPUT_HANDLE);	
						ReadConsoleW(con, wword, MAX_FILE_PATH, &read, NULL);
						int size = WideCharToMultiByte(CP_UTF8, 0, wword, read, word, sizeof(word), NULL, NULL);
						word[size-2] = '\0';

					}
					else
					{
						while(!fileok)
						{
							printf("Negerai, patikrinkit ar teisingai nurodėte failo pavadinimą su .txt galūne\n");
							ReadConsoleW(con, wfname, MAX_FILE_PATH, &read, NULL);
							int size = WideCharToMultiByte(CP_UTF8, 0, wfname, read, filename, sizeof(filename), NULL, NULL);
							filename[size-2] = '\0';
							fsize = strlen(filename);
							endidx = fsize - 1;
							fileok = ((filename[endidx-2] == 't') && 
							  		  (filename[endidx-1] == 'x') && 
									  (filename[endidx] == 't')
						  			  ) ? 1 : 0;
							
							if(fileok)
							{
								// galima ankščiau break'inti while loop'ą, kad neinu iš naujo per while(!fileok) 
								break;
							}
						}
						words = loadfile(filename);
						printf("Reikia ieškomo žodžio:\n");
						ReadConsoleW(con, wword, MAX_FILE_PATH, &read, NULL);
						int size = WideCharToMultiByte(CP_UTF8, 0, wword, read, word, sizeof(word), NULL, NULL);
						word[size-2] = '\0';
					}
					
				}
				else
				{
					tempfn = argv[1];
					int i = 0;
					while(*tempfn)
					{
						word[i] = *tempfn;
						tempfn++;
						i++;
					}
					word[i] = '\0';

					printf("Reikia failo pavadinimo/vietos (galite, laikydami ant failo, užtempti ant command prompt):\n");		
					
					// wchar_t wfname[MAX_FILE_PATH];
					// unsigned long read;
					// void *con = GetStdHandle(STD_INPUT_HANDLE);	
					ReadConsoleW(con, wfname, MAX_FILE_PATH, &read, NULL);
					int size = WideCharToMultiByte(CP_UTF8, 0, wfname, read, filename, sizeof(filename), NULL, NULL);
					filename[size-2] = '\0';
					// fsize = strlen(filename);
					// endidx = fsize - 1;
					words = loadfile(filename);
				}
			}
		}
		else
		{
			// negerai dėl didžiųjų raidžių, panašu, kad kažkoks bug'as
			// šitas printf leidžia words = loadfile(filename) šitam "else",
			// o ne išėjus pagrindinio if else
			// printf("");
			fsize = strlen(argv[2]);
			int j = 0;
			while(*argv[2])
			{
				word[j] = *argv[2];
				argv[2]++;
				j++;
			}

			word[fsize] = '\0';
	
			fsize = strlen(argv[1]);
			int i = 0;
			while(*argv[1])
			{
				filename[i] = *argv[1];
				argv[1]++;
				i++;
			}
			filename[fsize] = '\0';
			words = loadfile(filename);
			
		}
	}
	else
	{
		printf("Per daug argumentų. Kol kas negalima ieškoti žodžių sekos, tik vieną žodį!!!\n");
		exit(1);
	}

	loadcaseffile();
	
	int wordlen = 0;
	unsigned char *wordcopy = NULL;
	for (int i = 0; word[i] != '\0'; i++)
	{
		wordlen++;
		if(wordlen == 1 || (wordlen % 2 == 0))
		{
			wordcopy = realloc(wordcopy, (i + 2) * sizeof(unsigned char));
		}
		wordcopy[i] = word[i];	
		if(word[i+1] == '\0')
		{
			wordcopy[wordlen] = '\0';
		}
	}

	int wordnsl = 0;
	casefold(word, &wordnsl);
	// words = loadfile(filename);

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
	struct StrLen asl;
	int wc = 0;

	unsigned char *wordscopy;
	int nf = 1;
	int strnsl = 0;

	for (int i = 0; words[i] != NULL; i++)
	{

		int lnb = 0;
		int spnb = 0;
		
		int wordsl = strlen(words[i]) + 1;
		wordscopy = malloc(wordsl * sizeof(char));
		strcpy(wordscopy, words[i]);
		
		casefold(words[i], &strnsl);
		
		asl = strleninfo(words[i]);
		
		wc = comparestr(words[i], word, op, wordlen, asl.realstrlen);
		iffoundword(asl.allstrlen, wordlen, wc, wordcopy, wordscopy, nf, (wordnsl + strnsl));
		notfound += wc;

	}
	
	if(!notfound)
	{
		nf = 0;
		if(!nf)
		{
			iffoundword(asl.allstrlen, wordlen, wc, wordcopy, wordscopy, nf, (wordnsl + strnsl));
		}
	}
	free(wordscopy);

	printf("Baigta");
	return 0;
}