#ifndef PARSE_H
#define PARSE_H

#include <string.h>

typedef enum {
	NONE,
	DOUBLE,
	FLOAT,
	INT,
	LONG,
	CHAR,
	SHORT,

	UINT,
	ULONG,
	UCHAR,
	USHORT,

	LLONG,
	LDOUBLE,

	IMPLEMENTED
} Type;

typedef struct {
	char *opt, *lopt;
	Type type;
}Option;

typedef union {
	double T_double;
	float T_float;
	int T_int;
	long T_long;
	char T_char;
	short T_short;
	unsigned int T_unsignedint;
	unsigned char T_unsignedchar;
	unsigned short T_unsignedshort;
	unsigned long T_unsignedlong;
	long double T_longdouble;
	long long T_longlong;
	/*		double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;
			double T_double;*/
} Value;

typedef struct _args{
	char *opt;
	//char *
	//void *val;
	Value data;
	struct _args *next;
}Args;

Args* Parser(const int argc, const char const * const *argv, const Option *list, const int Nopt);

#endif /* end of include guard: PARSE_H */
