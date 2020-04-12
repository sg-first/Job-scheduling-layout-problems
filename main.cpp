#include "CPart.h"
#include "CAreaList.h"
#include "CLayoutList.h"
#include "CLayout.h"
#include "ant.h"
#include <time.h>
#include <ctype.h>
#include <iostream>

//最大循环次数NcMax
int NcMax = 500;

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

//--------------------------主函数--------------------------------------------------
int main()
{
    //初始数据
    double C[partNum][3] = { {41,12,1},{25,34,1},{19,44,1},{115,22,1},{25,51,1},{16,22,1},{71,22,1},{44,109,1},{41,29,1},{90,87,1},{35,137,1},{31,68,1},{129,44,1},{36,15,1},{17,29,1},{19,54,1},{146,25,1} };
    caluDist cd(C); //包含初始化测试数据

    int start = 1;
	if (start)
	{
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
		//随机选择一个节点计算由最近邻方法得到的一个长度
        int node = rand() % partNum;

		//各条路径上初始化的信息素强度
        acs->InitParameter(cd.testData,cd.allDistance);

		//全局最优路径
        int globalTour[partNum][2];
		//全局最优长度
		double gloalbestValue = 0.0;
		for (int i = 0; i < NcMax; i++)
		{
			//局部最优路径
            int localTour[partNum][2];
			//局部最优长度
			double localBestValue = 0.0;
			//当前路径长度
			double tourCost;
            for (int j = 0; j < antNum; j++) //每个蚂蚁行动
			{
                CLayout Layout(AreaWidth, AreaHeight, AreaWeight);
                array<CPart,partNum> testCopy=cd.testData; //对于每个蚂蚁，拷贝一次
				int* tourPath = ants[j]->Search();
                tourCost = cd.calculateSumOfDistance(tourPath, testCopy,Layout); //计算每个链的总价值和利用率
				if (tourCost > localBestValue || abs(localBestValue - 0.0) < 0.000001)
				{
					//cout << "Main: tourWeight: " << tourWeight << endl;
                    for (int m = 0; m < partNum; m++)
					{
						int row = *(tourPath + 2 * m);
						int col = *(tourPath + 2 * m + 1);
						localTour[m][0] = row;
						localTour[m][1] = col;
					}
					localBestValue = tourCost;
				}
			}
			//全局比较，并记录路径和长度
			//if ((localBestValue > gloalbestValue || abs(gloalbestValue - 0.0) < 0.000001) && tourWeight <= CON_WEIGHT)
			if (localBestValue > gloalbestValue || abs(gloalbestValue - 0.0) < 0.000001)
			{
                for (int m = 0; m < partNum; m++)
				{
					globalTour[m][0] = localTour[m][0];
					globalTour[m][1] = localTour[m][1];
				}
				gloalbestValue = localBestValue;
			}
			acs->UpdateGlobalPathRule(*globalTour, gloalbestValue);
			//输出所有蚂蚁循环一次后的迭代最优路径
            cout << "Iterative optimal path " << i + 1 << localBestValue << "." << endl;
            for (int m = 0; m < partNum; m++)
			{
				cout << localTour[m][0] << ".";
				//CargoNum[m] = 0;
			}
			cout << endl;
		}
		//输出全局最优路径
        cout << "Total value:" << gloalbestValue << endl;

        cout << "Loading result:";
		double sum = 0;
		double sumWeight = 0;
        CLayout Layout(AreaWidth, AreaHeight, AreaWeight);
        CPart copyData[partNum];//将原始数据复制到里面，不然放入的时候会改变零件的数量
        for (int i = 0; i < partNum; i++)
		{
            copyData[i] = cd.testData[i];
		}

        int tour[partNum];
        for (int i = 0; i < partNum; i++)
		{
			tour[i] = globalTour[i][0];
			copyData[i].setId(i);
		}
		sum=Layout.Calculate(tour, copyData);
		cout << endl;
        cout << "Total volume:" << sum << " Utilization ratio:" << sum /Layout.m_dVolume  << endl;
        cout << "Total weight:" << sumWeight << endl;
		time(&timerl);
		double t = timerl - timer;
		//cout << testData[globalTour[0][0]].getHeight() << " " << testData[globalTour[0][0]].getWidth() << endl;
        //Layout.showLayoutList();
		cout << Layout.AvailSize() << endl;
        cout << "Number of parts:" << Layout.getLayoutListSize() << endl;
        cout << "time:" << t << endl;
		Layout.showLayoutPartNo();
		//initgraph(640, 480);
		//setorigin(0,0);
		return 0;
	}
	
}
