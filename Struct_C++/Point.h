#pragma once

class Point {
private:
	int x;
	int y;
public:
	void SetPosition(int a, int b);
	void Move(int a, int b);
	void Show();
public:
	Point();
	~Point();
};
