#pragma once
#include "CLayout.h"
#include <list>
#include "caluDist.h"
using namespace std;

class solution
{
private:
    double time=0;

    vector<int> getAllTime()
    {
        vector<int> allTime; //记录每个结束时间
        for(int i=0;i<stoveNum;i++)
            allTime.push_back(0); //初始化
        for(CLayout &layout : this->allLayout) //计算每个的时间
            allTime[layout.getID()]+=layout.getTime(); //layout添加到解决方案前必然已经计算，所以这里可以直接调用
        return allTime;
    }

public:
    solution(tourType tourPath) : tourPath(tourPath) {}
    solution() {} //正常不要用

    list<CLayout> allLayout;
    tourType tourPath;

    void caluTime()
    {
        vector<int> allTime=this->getAllTime();
        for(int i=0;i<allTime.size();i++)
        {
            if(allTime[i]>this->time) //用时最长的炉子的时长就是总时间
                this->time=allTime[i];
        }
    }

    double getTime() { return this->time; }

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

    CLayout getNextStove(caluDist &cd)
    {
        int stoveNum=cd.allStove.size();
        int nowSize=this->allLayout.size();
        if(nowSize<stoveNum)
        {
            //如果还有没装的，装剩下面积最大的（按allStove下标）
            return CLayout(*(cd.allStove[nowSize])); //装下一个没装的炉子
        }
        else
        {
            //如果都装了，找最快结束的
            vector<int> allTime=this->getAllTime();
            int minSub=0;
            for(int i=0;i<allTime.size();i++)
            {
                if(allTime[i]<allTime[minSub]) //找能最快结束的炉子
                    minSub=i;
            }
            return CLayout(*(cd.allStove[minSub]));
        }
    }
};
