#pragma once
#include "CPart.h"
#include "CLayout.h"

const int partNum=17;

//the max coat is 1153 total weight = 499 (the optimal solution = 1153)
const int AreaWidth = 200;
const int AreaHeight = 200;
const int AreaWeight = 2000;
const int CON_VOLUME = AreaWidth*AreaHeight ;  //�������ݻ� ���ﵱ�����
const int CON_WEIGHT = AreaWeight;//����

class caluDist
{
private:
    int getMaxDeadLine()
    {
        int max=0;
        for (int i = 0; i < partNum; i++)
        {
            if(this->C[i][4]>max)
                max=this->C[i][4];
        }
        return max;
    }

    void deadLineInv()
    {
        int max=getMaxDeadLine();
        for (int i = 0; i < partNum; i++)
            this->C[i][4]=max-this->C[i][4];
    }

public:
    double (*C)[3];

    caluDist(double C[partNum][3])
    {
        this->C=C;
        this->InitTestPart();
    }

    //��������
    CPart testData[partNum];
    void InitTestPart()
    {
        for (int i = 0; i < partNum; i++)
        {
            double weight, width, height;
            width = C[i][0];
            height = C[i][1];
            weight = 1;
            //cout << "Weight: " << C[i][2] << endl;
            testData[i].setInfo(width, height, weight);
            testData[i].setAmount(1);
            testData->setId(i);
            //cout << "Weight: " << testData[i].getWeight() << endl;
        }
    }

    //�����ʾ�������֮������
    double allDistance[partNum][partNum];
    double allWeight[partNum][partNum];

    //��������������ۼ����
    double calculateDistance(int i, int j)
    {
        //return (C[i][0] + C[j][0]);
        return testData[i].getArea() + testData[j].getArea();
    }

    double calculateWeight(int i, int j)
    {
        //return (C[i][0] + C[j][0]);
        return testData[i].getWeight() + testData[j].getWeight();
    }

    //�ɾ����ʾ��������֮������ �������֮��Ӧ�������
    void calculateAllDistance()
    {
        for (int i = 0; i < partNum; i++)
        {
            for (int j = 0; j < partNum; j++)
            {
                if (i != j)
                {
                    allDistance[i][j] = testData[j].getArea(); //fix:�ĳɽ�ֹ����
                    allDistance[j][i] = testData[i].getArea();
                }
                else
                    allDistance[i][j] = testData[j].getArea();
            }
        }
    }
    //�ɾ����ʾ��������֮�������
    void calculateAllWeight()
    {
        for (int i = 0; i < partNum; i++)
        {
            for (int j = 0; j < partNum; j++)
            {
                if (i != j)
                {
                    allWeight[i][j] = calculateWeight(i, j);
                    allWeight[j][i] = allWeight[i][j];
                }
                else
                    //allDistance[i][j] = C[i][0];
                    allWeight[i][j] = testData[i].getWeight();
            }
        }
    }

    //��þ���n������·���õ��������
    double calculateSumOfDistance(int* tour, double& utilization)
    {
        double sumVolume = 0;
        double sumCost = 0;
        CLayout Layout;
        Layout.Init(AreaWidth, AreaHeight, AreaWeight,partNum);
        CPart copyData[partNum];//��ԭʼ���ݸ��Ƶ����棬��Ȼ�����ʱ���ı����������
        for (int i = 0; i < partNum; i++)
        {
            copyData[i] = testData[i];
        }
        sumCost = Layout.Calculate(tour, copyData);
        //cout << "Calculate: sumCost: " << sumCost << endl;
        utilization = sumVolume / CON_VOLUME;
        return sumCost;
    }
};
