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

    //测试数据
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

    //矩阵表示两两结点之间的体积
    double allDistance[partNum][partNum];
    double allWeight[partNum][partNum];

    //计算两个货物的累加体积
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

    //由矩阵表示两两货物之间的体积 这里改完之后应该是面积
    void calculateAllDistance()
    {
        for (int i = 0; i < partNum; i++)
        {
            for (int j = 0; j < partNum; j++)
            {
                if (i != j)
                {
                    allDistance[i][j] = testData[j].getArea(); //fix:改成截止日期
                    allDistance[j][i] = testData[i].getArea();
                }
                else
                    allDistance[i][j] = testData[j].getArea();
            }
        }
    }
    //由矩阵表示两两货物之间的重量
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

    //获得经过n个结点的路径得到的总体积
    double calculateSumOfDistance(int *tour, array<CPart,partNum> &allPart, CLayout &Layout)
    {
        return Layout.Calculate(tour, allPart);
    }
};
