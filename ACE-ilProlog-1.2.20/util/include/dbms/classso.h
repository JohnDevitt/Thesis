
#ifdef cygwin
	#define G_SO __declspec(dllexport)
	#define CLASS_SO __attribute__ ((dllexport))
#else
	#define G_SO
	#define CLASS_SO
#endif
