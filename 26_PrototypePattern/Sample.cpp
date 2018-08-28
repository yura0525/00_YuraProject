#include <iostream>
#include <list>
#include <map>

template<class T>
class TSingleton
{
public:
	static T& GetInstance()
	{
		static T inst;
		return inst;
	}
};

using namespace std;
class TPosition
{
public:
	TPosition()
	{
	}
	TPosition(int ix, int iy)
	{
		m_ix = ix;
		m_iy = iy;
	}
	int m_ix;
	int m_iy;
};

//프로토타입패턴은 원본을 복사해서 재사용한다. TGraphic* Clone()함수가 있어야한다.
class TGraphic
{
public:
	virtual TGraphic* Clone() = 0;
	virtual void Draw(TPosition pos) = 0;
};

class TTriangle : public TGraphic
{
public:
	TGraphic* Clone()
	{
		return new TTriangle(*this);
	}
	void Draw(TPosition pos)
	{
		cout << pos.m_ix << pos.m_iy;
	}
};
class TRectangle : public TGraphic
{
public:
	TGraphic* Clone()
	{
		return new TRectangle(*this);
	}
	void Draw(TPosition pos)
	{
		cout << pos.m_ix << pos.m_iy;
	}
};

//Composite 패턴은 여러개를 조합해서 다시 등록해서 사용한다.
class TGraphicComposite : public TGraphic
{
public:
	list<TGraphic*> m_Components;

	TGraphic* Clone()
	{
		//Clone에서 깊은 복사
		TGraphicComposite* pGC = new TGraphicComposite(*this);

		list<TGraphic*>::iterator iterSrc;

		for (iterSrc = m_Components.begin(); iterSrc != m_Components.end(); iterSrc++)
		{
			TGraphic *pNewGraphic = (*iterSrc)->Clone();
			pGC->m_Components.push_back(pNewGraphic);

			/*(*iterDest) = pNewGraphic;
			iterDest++;*/
		}

		return pGC;
	}

	void Draw(TPosition pos)
	{
		cout << pos.m_ix << pos.m_iy;
	}
};

class TMouse
{
public:
	TPosition m_pos;
	bool m_bButton[3];
public:
	bool IsLeftButtonPushed()
	{
		return m_bButton[0];
	}

	TPosition GetPosition()
	{
		return m_pos;
	}
};

TMouse g_Mouse;

class TDocument
{
public:
	list<TGraphic*> m_docList;

	void Add(TGraphic* pObj)
	{
		m_docList.push_back(pObj);
	}
};

class TGraphicEditor
{
public:

	TDocument	m_doc;

	void AddNewGraphics(TGraphic* pSelected)
	{
		TGraphic * pObj = pSelected->Clone();
		while (g_Mouse.IsLeftButtonPushed())
		{
			pObj->Draw(g_Mouse.GetPosition());
		}
		m_doc.Add(pObj);
	}
};


#define I_Palette TPalette::GetInstance()

//싱글턴 2가지 방식
//1 static TPalette* g_pPalette; 객체를 갖거나
//2 public TSingleton<TPalette> 상속한다.

//TPalette* TPalette::g_pPalette = NULL;
//싱글턴 디자인패턴, 객체를 오직 하나만 생성가능하다.
class TPalette : public TSingleton<TPalette>
{
private:
	std::map<int, TGraphic*> m_items;
	friend class TSingleton<TPalette>;

protected:
	TPalette()
	{
		//원시 모델(ex) 네모, 세모)
		TGraphic* pGraphic = new TTriangle;
		m_items[0] = pGraphic;

		TGraphic* pGraphic1 = new TRectangle;
		m_items[1] = pGraphic;
	}
public:
	void Draw()
	{

	}
	//원시 모델 추가 등록(네모와 세모를 조합하여 집 모형을 추가등록)
	void RegisterNewGraphic(TGraphic* pGraphic)
	{
		m_items[m_items.size() + 1] = pGraphic;
	}
	int GetItemOrder()
	{
		//g_Mouse 충돌처리하여 마우스가 클릭된곳에 있는 item을 얻는다.
		int iItem = 0;
		//g_Mouse.GetPosition();
		return iItem;
	}

	TGraphic* GetSelectedObj()
	{
		return m_items[ GetItemOrder()];
	}

	/*static TPalette* g_pPalette;
	static TPalette* GetInstance()
	{
		if (g_pPalette == NULL)
		{
			g_pPalette = new TPalette;
		}
		return g_pPalette;
	}*/
};

int main()
{
	TPalette palette = I_Palette;
	TPalette paletteA = TPalette::GetInstance();

	I_Palette.Draw();
	TGraphicEditor editor;

	editor.AddNewGraphics(I_Palette.GetSelectedObj());

	TGraphicComposite etc;
	list<TGraphic*>::iterator iter;

	for (iter = editor.m_doc.m_docList.begin(); iter !=editor.m_doc.m_docList.end(); iter++)
	{
		TGraphic* pData = (*iter);
		etc.m_Components.push_back(pData);
	}
	I_Palette.RegisterNewGraphic(&etc);

	/*TTriangle tri;
	TRectangle rec;
	TGraphicComposite etc;
	etc.m_Components.push_back(&tri);
	etc.m_Components.push_back(&rec);

	palette.RegisterNewGraphic(&etc);

	TRectangle rec1;
	TRectangle rec2;
	TGraphicComposite etc1;
	etc.m_Components.push_back(&rec1);
	etc.m_Components.push_back(&rec2);

	palette.RegisterNewGraphic(&etc1);

	TGraphicComposite etc3;
	TGraphicComposite etc4;
	etc.m_Components.push_back(&etc3);
	etc.m_Components.push_back(&etc4);
	palette.RegisterNewGraphic(&etc);*/

	return 0;
}
