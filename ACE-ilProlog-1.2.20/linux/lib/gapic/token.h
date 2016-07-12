#ifndef TOKEN_H
#define TOKEN_H

#include "toinclude.h"

#define SEPARATORBIT 128

#define EOFFILE 0
#define NAME_TOKEN 1
#define VARIABLE_TOKEN 2
#define INTEGER_TOKEN 3
#define FLOAT_NUMBER_TOKEN 4
#define CHAR_CODE_LIST_TOKEN 5
#define OPEN_TOKEN (6+SEPARATORBIT)
#define OPEN_CT_TOKEN (7+SEPARATORBIT)
#define CLOSE_TOKEN (8+SEPARATORBIT)
#define OPEN_LIST_TOKEN (9+SEPARATORBIT)
#define CLOSE_LIST_TOKEN (10+SEPARATORBIT)
#define OPEN_CURLY_TOKEN (11+SEPARATORBIT)
#define CLOSE_CURLY_TOKEN (12+SEPARATORBIT)
#define HEAD_TAIL_SEPARATOR_TOKEN (13+SEPARATORBIT)
#define COMMA_TOKEN (14+SEPARATORBIT)
#define END_TOKEN 15

#define TERMTYPE 40 /* meta token */

#define separator_token(X) ((X) & SEPARATORBIT)

#define no_term_token(X) (separator_token(X) || (X == END_TOKEN) || \
                         (X == CHAR_CODE_LIST_TOKEN))

#define LOOKAHEAD_CHAR ((bufferptr >= bufferend) ? \
          lookahead_char2() : *bufferptr)

#define open_token(X) open_char(X)
#define close_token(X) close_char(X)
#define open_list_token(X) open_list_char(X)
#define close_list_token(X) close_list_char(X)
#define open_curly_token(X) open_curly_char(X)
#define close_curly_token(X) close_curly_char(X)
#define head_tail_separator_token(X) head_tail_separator_char(X)
#define comma_token(X) comma_char(X)
#define end_token(X) end_char(X)
#define semicolon_token(X) semicolon_char(X)
#define cut_token(X) cut_char(X)

#define MAXTOKENLENGTH 1024

extern unsigned char *tokenstring;
extern int integertoken;
extern double floattoken;
extern int parseerror;
extern int linenumber;
extern unsigned char *bufferend, *bufferptr, *inputbuffer;
extern unsigned int buffersize;
extern unsigned char chararr[];
extern int dataread_pos;
extern int dataread_pos_real;
extern int dataread_allocated;
extern char *dataread;

/* character definitions */

/* symbols for use in inittokenizer */

#define _PROLOGCHAR              1
#define _GRAPHIC_CHAR            2
#define _GRAPHIC_TOKEN_CHAR      4
#define _ALPHA_NUMERIC_CHAR      8
#define _HEXADECIMAL_DIGIT_CHAR 16
#define _SOLO_CHAR              32
#define _SYMBOLIC_CONTROL_CHAR  64
#define _LAYOUT_CHAR           128


#define PROLOGCHAR(X) ((GRAPHIC_CHAR(X)) || (ALPHA_NUMERIC_CHAR(X)) || \
                       (SOLO_CHAR(X)) || (LAYOUT_CHAR(X)) || (meta_char(X)))

#define GRAPHIC_CHAR(X)        ((X) == '#' || (X) == '$' || (X) == '&' || \
                                (X) == '*' || (X) == '+' || (X) == '-' || \
                                (X) == '.' || (X) == '/' || (X) == ':' || \
                                (X) == '<' || (X) == '=' || (X) == '>' || \
                                (X) == '?' || (X) == '@' || (X) == '^' || \
                                (X) == '~' || (extra_graphic_char(X)))

#define GRAPHIC_TOKEN_CHAR(X) (GRAPHIC_CHAR(X) || backslash_char(X))

#define ALPHA_NUMERIC_CHAR(X)  ((alpha_char(X)) || (decimal_digit_char(X)))

#define HEXADECIMAL_DIGIT_CHAR(X) (((X) >= '0' && (X) <= '9') || \
                                   ((X) >= 'A' && (X) <= 'F') || \
                                   ((X) >= 'a' && (X) <= 'f'))

#define SOLO_CHAR(X) ((cut_char(X)) || \
                      (open_char(X)) || \
                      (close_char(X)) || \
                      (comma_char(X)) || \
                      (semicolon_char(X)) || \
                      (open_list_char(X)) || \
                      (close_list_char(X)) || \
                      (open_curly_char(X)) || \
                      (close_curly_char(X)) || \
                      (head_tail_separator_char(X)) || \
                      (end_line_comment_char(X)) || \
                      (extra_solo_char(X)))

#define SYMBOLIC_CONTROL_CHAR(X) (symbolic_alert_char(X) || \
                                  symbolic_backspace_char(X) || \
                                  symbolic_form_feed_char(X) || \
                                  symbolic_new_line_char(X) || \
                                  symbolic_carriage_return_char(X) || \
                                  symbolic_horizontal_tab_char(X) || \
                                  symbolic_vertical_tab_char(X))

#define LAYOUT_CHAR(X) ((space_char(X)) || \
                        (new_line_char(X)) || \
                        (extra_layout_char(X)))


#define prologchar(X) (chararr[X] & _PROLOGCHAR)

#define graphic_char(X) (chararr[X] & _GRAPHIC_CHAR)

#define graphic_token_char(X) (chararr[X] & _GRAPHIC_TOKEN_CHAR)

#define alpha_numeric_char(X)  (chararr[X] & _ALPHA_NUMERIC_CHAR)

#define alpha_char(X)          ((underscore_char(X)) || (letter_char(X)))
#define letter_char(X)         ((capital_letter_char(X)) || (small_letter_char(X)))
#define small_letter_char(X)   (((char)(X) >= 'a' && (char)(X) <= 'z') || \
                                extra_small_letter_char(X))
#define capital_letter_char(X) (((char)(X) >= 'A' && (char)(X) <= 'Z') || \
                                extra_capital_letter_char(X))
#define decimal_digit_char(X)  ((char)(X) >= '0' && (char)(X) <= '9')
#define binary_digit_char(X)   ((char)(X) >= '0' && (char)(X) <= '1')
#define octal_digit_char(X)    ((char)(X) >= '0' && (char)(X) <= '7')

#define hexadecimal_digit_char(X) (chararr[X] & _HEXADECIMAL_DIGIT_CHAR)

#define underscore_char(X)     ((X) == '_')

#define solo_char(X) (chararr[X] & _SOLO_CHAR)

#define cut_char(X)                 ((X) == '!')
#define open_char(X)                ((X) == '(')
#define close_char(X)               ((X) == ')')
#define comma_char(X)               ((X) == ',')
#define semicolon_char(X)           ((X) == ';')
#define open_list_char(X)           ((X) == '[')
#define close_list_char(X)          ((X) == ']')
#define open_curly_char(X)          ((X) == '{')
#define close_curly_char(X)         ((X) == '}')
#define head_tail_separator_char(X) ((X) == '|')
#define end_line_comment_char(X)    ((X) == '%')

#define eof_char(X)                 ((X)==0)

#define layout_char(X) (chararr[X] & _LAYOUT_CHAR)

#define space_char(X)        ((X) == ' ')
#define new_line_char(X)     ((X) == 10) /* implementation dependent */

#define meta_char(X)   ((backslash_char(X)) ||  \
                        (single_quote_char(X)) || \
                        (double_quote_char(X)) || \
                        (back_quote_char(X)))

#define backslash_char(X)    ((X) == '\\')
#define single_quote_char(X) ((X) == '\'')
#define double_quote_char(X) ((X) == '"')
#define back_quote_char(X)   ((X) == '`')

#define comment_1_char(X) ((X) == '/')
#define comment_2_char(X) ((X) == '*') 

#define end_char(X) ((X) == '.')

#define symbolic_alert_char(X)           ((X) == 'a')
#define symbolic_backspace_char(X)       ((X) == 'b')
#define symbolic_form_feed_char(X)       ((X) == 'f')
#define symbolic_new_line_char(X)        ((X) == 'n')
#define symbolic_carriage_return_char(X) ((X) == 'r')
#define symbolic_horizontal_tab_char(X)  ((X) == 't')
#define symbolic_vertical_tab_char(X)    ((X) == 'v')
#define symbolic_hexadecimal_char(X)     ((X) == 'x')

#define symbolic_control_char(X) (chararr[X] & _SYMBOLIC_CONTROL_CHAR)

#define positive_sign_char(X) ((X) == '+')
#define negative_sign_char(X) ((X) == '-')
#define sign_char(X)          (positive_sign_char(X) || negative_sign_char(X))
#define decimal_point_char(X) ((X) == '.')
#define exponent_char(X)      ((X) == 'e' || (X) == 'E')

#define variable_indicator_char(X) underscore_char(X)


/* error flags */

#define ERR_NOERROR 0
#define ERR_MISSING_QUOTE 1
#define ERR_TOKEN_TOO_LONG 2
#define ERR_UNEXPECTED_EOF 3
#define ERR_BACK_QUOTED_STRING 4
#define ERR_NONDET 5
#define ERR_INCOMPLETE_REDUCTION 6
#define ERR_INVALID_CHAR  7

#define is_latin1_upper(X) ((((X) >= 192) && ((X) <= 214)) || \
                           (((X) >= 216) && ((X) <= 222)))
#define is_latin1_lower(X) ((((X) >= 223) && ((X) <= 246)) || \
                            (((X) >= 248) /* && ((X) <= 255)*/ ))


#define extra_graphic_char(X) ((((X) >= 161) && ((X) <= 191)) || ((X) == '`'))
#define extra_small_letter_char(X) (is_latin1_lower(X))
#define extra_capital_letter_char(X) (is_latin1_upper(X))
#define extra_solo_char(X) (0)
#define extra_layout_char(X) ((carriage_return_char(X)) || \
                              (tab_char(X)) || \
                              (formfeed_char(X)) || \
                              (vertab_char(X)))

#define carriage_return_char(X) ((X) == 13)  /* ascii */
#define tab_char(X)             ((X) == 9)   /* ascii */
#define formfeed_char(X)        ((X) == 12)  /* ascii */
#define vertab_char(X)          ((X) == 11)  /* ascii */


int lookahead_char2(void);
int lookahead_char2_ext(struct file_struct *fs);
void seterr(int);
int token(void);
void showerror(void);
int advance_char(void);
int advance_char_ext(struct file_struct* fs);


#endif
