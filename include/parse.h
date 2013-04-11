#ifndef PARSE_H
#define PARSE_H

#include <string.h>

typedef enum {
	NONE,
	VOID,
	DOUBLE,
	FLOAT,
	INT,
	LONG,
	CHAR,
	SHORT,
	BOOL,

	UINT,
	ULONG,
	UCHAR,
	USHORT,

	LLONG,
	LDOUBLE,

	IMPLEMENTED
} Type;

typedef union {
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
	char       * opt,  * lopt;
	char       * help;
	Type         type;
	Value        val;
}Option;

typedef struct _args{
	char *opt;
	//char *
	//void *val;
	Value data;
	struct _args *next;
}Args;

Option Option_Create(const char *opt, const char *lopt, Type type, void *def, const char *help);

#endif /* end of include guard: PARSE_H */
