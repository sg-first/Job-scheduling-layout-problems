#pragma once
#include "CPart.h"
#include "CLayout.h"

//货物的数量
const int N=17;

//the max coat is 1153 total weight = 499 (the optimal solution = 1153)
const int AreaWidth = 200;
const int AreaHeight = 200;
const int AreaWeight = 2000;
const int CON_VOLUME = AreaWidth*AreaHeight ;  //背包的容积 这里当做面积
const int CON_WEIGHT = AreaWeight;//重量

//初始数据
//fix:这么写不行，但先这样
const double C[N][3] = { {41,12,1},{25,34,1},{19,44,1},{115,22,1},{25,51,1} ,{16,22,1},{71,22,1},{44,109,1},{41,29,1},{90,87,1},{35,137,1},{31,68,1},{129,44,0},{36,15,0},{17,29,0},{19,54,0},{146,25,0 } };

class caluDist
{
public:
    caluDist()
    {
        this->InitTestPart(C);
    }

    //测试数据
    CPart testData[N];
    void InitTestPart(const double C[N][3])
    {
        for (int i = 0; i < N; i++)
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

    //矩阵表示两两结点之间的体积
    double allDistance[N][N];
    double allWeight[N][N];

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
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (i != j)
                {
                    allDistance[i][j] = calculateDistance(i, j);
                    allDistance[j][i] = allDistance[i][j];
                }
                else
                    //allDistance[i][j] = C[i][0];
                    allDistance[i][j] = testData[i].getArea();
            }
        }
    }
    //由矩阵表示两两货物之间的重量
    void calculateAllWeight()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
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
    double calculateSumOfDistance(int* tour, double& utilization)
    {
        double sumVolume = 0;
        double sumCost = 0;
        CLayout Layout;
        Layout.Init(AreaWidth, AreaHeight, AreaWeight,N);
        CPart copyData[N];//将原始数据复制到里面，不然放入的时候会改变零件的数量
        for (int i = 0; i < N; i++)
        {
            copyData[i] = testData[i];
        }
        sumCost = Layout.Calculate(tour, copyData);
        //cout << "Calculate: sumCost: " << sumCost << endl;
        utilization = sumVolume / CON_VOLUME;
        return sumCost;
    }
};
