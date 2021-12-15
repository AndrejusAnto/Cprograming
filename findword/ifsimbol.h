int ifsimbol(unsigned char c)
{
	int number = 0;
	switch(c)
	{
	  case '!':
	  case '@':
	  case '|':
	  case '+':
	  case '#':
	  case '$':
	  case '%':
	  case '^':
	  case '&':
	  case '*':
	  case '(':
	  case ')':
	  case '-':
	  case '{':
	  case '}':
	  case '[':
	  case ']':
	  case ':':
	  case ';':
	  case '"':
	  case '\'':
	  case '<':
	  case '>':
	  case '.':
	  case '/':
	  case '?':
	  case '~':
	  case '`':
	  case ',':
	  case '_':
		number = 1;
		break;
	}
	return number;
}