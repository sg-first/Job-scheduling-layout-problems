#pragma once
#include "caluDist.h"

//��Ϣ�������ӣ���������ʽ���ӣ�ȫ����Ϣ�ػӷ��������ֲ���Ϣ�ػӷ�����, ״̬ת�ƹ�ʽ�е�q0
const double alpha = 2, beta = 4, rou = 0.6, alpha1 = 0.1, qzero = 0.01; //these parameters produce the best solution
//��������
const int antNum=15;

class ACSAnt;

class AntColonySystem
{
private:
    double info[partNum][partNum], visible[partNum][partNum];//�ڵ�֮�����Ϣ��ǿ��,�ڵ�֮����ܼ���
public:
    //���㵱ǰ�ڵ㵽��һ�ڵ�ת�Ƶĸ���
    double Transition(int i, int j);
    //�ֲ����¹���
    void UpdateLocalPathRule(int i, int j);
    //��ʼ��
    void InitParameter(vector<CPart> testData, double allDistance[][partNum]);
    //ȫ����Ϣ�ظ���
    void UpdateGlobalPathRule(tourType bestTour, int gloalbestValue);
};


class ACSAnt
{
private:
    AntColonySystem* antColony;
protected:
    int startNode, currentNode;//��ʼ�ڵ��ţ���ǰ�ڵ���
    int allowed[partNum];//���ɱ�
    tourType Tour;//��ǰ·��
    int currentTourIndex;//��ǰ·����������0��ʼ���洢���Ͼ������еı��
public:
    ACSAnt(AntColonySystem* acs, int start)
    {
        antColony = acs;
        startNode = start;
        //��ʼ��Tour
        for(int i=0;i<partNum;i++)
            Tour.push_back({0,0});
    }
    //��ʼ����
    tourType Search();
    //ѡ����һ�ڵ�
    int Choose();
    //�ƶ�����һ�ڵ�
    void MoveToNextNode(int nextNode);
};

