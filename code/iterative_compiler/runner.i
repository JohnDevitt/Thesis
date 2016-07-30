/* example.i */
%module runner
%{
/* Put header files here or function declarations like below */
extern double timer(char *filepath);
%}

extern double timer(char *filepath);