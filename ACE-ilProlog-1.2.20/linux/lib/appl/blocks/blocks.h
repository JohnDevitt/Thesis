#ifndef BLOCKS_INC
#define BLOCKS_INC


 

#include <math.h>
#include <vector>
#include "../../gapic/gapicgc.h"
 

 

    


typedef vector<int> BlocksTower;



typedef enum {
  BLOCKS_GOAL_STACK,
  BLOCKS_GOAL_UNSTACK,
  BLOCKS_GOAL_ON
} BlocksGoalMain;

class BlocksTerm {
 protected:
  vector<int> m_args;
 public:
  ostream& write(ostream& os) const {
    os <<"[";
    for(int i=0;i<m_args.size();i++) os << (i==0?"":",") << m_args[i]; 
    return os << "]";
  }
  int lastBlockID() const;
  int getArgCount() const {return m_args.size();}
  int getArg(int i) const {return m_args[i];}
};

class BlocksGoal  : public BlocksTerm {
 private:
  BlocksGoalMain m_main;
 public:
  ostream& write(ostream& os) const {
    os<< "goal"; return BlocksTerm::write(os);}
  BlocksGoalMain getMain() const {return m_main;}
  BlocksGoal(GAPIC_term_ref src) throw(GapicObjectCreationError);
};

class BlocksAction : public BlocksTerm {
 public:
  ostream& write(ostream& os) const {
    os<< "action"; return BlocksTerm::write(os);}
  BlocksAction(GAPIC_term_ref src) throw(GapicObjectCreationError);
};

class BlocksState {
 private:
  vector<BlocksTower> m_blocks;
  BlocksGoal *m_goal;
 public:
  ostream& write(ostream& os) const {
    os<<"state{";m_goal->write(os);os << ",blocks";
    for(int i=0;i<m_blocks.size();i++) { 
      os << "[";
      for(int j=0;j<m_blocks[i].size();j++)
	os<<((j==0)?"":",")<<m_blocks[i][j];
      os << "]";
    }
    os<<"}";
    return os;
  }
  const vector<BlocksTower>& getBlocks() const {return m_blocks;}
  int lastBlockID() const;
  const BlocksGoal* getGoal() const {return m_goal;}
  BlocksState(GAPIC_term_ref src) throw(GapicObjectCreationError);
  ~BlocksState() {delete m_goal;}
};

class BlocksStateAction {
 private:
  BlocksState *m_state;
  BlocksAction *m_action;
 public:
  ostream& write(ostream& os) const {
    os<<"[";m_state->write(os);m_action->write(os);return os<<"]";}
  const BlocksState* getState() const {return m_state;}
  const BlocksAction* getAction() const {return m_action;}
  int lastBlockID() const;
  void mapToCanonical(vector<int>& idmap) const;
  BlocksStateAction(GAPIC_term_ref src) throw(GapicObjectCreationError);
  ~BlocksStateAction() {delete m_state; delete m_action;}
}; 

typedef vector<int> BlockIdMap;


class BlocksDistance {
 private:
  double m_goal_weight;
  double m_action_main_weight;
  double m_action_weight;
  int mapab(int* map_a,int* map_b,int id_a,int id_b, int& lastmap) const;
  void init() {
    m_goal_weight=2.0;m_action_main_weight=5.0;m_action_weight=2.0;}
 public:
  ostream& Print(ostream& os) const {os << "[BlocksDistance]";}
	void compute(GAPIC_term_ref blocks_a, GAPIC_term_ref blocks_b, GAPIC_float* dist) const;

  BlocksDistance(GAPIC_term_ref src) throw(GapicObjectCreationError);
  BlocksDistance() {init();}
};

extern gapic_type GapicBlocksDistanceType;
typedef GapicCollectable<BlocksDistance,&GapicBlocksDistanceType> 
	BlocksDistanceCollectable;


  

	
class BlocksArray {
 private:
  void getOnArrays(GAPIC_term_ref, int, int, int*, int* ) const;
  void getArrayLength(GAPIC_term_ref, int*, int* ) const;
  void getActionGoalArray(GAPIC_term_ref, GAPIC_term_ref, int* ) const;
 public:
  int *upArray;
  int *downArray;
  int *actionGoalArray;
  int *labelArray;
  int num_edges;
  int num_vertices;
  BlocksArray( );
  BlocksArray( int, int, int*, int*, int*, int* );
  BlocksArray( GAPIC_term_ref );
  ~BlocksArray( ){ delete[] upArray;
		   delete[] downArray;
		   delete[] actionGoalArray;
		   delete[] labelArray;}
};

                               
class InterfaceBlocksKernel {
 public:
	void compute(GAPIC_term_ref blocks_a, GAPIC_term_ref blocks_b, GAPIC_float* kernel);  

  virtual double kernel( BlocksArray*, BlocksArray* ) = 0;
  ostream& Print(ostream& os) const {os << "[BlocksKernelInterface]";}
};

class AbstractBlocksKernel : public InterfaceBlocksKernel{
 protected:
  int *productadjlist;
  int *outdegree;     
  int *tmpnumwalks; 
  static const int max_depth = 13;       
  int mvs, mos;
  void init();
  void makeArrays();
  void checkArrays( int, int, int, int );
  void print( int dim_12 );
  int tnwI( int, int ) const;
  int adjI( int, int ) const;
  int labelMatch( int,int,int[],int[] ) const; 
  virtual void clean();
  virtual double compute_kernel( int );
  virtual void add_goalAction( int, int, int*, int* );
  virtual void make_product( int, int, int, int, int*, int*, int*, int*, int*, int*, int );
  virtual double weight( int ) = 0;
  virtual double kernel( int, int, int, int, int*, int*, int*, int*, int*, int*, int*, int* );
 public:                        
  virtual double kernel( BlocksArray*, BlocksArray* );
};                              

class GeomBlocksKernel : public AbstractBlocksKernel{
 public:                        
    GeomBlocksKernel(GAPIC_term_ref) throw(GapicObjectCreationError);
    GeomBlocksKernel( double p ){ parameter = p; init(); };
    virtual ~GeomBlocksKernel(){ clean(); };
 protected:
    void init3();
    double parameter;      
    virtual double weight( int );
};

class ExpBlocksKernel : public AbstractBlocksKernel{
 public:
    ExpBlocksKernel(GAPIC_term_ref) throw(GapicObjectCreationError);
    ExpBlocksKernel( double p ){ parameter = p; init(); };
    virtual ~ExpBlocksKernel(){ clean(); };
 protected:
    double parameter;      
    virtual double weight( int );
};

class RawBlocksKernel : public AbstractBlocksKernel {
 public:
    RawBlocksKernel(GAPIC_term_ref) throw(GapicObjectCreationError);
    RawBlocksKernel(){ init(); };
    virtual ~RawBlocksKernel(){ clean(); };
 protected:
    virtual double weight( int ) { return 1.0; } ;
};

class rbfWrapper : public InterfaceBlocksKernel{
 protected:
    double parameter;      
    InterfaceBlocksKernel* abk;
 public:
    rbfWrapper() {};
    rbfWrapper( InterfaceBlocksKernel*, double );
    virtual ~rbfWrapper() { };
    virtual double kernel( BlocksArray*, BlocksArray* );
};

class normalWrapper : public InterfaceBlocksKernel{
 protected:
    InterfaceBlocksKernel* abk;
 public:
    normalWrapper() {};
    normalWrapper( InterfaceBlocksKernel* );
    virtual ~normalWrapper() { };
    virtual double kernel( BlocksArray*, BlocksArray* );
};


class ExpRBFWrapped : public rbfWrapper{
 public:
    ExpRBFWrapped( GAPIC_term_ref term ) throw(GapicObjectCreationError) {
        GAPIC_term_ref item=GAPIC_new_term_ref();
	abk = new ExpBlocksKernel(term);
        GAPIC_get_list(term,item,term);
        GAPIC_get_float(item,&parameter);
	std::cout << "** RBFWrapper par=" << parameter << std::endl;
    };
    virtual ~ExpRBFWrapped() { delete abk; };
};

class GeomRBFWrapped : public rbfWrapper{
 public:
    GeomRBFWrapped( GAPIC_term_ref term ) throw(GapicObjectCreationError){
	abk = new GeomBlocksKernel(term);
	parameter = 1.0;
	std::cout << " ** rbf wrapper par=" << parameter << std::endl;
    };
    virtual ~GeomRBFWrapped() { delete abk; };
};

class ExpNormWrapped : public normalWrapper{
 public:
    ExpNormWrapped( GAPIC_term_ref term ) throw(GapicObjectCreationError) {
	abk = new ExpBlocksKernel(term);
	std::cout << " ** normal wrapper " << std::endl;
    };
    virtual ~ExpNormWrapped() { delete abk; };
};

class GeomNormWrapped : public normalWrapper{
 public:
    GeomNormWrapped( GAPIC_term_ref term ) throw(GapicObjectCreationError){
	abk = new GeomBlocksKernel(term);
	std::cout << " ** normal wrapper " << std::endl;
    };
    virtual ~GeomNormWrapped() { delete abk; };
};

class distWrapper{
 protected:
    InterfaceBlocksKernel* abk;
 public:
    distWrapper( InterfaceBlocksKernel* );
    virtual ~distWrapper() { };
    virtual double dist( BlocksArray*, BlocksArray* );
};

extern gapic_type InterfaceBlocksKernelType;
extern gapic_type GeomBlocksKernelType;
extern gapic_type RawBlocksKernelType;
extern gapic_type ExpBlocksKernelType;
extern gapic_type ExpRBFWrappedType;
extern gapic_type GeomRBFWrappedType;
typedef GapicNonExMutCollectable<InterfaceBlocksKernel,&InterfaceBlocksKernelType> 
	InterfaceBlocksKernelCollectable;


class BlocksKernMetric_ExpRBFWrapped {
 private:
  ExpRBFWrapped m_kernel;
 public:
  ostream& Print(ostream& os) const {os << "[BlocksDistance]";}
	void compute(GAPIC_term_ref blocks_a, GAPIC_term_ref blocks_b, GAPIC_float* dist);

  BlocksKernMetric_ExpRBFWrapped(GAPIC_term_ref src) throw(GapicObjectCreationError) : m_kernel(src) {}
};

extern gapic_type GapicBlocksKernMetric_ExpRBFWrappedType;
typedef GapicNonExMutCollectable<BlocksKernMetric_ExpRBFWrapped ,
  &GapicBlocksKernMetric_ExpRBFWrappedType> 
	BlocksKernMetric_ExpRBFWrappedCollectable;

inline void BlocksKernMetric_ExpRBFWrapped::
compute(GAPIC_term_ref blocks_a,GAPIC_term_ref blocks_b,
	double *dist) {
  GAPIC_float kab,kaa,kbb;
  m_kernel.compute(blocks_a,blocks_a,&kaa);
  m_kernel.compute(blocks_a,blocks_b,&kab);
  m_kernel.compute(blocks_b,blocks_b,&kbb);
  *dist=sqrt(kaa+kbb-(2*kab));
} 

#endif

