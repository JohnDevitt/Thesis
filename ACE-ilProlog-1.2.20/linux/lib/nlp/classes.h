#ifndef CLASSES_INC
#define CLASSES_INC

#include <vector.h>
#include "const.h"

 

class GroundNodeStruct;
class GroundNodeData;
class GroundNode;
class GroundLink;
typedef vector<GroundLink> GroundLinkVector;
typedef vector<GroundLink> GroundLinkList;

typedef GroundNode* GroundNodePtr;
typedef vector<GroundNodePtr> GroundNodePtrVector;

class NlpOrder;

 

class NlpClause;
class NlpPredDef;
typedef NlpClause* NlpClausePtr;
typedef NlpPredDef* NlpPredDefPtr;
typedef vector<NlpClausePtr> NlpClauseVector;
typedef vector<NlpPredDefPtr> NlpPredDefVector;
class NlpWeight;
class InputNlpWeight;
class ConNlpWeight;
class DisNlpWeight;
typedef NlpWeight *NlpWeightPtr;
class nlp_c_net;
class nlp_c_ground_net;
class NlpWeights;

 

class CombFunction;
class ClauseAdder;
class Multiplier;
class GroundLink;
typedef CombFunction *CombFunctionPtr;


#endif
