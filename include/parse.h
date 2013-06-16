#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

	T_IMPLEMENTED
} Type;

typedef void (*args_func)(void*);

typedef union {
	args_func        T_func;
	bool           * T_bool;
	void           * T_void;
	double         * T_double;
	float          * T_float;
	int            * T_int;
	long           * T_long;
	char           * T_char;
	short          * T_short;
	unsigned int   * T_uint;
	unsigned char  * T_uchar;
	unsigned short * T_ushort;
	unsigned long  * T_ulong;
	long double    * T_ldouble;
	long long      * T_llong;
} Value;

typedef struct {
	const char       * opt,  * lopt;
	const char       * help;
	Type         type;
	Value        val;
}Option;

Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help);

struct lst_args {
	Option opt;
	struct lst_args *next;
};

typedef struct _args {
	//char *opt;
	//char *
	//void *val;
	//Value data;
	//Option opt;
	//struct _args *next;
	struct lst_args *args;
} Args;

Args *Args_New(void);
void Args_add(Args *this, const char *opt, const char *lopt, Type type, void *def, const char *help);
void Args_Free(Args *this);

#endif /* end of include guard: PARSE_H */
