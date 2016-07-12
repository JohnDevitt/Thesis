#define  MAXCTABLE 32768
#include <stdio.h>
#include <stdlib.h>

#define BITSPERCHAR 7
#define BITOFFSET 32

typedef struct compress_info_type {
  unsigned int nextIndex;
  unsigned int chainIndex;
} compress_info_type;

typedef struct compress_data_type {
  FILE *file_out;
  compress_info_type *table;
  unsigned int border; 
  unsigned int current_index;
  unsigned int max;
  unsigned char totalBits;
  unsigned char c;
  unsigned char bits;
} compress_data_type;


enum input_source {WFILE, INCORE, LIBINCORE};

enum input_encoding {READABLE, COMPACT, COMPRESSED};

typedef struct decompress_data_type {
  FILE *file_in;
  enum input_source source;
  enum input_encoding encoding;
  unsigned int *table;
  unsigned char *buffer;
  int lengthBuffer;
  int bufferIndex;
  unsigned char c; 
  unsigned char bits;
  unsigned char totalBits;
  unsigned int Border;
  unsigned int index;
  unsigned int max;
  unsigned int oldindex;
  dlong *S; /* for in_core loading */
} decompress_data_type;


static inline void init_compression(unsigned char c,  compress_data_type *x) {
  int i;
  compress_info_type *localtable = x->table; 
  for (i = 0; i<256; i++) localtable[i].nextIndex=0;
  x->c = 0;
  x->bits = 0;
  x->totalBits = 8;
  x->border = 256; 
  x->current_index = c;
  x->max = 255;
}

static inline int start_compression(FILE *F, unsigned char c, compress_data_type *x) {
  x -> table = (compress_info_type *)
                         malloc(MAXCTABLE * sizeof(compress_info_type));
  if ( !(x->table)) return (0);
  x->file_out = F;
  init_compression(c, x);
  return (1);
}

static inline void print_bits(compress_data_type *x) {
  unsigned int allBits, bitsTodo;
  FILE *F = x->file_out;
  allBits = x->current_index + ((x->c) << (x->totalBits));
  bitsTodo = x->bits + x->totalBits;
  while (bitsTodo >= BITSPERCHAR) {
    unsigned char c;
    bitsTodo -= BITSPERCHAR;
    c = allBits >> bitsTodo;
    fputc(c+BITOFFSET, F);
    allBits -= (c << bitsTodo); 
  }
  x->c = allBits;
  x->bits = bitsTodo;
}

static inline void print_last_bits(compress_data_type *x) {
  if (x->bits > 0) {
    unsigned char c;
    c = (x->c) << (BITSPERCHAR - x->bits);
    fputc(c+BITOFFSET, x->file_out);
  }
}

static inline void stop_compression(compress_data_type *x) {
  free(x -> table);
  print_bits(x);
  print_last_bits(x);
}

static inline void compress_char(unsigned char c, compress_data_type *x ) {
  unsigned int index = x -> current_index;
  unsigned int i = x -> table[index].nextIndex;
  unsigned int prev = 0;
  compress_info_type *localtable = x->table; 
  while (i != 0) {
     unsigned int j = localtable[i].chainIndex;
     if ((j >> 24) == c) break;
     else { prev = i; i = j & 0xffffff; }
  }
  if (i==0) {
     unsigned int j;
     x -> max ++;
     j = x -> max;
     print_bits(x);
     if (j == x->border) {
       x->border =  x->border << 1;
       x->totalBits ++;
     }
     if (prev == 0) localtable[index].nextIndex = j;
     else localtable[prev].chainIndex += j;
     if (x -> totalBits > 15) {
       print_last_bits(x);
       init_compression(c, x);
     } else {
       localtable[j].nextIndex = 0;
       localtable[j].chainIndex = c << 24; 
       x -> current_index = c;
     }
  } else x->current_index = i;
}

static inline void init_decompression(decompress_data_type *x){
  x -> c = 0;
  x -> bits = 0;
  x -> Border = 256;
  x -> totalBits = 8;
  x -> index = 0;
  x -> max = 255;
  x -> bufferIndex = -1;
  x -> oldindex = 0;
}

static inline dlong start_decompression(decompress_data_type *x) {
  x -> table = (unsigned int *) malloc(MAXCTABLE*sizeof(unsigned int*));
  if (!(x->table)) exit_mess("out of memory for decompression", 1);
  init_decompression(x);
  x -> lengthBuffer = 250;
  x -> buffer = (unsigned char *)malloc((unsigned) x -> lengthBuffer);
  if (!(x->buffer)) exit_mess("out of memory for decompression", 1);
  return ( (dlong) x -> buffer);
}

static inline void stop_decompression(decompress_data_type *x){
  free(x -> table);
  free(x -> buffer);
}

static inline int read_bits(decompress_data_type *x){
  unsigned char left;
  unsigned int index, nr_bits, current_bits;
  current_bits = x -> c;
  nr_bits = x -> bits;
  while (nr_bits <  x -> totalBits) {
    unsigned char c;
    if (x->source==WFILE) c = fgetc(x -> file_in) - BITOFFSET;
    else {
      c = ((char*)x -> file_in)[0] - BITOFFSET;
      x->file_in = (FILE*) (((char*) x->file_in) + 1);
    }
    current_bits = (current_bits << BITSPERCHAR) + c; 
    nr_bits += BITSPERCHAR;
  } 
  left = nr_bits - x -> totalBits;
  if (left > 0) {
    index = current_bits >> left;
    x -> c = current_bits - (index << left);
    x -> bits = left;
  } else {
    index = current_bits;
    x -> c = 0;
    x -> bits = 0;
  }
  if (x->source == WFILE) {
    if (feof(x->file_in)) {
      return (0);
    }
    else return (index);
  } else {
    return (index);
  }
}

static inline unsigned char decompress_ungetc(unsigned char c,
				       decompress_data_type *x) {
  return x->buffer[++(x->bufferIndex)] = c;
}

static inline unsigned char decompress_char(decompress_data_type *x) {
  unsigned int index, oldindex, reserve, freeInBuffer;
  unsigned char* bufferptr;
  unsigned int *table = x -> table;
  if (x -> bufferIndex < 0) { /* we have to read a new index and
                                 fill the buffer again with characters */ 
    if (x ->totalBits > 15) {
      init_decompression(x);
    }
    index = read_bits(x);
    if (!index) return (255);
    bufferptr = x->buffer; 
    freeInBuffer = x ->lengthBuffer;
    oldindex = x -> oldindex;
    x -> oldindex = index;
    if (index == x -> max) { 
         /* The new string is the same as the last + one character */
      reserve = 1;
      index = oldindex;
      bufferptr++;
      freeInBuffer--;
    } else  reserve = 0;
    while (index > 255){
      *bufferptr = table[index] >> 24;
      bufferptr++;
      freeInBuffer--;
      if (!freeInBuffer) {
	freeInBuffer = x -> lengthBuffer;
	x -> lengthBuffer *= 2; 
	realloc(x->buffer, sizeof(char) * x -> lengthBuffer);
      } 
      index = table[index] & 0xffffff;
    };
    *bufferptr = index; /* the last character */
    if (reserve) *(x-> buffer) = index;
    x -> bufferIndex =  x -> lengthBuffer - freeInBuffer;
    if (oldindex) {
      x -> table[x->max] = (index << 24) + oldindex;
    }
    x -> max ++;
    if (x-> max == x->Border) {
      x->totalBits ++;
      x->Border <<= 1;
    }
  } 
  return (x->buffer[x->bufferIndex--]);
}





