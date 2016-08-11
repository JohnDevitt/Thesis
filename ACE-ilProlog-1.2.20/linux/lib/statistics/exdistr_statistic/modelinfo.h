


#ifndef MODELINFO_INC
#define MODELINFO_INC

#include "../../gapic/gapicgc.h"

extern gapic_type ModelInfoType;
extern gapic_type MICompoundType;


  
	 
	 
 


class ModelInfo {
public:
	ostream& Print(ostream& os) const;
	virtual void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref info) = 0;
	          
};

typedef ModelInfo* MIPtr;

class MICompound {
protected:
	GAPIC_int m_WCountIdx;
	GAPIC_int m_NbChildren;
	MIPtr* m_Children;
public:
	MICompound(GAPIC_term_ref children);
	virtual ~MICompound();
	void initialize(GAPIC_term_ref children);
	ostream& Print(ostream& os) const;	
	void get_info(ExdistrStatistic* tot_s, GAPIC_term_ref fct);

};

typedef GapicNonExMutCollectable<ModelInfo, &ModelInfoType> ModelInfoCollectable;
typedef GapicNonExMutCollectable<MICompound, &MICompoundType> MICompoundCollectable;

#endif


     
     
         
    
 
 
