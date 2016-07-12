#ifndef GOSOLVER_INC
#define GOSOLVER_INC

   
 

 


#include <gosolverso.h>
#include "../gapic/gapicgc.h"
#include "../gapic/refcount.h"
 
 
class GosolverCompressedPosition;
class GosolverPosition;
class GosolverCompressedImage;


class GosolverCompressedPosition : public RefCountObject {
 public:
  virtual void AbsoluteActivate() const=0;
  virtual void IncrementalActivate(int delta_depth) const=0;
  virtual const GosolverCompressedImage* RootAncestor() const=0; 
  virtual int GetBoardSize() const=0;
  virtual int Depth() const=0;
  virtual void DoActivate() const=0;
  virtual ostream& Print(ostream& os) const=0;
};            
   
typedef RefCountPtr<GosolverCompressedPosition> GosolverCompressedPositionPtr;
  
class GosolverCompressedPath : public GosolverCompressedPosition {
 private:
  static GAPIC_functor child_functor;
  int move;
  GosolverCompressedPositionPtr parent;
 public: 
  virtual void AbsoluteActivate() const;
  virtual void IncrementalActivate(int delta_depth) const;
  virtual const GosolverCompressedImage* RootAncestor() const;
  virtual int GetBoardSize() const;
  virtual void DoActivate() const;
  virtual int Depth() const;
  virtual ostream& Print(ostream& os) const;
  GosolverCompressedPath(int _move,
			 GosolverCompressedPositionPtr _parent)
    : move(_move),parent(_parent) {}
  GosolverCompressedPath(GAPIC_term_ref)
    throw(GapicObjectCreationError);
};

class GosolverBoardImage {
 public:
  short int data[46];
};

class GosolverCompressedImage : public GosolverCompressedPosition {
 private:
  GosolverBoardImage *my_position;
  const short int *position_ptr;
  short int boardsize;
 public:
  inline const short int *GetPosition() const {
    if(my_position!=NULL) return my_position->data; else return position_ptr;}
  virtual void AbsoluteActivate() const;
  virtual void IncrementalActivate(int delta_depth) const;
  virtual const GosolverCompressedImage* RootAncestor() const;
  virtual void DoActivate() const;
  virtual int Depth() const;
  virtual ostream& Print(ostream& os) const;
  virtual int GetBoardSize() const;
  GosolverCompressedImage(const short int *_pos, int _boardsize)
    : position_ptr(_pos),my_position(NULL),boardsize(_boardsize) {}
  GosolverCompressedImage() 
    : boardsize(19),position_ptr(NULL) {
    my_position=new GosolverBoardImage();
    for(int i=0;i<46;i++) my_position->data[i]=0;}
  GosolverCompressedImage(GAPIC_term_ref)
    throw(GapicObjectCreationError);
  ~GosolverCompressedImage() {
    if(my_position!=NULL) 
      delete my_position;
  }
};

void daminc_check(); 
class GosolverPosition : public MutableRefCountObject {
 private:
  GosolverCompressedPositionPtr compressed;
  static GosolverPosition* active_position;
  int active;
  static void Pascal2list(int *pascal_string,GAPIC_term_ref list) {
    GAPIC_term_ref lst=GAPIC_new_term_ref();
    GAPIC_term_ref tail=GAPIC_new_term_ref();
    GAPIC_put_atom(lst,get_GAPIC_atom_nil_0());
    for(int i=1;i<=pascal_string[0];i++) {
      GAPIC_put_term(tail,lst);
      GAPIC_term_ref head=GAPIC_new_term_ref();
      GAPIC_put_integer(head,pascal_string[i]);
      GAPIC_put_list(lst,head,tail);
    }
    GAPIC_unify(list,lst);
  }
 public: 
  GosolverCompressedPositionPtr GetCompressed() const {
    return compressed;}
  static const GosolverCompressedPosition* 
    GetActiveCompressedPosition() {

    if((GosolverPosition::active_position)==NULL)
      return NULL;

    return (GosolverPosition::active_position)->compressed.Get();}
  void _Activate(); 
	void Activate() {


        
        
    if(!active) {_Activate();}
    

  }     
	void Depth(GAPIC_int* depth) const {


    *depth=compressed->Depth();}
	void Boardsize(GAPIC_int* bs) const {


    *bs=compressed->GetBoardSize();}
	void Intersection2string(GAPIC_int inter, GAPIC_int* string) {


    Activate();GosolverKernel::Intersection2string(inter,string);}
	GAPIC_result Relative_intersection(GAPIC_int src, GAPIC_int axes, GAPIC_int relx, GAPIC_int rely, GAPIC_int* dst) {


    Activate(); 
    if(GosolverKernel::Relative_intersection(src,axes,relx,rely,dst))
      return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	void unsafe_root_ancestor(GAPIC_int* root) {

    *root=(int)(compressed->RootAncestor());}
	void Axes_transform(GAPIC_int src, GAPIC_int axes, GAPIC_int* dst) {


    Activate();GosolverKernel::Axes_transform(axes,src,*dst);}
	void Axes_reverse_transform(GAPIC_int src, GAPIC_int axes, GAPIC_int* dst) {


    Activate();GosolverKernel::Axes_reverse_transform(axes,src,*dst);}
	void Show() {


    Activate();GosolverKernel::Show();}
	void Block_get_size(GAPIC_int block, GAPIC_int* size) {


    Activate();GosolverKernel::Block_get_size(block,size);}
	void Block_get_liberty_count(GAPIC_int block, GAPIC_int* libcnt) {


    Activate();GosolverKernel::Block_get_liberty_count(block,libcnt);}
	void Block_get_seclib_count(GAPIC_int block, GAPIC_int* seclibcnt) {


    Activate();GosolverKernel::Block_get_seclib_count(block,seclibcnt);}
	void Block_get_color(GAPIC_int block, GAPIC_int* color) {


    Activate();GosolverKernel::Block_get_color(block,color);}
	GAPIC_result Stone_get_block(GAPIC_int stone, GAPIC_int* block) {


    Activate();
    if(GosolverKernel::Stone_get_block(stone,block))
      return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	GAPIC_result Stone_in_block(GAPIC_int stone, GAPIC_int block) {


    Activate();
    if(GosolverKernel::Stone_in_block(stone,block))
      return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	void Block_get_stone(GAPIC_int block, GAPIC_int i, GAPIC_int* stone) {


    Activate();
    GosolverKernel::Block_get_stone(block,i,stone);
  }
	void Block_get_adjacent_count(GAPIC_int block, GAPIC_int* adj_count) {


    Activate();
    GosolverKernel::Block_get_adjacent_count(block,adj_count);
  }
	void Block_get_adjacent(GAPIC_int block, GAPIC_int i, GAPIC_int* adj) {


    Activate();
    GosolverKernel::Block_get_adjacent(block,i,adj);
  }
	GAPIC_result Blocks_are_adjacent(GAPIC_int b1, GAPIC_int b2) {


    Activate();
    if(GosolverKernel::Blocks_are_adjacent(b1,b2))
      return GAPIC_SUCCESS; else return GAPIC_FAIL; 
  }
	void Liberty_get_block(GAPIC_int liberty, GAPIC_term_ref blocklist) {


    int blocks[5];
    Activate();
    GosolverKernel::Liberty_get_block(liberty,blocks);
    Pascal2list(blocks,blocklist);}
	GAPIC_result Liberty_of_block(GAPIC_int liberty, GAPIC_int block) {


    Activate();
    if(GosolverKernel::Liberty_of_block(liberty,block))
      return GAPIC_SUCCESS; else return GAPIC_FAIL;}
	void Block_get_liberty(GAPIC_int block, GAPIC_int i, GAPIC_int* liberty) {


    Activate();
    GosolverKernel::Block_get_liberty(block,i,liberty);
  }
	void Block_get_seclib(GAPIC_int block, GAPIC_int i, GAPIC_int* seclib) {


    Activate();
    GosolverKernel::Block_get_seclib(block,i,seclib);
  }
	void Intersection_get_liberty_count(GAPIC_int inter, GAPIC_int* libcnt) {


    Activate();GosolverKernel::Intersection_get_liberty_count(inter,libcnt);}
	void Tree_eval(GAPIC_int inter, GAPIC_term_ref tokill_term, GAPIC_int depth, GAPIC_int side_to_move, GAPIC_int attacker, GAPIC_float* mean, GAPIC_float* variance) {

    Activate();
    short int tokill[300];
    GAPIC_term_ref tokill_el=GAPIC_new_term_ref();
    tokill[0]=0;
    while(GAPIC_is_list(tokill_term)) {
      GAPIC_int tmp;
      GAPIC_get_list(tokill_term,tokill_el,tokill_term);
      GAPIC_get_integer(tokill_el,&tmp);
      tokill[(tokill[0])++]=tmp;
    }
    DectreeResultList mv;
    eval_order(inter, tokill, depth, side_to_move, attacker, 
	       &mv);
    mean[0]=mv.get_mean()[0];
    variance[0]=mv.get_var()[0];
  }
  ostream& Print(ostream& os) const;
  GosolverPosition(int move,GosolverPosition *parent) 
    : active(0),
    compressed(new GosolverCompressedPath(move,parent->GetCompressed())) {}
  GosolverPosition(const short int *img,int boardsize) 
    : active(0),compressed(new GosolverCompressedImage(img,boardsize)) {}
  GosolverPosition() : active(0), compressed(new GosolverCompressedImage()) {}
  GosolverPosition(GAPIC_term_ref src) 
    throw(GapicObjectCreationError)
    : active(0),
    compressed((get_GAPIC_functor_conj_2().CmpGenTerm(src)
		==GAPIC_SUCCESS)?
	       (GosolverCompressedPositionPtr)new GosolverCompressedImage(src):
	       (GosolverCompressedPositionPtr)new GosolverCompressedPath(src))
    {}
};

extern gapic_type GapicGosolverPositionType;
typedef GapicNonExMutCollectable<GosolverPosition,&GapicGosolverPositionType> 
	GosolverPositionCollectable;

#endif

