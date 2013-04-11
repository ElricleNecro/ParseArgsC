#include "parse.h"

char *retire(const char *src, const char *mv)
{
	int length = strlen(src);
	char res[length];

	for(int i=0, j=0; i<length && j<length; i++)
	{
		if( strcmp(src[i], mv) )
		{
			res[j] = src[i];
			j++;
		}
	}

	return res;
}

Option* in(const char *arg, const Option *list, const int Nopt)
{
	for (int i = 0; i < Nopt; i++)
		if (    ( list[i].opt  != NULL && !strcmp(retire(arg, "-"),  list[i].opt ) )   ||
			( list[i].lopt != NULL && !strcmp(retire(arg, "--"), list[i].lopt) )
		   )
			return &list[i];
	return NULL;
}

void add(Args *res, Args *adding)
{
	Args *move = NULL;

	do
	{
		move = res->next;
	}while(res->next != NULL);

	move->next = adding;
}

Args* Parser(const int argc, const char **argv, const Option *list, const int Nopt)
{
	Option opt;
	Args  *res = NULL;

	for (int i = 0; i < argc; i++)
	{
		if( (opt = in(argv[i], list, Nopt)) )
		{
			Args *tmp = malloc(sizeof(Val));

			tmp->opt = argv[i];
			switch(opt.type)
			{
				case DOUBLE:
					tmp.data.T_double;
					break;
				case FLOAT:
					tmp.data.T_float;
					break;
				case INT:
					tmp.data.T_int;
					break;
				case LONG:
					tmp.data.T_long;
					break;
				case CHAR:
					tmp.data.T_char;
					break;
				case SHORT:
					tmp.data.T_short;
					break;
				case UDOUBLE:
					tmp.data.T_unsigneddouble;
					break;
				case UFLOAT:
					tmp.data.T_unsignedfloat;
					break;
				case UINT:
					tmp.data.T_unsignedint;
					break;
				case ULONG:
					tmp.data.T_unsignedlong;
					break;
				case UCHAR:
					tmp.data.T_unsignedchar;
					break;
				case USHORT:
					tmp.data.T_unsignedshort;
					break;
				case LLONG:
					tmp.data.T_longlong;
					break;
				case LDOUBLE:
					tmp.data.T_longdouble;
					break;
				default:
					fprintf(stderr, "Aïe, Type inconnu\n");
					return NULL;
					break;
			}

			if( res != NULL )
				add(res, tmp);
			else
				res = tmp;
		}
	}
}

