#include <testing.h>

#include "parse.h"

Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help)
{
	// Création de la structure d'option :
	Option new = {.opt = opt, .lopt = lopt, .help = help};

	// On récupère le nom de l'option :
	//new.opt  = opt;
	//new.lopt = lopt;

	// On récupère le type de l'option :
	new.type = type;

	// L'aide à afficher en cas d'erreur ou de demande d'aide :
	//new.help = help;

	// On récupère le type de la variable :
	switch(type)
	{
		case T_FUNC:
			new.val.T_func    = (args_func)def;
		case T_BOOL:
			new.val.T_bool    = (bool*)def;
			break;
		case T_DOUBLE:
			new.val.T_double  = (double*)def;
			break;
		case T_FLOAT:
			new.val.T_float   = (float*)def;
			break;
		case T_INT:
			new.val.T_int     = (int*)def;
			break;
		case T_LONG:
			new.val.T_long    = (long*)def;
			break;
		case T_NONE:
		case T_VOID:
		case T_CHAR:
		case T_UCHAR:
			new.val.T_char    = (char*)def;
			break;
		case T_SHORT:
			new.val.T_short   = (short*)def;
			break;
		case T_UINT:
			new.val.T_uint    = (unsigned int*)def;
			break;
		case T_ULONG:
			new.val.T_ulong   = (unsigned long*)def;
			break;
		case T_USHORT:
			new.val.T_ushort  = (unsigned short*)def;
			break;
		case T_LLONG:
			new.val.T_llong   = (long long*)def;
			break;
		case T_LDOUBLE:
			new.val.T_ldouble = (long double*)def;
			break;
		default:
			// Ah ! On est tombé sur un type non supporté par la lib, il va falloir quitter :
			fprintf(stderr, "%s::Unsupported Type found while creating Option (-%s, --%s)!\n", __func__, opt, lopt);
			exit(EXIT_FAILURE);
			// Le fait de quitter est discutable, je pourrais tomber par défaut sur un type void, ou char* et laisser l'utilisateur gérer l'erreur lui-même.
	}

	// On retourne la structure nouvellement créé :
	return new;
}

void parse_help(void *args)
{
	struct lst_args *lst = args;
	// On récupère l'option courte pour l'afficher si elle existe :
	if( lst->opt.opt != NULL )
		printf("%s", lst->opt.opt);

	// Idem pour l'option longue :
	if( lst->opt.lopt != NULL )
		printf(", %s\t", lst->opt.lopt);
	// Que l'on remplace par un vide si elle n'existe pas :
	else
		printf("        \t");
	printf("\t");

	// Affichage de la description de l'option :
	if( lst->opt.help != NULL )
		printf("%s\n", lst->opt.help);

	// Si il y a d'autres options, on les affiches :
	if( lst->next != NULL )
		parse_help(lst->next);
}

Args* Args_New(void)
{
	// Création et allocation de la structure qui contiendras les arguments :
	Args *new = NULL;
	new = malloc(sizeof(Args));

	// Allocation du premier élément de la liste chaîné constituant la liste d'option :
	new->args = malloc(sizeof(struct lst_args));
	new->args->next = NULL;

	// Création de l'option d'aide :
	new->args->opt  = Option_Create("-h", "--help", T_VOID, (void*)parse_help, "Print this help.");
	//strcmp("-h", "--help");

	return new;
}

void Args_add(Args *this, const char *opt, const char *lopt, Type type, void *def, const char *help)
{
	// Nouveau chaînon pour les options :
	struct lst_args * new = NULL;
	new = malloc(sizeof(struct lst_args));
	if( new == NULL )
	{
		perror("Allocation error:");
		// Coix discutable :
		exit(EXIT_FAILURE);
	}

	// Création de l'option :
	new->opt   = Option_Create(opt, lopt, type, def, help);
	new->next  = NULL;

	// Ajout de l'option à la fin de la liste :
	struct lst_args * tmp = this->args;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}

	tmp->next = new;
}

void Args_Free(Args *this)
{
	struct lst_args *old= NULL, *act = this->args;

	do
	{
		old = act;
		act = act->next;
		free(old);
	}while(act != NULL);

	free(this);
}

void Option_Convert(Option *new, const char *arg)
{
	switch(new->type)
	{
		case T_NONE:
		case T_VOID:
		case T_CHAR:
		case T_UCHAR:
			new->val.T_char     = arg;
			break;
		case T_FLOAT:
			*new->val.T_float   = (float)atof(arg);
			break;
		case T_DOUBLE:
			*new->val.T_double  = atof(arg);
			break;
		case T_LDOUBLE:
			*new->val.T_ldouble = (long double)atof(arg);
			break;
		case T_SHORT:
			*new->val.T_short   = (short)atoi(arg);
			break;
		case T_INT:
			*new->val.T_int     = atoi(arg);
			break;
		case T_LONG:
			*new->val.T_long    = atol(arg);
			break;
		case T_LLONG:
			*new->val.T_llong   = (long long)atoll(arg);
			break;
		case T_USHORT:
			*new->val.T_ushort  = (unsigned short)atoi(arg);
			break;
		case T_UINT:
			*new->val.T_uint    = (unsigned int)atoi(arg);
			break;
		case T_ULONG:
			*new->val.T_ulong   = (unsigned long)atoi(arg);
			break;
		default:
			exit(EXIT_FAILURE);
	}
}

bool Args_Parse(Args *this, const int argc, const char **argv)
{
	for(int i = 0; i < argc; i++)
	{
		fprintf(stderr, "%d :: %s\n", i, argv[i]);
	}
	struct lst_args *next = NULL;

	for (int i = 1; i < argc; i++) {
		next = this->args;
		fprintf(stderr, "Doing : %s\n", argv[i]);
		do
		{
			fprintf(stderr, "\tTesting --> (%s, %s)\n", next->opt.opt, next->opt.lopt);
			if( (next->opt.opt != NULL && !strcmp(argv[i], next->opt.opt)) || (next->opt.lopt != NULL && !strcmp(argv[i], next->opt.lopt)) )
			{
				// On converti l'option et/ou son argument en "valeur" :
				if( next->opt.type == T_BOOL )
					*next->opt.val.T_bool = true;
				else
				{
					Option_Convert(&next->opt, argv[i+1]);
					i++;
				}
			}
			//if( (next->opt.opt != NULL) )
			//{
				//if( !strcmp(argv[i], next->opt.opt) )
				//{
					//// On converti l'option et/ou son argument en "valeur" :
					//if( next->opt.type == T_BOOL )
						//*next->opt.val.T_bool = true;
					//else
					//{
						//Option_Convert(&next->opt, argv[i+1]);
						//i++;
					//}
				//}
			//}
			//else if( next->opt.lopt != NULL )
			//{
				//if( !strcmp(argv[i], next->opt.lopt) )
				//{
					//// On converti l'option et/ou son argument en "valeur" :
					//if( next->opt.type == T_BOOL )
						//*next->opt.val.T_bool = true;
					//else
					//{
						//Option_Convert(&next->opt, argv[i+1]);
						//i++;
					//}
				//}
			//}
			next = next->next;
		}while(next != NULL);
	}

	return true;
}

