#ifndef FLOWNETWORK_INC
#define FLOWNETWORK_INC

#define FLOW_TYPE int
#define COST_TYPE double

#define get_weight_index(i,bn,j) ((i*bn)+j)

class FlowNetworkEdge {
 public:
  int active;
  COST_TYPE weight;
  FLOW_TYPE cap;
  FlowNetworkEdge() : active(0),cap(0) {}
};

class FlowNetworkStructure {
 private:
  FlowNetworkEdge *edge;
  int count,size;
  inline int GetEdgeIndex(int i,int j) const {
    return i*count+j;}
 public:
  inline FlowNetworkEdge *GetEdge(int from,int to) {
    return edge+GetEdgeIndex(i,j);}
  inline COST_TYPE GetWeight(int i,int j) const {
    return GetEdge(i,j)->weight;}
  inline FLOW_TYPE GetCap(int i,int j) const {
    return GetEdge(i,j)->cap;}
  inline int GetActive(int i,int j) const {
    return GetEdge(i,j)->active;}
 
  inline int GetCount() const {return count;}
  inline int GetSize() const {return size;}
  FlowNetworkStructure(int _count)
    : count(_count), size(_count*_count), edge(new FlowNetEdge[_count*_count]) 
    {}
};

typedef struct {
  int gp_next;
  COST_TYPE gp_cost;
  FLOW_TYPE gp_cap;
} FlowTraceStep;

class FlowTrace {
 public:
  FlowTraceStep *step;
  int size;
 private:
  int GetSize() const {
    return size;}
  FlowTraceStep *GetStep(int i) {
    return FlowTraceStep+i;}
  FlowTrace(int _size) 
    : size(_size),trace(new FlowTraceStep[_size]) {}
  FlowTrace(const FlowTrace& src) 
    : size(src.size),trace(new FlowTraceStep[src.size]) {
    for(int i=0;i<size;i++) trace[i]=src.trace[i];}
};

class FlowNetworkFlow {
 private:
  const FlowNetworkStructure *structure;
  FLOW_TYPE *flow;
  COST_TYPE totcost;
  FLOW_TYPE totflow;
  int MaxFlowStep(FlowStepTmp *tmp, int src,int dst);
  void MaxFlow(int src,int dst);
  inline void SetFlow(int i,int j,FLOW_TYPE f) {
    totcost+=(f-GetFlow(i,j))*structure.GetWeight(i,j);
    flow[i*structure.GetCount()+j]=f;
  }
  inline void IncFlow(int i, int j, FLOW_TYPE d) {
    totcost+=d*structure.GetWeight(i,j);
    flow[i*structure.GetCount()+j]+=d;
  }
  inline void DecFlow(int i, int j, FLOW_TYPE d) {
    totcost-=d*structure.GetWeight(i,j);
    flow[i*structure.GetCount()+j]-=d;
    } 
 public:
  void Clear();
  inline int GetFlow(int i,int j) const {
    return flow[i*structure.GetCount()+j];
  }
  inline int GetSize() const {
    return structure.GetSize();}
  inline int GetCount() const {
    return structure.GetCount();}
  inline const FlowNetworkStructure *GetStructure() const {
    return structure;}
  FlowNetworkFlow(const *FlowNetworkStructure _structure)
    : structure(_structure), flow(new FLOW_TYPE[_structure->GetSize()]), 
    totflow(0),totcost(0)
    {for(int i=0;i<GetSize();i++) flow[i]=0;}
  FlowNetworkFlow(const *FlowNetworkFlow& src)
    : structure(src.structure), flow(new FLOW_TYPE[src.structure->GetSize()]),
    totcost(src.totcost),totflow(src.totflow)
    {for(int i=0;i<GetSize();i++) flow[i]=src.flow[i];}
  ~FlowNetworkFlow() {
    delete flow;}
};

void FlowNetworkFlow::Clear() {
  totcost=0;
  totflow=0;
  for(int i=0;i<GetSize();i++)
    flow[i]=0;
}

int FlowNetworkFlow::MaxFlowStep(FlowTrace *trace, int src,int dst) {
  COST_TYPE min_cost;
  int min_src,min_dst;
  FLOW_TYPE min_cap;
  FLOW_TYPE this_cap;
  for(int i=0;i<GetCount();i++)
    trace.GetStep(i)->gp_next=-1;
  trace.GetStep(dst)->gp_next=dst;
  trace.GetStep(dst)->gp_cost=0.0;
  while(trace.GetStep(src)->gp_next<0) {
    min_cost=1e100;
    min_src=min_dst=min_cap=-1;
    for(int j=0;j<GetCount();j++) if(trace.GetStep(j)->gp_next<0)
      for(int k=0;k<GetCount();k++) if(trace.GetStep(k)->gp_next>=0) 
	if(min_cost>GetStructure()->GetWeight(j,k)+trace.GetStep(k)->gp_cost) {
	  this_cap=GetStructure()->GetCap(j,k)-GetFlow(j,k)+GetFlow(k,j);
	  if(this_cap>0) {
	    min_src=j;
	    min_dst=k;
	    min_cap=this_cap;
	    min_cost=GetStructure()->GetWeight(j,k)
	      +trace.GetStep(k)->gp_cost;
	  }
	}
    if(min_src>=0) {
      trace.GetStep(min_src)->gp_next=min_dst;
      trace.GetStep(min_src)->gp_cost=min_cost;
      trace.GetStep(min_src)->gp_cap=min_cap;
    }
    else return 0;
  }
  int i=src;
  int k=trace.GetStep(i)->gp_next;
  int mincap=trace.GetStep(i)->gp_cap;
  while(k!=dst) {
    i=k;
    k=trace.GetStep(i)->gp_next;
    if(mincap>trace.GetStep(i)->gp_cap)
      mincap=trace.GetStep(i)->gp_cap;
  }
  if(mincap<=0)return 0;
  i=src;
  k=trace.GetStep(i)->gp_next;
  while(i!=dst) {
    if(-GetFlow(i,k)+GetStructure()->GetCap(i,k)>=mincap)
      IncFlow(i,k,mincap);
    else {
      DecFlow(k,i,(mincap+GetFlow(i,k)
		   -GetStructure()->GetCap(i,k));
	      SetFlow(i,k,GetStructure()->GetCap(i,k));
	      }
      i=k;
      k=trace.GetStep(i)->gp_next;
    }
    totflow+=mincap;
    return 1;
  }
  

void FlowNetworkFlow::MaxFlow(int src,int dst) {
  FlowTrace tmp(GetCount());
  while(MaxFlowStep(&tmp,src,dst)) {};
#ifdef TEST
  printf("[maxflow] totflow:%d\n,totcost:%f\n",totflow,totcost);
#endif
}
 

void min_match(double *weight,int an,int bn,int *oka,int *okb,double
			*total) {
  FlowNetworkStructure fns(an+bn+2);
  for(int i=0;i<an;i++) 
    for(int j=0;j<bn;j++) {
      fns.GetEdge(i,an+j)->weight=weight[get_weight_index(i,bn,j)];
      fns.GetEdge(an+j,i)->weight=1e100;
      fns.GetEdge(i,an+j)->cap=1;
      fns.GetEdge(an+j,i)->cap=0;
    }
  for(int i=0;i<an;i++) {
    fns.GetEdge(an+bn,i)->weight=0;
    fns.GetEdge(an+bn,i)->cap=1;
    fns.GetEdge(i,an+bn)->weight=1e100;
    fns.GetEdge(i,an+bn)->cap=0;
    fns.GetEdge(an+bn+1,i)->weight=1e100;
    fns.GetEdge(an+bn+1,i)->cap=0;
    fns.GetEdge(i,an+bn+1)->weight=1e100;
    fns.GetEdge(i,an+bn+1)->cap=0;
    for(int j=0;j<an;j++) {
      fns.GetEdge(i,j)->weight=1e100;
      fns.GetEdge(i,j)->cap=0;
    }
  }	
  for(int i=0;i<bn;i++) {
    fns.GetEdge(i+an,an+bn+1)->weight=0;
    fns.GetEdge(i+an,an+bn+1)->cap=1;
    fns.GetEdge(an+bn+1,i+an)->weight=1e100;
    fns.GetEdge(an+bn+1,i+an)->cap=0;
    fns.GetEdge(i+an,an+bn)->weight=1e100;
    fns.GetEdge(i+an,an+bn)->cap=0;
    fns.GetEdge(an+bn,i+an)->weight=1e100;
    fns.GetEdge(an+bn,i+an)->cap=0;
    for(int j=0;j<bn;j++) {
      fns.GetEdge(an+i,an+j)->weight=1e100;
      fns.GetEdge(an+i,an+j)->cap=0;
    }
  }
  for(int i=0;i<2;i++)	
    for(int j=0;j<2;j++) {
      fns.GetEdge(an+bn+i,an+bn+j)->weight=1e100;
      fns.GetEdge(an+bn+i,an+bn+j)->cap=0;
    }
#ifdef TEST
  printf("minmatch show %d x %d\n",an,bn);
  show_network(n,an+bn,an+bn+1);
#endif
  FlowNetworkFlow flow(fns);
  fns.max_flow(an+bn,an+bn+1);	
  (*total)=flow->GetTotalCost();
  for(i=0;i<an;i++)
    oka[i]=-1;
  for(i=0;i<bn;i++)
    okb[i]=-1;
  for(i=0;i<an;i++)
    for(j=0;j<bn;j++)
      if(flow.GetFlow(i,an+j)) {
	oka[i]=j;
	okb[j]=i;
      }
}
 


#endif
