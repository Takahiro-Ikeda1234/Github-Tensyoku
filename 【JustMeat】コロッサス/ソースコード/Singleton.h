#pragma once
template <typename T>
class Singleton
{
protected:
	Singleton() {};
	virtual ~Singleton() {};
	Singleton(const Singleton& singleton) {};
	Singleton& operator=(const Singleton& singleton) {};
public:
	static T* Inst()
	{
		static T inst;
		return &inst;
	}
};