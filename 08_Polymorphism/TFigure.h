#pragma once
class TFigure
{
public:
	TFigure();
	virtual ~TFigure();

	virtual void Draw() = 0;
};

