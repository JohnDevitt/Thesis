


#ifndef STORED_ATTRIBUTE_INC
#define STORED_ATTRIBUTE_INC

#include <gapic/struct/c_term.h>

extern gapic_type StoredAttributeType;
extern gapic_type StoredNominalAttributeType;
extern gapic_type StoredRealAttributeType;
extern gapic_type GroupSAType;


class StoredAttribute : public TargetValue {
public:
  StoredAttribute() {}
  StoredAttribute(const StoredAttribute& src) : TargetValue(src) {}
  virtual StoredAttribute* get_tv_arr() const = 0;
  virtual int hasContinuousTarget() const = 0;
  virtual int compareTarget(GAPIC_int id1, GAPIC_int id2) = 0;
	virtual void getTargetRef(GAPIC_term_ref target) const = 0;

	virtual void getIndexTargetRef(GAPIC_term_ref target) const = 0;

	virtual void resize(GAPIC_int size);

};

typedef StoredAttribute* SAPtr;

class NominalAttrInfo {
public:
	GAPIC_int m_NbClasses;
	CTermPtr* m_ClassNames;
public:
	void initialize(GAPIC_term_ref ref);
	void getClassName(GAPIC_int idx, GAPIC_term_ref ref) const;
	void unifyClassName(GAPIC_int idx, GAPIC_term_ref ref) const;
	inline const CTerm* getClassName(GAPIC_int i) const { return m_ClassNames[i]; };
};


class StoredNominalAttribute : public StoredAttribute {
protected:
	NominalAttrInfo m_Info;
	GAPIC_int m_Size;
	GAPIC_int m_Current;
	unsigned char *m_Data;
public:
	StoredNominalAttribute(GAPIC_term_ref term);
	~StoredNominalAttribute();
	StoredNominalAttribute(const StoredNominalAttribute& src);
	virtual StoredNominalAttribute* get_tv_arr() const;
	virtual GAPIC_int getNbClasses() const;
	GAPIC_int getValue(GAPIC_int ex_id) const;

	void setValue(GAPIC_int ex_id, GAPIC_int ex_cl);

	virtual void getInfo(GAPIC_term_ref info) const;

	virtual void selectTarget(GAPIC_int ex_id);
	virtual void getTargetRef(GAPIC_term_ref target) const;
	virtual void getIndexTargetRef(GAPIC_term_ref target) const;
	virtual GAPIC_int getIntValue() const;
	virtual void* getExtraInfo() const;
	virtual int hasContinuousTarget() const;
	virtual int compareTarget(GAPIC_int id1, GAPIC_int id2);
	virtual ostream& PrintObject(ostream& os) const;
	virtual void copyFrom(TargetValue* val);
};


class StoredRealAttribute : public StoredAttribute {
protected:
	GAPIC_int m_Size;
	GAPIC_float m_Current;
	GAPIC_float *m_Data;
public:
	StoredRealAttribute(GAPIC_term_ref term);
	StoredRealAttribute(const StoredRealAttribute& src);
	~StoredRealAttribute();
	virtual StoredRealAttribute* get_tv_arr() const;
	G_SO double getValue(GAPIC_int ex_id) const;

	bool setValue(GAPIC_int ex_id, GAPIC_float value);

	void setTargetValue(GAPIC_int ex_id, GAPIC_float value);
	virtual void selectTarget(GAPIC_int ex_id);
	virtual void getTargetRef(GAPIC_term_ref target) const;
	virtual void getIndexTargetRef(GAPIC_term_ref target) const;
	virtual GAPIC_float getFloatValue() const;
	virtual int hasContinuousTarget() const;
	virtual int compareTarget(GAPIC_int id1, GAPIC_int id2);
	virtual ostream& PrintObject(ostream& os) const;
	virtual void copyFrom(TargetValue* val);
	virtual void resize(GAPIC_int size);
};

class GroupSA : public StoredAttribute {
protected:
	GAPIC_int m_NbTarget;
	SAPtr* m_SAList;
public:
	GroupSA(GAPIC_term_ref ref);
	GroupSA(const GroupSA& src);
	virtual GroupSA* get_tv_arr() const;
	virtual void selectTarget(GAPIC_int ex_id);
	virtual void getTargetRef(GAPIC_term_ref target) const;
	virtual void getIndexTargetRef(GAPIC_term_ref target) const;
	virtual int hasContinuousTarget() const;
	virtual int compareTarget(GAPIC_int id1, GAPIC_int id2);
	virtual ostream& PrintObject(ostream& os) const;
	virtual void copyFrom(TargetValue* val);
};

#endif
