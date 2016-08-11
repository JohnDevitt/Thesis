

#ifndef KBR_INC
#define KBR_INC



#include "gapic/gapicgc.h"
#include <iostream>
#include <fstream>

#include "ckbr.h"

class KBRTheory : public CKBRTheory {
 public:
	void getBaseExamples(GAPIC_term_ref base) {

    safe_int_vect cb;
    CKBRTheory::getBaseExamples(cb);
    GAPIC_term_ref list=GAPIC_new_term_ref();
    GAPIC_put_atom(list,get_GAPIC_atom_nil_0());
    GAPIC_term_ref item=GAPIC_new_term_ref();
    for(int i=0;i<cb.size();i++) {
      GAPIC_put_integer(item,cb[i]);
      GAPIC_put_list(list,item,list);
    }
    GAPIC_put_term(base,list);
  }
	void addElement(GAPIC_float val, GAPIC_int* id) {

    CKBRTheory::addElement(val,id);}
	void finalizeAdd(GAPIC_int* keep) {

    *keep=CKBRTheory::finalizeAdd();}
	void retractAdd(GAPIC_int id) {

    CKBRTheory::retractAdd(id);}
	void delElement(GAPIC_int id) {

    CKBRTheory::delElement(id);}
	void storeKernelValue(GAPIC_int i, GAPIC_int j, GAPIC_float val) { 

    CKBRTheory::storeKernelValue(i,j,val);}
	void getKernelValue(GAPIC_int i, GAPIC_int j, GAPIC_float* v) {

    CKBRTheory::getKernelValue(i,j,v);}
	GAPIC_result isActive(GAPIC_int i) const {

    if(CKBRTheory::isActive(i)) return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	void maxPredict(GAPIC_int id, GAPIC_float* value) {

    CKBRTheory::maxPredict(id,value);}
	void probPredict(GAPIC_int id, GAPIC_float value, GAPIC_float* prob) {

    CKBRTheory::probPredict(id,value,prob);}
	void getDim(GAPIC_int* dim) const {

    CKBRTheory::getDim(dim);}
	void getValue(GAPIC_int id, GAPIC_float* value) {

    CKBRTheory::getValue(id,value);}
};

extern gapic_type GapicKBRTheoryType;
typedef GapicNonExMutCollectable<KBRTheory,&GapicKBRTheoryType> 
	KBRTheoryCollectable;

#endif
 
