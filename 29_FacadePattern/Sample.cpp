#include <iostream>

class ImageWork
{

};

class Image
{

};

class ImageLoad
{

};

class ImageResult
{

};

//�ڵ鸵, Facade
class FacadeHandler
{
public:
	void DoWork(ImageWork& imageWork, ImageResult& result)
	{
		//....
		//m_Image = ImageLoad(imageWork.m_szName);
		////��������� �����ض�.
		//result = imageWork.DoWork(m_Image);
	}
private:
	Image m_Image;
};

int main()
{
	/*ImageWork image("aaa.bmp", blank);
	ImageResult result;

	FacadeHandler work;
	work.DoWork(image, result);*/
	return 0;
}