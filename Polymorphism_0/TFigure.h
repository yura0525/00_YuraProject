#pragma once
class TFigure
{
protected:
	int m_iX;
public:
	TFigure();
	virtual ~TFigure();
	virtual void Draw() = 0;
};

