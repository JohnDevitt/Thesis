
#ifndef CTIMING_INC



typedef struct {
	unsigned long m_Time;
	unsigned long m_Count;
	unsigned long m_GCTime;
	string m_Name;
} GapicTimeInfo;

class GapicTimers {
protected:
	int m_Debug;
	int m_Calibrated;
	vector<GapicTimeInfo> m_Timers;
	unsigned long m_PrevTime;
	unsigned long m_PrevGCTime;
	unsigned long m_PrevCalib[2];
	float m_Calibration;
	int m_Selected;
	vector<int> m_freelist;
public:
	GapicTimers();
	void initTime();
	void resetTimers();

	void resetTimer(GAPIC_int id);

	void selectTimer(GAPIC_int id);

	void getTimerInfo(GAPIC_int id, GAPIC_float* time, GAPIC_int* count);

	void setTimerName(int id, const char* name);
	void setTimerName(GAPIC_int id, const GAPIC_atom& name) {

	  setTimerName(id,name.GetString());}
	void newTimer(GAPIC_int* id);

	void delTimer(GAPIC_int id);

	void startCalibration(GAPIC_int step);

	void endCalibration(GAPIC_int count);

	void getTimersData(GAPIC_term_ref data);

	void getTimerData(int id, GapicListCons* list) const;
	double getTimerTime(int id) const;
	double getTimerGCTime(int id) const;
	float getTime() const;   
	void printTimer(GAPIC_int id) const {

	  printTimer(cout,id);}
	void printTimer(ostream& os, int id) const;
	ostream& Print(ostream& os) const;
	void expandTo(GAPIC_int size);

	inline int getDebug() { return m_Debug; };
	inline void setDebug(int debug) { m_Debug = debug; };
	inline int isCalibrated() const { return m_Calibrated; }
};

extern gapic_type GapicCTimersType;
typedef GapicNonExMutCollectable<GapicTimers,&GapicCTimersType>
	GapicTimersCollectable;

#endif
