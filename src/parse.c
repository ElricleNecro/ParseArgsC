#include "parse.h"

Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help)
{
	Option new;

	new.opt  = opt;
	new.lopt = lopt;
	new.type = type;
	new.help = help;

	switch(type)
	{
		case NONE:
		case VOID:
			new.val.T_void    = (void*)def;
			break;
		case BOOL:
			new.val.T_bool    = (bool*)def;
			break;
		case DOUBLE:
			new.val.T_double  = (double*)def;
			break;
		case FLOAT:
			new.val.T_float   = (float*)def;
			break;
		case INT:
			new.val.T_int     = (int*)def;
			break;
		case LONG:
			new.val.T_long    = (long*)def;
			break;
		case CHAR:
			new.val.T_char    = (char*)def;
			break;
		case SHORT:
			new.val.T_short   = (short*)def;
			break;
		case UINT:
			new.val.T_uint    = (unsigned int*)def;
			break;
		case ULONG:
			new.val.T_ulong   = (unsigned long*)def;
			break;
		case UCHAR:
			new.val.T_uchar   = (unsigned char*)def;
			break;
		case USHORT:
			new.val.T_ushort  = (unsigned short*)def;
			break;
		case LLONG:
			new.val.T_llong   = (long long*)def;
			break;
		case LDOUBLE:
			new.val.T_ldouble = (long double*)def;
			break;
		default:
			fprintf(stderr, "%s::Unsupported Type found while creating Option (-%s, --%s)!\n", __func__, opt, lopt)
			exit(EXIT_FAILURE);
	}

	return new;
}

