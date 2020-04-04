#pragma once
class CPart
{
private:
	double dWeight;//����
	double dHeight;//
	double dWidth;
	double dArea;//���
	int nAmount;//��ʼ�ж��ٿ�
	int nSurplusAmount;//ʣ����ٿ�
	int nId;//������
public:
	CPart(double dWeight, double dHeight, double dWidth) :dWeight(dWeight), dHeight(dHeight), dWidth(dWidth) { dArea = dHeight * dWidth; }
	CPart() {}
	double getWeight() { return dWeight; }
	double getHeight() { return dHeight; }
	double getWidth() { return dWidth; }
	double getArea() { return dArea; }
	int getSurplusAmount() { return nSurplusAmount; }
	void setInfo(double dWidth, double dHeight, double dWeight) { this->dWeight = dWeight; this->dWidth = dWidth; this->dHeight = dHeight;dArea = dHeight * dWidth; }
	void setAmount(int amount) { this->nAmount = amount; this->nSurplusAmount = nAmount; }
	void changeAmount(int nPutInAmount) { this->nSurplusAmount -= nPutInAmount; }//������޸�ʣ������
	void setId(int nId) { this->nId = nId; }
	int getID() { return this->nId; }
};
