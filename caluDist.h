#pragma once
#include "CPart.h"
#include "CLayout.h"
#include <array>

const int AreaWidth = 200;
const int AreaHeight = 200;
const int AreaWeight = 2000;

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
    array<CPart,partNum> testData;
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
            testData[i].setId(i);
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
    double calculateSumOfDistance(int *tour, array<CPart,partNum> &allPart, CLayout &Layout)
    {
        return Layout.Calculate(tour, allPart);
    }
};
