#pragma once
class CAreaList
{
	double x;//���½ǵ�����
	double y;
	double Height;//����ĸ߶�
	double Width;//����Ŀ��
public:
	bool operator<(const CAreaList& t2)//����Ӧ��������ԭ��
	{
		if (x != t2.x)
		{
			return x < t2.x;//�������������
		}
		else
		{
			return y < t2.y;
		}
	}
	/*bool operator<(const CAreaList& t2)
	{
		return this->getArea() < t2.Height * t2.Width;
	}*/
	CAreaList() {};
	~CAreaList() {};
	double getX() { return x; }
	double getY() { return y; }
	double getWidth() { return Width; }
	double getHeight() { return Height; }
	double getArea() { return Width * Height; }
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setWidth(double Width) { this->Width = Width; }
	void setHeight(double Height) { this->Height = Height; }
};


