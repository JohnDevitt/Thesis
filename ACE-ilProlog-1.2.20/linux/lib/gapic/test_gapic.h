
       


#ifndef TEST_GAPIC_INC
#define TEST_GAPIC_INC

#include "../gapic/gapicgc.h"

       
 	
		
      
 	
 	  
              
 	
 	  

      
extern gapic_type GapicHelloworldType;

class GapicHelloworld {
private:
	GAPIC_atom* data;
public:
	             
	GapicHelloworld();
	GapicHelloworld(GAPIC_term_ref term) throw(GapicObjectCreationError);
	      
	~GapicHelloworld();
	   
	ostream& Print(ostream& os) const;
	        
	void setData(const GAPIC_atom& atom);

};

       
class GapicHelloworldCollectable : public GapicNonExMutCollectable<GapicHelloworld, &GapicHelloworldType> {
public:
	GapicHelloworldCollectable(GapicHelloworld *hello) :
		GapicNonExMutCollectable<GapicHelloworld, &GapicHelloworldType>(hello) {};
};

#endif
