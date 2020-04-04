#include "ant.h"
#include <math.h>

double AntColonySystem::totalVolume;
double AntColonySystem::totalCost;

//���㵱ǰ�ڵ㵽��һ�ڵ�ת�Ƶĸ���
double AntColonySystem::Transition(int i, int j)
{
    if (i != j)
    {
        return (pow(info[i][j], alpha) * pow(visible[i][j], beta)); //��һ���������Ѿ�ѡ��
    }
    else
    {
        return 0.0;
    }
}
//�ֲ����¹���
void AntColonySystem::UpdateLocalPathRule(int i, int j)
{
    info[i][j] = (1.0 - alpha1) * info[i][j] + alpha1 * (1.0 / (totalCost)); // 1/N *totalVolume
    info[j][i] = info[i][j];
}
//��ʼ��
void AntColonySystem::InitParameter(double value, CPart testData[N], double allDistance[N][N])
{
    //��ʼ��·���ϵ���Ϣ��ǿ��tao0
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            info[i][j] = value;
            info[j][i] = value;
            if (i != j)
            {
                //visible[i][j] = C[j][1] / totalCost + 1 / allDistance[i][j];
                visible[i][j] = testData[j].getArea() / totalCost + 1 / allDistance[i][j]; //������ʹ��allDistance��ʼ��
                visible[j][i] = visible[i][j];
            }
        }
    }
}

//ȫ����Ϣ�ظ���
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



//��ʼ����
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

//ѡ����һ�ڵ�
int ACSAnt::Choose()
{
    int nextNode = -1;
    double q = rand() / (double)RAND_MAX;
    //��� q <= q0,������֪ʶ�������򰴸���ת�ƣ�
    if (q <= qzero)
    {
        double probability = -1.0;//ת�Ƶ���һ�ڵ�ĸ���
        for (int i = 0; i < N; i++)
        {
            //ȥ�����ɱ������߹��Ľڵ�,��ʣ�½ڵ���ѡ�������ʵĿ��нڵ�
            if (1 == allowed[i])
            {
                double prob = antColony->Transition(currentNode, i); //��һ���������Ѿ�ѡ��
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
        //������ת��
        double p = rand() / (double)RAND_MAX;//����һ�������,�����ж������ĸ������
        double sum = 0.0;
        double probability = 0.0;//���ʵ�����㣬p �����ĸ�����Σ���õ���ת�Ƶķ���
        //������ʹ�ʽ�ķ�ĸ��ֵ
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

//�ƶ�����һ�ڵ�
void ACSAnt::MoveToNextNode(int nextNode)
{
    //�����ǲ���Ҫ��������������޸ģ�
    allowed[nextNode] = 0;
    Tour[currentTourIndex][0] = currentNode;
    Tour[currentTourIndex][1] = nextNode;
    currentTourIndex++;
    currentNode = nextNode;
}
