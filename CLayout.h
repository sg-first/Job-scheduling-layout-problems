#pragma once
#define ANT_MAX_COUNT 50
#include "CAreaList.h"
#include "CLayoutList.h"
#include "CPart.h"
#include "CRectangle.h"
#include <list>

using namespace std;
class CLayout
{
private:
	double m_dHeight;
	double m_dWidth;
	double m_dWeight;//����
	//double m_dWeightLeft[ANT_MAX_COUNT];//����ʣ��
	double m_dWeightLeft;//����ʣ��
	double m_dUsedArea;
	int nPartsAmount;
	list<CAreaList> m_lstAvailable;
	list<CAreaList> m_lstUseless;
	list<CLayoutList> m_lstLayout;
public:
	CLayout() {}
	~CLayout() {}
	void Init(double dAreaWidth, double dAreaHeight, double dAreaWeight,int nPartsAmount);//��ʼ������
	int CanBePutIn(CPart Part);
	int PutIn(CPart& Part, bool bTrans);//����װ������� ����Ϊ���޸������ʣ������
	double UpdateList(double dWidth, double dHeight, int nHorizonAmount, int nVerticalAmount, CPart Part, double dWeight);
	void Merge();
	void FindUseless(CPart* data);
	double Calculate(int* tour, CPart* Part);
	list<CAreaList>::iterator getAvaliableBegin() { return m_lstAvailable.begin(); }
	list<CAreaList>::iterator getAvaliableEnd() { return m_lstAvailable.end(); }
	void testMerge();
	void showLayoutList();
	void showLayoutPartNo();
	int getLayoutListSize() { return this->m_lstLayout.size(); }
	int AvailSize() { return m_lstAvailable.size(); }
};

