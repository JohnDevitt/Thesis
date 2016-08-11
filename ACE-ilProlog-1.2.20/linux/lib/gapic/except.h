
#ifndef GAPIC_EXCEPT_INC
#define GAPIC_EXCEPT_INC

#include <string.h>

 

class GapicObjectCreationError {
 private:
  char msg[1024];
 public:
  const char* GetMsg() const {return msg;}
  GapicObjectCreationError(const char* _msg,
			   GapicObjectCreationError& src) {
    strcpy(msg,_msg);strcat(msg,src.GetMsg());}
  GapicObjectCreationError(const char* _msg,
			   const char* src) {
    strcpy(msg,_msg);strcat(msg,src);}
  GapicObjectCreationError(const char* _msg) {strcpy(msg,_msg);};
  GapicObjectCreationError() {strcpy(msg,"");};
};

#endif
