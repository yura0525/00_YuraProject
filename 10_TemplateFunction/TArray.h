#pragma once
template<class T>
class TArray
{
private:
	T*	m_pData;
	int m_imaxSize;
	int m_iCurPos;
public:
	TArray(int size = 100);
	~TArray();

	T	Add(T value);

	bool SetData(int pos, T data);
	bool GetData(int pos, T& data);
};

template<class T>
T TArray<T>::Add(T value)
{
	if (m_iCurPos < 0 || m_iCurPos >= m_imaxSize)
		return false;

	m_pData[m_iCurPos++] = value;
	return value;
}

template<class T>
TArray<T>::TArray(int size)
{
	m_imaxSize = size;
	m_iCurPos = 0;
	m_pData = new T[m_imaxSize];
}

template<class T>
TArray<T>::~TArray()
{
	delete[] m_pData;
}

template<class T>
bool TArray<T>::SetData(int pos, T data)
{
	if (pos < 0 || pos >= m_imaxSize)
		return false;

	m_pData[pos] = data;
	return true;
}
template<class T>
bool TArray<T>::GetData(int pos, T& data)
{
	if (pos < 0 || pos >= m_imaxSize)
		return false;

	data = m_pData[pos];
	return true;
}