#include "ant.h"
#include <math.h>

double AntColonySystem::totalVolume;
double AntColonySystem::totalCost;

//计算当前节点到下一节点转移的概率
double AntColonySystem::Transition(int i, int j)
{
    if (i != j)
    {
        return (pow(info[i][j], alpha) * pow(visible[i][j], beta)); //第一个参数是已经选的
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
                visible[i][j] = testData[j].getArea() / totalCost + 1 / allDistance[i][j]; //这里有使用allDistance初始化
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
        info[col][row] = info[row][col]; //fix:
    }
}



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
                double prob = antColony->Transition(currentNode, i); //第一个参数是已经选的
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
