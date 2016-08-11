#ifndef CSMAIN_INC
#define CSMAIN_INC

void  citeseer_preproc_1(const char *indir,const char *outdir);
void citeseer_load_1(const char*dir);
void citeseer_stats_1(int *docs, int *words, int *authors);
void citeseer_printdoc_1(int docid);
int citeseer_has_graph_info_1(int did);
int citeseer_has_word_info_1(int did) ;
int citeseer_cite_count_1(int did, int *cite_count) ;
int citeseer_get_cite_1(int did, int i, int *cite_did, int *cite_gid);
int citeseer_get_year_1(int did, int *year);
int citeseer_author_count_1(int did, int *author_count);
int citeseer_get_author_1(int did, int i, const char **author_name);
int citeseer_get_author_id_1(int did, int i, int* authorid);
int citeseer_title_length_1(int did, int *title_length);
int citeseer_get_title_word_1(int did, int i, const char **title_word);
int citeseer_get_key_1(int did, const char **key);
int citeseer_word_count_1(int did, int *word_count);
int citeseer_get_word_1(int did, int i, const char **docword);


void citeseer_get_authors_1(int docid, int* authors);
int citeseer_get_nb_authors_1();
int citeseer_id2author_1(int idx, const char **name);
void citeseer_print_author_1(int idx);
double citeseer_author_distance_1(int i1, int i2);
void citeseer_last_did_1(int *did);

#endif
