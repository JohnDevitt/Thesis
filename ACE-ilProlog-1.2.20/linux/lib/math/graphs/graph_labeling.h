#ifndef LABELED_GRAPH_INC
#define LABELED_GRAPH_INC

template <class label_class> GraphVectorLabeling {
 private:
  vector<label_class> m_labels;
 public:
  inline void set_label(graph_node_id node, label_class label) {
    if(m_labels.size()<=node) m_labels.resize(node+1);
    m_labels[node]=label;}
  inline const label_class& get_label(graph_node_id node) const {
    return m_labels[node];}
};

#endif
