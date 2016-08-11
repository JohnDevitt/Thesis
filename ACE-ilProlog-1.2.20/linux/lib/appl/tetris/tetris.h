#ifndef TETRIS_INC
#define TETRIS_INC


 
#include <cstdlib>
#include <iostream>
#include <math.h>
using namespace std;
#include "../../gapic/gapicgc.h"
#include "../../gapic/refcount.h"

class TetrisEnvironment;
class TetrisState;
class TetrisAction;
extern const char blockshape[7][4][4][4];

class TetrisEnvironment {
 private:
  int make_action_nr(int col, int row, int orient) {
    return orient+(4*(col+(10*row)));
  }
  int action_nr_row(int nr) {return nr/40;}
  int action_nr_orient(int nr) {return nr%4;}
  int action_nr_col(int nr) {return (nr/4)%10;}
 public:
  ostream& Print(ostream& os) const {
    os << "This is the Tetris Environment.\n";
  }
	void beginState(GAPIC_term_ref wt, GAPIC_term_ref state);

	void beginState2(GAPIC_term_ref wt, GAPIC_term_ref state);

	void possibleActions(GAPIC_term_ref wt, GAPIC_term_ref state, GAPIC_term_ref action_list);

	void apply(GAPIC_term_ref wt, GAPIC_term_ref state, GAPIC_term_ref action, GAPIC_term_ref next_state, GAPIC_float* reward);

	GAPIC_result stopCondition(GAPIC_term_ref wt, GAPIC_term_ref state);

	void getActionFromTeacher(GAPIC_term_ref wt, GAPIC_term_ref state, GAPIC_term_ref action);

};
  
extern gapic_type GapicTetrisEnvironmentType;
typedef GapicNonExMutCollectable<TetrisEnvironment,
  &GapicTetrisEnvironmentType> TetrisEnvironmentCollectable;


class TetrisState {
  private:
    char field[21][10];
    int fall_type;
    int fall_orient;
    int next_type;
    void deleteRow(int row);
  public:
    TetrisState(const TetrisState& state) {
      for (int i=0; i<21; i++)
	for (int j=0; j<10; j++)
	  this->field[i][j] = state.getSquare(i,j);
      this->fall_type = state.getFallType();
      this->next_type = state.getNextType();
    };
    TetrisState() {
      for (int row=0;row<21;row++)
	for(int col=0;col<10;col++)
	  this->field[row][col] = 0;
      this->fall_type = int(7.0 * rand()/(RAND_MAX + 1.0));
      this->next_type = int(7.0 * rand()/(RAND_MAX + 1.0));
    };
    char getSquare(int row, int col) const {
      if (row>-1 && row<21 && col>-1 && col<10) 
	return field[row][col];
      else return 0;
    }  
    void setSquare(int row, int col, char value) {
      if (row>-1 && row<21 && col>-1 && col<10) 
	field[row][col] = value;
    }
    int getFallType() const {return fall_type;};
    int getNextType() const {return next_type;};
    float apply(TetrisAction* action);
    int canDropAt(int col, int row, int orient) const;
    int canFitAt(int col, int row, int orient) const;
    int hasSupportAt(int col, int row, int orient) const;
    int topBlock(int col) const;
    int topBlock0(int col) const;
    int holeInCol(int col) const;
    int countHoles(int col) const;
    int topReached() const {
      int reached = 0;
      for (int j=0; j<10; j++)  if (field[20][j]==1) reached=1;
      return reached;
    };
    ostream& Print(ostream& os) const {
      os << "Falling: \n";
      for (int i=1;i>=0;i--) {
	os << "        ";
        for (int j=0;j<4;j++)
	  os << ((blockshape[fall_type][0][i][j]==0)?" ":"X");
	  os << "\n";
      }
      for (int row=19;row>=0;row--) {
        os << row << ((row>9)?": |":" : |");  for (int col=0;col<10;col++)    os << ((field[row][col]==0)?" ":"X");
	  os << "|\n";
      }
      os << "Next: \n";
      for (int i=1;i>=0;i--) {
        for (int j=0;j<4;j++)
          os << ((blockshape[next_type][0][i][j]==0)?" ":"X");
          os << "\n";
      }
      os << "\n\n";
    };
	void getTopBlock(GAPIC_int col, GAPIC_int* row) const;

	void getMinHeight(GAPIC_int* row) const;

	void getMaxHeight(GAPIC_int* row) const;

	void getAvgHeight(GAPIC_int* row) const;

	void getMaxError(GAPIC_int orient, GAPIC_int col, GAPIC_int* error) const;

	void getBlockType(GAPIC_int* type) const;

	void getHoleDepth(GAPIC_int col, GAPIC_int* depth) const;

	void getNumberOfHoles(GAPIC_int* nb) const;

	void dropRow(GAPIC_int orient, GAPIC_int col, GAPIC_int* row) const;

	void lowestDropRow(GAPIC_int* row) const;

	void lowestFitsRow(GAPIC_int* row) const;

	GAPIC_result holePresent(GAPIC_int col) const;

	GAPIC_result holeDeleted(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result affectsCovered(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fits(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result increasesHeight(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result decreasesDepth(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fillsRow(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fillsDouble(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fillsTriple(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fillsTetris(GAPIC_int orient, GAPIC_int col) const;

	GAPIC_result fallingBlockWidth(GAPIC_int orient, GAPIC_int width) const;

	GAPIC_result fallingBlockHeight(GAPIC_int orient, GAPIC_int height) const;

};

extern gapic_type GapicTetrisStateType;
typedef GapicNonExMutCollectable<TetrisState,
  &GapicTetrisStateType> TetrisStateCollectable;

const int debug_tetris_kernel=0;

class TetrisDistance {
 private:
  double m_height_w;
  double m_maxheight_w;
  double m_avgheight_w;
  double m_holes_w;
  double m_hole_depth_w;
  double m_cell_w;
  double m_alarm_w;
  double m_diff_w;
  double m_sdiff_w;
  void getShape(const TetrisState *state, vector<int>& shape) const;
  int getJump(const TetrisState *state, int col) const;
 public:
	void kernel(GAPIC_term_ref sa_a, GAPIC_term_ref sa_b, GAPIC_float* kern) const {

    GAPIC_term_ref arg=GAPIC_new_term_ref();
    GAPIC_get_arg(1,sa_a,arg);
    const TetrisState* state_a = HeapGetDataFromTerm<TetrisStateCollectable>(arg);
    GAPIC_get_arg(1,sa_b,arg);
    const TetrisState* state_b = HeapGetDataFromTerm<TetrisStateCollectable>(arg);
    *kern=computeKernel(*state_a,*state_b);
    if(debug_tetris_kernel)
      cerr << "kern=" << *kern << endl;
  }
  double computeNormKernel(const TetrisState& a,const TetrisState& b) const {
    double ab=computeKernel(a,b);
    double aa=computeKernel(a,a);
    double bb=computeKernel(b,b);
    if(((aa<0.000001)||(bb<0.000001))&&(ab<0.000001))
      return 0;
    return ab/sqrt(aa*bb);
  }
  double computeKernel(const TetrisState& a,const TetrisState& b) const;
  double computeDistance(const TetrisState& a,const TetrisState& b) const;
  ostream& Print(ostream& os) const {os << "[TetrisDistance]";}
	void computeDistance(GAPIC_term_ref tetris_a, GAPIC_term_ref tetris_b, GAPIC_float* dist) const;

	void computeKernel(GAPIC_term_ref tetris_a, GAPIC_term_ref tetris_b, GAPIC_float* dist) const;

      
  TetrisDistance() 
    : m_height_w(0.0),
    m_avgheight_w(1.0),
    m_maxheight_w(1.0),
    m_holes_w(1.0),
    m_hole_depth_w(1.0),
    m_diff_w(0.0),
    m_sdiff_w(0.0),
    m_cell_w(0.00),
    m_alarm_w(0.0) {} 
     
    
    
    
    
    
    
    
     
};

extern gapic_type GapicTetrisDistanceType;
typedef GapicCollectable<TetrisDistance,&GapicTetrisDistanceType> 
	TetrisDistanceCollectable;

class TetrisAction {
  private:
    int col;
    int row;
    int orient;
  public:
    ostream& Print(ostream& os) const {
      if (row==-1) 
        os << "Action Collumn: " << col << " Orientation: " << orient << "\n";
      else
	os << "Action Collumn: " << col << " Row: " << row << " Orientation: " << orient << "\n";
    }
    TetrisAction() { col = 0; orient = 0; row = -1;};
    TetrisAction(int col, int orient) {
      this->col = col;
      this->row = -1;
      this->orient = orient;
    };
    TetrisAction(int col, int row, int orient) {
      this->col = col;
      this->row = row;
      this->orient = orient;
    };    
    const int getOrient() const {return orient;};
    const int getCol() const {return col;};
    const int getRow() const {return row;};
	void getInfo(GAPIC_int* orient, GAPIC_int* col) const;

};


extern gapic_type GapicTetrisActionType;
typedef GapicNonExMutCollectable<TetrisAction,
  &GapicTetrisActionType> TetrisActionCollectable;

#endif
