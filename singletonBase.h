#pragma once

template <typename T>
class singletonBase
{
protected:
	static T* singleton;

	singletonBase() {};
	~singletonBase() {};
public:
	//ΩÃ±€≈Ê ¡¢±Ÿ¿⁄
	static T* getSingleton(void);

	//ΩÃ±€≈Ê «ÿ¡¶ «‘ºˆ
	void releaseSingleton(void);
};

//ΩÃ±€≈Ê √ ±‚»≠
template <typename T>
T* singletonBase<T>::singleton = 0;

//ΩÃ±€≈Ê ¡¢±Ÿ¿⁄
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;

	return singleton;
}

//ΩÃ±€≈Ê «ÿ¡¶ «‘ºˆ
template <typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}