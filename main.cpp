#include "CPart.h"
#include "CAreaList.h"
#include "CLayoutList.h"
#include "CLayout.h"
#include "caluDist.h"
#include <time.h>

#include <ctype.h>

#include <iostream>
#include <cmath>
using namespace std;

//该程序是以蚁群系统为模型写的蚁群算法程序(强调：非蚂蚁周模型)，以背包问题为测试对象
//通过微调参数，都可以获得较好的解
/*
//-------------------第一个问题自己设计的数据------------------------
const int CON_VOLUME = 57;//背包的容积
//货物的数量
#define N 6
//volume and weigh of each cargo
double C[N][2]={
	{10,2},{5,13},{7,64},{13,40},{18,54}, {20, 10}};

//----------上面参数是固定的，下面的参数是可变的-----------
//蚂蚁数量
#define M 6
//最大循环次数NcMax
int NcMax = 50;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
double alpha = 2, beta = 3, rou = 0.6, alpha1 = 0.1,  qzero = 0.01;
//------------------------End poblem--------------------------------------------------
*/
/*
//-------------------------The second problem ---------------------------------------
// the max coat is 1063 total weight = 300 (the optimal solution = 1063)
const int CON_VOLUME = 300;//背包的容积
//货物的数量
#define N 50
//weight and cost of each cargo: 0-weight; 1-cost
double C[N][2]={
		{71, 26},{34, 59},{82, 30},{23, 19},{1, 66},{88, 85},{12, 94},{57, 8},{10, 3},{68, 44},
		{5, 5},{33, 1},{37, 41},{69, 82},{98, 76},{24, 1},{26, 12},{83, 81},{16, 73},{26, 32},
		{18, 74},{43, 54},{52, 62},{71, 41},{22, 19},{65, 10},{68, 65},{8, 53},{40, 56},{40, 53},
		{24, 70},{72, 66},{16, 58},{34, 22},{10, 72},{19, 33},{28, 96},{13, 88},{34, 68},{98, 45},
		{29, 44},{31, 61},{79, 78},{33, 78},{60, 6},{74, 66},{44, 11},{56, 59},{54, 83},{17, 48}
	};
//----------上面参数是固定的，下面的参数是可变的-----------
//蚂蚁数量
#define M 50
//最大循环次数NcMax
int NcMax = 1000;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
double alpha = 2, beta = 4, rou = 0.3, alpha1 = 0.1,  qzero = 0.01;
//------------------------End problem--------------------------------------------------
*/
//-----------------------------------the third problem----------------------------------------------

//蚂蚁数量
//#define M 50
#define M 10
//最大循环次数NcMax
//int NcMax = 1000;
int NcMax = 200;
//信息启发因子，期望启发式因子，全局信息素挥发参数，局部信息素挥发参数, 状态转移公式中的q0
//double alpha = 2, beta = 5, rou = 0.3, alpha1 = 0.1, qzero = 0.01; //these parameters produce the best solution
double alpha = 1, beta = 2, rou = 0.3, alpha1 = 0.1, qzero = 0.01; //these parameters produce the best solution
//------------------------End poblem--------------------------------------------------


//===========================================================================================================
//局部更新时候使用的的常量，它是由最近邻方法得到的一个长度
//什么是最近邻方法?:)就是从源节点出发，每次选择一个距离最短的点来遍历所有的节点得到的路径


//每个节点都可能作为源节点来遍历
double totalVolume;
double totalCost;

int CargoNum[N];  //number of each type cargo

class ACSAnt;

class AntColonySystem
{
private:
	double info[N][N], visible[N][N];//节点之间的信息素强度,节点之间的能见度
public:
	AntColonySystem()
	{
	}
	//计算当前节点到下一节点转移的概率
	double Transition(int i, int j);
	//局部更新规则
	void UpdateLocalPathRule(int i, int j);
	//初始化
    void InitParameter(double value, CPart testData[], double allDistance[][N]);
	//全局信息素更新
	void UpdateGlobalPathRule(int* bestTour, int gloalbestValue);
};

//计算当前节点到下一节点转移的概率
double AntColonySystem::Transition(int i, int j)
{
	if (i != j)
	{
		return (pow(info[i][j], alpha) * pow(visible[i][j], beta));
	}
	else
	{
		return 0.0;
	}
}
//局部更新规则
void AntColonySystem::UpdateLocalPathRule(int i, int j)
{
	info[i][j] = (1.0 - alpha1) * info[i][j] + alpha1 * (1.0 / (totalCost)); // 1/N *totalVolume
	info[j][i] = info[i][j];
}
//初始化
void AntColonySystem::InitParameter(double value, CPart testData[N], double allDistance[N][N])
{
	//初始化路径上的信息素强度tao0
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			info[i][j] = value;
			info[j][i] = value;
			if (i != j)
			{
				//visible[i][j] = C[j][1] / totalCost + 1 / allDistance[i][j];
				visible[i][j] = testData[j].getArea() / totalCost + 1 / allDistance[i][j];
				visible[j][i] = visible[i][j];
			}
		}
	}
}

//全局信息素更新
void AntColonySystem::UpdateGlobalPathRule(int* bestTour, int gloalbestValue)
{
	for (int i = 0; i < N; i++)
	{
		int row = *(bestTour + 2 * i);
		int col = *(bestTour + 2 * i + 1);
		info[row][col] = (1.0 - rou) * info[row][col] + rou * 1 / (totalCost - gloalbestValue);
		info[col][row] = info[row][col];
	}
}

class ACSAnt
{
private:
	AntColonySystem* antColony;
protected:
	int startNode, currentNode;//初始节点编号，当前节点编号
	int allowed[N];//禁忌表	
	int Tour[N][2];//当前路径
	int currentTourIndex;//当前路径索引，从0开始，存储蚂蚁经过城市的编号
public:
	ACSAnt(AntColonySystem* acs, int start)
	{
		antColony = acs;
		startNode = start;
	}
	//开始搜索
	int* Search();
	//选择下一节点
	int Choose();
	//移动到下一节点
	void MoveToNextNode(int nextNode);

};

//开始搜索
int* ACSAnt::Search()
{
	currentNode = startNode;
	int toNode;
	currentTourIndex = 0;
	for (int i = 0; i < N; i++)
	{
		allowed[i] = 1;
	}
	allowed[currentNode] = 0;
	int endNode;
	int count = 0;
	do
	{
		count++;
		endNode = currentNode;
		toNode = Choose();
		if (toNode >= 0)
		{
			MoveToNextNode(toNode);
			antColony->UpdateLocalPathRule(endNode, toNode);
			currentNode = toNode;
		}
	} while (toNode >= 0);
	MoveToNextNode(startNode);
	antColony->UpdateLocalPathRule(endNode, startNode);

	return *Tour;
}

//选择下一节点
int ACSAnt::Choose()
{
	int nextNode = -1;
	double q = rand() / (double)RAND_MAX;
	//如果 q <= q0,按先验知识，否则则按概率转移，
	if (q <= qzero)
	{
		double probability = -1.0;//转移到下一节点的概率
		for (int i = 0; i < N; i++)
		{
			//去掉禁忌表中已走过的节点,从剩下节点中选择最大概率的可行节点
			if (1 == allowed[i])
			{
				double prob = antColony->Transition(currentNode, i);
				if (prob > probability)
				{
					nextNode = i;
					probability = prob;
				}
			}
		}
	}
	else
	{
		//按概率转移			
		double p = rand() / (double)RAND_MAX;//生成一个随机数,用来判断落在哪个区间段
		double sum = 0.0;
		double probability = 0.0;//概率的区间点，p 落在哪个区间段，则该点是转移的方向
		//计算概率公式的分母的值
		for (int i = 0; i < N; i++)
		{
			if (1 == allowed[i])
			{
				sum += antColony->Transition(currentNode, i);
			}
		}
		for (int j = 0; j < N; j++)
		{
			if (1 == allowed[j] && sum > 0)
			{
				probability += antColony->Transition(currentNode, j) / sum;
				if (probability >= p || (p > 0.9999 && probability > 0.9999))
				{
					nextNode = j;
					break;
				}
			}
		}
	}
	return nextNode;
}

//移动到下一节点
void ACSAnt::MoveToNextNode(int nextNode)
{
	//这里是不是要根据零件的数量修改？
	allowed[nextNode] = 0;
	Tour[currentTourIndex][0] = currentNode;
	Tour[currentTourIndex][1] = nextNode;
	currentTourIndex++;
	currentNode = nextNode;
}

//------------------------------------------
//选择下一个节点，配合下面的函数来计算的长度
int ChooseNextNode(int currentNode, int visitedNode[],double allDistance[N][N])
{
	int nextNode = -1;
	double shortDistance = 0.0;
	for (int i = 0; i < N; i++)
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

//给一个节点由最近邻距离方法计算长度
double CalAdjacentDistance(int node, double& sumCost, caluDist &cd)
{
	double sum = 0.0;
	sumCost = 0;
	int visitedNode[N];
	for (int j = 0; j < N; j++)
	{
		visitedNode[j] = 1;
	}
	visitedNode[node] = 0;
	int currentNode = node;
	int nextNode;
	do
	{
        nextNode = ChooseNextNode(currentNode, visitedNode, cd.allDistance);
		if (nextNode >= 0)
		{
            sum += cd.allDistance[currentNode][nextNode] - cd.allDistance[currentNode][currentNode];
            sumCost += cd.C[currentNode][1];
			currentNode = nextNode;
			visitedNode[currentNode] = 0;
		}
	} while (nextNode >= 0);
    sum += cd.allDistance[currentNode][node] - cd.allDistance[currentNode][currentNode];

	//sumCost += C[currentNode][1];
    sumCost += cd.testData[currentNode].getArea();
	return sum;
}

void CLayout::FindUseless(CPart* data)
{
	if (m_lstAvailable.empty()) return;
	list<CAreaList>::iterator itAvail = m_lstAvailable.begin();
	int Useless = 1;
	for (int i = 0; i < N; i++)
	{
		int nCanbePutIn = this->CanBePutIn(data[i]);
		if (nCanbePutIn != 0)
		{
			//cout << "Canputin" << endl;
			Useless = 0;
			return;
		}
	}
	if (Useless)
	{
		CAreaList tmpArea;
		tmpArea.setX(itAvail->getX());
		tmpArea.setY(itAvail->getY());
		tmpArea.setWidth(itAvail->getWidth());
		tmpArea.setHeight(itAvail->getHeight());
		m_lstUseless.push_back(tmpArea);
		//cout << "Useless" << endl;
		//cout << "FindUseless UselessSize " << m_lstUseless.size() << endl;
		m_lstAvailable.pop_front();
		//m_lstAvailable.erase(itAvail);
	}
}


//---------------------------------结束---------------------------------------------

//--------------------------主函数--------------------------------------------------
int main()
{
    //初始数据
    double C[N][3] = { {41,12,1},{25,34,1},{19,44,1},{115,22,1},{25,51,1},{16,22,1},{71,22,1},{44,109,1},{41,29,1},{90,87,1},{35,137,1},{31,68,1},{129,44,0},{36,15,0},{17,29,0},{19,54,0},{146,25,0 } };
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
		ACSAnt* ants[M];
		//蚂蚁均匀分布在城市上
		for (int k = 0; k < M; k++)
		{
			ants[k] = new ACSAnt(acs, (int)(k % N));
		}
        cd.calculateAllDistance();
        cd.calculateAllWeight();
		//随机选择一个节点计算由最近邻方法得到的一个长度
		int node = rand() % N;

        totalVolume = CalAdjacentDistance(node, totalCost,cd);

		//各条路径上初始化的信息素强度
		double initInfo = 1 / (totalCost);      //1 / (N * totalVolume);
        acs->InitParameter(initInfo,cd.testData,cd.allDistance);

		//全局最优路径
		int globalTour[N][2];
		//全局最优长度
		double gloalbestValue = 0.0;
		for (int i = 0; i < NcMax; i++)
		{
			//局部最优路径
			int localTour[N][2];
			//局部最优长度
			double localBestValue = 0.0;
			//当前路径长度
			double tourCost;
			double tourUtil = 0;
			for (int j = 0; j < M; j++)
			{
				CLayout Layout;
				Layout.Init(AreaWidth, AreaHeight, AreaWeight, N);
				CPart testCopy[N];
				for (int i = 0; i < N; i++)
				{
                    testCopy[i] = cd.testData[i];
				}
				int* tourPath = ants[j]->Search();
                cout<<tourPath[0]<<" "<<tourPath[1]<<" "<<tourPath[2]<<" "<<tourPath[3]<<" "<<tourPath[3]<<endl; //fix:调试中，看看数据结构
                tourCost = cd.calculateSumOfDistance(tourPath, tourUtil);			//计算每个链的总价值和利用率
				//tourCost = Layout.Calculate(tourPath, tourUtil, testCopy);		//计算每个链的总价值和利用率
				//tourWeight = calculateSumOfWeight(tourPath, tourUtil2);//计算重量利用率
				//cout << "Main: tourWeight: " << tourWeight << endl;
				//局部比较，并记录路径和长度
				//if ((tourCost > localBestValue || abs(localBestValue - 0.0) < 0.000001) && tourWeight <= CON_WEIGHT)
				if (tourCost > localBestValue || abs(localBestValue - 0.0) < 0.000001)
				{
					//cout << "Main: tourWeight: " << tourWeight << endl;
					for (int m = 0; m < N; m++)
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
				for (int m = 0; m < N; m++)
				{
					globalTour[m][0] = localTour[m][0];
					globalTour[m][1] = localTour[m][1];
				}
				gloalbestValue = localBestValue;
			}
			acs->UpdateGlobalPathRule(*globalTour, gloalbestValue);
			//输出所有蚂蚁循环一次后的迭代最优路径
            cout << "Iterative optimal path " << i + 1 << localBestValue << "." << endl;
			for (int m = 0; m < N; m++)
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
		CLayout Layout;
		Layout.Init(AreaWidth, AreaHeight, AreaWeight, N);
		CPart copyData[N];//将原始数据复制到里面，不然放入的时候会改变零件的数量
		for (int i = 0; i < N; i++)
		{
            copyData[i] = cd.testData[i];
		}

        int tour[N];
		for (int i = 0; i < N; i++)
		{
			tour[i] = globalTour[i][0];
			copyData[i].setId(i);
		}
		sum=Layout.Calculate(tour, copyData);
		cout << endl;
        cout << "Total volume:" << sum << " Utilization ratio:" << sum / CON_VOLUME << endl;
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
//--------------------------主函数结束--------------------------------------------------
