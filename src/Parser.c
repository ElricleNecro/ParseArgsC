#include "Parser.h"

Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help) {
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
	switch(type) {
		case T_FUNC:
			new.val.T_func    = (args_func)def;
			break;
		case T_LIST:
			new.val.T_list    = (CList*)def;
			break;
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
			new.val.T_char    = (const char**)def;
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

void parse_help(struct lst_args *lst, int i, const char **argv) {
	if( i == 0 )
		printf("%s:\n", argv[0]);

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
		parse_help(lst->next, i+1, argv);
}

Args* Args_New(void) {
	// Création et allocation de la structure qui contiendras les arguments :
	Args *new = NULL;
	new = malloc(sizeof(Args));

	// Allocation du premier élément de la liste chaîné constituant la liste d'option :
	new->args = malloc(sizeof(struct lst_args));
	new->args->next = NULL;

	// Création de l'option d'aide :
	new->args->opt  = Option_Create("-h", "--help", T_FUNC, (void*)parse_help, "Print this help.");
	//strcmp("-h", "--help");

	new->end = new->args;
	new->rest = NULL;

	return new;
}

void Args_Add(Args *this, const char *opt, const char *lopt, Type type, void *def, const char *help) {
	// Nouveau chaînon pour les options :
	struct lst_args * new = NULL;
	new = malloc(sizeof(struct lst_args));
	if( new == NULL ) {
		perror("Allocation error:");
		// Choix discutable :
		exit(EXIT_FAILURE);
	}

	// Création de l'option :
	new->opt   = Option_Create(opt, lopt, type, def, help);
	new->next  = NULL;

	this->end->next = new;
	this->end = new;
}

void Args_Free(Args *this) {
	// Libération des options non traité :
	struct clst *cold= NULL, *cact = this->rest;
	while( cact != NULL ) {
		cold = cact;
		cact = cact->next;
		free(cold);
	}

	// Libération des arguments :
	struct lst_args *old= NULL, *act = this->args;
	do {
		old = act;
		act = act->next;
		free(old);
	} while(act != NULL);

	// Libération de la structure général :
	free(this);
}

void Option_Convert(Option *new, const char *arg) {
	switch(new->type) {
		case T_NONE:
		case T_VOID:
		case T_CHAR:
		case T_UCHAR:
			*new->val.T_char     = arg;
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

void Add_Rest(Args *this, const char *argv) {
	if( this->rest == NULL ) {
		this->rest       = malloc(sizeof(struct clst));
		this->rest->next = NULL;
		this->rest->opt  = argv;
		return ;
	}

	struct clst * new = NULL, *tmp = this->rest;
	new = malloc(sizeof(struct clst));
	new->opt = argv;
	new->next = NULL;

	while( tmp->next != NULL ) {
		tmp = tmp->next;
	}
	tmp->next = new;
}

Args_Error Args_Parse(Args *this, const int argc, const char **argv) {
	for (int i = 1; i < argc; i++) {
#ifdef __DEBUG_PARSER
		fprintf(stderr, "Parsing '%s':", argv[i]);
#endif
		for(struct lst_args *next = this->args; next != NULL; next = next->next) {
#ifdef __DEBUG_PARSER
			fprintf(stderr, "\tTesting ('%s', '%s'): ", (next->opt.opt==NULL)?"":next->opt.opt, (next->opt.lopt==NULL)?"":next->opt.lopt );
#endif
			if( !strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") ) {
				void (*f)(struct lst_args *, int, const char**) = (void (*)(struct lst_args *, int, const char**))next->opt.val.T_void;
				f(this->args, 0, &argv[0]);
				return HELP;
			} else if( (next->opt.opt != NULL && !strcmp(argv[i], next->opt.opt)) || (next->opt.lopt != NULL && !strcmp(argv[i], next->opt.lopt)) ) {
				// On converti l'option et/ou son argument en "valeur" :
				if( next->opt.type == T_BOOL ) {
					*next->opt.val.T_bool = true;
					break;
				} else if( next->opt.type == T_FUNC ) {
					args_func f = next->opt.val.T_func;
					f(this->args, argc - i, &argv[i]);
					fprintf(stderr, "Function handling not yet entirely implemented!\n");
					break;
				} else if( next->opt.type == T_LIST ) {
					if( *next->opt.val.T_list == NULL ) {
						*next->opt.val.T_list = calloc(1, sizeof(struct clst));
						(*next->opt.val.T_list)->opt = argv[i + 1];
						(*next->opt.val.T_list)->next = NULL;
						(*next->opt.val.T_list)->end = *next->opt.val.T_list;

						i += 1;
						break;
					}
					CList start = *next->opt.val.T_list, end = (*next->opt.val.T_list)->end;
					end->next = calloc(1, sizeof(struct clst));
					end       = end->next;
					end->opt  = argv[i+1];
					end->next = NULL;

					for(CList tmp = start; tmp != NULL; tmp = tmp->next)
						tmp->end = end;

					i += 1;
					break;
				} else {
#ifdef __DEBUG_PARSER
					printf("Found option %d not BOOL or FUNC, parsing.", i);
#endif
					Option_Convert(&next->opt, argv[i+1]);
					i++;
#ifdef __DEBUG_PARSER
					printf("Args was %d, next will be : %d\n", i, i+1);
#endif
					break;
				}
			} else if( argv[i][0] != '-' ) {
				Add_Rest(this, argv[i]);
				break;
			} else if( !strcmp(argv[i], "--") ) {
				i++;
				while( i < argc ) {
					Add_Rest(this, argv[i]);
					i++;
				}

				return TREAT_SUCCESS;
			} else if( next->next == NULL ) {
				fprintf(stderr, "Unrecognised parameter : '%s'\n", argv[i]);
				parse_help(this->args, 0, &argv[0]);
				return TREAT_ERROR;
			}
#ifdef __DEBUG_PARSER
			fprintf(stderr, "\n");
#endif
		}
	}

	return TREAT_SUCCESS;
}
