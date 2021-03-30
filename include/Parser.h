#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * \file
 *
 * First of all, you will excuse me for this poor english. As English is not my mother tongue,
 * there will be many orthographic or construction fault along this document.
 *
 * \section Goal
 * My goal was to create a library which permit us to parse CLI option:
 *	- without using global variable like the GNU one,
 *	- in an object oriented way,
 *	- easy to use
 *	- just needed some few function call to construct the CLI.
 *
 * \section Future evolution
 * As I am the only one developer at the time I write this, I haven't really plan something.
 * The actual improvement which will be done surly are:
 *	- improve code efficacy without becoming unreadable,
 *	- possibility to use Command instruction like git or svn
 *	- improve stability (will come with use).
 *
 * The way it use function pointer at the moment doesn't satisfy me. I need to think of how to handle
 * them properly and which information to give them.
 *
 * \section Licence
 * As indicate in the LICENCE file, this library is placed under GPLv3 (-- or LGPLv3, to decided --).
 * Why ? Because I want that everyone can use it freely but I don't want them to distribute modified
 * version. If Modification are done, why shouldn't we improve directly this one ? Let the improvement
 * be useful to all of us.
 *
 * \section Information
 * This library use ISO C99, so you will need a compiler which support it. I have tested it with
 * gcc-4.7, gcc-4.8 and clang-3.3.
 *
 * \section Example of use
 * Take this code:
 *
#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main(int argc, const char *argv[])
{
	bool   b_test = false;
	int    i_test = 10;
	double d_test = 3.14159;

	Args *args = Args_New();
	Args_Error err;

	Args_Add(args, "-t", NULL, T_BOOL, &b_test, "Un test des booléens.");
	Args_Add(args, "-i", "--int_test", T_INT, &i_test, "Un test des entiers.");
	Args_Add(args, "-d", "--double_test", T_DOUBLE, &d_test, "Un test des doubles.");

	err = Args_Parse(args, argc, argv);
	if( err == TREAT_ERROR )
	{
		Args_Free(args);
		return EXIT_FAILURE;
	}
	else if( err == HELP )
	{
		Args_Free(args);
		return EXIT_SUCCESS;
	}

	printf("%d\n%d\n%g\n", b_test, i_test, d_test);

	CList other = args->rest;

	while(other)
	{
		printf("Untreated options: '%s'\n", other->opt);
		other = other->next;
	}

	Args_Free(args);

	return EXIT_SUCCESS;
}
 *
 * We are declaring here 3 option (one boolean, one integer and one double precision)
 * and we print them. Plus we check all untreated argument.
 * I think this exemple talk for itself. More will come on more complex things.
 *
 */

/**
 * Enum used for treat all error's. More will come with usage and problems discovery.
 */
typedef enum {
	HELP,		/*!HELP was called.*/
	TREAT_ERROR,	/*!An error as been arise'd*/
	TREAT_SUCCESS,	/*!No error*/

	ARGS_ERROR
} Args_Error;

/**
 * Enum used to declare Option type to function @ref Option_Create and @ref Args_Add.
 */
typedef enum {
	T_NONE,
	T_VOID,
	T_DOUBLE,
	T_FLOAT,
	T_INT,
	T_LONG,
	T_CHAR,
	T_SHORT,
	T_BOOL,

	T_UINT,
	T_ULONG,
	T_UCHAR,
	T_USHORT,

	T_LLONG,
	T_LDOUBLE,

	T_FUNC,

	T_LIST,

	T_IMPLEMENTED
} Type;

struct lst_args;

typedef void (*args_func)(struct lst_args*, int, const char **);

/**
 * List containing all non parsed element.
 */
typedef struct clst {
	const char *opt;	/*!The string of the non parsed element.*/
	struct clst *next,	/*!Pointer to the next element.*/
		    *end;	/*!Pointer to the last element of the list (used only during construction).*/
} *CList;

/**
 * This union contains the value gives as default.
 */
typedef union {
	args_func        T_func;
	bool           * T_bool;
	void           * T_void;
	double         * T_double;
	float          * T_float;
	int            * T_int;
	long           * T_long;
	const char    ** T_char;
	short          * T_short;
	unsigned int   * T_uint;
	unsigned char  * T_uchar;
	unsigned short * T_ushort;
	unsigned long  * T_ulong;
	long double    * T_ldouble;
	long long      * T_llong;
	CList          * T_list;
} Value;

/**
 * The Option struct contain all needed information for most kind of information.
 */
typedef struct {
	const char *opt,	/*!Short option name.*/
	           *lopt;	/*!Long option name.*/
	const char *help;	/*!Help string.*/
	Type        type;	/*!Value Type.*/
	Value       val;	/*!The value itself.*/
} Option;

/**
 * The List itself with the option and a pointer to the next element.
 */
struct lst_args {
	Option opt;		/*!Option description.*/
	struct lst_args *next,	/*!Pointer to the next element.*/
			*end;	/*!Pointer to the end of the list (used only during construction of the option list).*/
} largs;

/**
 * Container for the option list plus the non parsed element list.
 */
typedef struct _args {
	//char *opt;
	//char *
	//void *val;
	//Value data;
	//Option opt;
	//struct _args *next;
	struct lst_args *args, *end;	/*!The Option list.*/
	struct clst *rest, *rend;	/*!The non parsed element list.*/
} Args;

/**
 * Create an Option from indicate parameter.
 *
 * @param[in] *opt Short option name or NULL if there is not.
 * @param[in] *lopt Long option name or NULL if there is not.
 * @param[in] type Option type @ref Type;
 * @param[in] *def Default value to use.
 * @param[in] *help Help string.
 *
 * @return Option @ref Option.
 */
Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help);

/**
 * Create the Args struct use for hold user options definition. First function to call.
 * You MUST call Args_Free on this variable once you don't use it anymore.
 *
 * @return The newly allocated Agrs struct.
 */
Args* Args_New(void);

/**
 * This function add an option to your list.
 *
 * @param[in] *this List in which add the new option.
 * @param[in] *opt Short option name or NULL if there is not.
 * @param[in] *lopt Long option name or NULL if there is not.
 * @param[in] type Option type @ref Type;
 * @param[in] *def Default value to use.
 * @param[in] *help Help string.
 */
void Args_Add(Args *this, const char *opt, const char *lopt, Type type, void *def, const char *help);

/**
 * This function parse your argument list.
 *
 * @param[in] *this List of options for use.
 * @param[in] argc Number of string to parse.
 * @param[in] **argv String array of length *argv[argc] to parse.
 *
 * @return TREAT_SUCCESS if there was no problem, HELP if '-h' or '--help' were given, TREAT_ERROR else.
 */
Args_Error Args_Parse(Args *this, const int argc, const char **argv);

/**
 * Call this function to free an argument list.
 *
 * @param[in] *this List to be free'd.
 */
void Args_Free(Args *this);

/**
 * This function will free the memory allocated for one T_LIST option.
 *
 * @param[in] opt List to be free'd.
 */
void clist_free(CList opt);

#endif /* end of include guard: PARSE_H */

//vim:spelllang=en
