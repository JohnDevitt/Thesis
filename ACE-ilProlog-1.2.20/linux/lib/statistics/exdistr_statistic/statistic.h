


#ifndef STATISTIC_INC
#define STATISTIC_INC

G_SO extern gapic_type ExdistrStatisticType;
extern gapic_type WCountESType;
extern gapic_type GroupESType;

class ExdistrStatistic : public GapicIDObject {
public:
	G_SO virtual void needs_ex_reg(GAPIC_int* yn);     

	G_SO virtual void register_ex(GAPIC_int exid);    

	G_SO virtual void finish_ex_reg();    

	virtual void reset() = 0; 

	          
	virtual void scale(GAPIC_float s) = 0;

	           
	     
	virtual void resize(GAPIC_int n, GAPIC_int s) = 0;

	            
	virtual void add(const ExdistrStatistic* s) = 0;

	        
	virtual void update(GAPIC_int query, GAPIC_float weight) = 0;

	          
	    
	virtual ExdistrStatistic* clone() const = 0;

	      
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog) = 0;

	          
	G_SO virtual void rule_info(GAPIC_term_ref prolog);

	 
	G_SO virtual double getSumWeight(GAPIC_int q);

	G_SO virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr);
	G_SO virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg);
	G_SO virtual void announce_querytype(GAPIC_int query, GAPIC_int type);
	G_SO virtual void announce_example_ids(GAPIC_term_ref ex_list);
	G_SO virtual ExdistrStatistic* getStatistic(GAPIC_int idx);
	      
	virtual ostream& PrintStat(ostream& os) const = 0;
	    
	G_SO ostream& Print(ostream& os) const;
	    
};

typedef ExdistrStatistic* ESPtr;

typedef GapicNonExMutCollectable<ExdistrStatistic, &ExdistrStatisticType> ExdistrStatisticCollectable;

class SingleES : public ExdistrStatistic {
protected:
	TargetValue* m_Target;
};

class WCountES : public ExdistrStatistic {
protected:
	GAPIC_int m_Size;
	GAPIC_float* m_SumWeights;
public:
	WCountES();
	WCountES(GAPIC_term_ref ref);
	virtual ~WCountES();
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual GAPIC_float getSumWeight(GAPIC_int q);
	virtual ostream& PrintStat(ostream& os) const;
	GAPIC_float totalWeight(GAPIC_int q);
	void copy(WCountES* other) {};
};


class GroupES : public ExdistrStatistic {
protected:
	GAPIC_int m_NbStats;
	ESPtr* m_ESList;
public:
	GroupES(const GroupES* other);
	GroupES(GAPIC_term_ref ref);
	virtual ~GroupES();
	virtual void resize(GAPIC_int n, GAPIC_int s);
	virtual void reset();
	virtual void add(const ExdistrStatistic* s);
	virtual void scale(GAPIC_float s);
	virtual void update(GAPIC_int query, GAPIC_float weight);
	virtual void to_prolog(GAPIC_int idx, GAPIC_int query, GAPIC_term_ref prolog);
	virtual ExdistrStatistic* clone() const;
	virtual ExdistrStatistic* getStatistic(GAPIC_int idx);
	virtual ostream& PrintStat(ostream& os) const;
	void getStatisticProlog(GAPIC_int idx, GAPIC_term_ref stat);

	void setNbStats(GAPIC_int nb);
	void setStatistic(GAPIC_int i, ExdistrStatistic* stat);
	inline GAPIC_int getNbStats() const {
		return m_NbStats;
	};
	virtual GAPIC_float getSumWeight(GAPIC_int q);
	virtual void announce_query2attr(GAPIC_int query, GAPIC_int attr);
	virtual void announce_regrvalues(GAPIC_int query, GAPIC_int example, GAPIC_float max, GAPIC_float min, GAPIC_float avg);
	virtual void announce_querytype(GAPIC_int query, GAPIC_int type);
	virtual void announce_example_ids(GAPIC_term_ref ex_list);
	virtual void needs_ex_reg(GAPIC_int *yn);     
	virtual void register_ex(GAPIC_int exid);    
	virtual void finish_ex_reg();    
};

class SingleStatOwner {
protected:
	int m_PSIdx, m_TSIdx;
public:
	void initialize(GAPIC_term_ref stats);
};

template <GAPIC_int nb>
class NStatOwner {
protected:
	int m_PSIdx[nb], m_TSIdx[nb];
public:
	void initialize(GAPIC_term_ref stats) {
		get_stat_index_multiple(stats, m_PSIdx, m_TSIdx, nb);
	};
};

class VStatOwner {
protected:
	GAPIC_int m_NbPS, m_NbTS;
	int *m_PSIdx, *m_TSIdx;
public:
	VStatOwner();
	virtual ~VStatOwner();
	void initialize(GAPIC_term_ref stats);
};

void get_stat_index_single(GAPIC_term_ref sidx, GAPIC_int* pos, GAPIC_int* tot);
void get_stat_index_multiple(GAPIC_term_ref sidx, GAPIC_int* pos, GAPIC_int* tot, GAPIC_int nb);

#endif
