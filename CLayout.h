#pragma once
#define ANT_MAX_COUNT 50
#include "CAreaList.h"
#include "CLayoutList.h"
#include "CPart.h"
#include "CRectangle.h"
#include <list>
#include <vector>
#include <array>

const int partNum=17;
const int stoveNum=1;

typedef vector<array<int,2> > tourType;

using namespace std;

class CLayout
{
private:
    double m_dWeightLeft; //重量剩余
	double m_dUsedArea;
	list<CAreaList> m_lstAvailable;
	list<CAreaList> m_lstUseless;
	list<CLayoutList> m_lstLayout;

public:
    const double m_dHeight;
    const double m_dWidth;
    const double m_dWeight; //重量
    const double m_dVolume; //体积

    CLayout(double dAreaWidth, double dAreaHeight, double dAreaWeight);//初始化区域
    int CanBePutIn(CPart Part);
	int PutIn(CPart& Part, bool bTrans);//返回装入的数量 引用为了修改零件的剩余重量
	double UpdateList(double dWidth, double dHeight, int nHorizonAmount, int nVerticalAmount, CPart Part, double dWeight);
	void Merge();
	void FindUseless(CPart* data);
    double Calculate(tourType tour, vector<CPart> &Part);
	list<CAreaList>::iterator getAvaliableBegin() { return m_lstAvailable.begin(); }
	list<CAreaList>::iterator getAvaliableEnd() { return m_lstAvailable.end(); }
	void testMerge();
	void showLayoutList();
	void showLayoutPartNo();
	int getLayoutListSize() { return this->m_lstLayout.size(); }
	int AvailSize() { return m_lstAvailable.size(); }
    double getSum();
};

