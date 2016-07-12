


#ifndef CLASS_RANDOMISATION_INC
#define CLASS_RANDOMISATION_INC
 
extern gapic_type ClassRandomisationType;
#include "../../math/matrix/matrix.h"

  

class ClassRandomisation {
protected:
public:
	ostream& Print(ostream& os) const {return os;}
	int get_perm_ex_nr(unsigned ex_nr, unsigned rand_nr) const;
	void make_table();

	void forget_table();

	void make_ex(unsigned* ex_nr);

	          
	void forget_ex(unsigned* ex_nr);

	ClassRandomisation() {}
};

typedef GapicNonExMutCollectable<ClassRandomisation, &ClassRandomisationType> ClassRandomisationCollectable;

#endif
