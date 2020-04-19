#include "CPart.h"
#include "CAreaList.h"
#include "CLayoutList.h"
#include "CLayout.h"
#include "ant.h"
#include <time.h>
#include <ctype.h>
#include <iostream>
#include <vector>
#include "solution.h"

//最大循环次数NcMax
const int NcMax = 500;

//===========================================================================================================
//局部更新时候使用的的常量，它是由最近邻方法得到的一个长度
//什么是最近邻方法?:)就是从源节点出发，每次选择一个距离最短的点来遍历所有的节点得到的路径

int CargoNum[partNum];  //number of each type cargo

//------------------------------------------
//选择下一个节点，配合下面的函数来计算的长度
int ChooseNextNode(int currentNode, int visitedNode[],double allDistance[partNum][partNum])
{
	int nextNode = -1;
	double shortDistance = 0.0;
    for (int i = 0; i < partNum; i++)
	{
		//去掉已走过的节点,从剩下节点中选择距离最近的节点
		if (1 == visitedNode[i])
		{
			if (shortDistance == 0.0)
			{
				shortDistance = allDistance[currentNode][i];
				nextNode = i;
			}
			if (shortDistance < allDistance[currentNode][i]) //
			{
				nextNode = i;
			}
		}
	}
	return nextNode;
}

bool isPartsSurplus(const vector<CPart> &testCopy)
{
    for(CPart c : testCopy)
    {
        if(c.getSurplusAmount()!=0)
            return true;
    }
    return false;
}

//--------------------------主函数--------------------------------------------------
int main()
{
    //初始数据
    double C[partNum][3] = { {41,12,1},{25,34,1},{19,44,1},{115,22,1},{25,51,1},{16,22,1},{71,22,1},{44,109,1},{41,29,1},{90,87,1},{35,137,1},{31,68,1},{129,44,1},{36,15,1},{17,29,1},{19,54,1},{146,25,1} };
    double D[stoveNum][3] = { {200,200,2000} };
    caluDist cd(C,D); //包含初始化测试数据
    time_t timer, timerl;

    time(&timer);
    unsigned long seed = timer;
    seed %= 56000;
    srand((unsigned int)seed);

    //由矩阵表示两两城市之间的距离
    cd.calculateAllDistance();
    //蚁群系统对象
    AntColonySystem* acs = new AntColonySystem();
    ACSAnt* ants[antNum];
    //蚂蚁均匀分布在城市上
    for (int k = 0; k < antNum; k++)
    {
        ants[k] = new ACSAnt(acs, (int)(k % partNum));
    }
    cd.calculateAllDistance();
    cd.calculateAllWeight();

    //各条路径上初始化的信息素强度
    acs->InitParameter(cd.testData,cd.allDistance);

    //全局最优布局
    solution globalSolu;
    for (int i = 0; i < NcMax; i++)
    {
        //局部最优
        solution localSolu;
        for (int j = 0; j < antNum; j++) //每个蚂蚁行动
        {
            vector<CPart> testCopy=cd.testData; //对于每个蚂蚁，拷贝一次（记录一个零件是否被放过）
            tourType tourPath = ants[j]->Search(); //对于每个蚂蚁有一个序列
            solution currentSolu(tourPath); //这个蚂蚁的解

            bool useSolu=true;
            while(isPartsSurplus(testCopy))
            {
                //装一个炉子
                CLayout Layout=currentSolu.getNextStove(cd);
                double utilization = cd.calculateSumOfDistance(tourPath, testCopy, Layout);
                Layout.caluTime();
                currentSolu.allLayout.push_back(Layout);
                currentSolu.caluTime(); //计算这些炉子所需总时间
                if(Layout.verifyDeadLine(currentSolu.getTime())==false) //有零件不满足截止日期
                {
                    useSolu=false; //这个解不使用
                    break;
                }
            }

            if(useSolu)
            {
                if (currentSolu.getTime() > localSolu.getTime() || abs(localSolu.getTime()) < 0.000001) //是局部最好的
                    localSolu=currentSolu;
            }
        }
        //全局比较，并记录最优布局
        if (localSolu.getTime() > globalSolu.getTime() || abs(globalSolu.getTime()) < 0.000001)
            globalSolu=localSolu;

        acs->UpdateGlobalPathRule(globalSolu.tourPath, globalSolu.getTime());
        //输出所有蚂蚁循环一次后的迭代最优路径
        cout << "Iterative optimal path " << i + 1 << globalSolu.getTime() << "." << endl;
        globalSolu.output();
    }
    //输出全局最优路径
    cout << "Total value:" << globalSolu.getTime() << endl;
    cout << " Utilization ratio:" << globalSolu.utilization() << endl;
    time(&timerl);
    double t = timerl - timer;
    cout << "time:" << t << endl;
    //initgraph(640, 480);
    //setorigin(0,0);
}
