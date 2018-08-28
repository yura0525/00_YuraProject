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

//핸들링, Facade
class FacadeHandler
{
public:
	void DoWork(ImageWork& imageWork, ImageResult& result)
	{
		//....
		//m_Image = ImageLoad(imageWork.m_szName);
		////흑백명암으로 변경해라.
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