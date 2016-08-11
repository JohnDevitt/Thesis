


#ifndef NUMERIC_SPLIT_INC
#define NUMERIC_SPLIT_INC

#include <gapic/gapicgc.h>
#include <gapic/struct/c_term.h>
#include <statistics/exdistr_statistic/target_value.h>
#include <statistics/exdistr_statistic/stored_attribute.h>
#include <statistics/exdistr_statistic/statistic.h>
#include <statistics/exdistr_statistic/heuristic.h>

#include <string>

extern gapic_type NSplitType;


void medianOfThree(GAPIC_float* mainarray, int* subarray, int begin, int end);
void partitionSmallToLarge(GAPIC_float* mainarray, int* subarray, int begin, int end, int &pivotindex);
void partitionLargeToSmall(GAPIC_float*  mainarray, int* subarray, int begin, int end, int &pivotindex);
void partitionSmallToLargeFullSort(GAPIC_float* mainarray, int* subarray, int begin, int end, int &pivotindex);
void partitionLargeToSmallFullSort(GAPIC_float*  mainarray, int* subarray, int begin, int end, int &pivotindex);
void findPivotSmaller(GAPIC_float* valuearray, int* idarray, int begin, int end);
void findPivotLarger(GAPIC_float* valuearray, int* idarray, int begin, int end);
int  allAtttributeValuesEqual(GAPIC_float* mainarray, int begin, int end);

void printListPairs(int nb, GAPIC_float* l1, int* l2, GAPIC_int* mapping, string s);
void printBatchUpdate(int begin, int end, GAPIC_float* l1, int* l2, GAPIC_int* mapping, StoredAttribute* mTarget1);
void print2DList(int nb, GAPIC_float** list, int* listlen, GAPIC_int* mapping);


class NSplit : public GapicIDObject {

protected:
	StoredAttribute* m_Target;
	ExdistrStatistic* m_TS;
	ExdistrStatistic* m_PS;
	ExsplitHeuristic* m_Heur;

public:
	NSplit(GAPIC_term_ref init);
	void init(TargetValue* target, ExdistrStatistic* ts, ExdistrStatistic* ps, ExsplitHeuristic* hr);

	void nb_examples(GAPIC_int nb);

	void add_example(GAPIC_int id, GAPIC_float weight, GAPIC_term_ref value);

	void find_best_split(GAPIC_float* split, GAPIC_term_ref heur, GAPIC_int* testtype);

	ostream& Print(ostream& os) const;

private:

	void getExampleValues(int exid1, GAPIC_term_ref valuelist);
	void getExampleMinAndMax(int exid1, GAPIC_float* minlist1, GAPIC_float* maxlist1);
	void removeDoublesFromExamples();

	GAPIC_float getMinOfMaxima();
	GAPIC_float getMaxOfMinima();
	void fillSmallerValueList(GAPIC_float maxOfMinima, int &cnt);
	void fillLargerValueList(GAPIC_float minOfMaxima, int &cnt);

	void sortAscending(GAPIC_float* mainarray, int* subarray, int nb1, int weightoption, string s);
	void sortDescending(GAPIC_float* mainarray, int* subarray, int nb1, int weightoption, string s);

	int isTargetBoundary(GAPIC_float* sortedarray, int* subarray, int index, int end);

	void findBestHeurSmaller(int* idlist1, GAPIC_float* valuelist1, int nbel1, int weightoption);
	void findBestHeurGreater(int* idlist1, GAPIC_float* valuelist1, int nbel1, int weightoption);
	void findBestHeurFromList(GAPIC_float &bestheur, int* idlist1, GAPIC_float* valuelist1, int nbel1, int weightoption);

	GAPIC_float findLowerBoundSmallerThr(int* idlist1, GAPIC_float* valuelist1, int nbel1);
	GAPIC_float findUpperBoundSmallerThr(int* idlist1, GAPIC_float* valuelist1, int nbel1);
	GAPIC_float findLowerBoundGreaterThr(int* idlist1, GAPIC_float* valuelist1, int nbel1);
	GAPIC_float findUpperBoundGreaterThr(int* idlist1, GAPIC_float* valuelist1, int nbel1);
	GAPIC_float reCalculateHeur(GAPIC_float split1, GAPIC_int testtype1);

	void refineThresholdSmallerTest(int* minidlist1, GAPIC_float* sortedminlist1);
	void refineThresholdGreaterTest(int* maxidlist1, GAPIC_float* sortedmaxlist1);
	void refineSmallerTestOneExample(GAPIC_float &temp, int id1);
	void refineGreaterTestOneExample(GAPIC_float &temp, int id1);

	void returnSmallerBestSplit(GAPIC_float* split, GAPIC_term_ref heur, GAPIC_int* testtype);
	void returnGreaterBestSplit(GAPIC_float* split, GAPIC_term_ref heur, GAPIC_int* testtype);

	void  fullSortAscending(GAPIC_float* mainarray, int* subarray, int begin, int end);
	void fullSortDescending(GAPIC_float* mainarray, int* subarray, int begin, int end);
	void  wellBehavedSortAscending(GAPIC_float* valuearray, int* idarray, int begin, int end, int nbelements);
	void wellBehavedSortDescending(GAPIC_float* valuearray, int* idarray, int begin, int end, int nbelements);
	void  partialSortAscending(GAPIC_float* valuearray, int* idarray, int begin, int end, int shortestLength, int weightoption);
	void partialSortDescending(GAPIC_float* valuearray, int* idarray, int begin, int end, int shortestLength, int weightoption);
	int  allTargetValuesEqual(int* subarray, int begin, int end);
	void  initializePartialSortAscending(GAPIC_float* valuearray, int* idarray, int nb1);
	void initializePartialSortDescending(GAPIC_float* valuearray, int* idarray, int nb1);
	void evaluateThreshold(int* idarray, int begin, int end, int pivotindex, int weightoption);

	int useVersion;         
	int talking;            
	int usePartialSorting;          
						             
	GAPIC_float unsortedFractions;            
								             

	int nbex;                   

	GAPIC_float smallerheur;        
	GAPIC_float smallerthr1;          
	GAPIC_float smallerthr2;         
	GAPIC_float greaterheur;        
	GAPIC_float greaterthr1;          
	GAPIC_float greaterthr2;         
	int bestindex;
	int leftindex;
	int rightindex;

	int PSindex;             
				                  
	GAPIC_float tempHeur;

	GAPIC_float** examples;        
	int* examplelength;	                    

	GAPIC_float* minlist;         
	GAPIC_float* maxlist;
	GAPIC_float* sortedminlist;          
	GAPIC_float* sortedmaxlist;
	GAPIC_float* smallervaluelist;
	GAPIC_float* largervaluelist;

	int* minsub;           
	int* maxsub;
	int* smalleridlist;           
	int* largeridlist;

	GAPIC_int* mapping;        
	int exid;      
};

typedef GapicNonExMutCollectable<NSplit, &NSplitType> NSplitCollectable;

#endif
