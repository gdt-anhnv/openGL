#ifndef _SINGLETON_H
#define _SINGLETON_H

template<typename T>
class Singleton
{
public:
	static T* GetInstance();
	static void destroy();
	static void AssignInstance(T*);

private:

	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};

protected:
	static T* m_instance;

	Singleton() { m_instance = static_cast <T*> (this); };
	~Singleton() {  };
};

template<typename T>
typename T* Singleton<T>::m_instance = 0;

template<typename T>
T* Singleton<T>::GetInstance()
{
	return m_instance;
}

template <typename T>
void Singleton<T>::AssignInstance(T* t)
{
	m_instance = t;
}

template<typename T>
void Singleton<T>::destroy()
{
	delete Singleton<T>::m_instance;
	Singleton<T>::m_instance = 0;
}

#endif