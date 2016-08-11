#ifndef FLOWNETWORK_INC
#define FLOWNETWORK_INC

#define FLOW_TYPE int
#define COST_TYPE double








    #define SO_EXP


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
  inline int GetEdgeIndex(int from,int to) const {
    return from*count+to;}
 public:
  inline FlowNetworkEdge *GetEdge(int from,int to) {
    return edge+GetEdgeIndex(from,to);}
  inline const FlowNetworkEdge *GetEdge(int from,int to) const {
    return edge+GetEdgeIndex(from,to);}
  inline COST_TYPE GetWeight(int i,int j) const {
    return GetEdge(i,j)->weight;}
  inline FLOW_TYPE GetCap(int i,int j) const {
    return GetEdge(i,j)->cap;}
  inline int GetActive(int i,int j) const {
    return GetEdge(i,j)->active;}
 
  inline int GetCount() const {return count;}
  inline int GetSize() const {return size;}
  ~FlowNetworkStructure() {if(edge!=NULL) delete edge;}
  FlowNetworkStructure(int _count)
    : count(_count), size(_count*_count), 
    edge(new FlowNetworkEdge[_count*_count]) 
    {}
};

typedef struct {
  int gp_next;
  COST_TYPE gp_cost;
  FLOW_TYPE gp_cap;
} FlowTraceStep;

class FlowTrace {
 private:
  FlowTraceStep *trace;
  int size;
 public:
  int GetSize() const {
    return size;}
  FlowTraceStep *GetStep(int i) {
    return trace+i;}
  ~FlowTrace() {if(trace!=NULL) delete trace;}
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
  int MaxFlowStep(FlowTrace *tmp, int src,int dst);
  inline void SetFlow(int i,int j,FLOW_TYPE f) {
    totcost+=(f-GetFlow(i,j))*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]=f;
  }
  inline void IncFlow(int i, int j, FLOW_TYPE d) {
    totcost+=d*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]+=d;
  }
  inline void DecFlow(int i, int j, FLOW_TYPE d) {
    totcost-=d*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]-=d;
    } 
 public:
  void Clear();
  inline COST_TYPE GetTotalCost() const {
    return totcost;}
  inline FLOW_TYPE GetTotalFlow() const {
    return totflow;}
  inline int GetFlow(int i,int j) const {
    return flow[i*structure->GetCount()+j];}
  inline int GetSize() const {
    return structure->GetSize();}
  inline int GetCount() const {
    return structure->GetCount();}
  inline const FlowNetworkStructure *GetStructure() const {
    return structure;}
  SO_EXP void MaxFlow(int src,int dst);
  FlowNetworkFlow(const FlowNetworkStructure *_structure)
    : structure(_structure), flow(new FLOW_TYPE[_structure->GetSize()]), 
    totflow(0),totcost(0)
    {for(int i=0;i<GetSize();i++) flow[i]=0;}
  FlowNetworkFlow(const FlowNetworkFlow& src)
    : structure(src.structure), flow(new FLOW_TYPE[src.structure->GetSize()]),
    totcost(src.totcost),totflow(src.totflow)
    {for(int i=0;i<GetSize();i++) flow[i]=src.flow[i];}
  ~FlowNetworkFlow() {
    delete flow;}
};

typedef double REAL_FLOW_TYPE;

class RealFlowNetworkEdge {
 public:
  int active;
  COST_TYPE weight;
  REAL_FLOW_TYPE cap;
  RealFlowNetworkEdge() : active(0),cap(0) {}
};

class RealFlowNetworkStructure {
 private:
  RealFlowNetworkEdge *edge;
  int count,size;
  inline int GetEdgeIndex(int from,int to) const {
    return from*count+to;}
 public:
  inline RealFlowNetworkEdge *GetEdge(int from,int to) {
    return edge+GetEdgeIndex(from,to);}
  inline const RealFlowNetworkEdge *GetEdge(int from,int to) const {
    return edge+GetEdgeIndex(from,to);}
  inline COST_TYPE GetWeight(int i,int j) const {
    return GetEdge(i,j)->weight;}
  inline REAL_FLOW_TYPE GetCap(int i,int j) const {
    return GetEdge(i,j)->cap;}
  inline int GetActive(int i,int j) const {
    return GetEdge(i,j)->active;}
 
  inline int GetCount() const {return count;}
  inline int GetSize() const {return size;}
  ~RealFlowNetworkStructure() {if(edge!=NULL) delete edge;}
  RealFlowNetworkStructure(int _count)
    : count(_count), size(_count*_count), 
    edge(new RealFlowNetworkEdge[_count*_count]) 
    {}
};

typedef struct {
  int gp_next;
  COST_TYPE gp_cost;
  REAL_FLOW_TYPE gp_cap;
} RealFlowTraceStep;

class RealFlowTrace {
 private:
  RealFlowTraceStep *trace;
  int size;
 public:
  int GetSize() const {
    return size;}
  RealFlowTraceStep *GetStep(int i) {
    return trace+i;}
  ~RealFlowTrace() {if(trace!=NULL) delete trace;}
  RealFlowTrace(int _size) 
    : size(_size),trace(new RealFlowTraceStep[_size]) {}
  RealFlowTrace(const RealFlowTrace& src) 
    : size(src.size),trace(new RealFlowTraceStep[src.size]) {
    for(int i=0;i<size;i++) trace[i]=src.trace[i];}
};

class RealFlowNetworkRealFlow {
 private:
  const RealFlowNetworkStructure *structure;
  REAL_FLOW_TYPE *flow;
  COST_TYPE totcost;
  REAL_FLOW_TYPE totflow;
  int MaxRealFlowStep(RealFlowTrace *tmp, int src,int dst);
  inline void SetRealFlow(int i,int j,REAL_FLOW_TYPE f) {
    totcost+=(f-GetRealFlow(i,j))*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]=f;
  }
  inline void IncRealFlow(int i, int j, REAL_FLOW_TYPE d) {
    totcost+=d*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]+=d;
  }
  inline void DecRealFlow(int i, int j, REAL_FLOW_TYPE d) {
    totcost-=d*structure->GetWeight(i,j);
    flow[i*structure->GetCount()+j]-=d;
    } 
 public:
  void Clear();
  inline COST_TYPE GetTotalCost() const {
    return totcost;}
  inline REAL_FLOW_TYPE GetTotalRealFlow() const {
    return totflow;}
  inline REAL_FLOW_TYPE GetRealFlow(int i,int j) const {
    return flow[i*structure->GetCount()+j];}
  inline int GetSize() const {
    return structure->GetSize();}
  inline int GetCount() const {
    return structure->GetCount();}
  inline const RealFlowNetworkStructure *GetStructure() const {
    return structure;}
  void MaxRealFlow(int src,int dst);
  RealFlowNetworkRealFlow(const RealFlowNetworkStructure *_structure)
    : structure(_structure), flow(new REAL_FLOW_TYPE[_structure->GetSize()]), 
    totflow(0),totcost(0)
    {for(int i=0;i<GetSize();i++) flow[i]=0;}
  RealFlowNetworkRealFlow(const RealFlowNetworkRealFlow& src)
    : structure(src.structure), flow(new REAL_FLOW_TYPE[src.structure->GetSize()]),
    totcost(src.totcost),totflow(src.totflow)
    {for(int i=0;i<GetSize();i++) flow[i]=src.flow[i];}
  ~RealFlowNetworkRealFlow() {
    delete flow;}
};


#endif
