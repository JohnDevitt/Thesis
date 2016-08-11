


#ifndef TARGET_VALUE_INC
#define TARGET_VALUE_INC

extern gapic_type TargetValueType;

class TargetValue {
private:
	GAPIC_int m_CurrentExample;
public:
	TargetValue();
	TargetValue(const TargetValue& src);
	virtual GAPIC_int getIntValue() const;
	virtual GAPIC_int getNbClasses() const;
	virtual GAPIC_float getFloatValue() const;
	virtual void selectTarget(GAPIC_int ex_id);
	virtual void* getExtraInfo() const;
	virtual TargetValue* get_tv_arr() const;
	TargetValue* getCopy() const;

	virtual void copyFrom(TargetValue* val);

	virtual void getInfo(GAPIC_term_ref info) const;

	inline GAPIC_int getCurrentExample() { return m_CurrentExample; }
	ostream& Print(ostream& os) const;
	virtual ostream& PrintObject(ostream& os) const;
	virtual void setTargetValue(GAPIC_int ex, GAPIC_float value);
};

typedef TargetValue* TVPtr;

typedef GapicNonExMutCollectable<TargetValue, &TargetValueType> TargetValueCollectable;

#endif
