#pragma once
#include "CLayout.h"
#include <list>
using namespace std;

class solution
{
private:
    //double value=0;

public:
    double value=0;
    solution(tourType tourPath) : tourPath(tourPath) {}
    solution() {} //正常不要用

    list<CLayout> allLayout;
    tourType tourPath;

    void caluValue(); //fix:根据allLayout计算value

    double getValue() { return this->value; }

    double utilization()
    {
        double total=0;
        double use=0;
        for(CLayout &layout : this->allLayout)
        {
            total+=layout.getArea();
            use+=layout.getUseArea();
        }
        return use/total;
    }

    void output()
    {
        for(CLayout &layout : this->allLayout)
        {
            layout.outputAllPart();
            cout<<endl;
        }
    }
};
