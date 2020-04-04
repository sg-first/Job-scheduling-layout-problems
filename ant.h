#pragma once
#include "caluDist.h"

//��Ϣ�������ӣ���������ʽ���ӣ�ȫ����Ϣ�ػӷ��������ֲ���Ϣ�ػӷ�����, ״̬ת�ƹ�ʽ�е�q0
const double alpha = 1, beta = 2, rou = 0.3, alpha1 = 0.1, qzero = 0.01; //these parameters produce the best solution

class ACSAnt;

class AntColonySystem
{
private:
    double info[N][N], visible[N][N];//�ڵ�֮�����Ϣ��ǿ��,�ڵ�֮����ܼ���
public:
    //ÿ���ڵ㶼������ΪԴ�ڵ�������
    static double totalVolume;
    static double totalCost;
    //���㵱ǰ�ڵ㵽��һ�ڵ�ת�Ƶĸ���
    double Transition(int i, int j);
    //�ֲ����¹���
    void UpdateLocalPathRule(int i, int j);
    //��ʼ��
    void InitParameter(double value, CPart testData[], double allDistance[][N]);
    //ȫ����Ϣ�ظ���
    void UpdateGlobalPathRule(int* bestTour, int gloalbestValue);
};


class ACSAnt
{
private:
    AntColonySystem* antColony;
protected:
    int startNode, currentNode;//��ʼ�ڵ��ţ���ǰ�ڵ���
    int allowed[N];//���ɱ�
    int Tour[N][2];//��ǰ·��
    int currentTourIndex;//��ǰ·����������0��ʼ���洢���Ͼ������еı��
public:
    ACSAnt(AntColonySystem* acs, int start)
    {
        antColony = acs;
        startNode = start;
    }
    //��ʼ����
    int* Search();
    //ѡ����һ�ڵ�
    int Choose();
    //�ƶ�����һ�ڵ�
    void MoveToNextNode(int nextNode);
};

