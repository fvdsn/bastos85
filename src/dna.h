#ifndef __DNA_DNA_H__
#define __DNA_DNA_H__

#define STRING_SIZE 32

enum base_type{
	DNA_VOID,
	DNA_INT,
	DNA_FLOAT,
	DNA_PTR,
	DNA_ARRAY,
	DNA_FUN_PTR,
	DNA_STRUCT
};
typedef struct dna_base_s{
	enum  base_type type;
	int alias;
	int size;
	char name[STRING_SIZE];
	char desc[STRING_SIZE];
}dna_base_t;
typedef dna_int_s{
	dna_base base;
	int  min;
	int max;
	int mid;
}dna_int_t;
typedef dna_float_s{
	dna_base base;
	float min;
	float max;
	float mid;
}dna_float_t;
typedef dna_ptr_s{
	dna_base base;
	dna_base target;
}dna_ptr_t;
typedef dna_fun_ptr_s{
	dna_base base;
	dna_base rvalue;
	int param_count;
	dna_base *param;
}dna_fun_ptr_t;
typedef dna_array_s{
	dna_base base;
	dna_base subtype;
	int size;
}dna_array_t;
typedef dna_struct_s{
	dna_base base;
	int field_count;
	dna_base **field;
}dna_struct_t;

dna_base_t  dna_base_new(int base_type);
dna_base_t *dna_new(
#endif
