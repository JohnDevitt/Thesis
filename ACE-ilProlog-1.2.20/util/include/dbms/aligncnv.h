#ifdef ALIGN_CONVERT_ENABLE

typedef union {
  struct four_chars{ unsigned char c1,c2,c3,c4; } four_chars;
  int value;
} conv_align_int;

typedef union {
  struct four_chars{ unsigned char c1,c2,c3,c4; } four_chars;
  float value;
} conv_align_float;

typedef union {
  struct eight_chars{ unsigned char c1,c2,c3,c4,c5,c6,c7,c8; } eight_chars;
  double value;
} conv_align_double;


inline void store_int_arg_align(unsigned char* where, int value) {
	conv_align_int cnv;
	cnv.value = value;
	where[0] = cnv.four_chars.c1;
	where[1] = cnv.four_chars.c2;
	where[2] = cnv.four_chars.c3;
	where[3] = cnv.four_chars.c4;
} 

inline int get_int_arg_align(const unsigned char* where) {
	conv_align_int cnv;
	cnv.four_chars.c1 = where[0];
	cnv.four_chars.c2 = where[1];
	cnv.four_chars.c3 = where[2];
	cnv.four_chars.c4 = where[3];
	return cnv.value;
} 

inline void store_float_arg_align(unsigned char* where, float value) {
	conv_align_float cnv;
	cnv.value = value;
	where[0] = cnv.four_chars.c1;
	where[1] = cnv.four_chars.c2;
	where[2] = cnv.four_chars.c3;
	where[3] = cnv.four_chars.c4;
} 

inline float get_float_arg_align(const unsigned char* where) {
	conv_align_float cnv;
	cnv.four_chars.c1 = where[0];
	cnv.four_chars.c2 = where[1];
	cnv.four_chars.c3 = where[2];
	cnv.four_chars.c4 = where[3];
	return cnv.value;
} 

inline void store_double_arg_align(unsigned char* where, double value) {
	conv_align_double cnv;
	cnv.value = value;
	where[0] = cnv.eight_chars.c1;
	where[1] = cnv.eight_chars.c2;
	where[2] = cnv.eight_chars.c3;
	where[3] = cnv.eight_chars.c4;
	where[4] = cnv.eight_chars.c5;
	where[5] = cnv.eight_chars.c6;
	where[6] = cnv.eight_chars.c7;
	where[7] = cnv.eight_chars.c8;	
} 

inline double get_double_arg_align(const unsigned char* where) {
	conv_align_double cnv;
	cnv.eight_chars.c1 = where[0];
	cnv.eight_chars.c2 = where[1];
	cnv.eight_chars.c3 = where[2];
	cnv.eight_chars.c4 = where[3];
	cnv.eight_chars.c5 = where[4];
	cnv.eight_chars.c6 = where[5];
	cnv.eight_chars.c7 = where[6];
	cnv.eight_chars.c8 = where[7];	
	return cnv.value;
} 

#endif
