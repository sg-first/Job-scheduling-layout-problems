#pragma once
#include "CPart.h"
#include "CLayout.h"
#include <array>
#include <vector>
#include <algorithm>

const int AreaWidth = 200;
const int AreaHeight = 200;
const int AreaWeight = 2000;

class caluDist
{
public:
    double (*C)[5];
    vector<CLayout*> allStove;

    caluDist(double C[partNum][5], double D[stoveNum][3])
    {
        this->C=C;
        //初始化零件
        for (int i = 0; i < partNum; i++)
        {
            double width = C[i][0];
            double height = C[i][1];
            double weight = C[i][2]; //fix:改成实际重量
            CPart part(weight, width, height, i, C[i][3], C[i][4]);
            part.setAmount(1);
            this->testData.push_back(part);
        }
        //初始化炉子
        for(int i=0;i<stoveNum;i++)
        {
            auto Di=D[i];
            auto layout=new CLayout(Di[0],Di[1],Di[2],this->allStove.size()); //下标就是ID
            this->allStove.push_back(layout);
        }
        sort(this->allStove.begin(), this->allStove.end(),
             [](CLayout* &a, CLayout* &b){ return a->getArea() > b->getArea(); }); //降序排列
    }

    ~caluDist()
    {
        for(auto i : this->allStove)
            delete i;
    }

    //测试数据
    vector<CPart> testData;

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
    double calculateSumOfDistance(tourType tour, vector<CPart> &allPart, CLayout &Layout)
    {
        return Layout.Calculate(tour, allPart);
    }
};
