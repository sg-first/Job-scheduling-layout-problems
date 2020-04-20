#pragma once

class CPart
{
private:
    double dWeight;//重量
    double dHeight;
    double dWidth;
    double dArea; //面积
    int nAmount; //初始有多少块
    int nSurplusAmount; //剩余多少块
    int nId; //零件编号

    int deadline;
    int time;

public:
    CPart(double dWeight, double dHeight, double dWidth, int nId, int deadline, int time) :
        dWeight(dWeight), dHeight(dHeight), dWidth(dWidth), dArea(dHeight * dWidth), nId(nId), deadline(deadline), time(time) {}
    CPart(){}
    double getWeight() const { return dWeight; }
    double getHeight() const { return dHeight; }
    double getWidth() const { return dWidth; }
    double getArea() const { return dArea; }
    int getSurplusAmount() const { return nSurplusAmount; }
    void setAmount(int amount)
    {
        this->nAmount = amount;
        this->nSurplusAmount = nAmount;
    }
	void changeAmount(int nPutInAmount) { this->nSurplusAmount -= nPutInAmount; }//放入后修改剩余数量
    int getID() const { return this->nId; }
    int getDeadline() const { return this->deadline; }
    int getTime() const { return this->time; }
};

