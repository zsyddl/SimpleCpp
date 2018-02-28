#pragma once
#include "Thread.h"

#include "sigc++/sigc++.h"
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>
//可以把任意函数 作为 线程函数执行
//
#define SCCORE_API
using namespace sigc;
namespace SimpleCpp{

template<class T_return >
class  CThreadAnyFunction0 :
	public CThread
{
public:
	CThreadAnyFunction0(sigc::slot<T_return> function)
		: m_pFunction(function)
	{

	}
	virtual ~CThreadAnyFunction0(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction();
		return nRet;
	};

protected:
	sigc::slot<T_return> m_pFunction;;
};

template <class T_return, class T_arg1>
class CThreadAnyFunction1 :
	public CThread
{
public:
	CThreadAnyFunction1(sigc::slot<T_return, T_arg1> function, 
		T_arg1 arg1)
		: m_pFunction(function)
		, m_arg1(arg1)
	{

	}
	virtual ~CThreadAnyFunction1(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1> m_pFunction;;
	T_arg1 m_arg1;

};
template <class T_return, class T_arg1, class T_arg2>
class  CThreadAnyFunction2 :
	public CThread
{
public:
	CThreadAnyFunction2(sigc::slot<T_return, T_arg1, T_arg2> function, 
		T_arg1 arg1, T_arg2 arg2)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg2(arg2)
	{

	}
	virtual ~CThreadAnyFunction2(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2> m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
};
template <class T_return, class T_arg1, class T_arg2 , class T_arg3>
class  CThreadAnyFunction3 :
	public CThread
{
public:
	CThreadAnyFunction3(sigc::slot<T_return, T_arg1, T_arg2, T_arg3> function, 
		T_arg1 arg1, T_arg2 arg2, T_arg3 arg3)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg2(arg2)
		, m_arg3(arg3)
	{

	}
	virtual ~CThreadAnyFunction3(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2, m_arg3);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2, T_arg3> m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
	T_arg3 m_arg3;
};
template <class T_return, class T_arg1, class T_arg2 , class T_arg3 , class T_arg4>
class  CThreadAnyFunction4 :
	public CThread
{
public:
	CThreadAnyFunction4(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4> function, 
		T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg2(arg2)
		, m_arg3(arg3)
		, m_arg4(arg4)
	{

	}
	virtual ~CThreadAnyFunction4(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2, m_arg3, m_arg4);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4> m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
	T_arg3 m_arg3;
	T_arg4 m_arg4;
};
template <class T_return, class T_arg1, class T_arg2 , class T_arg3 , class T_arg4 , class T_arg5 >
class  CThreadAnyFunction5 :
	public CThread
{
public:

	CThreadAnyFunction5(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5> function, 
		T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg3(arg2)
		, m_arg3(arg3)
		, m_arg4(arg4)
		, m_arg5(arg5)
	{

	}

		virtual ~CThreadAnyFunction5(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5> m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
	T_arg3 m_arg3;
	T_arg4 m_arg4;
	T_arg5 m_arg5;

};
template <class T_return, class T_arg1, class T_arg2 , class T_arg3 , class T_arg4 , class T_arg5 , class T_arg6>
class  CThreadAnyFunction6 :
	public CThread
{
public:
	CThreadAnyFunction6(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6> function, 
		T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg3(arg2)
		, m_arg3(arg3)
		, m_arg4(arg4)
		, m_arg5(arg5)
		, m_arg6(arg6)
	{

	}
	virtual ~CThreadAnyFunction6(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6 > m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
	T_arg3 m_arg3;
	T_arg4 m_arg4;
	T_arg5 m_arg5;
	T_arg6 m_arg6;
};

template <class T_return, class T_arg1, class T_arg2 , class T_arg3 , class T_arg4 , class T_arg5 , class T_arg6, class T_arg7 >
class  CThreadAnyFunction7 :
	public CThread
{
public:
	CThreadAnyFunction7(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7> function, 
						T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6, T_arg7 arg7)
		: m_pFunction(function)
		, m_arg1(arg1)
		, m_arg3(arg2)
		, m_arg3(arg3)
		, m_arg4(arg4)
		, m_arg5(arg5)
		, m_arg6(arg6)
		, m_arg7(arg7)
	{
	};
	virtual ~CThreadAnyFunction7(void){
	};

	virtual int Run()
	{
		int nRet = 0;
		m_pFunction(m_arg1, m_arg2, m_arg3, m_arg4, m_arg5, m_arg6, m_arg7);
		return nRet;
	};

protected:
	sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7 > m_pFunction;;
	T_arg1 m_arg1;
	T_arg2 m_arg2;
	T_arg3 m_arg3;
	T_arg4 m_arg4;
	T_arg5 m_arg5;
	T_arg6 m_arg6;
	T_arg7 m_arg7;
};

template <class T_return, class T_arg1=nil, class T_arg2=nil, class T_arg3=nil, class T_arg4=nil, class T_arg5=nil, class T_arg6=nil, class T_arg7=nil >
class  CThreadAnyFunction
	: public CThreadAnyFunction7<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7> function,
						T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6, T_arg7 arg7)
		: CThreadAnyFunction7(function, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	{

	};
	~CThreadAnyFunction(){

	}
};
template <class T_return>
class  CThreadAnyFunction<T_return, nil, nil, nil, nil, nil, nil, nil> 
	: public CThreadAnyFunction0<T_return >
{
public:
	CThreadAnyFunction(sigc::slot<T_return> function)
		: CThreadAnyFunction0(function)
	{
		
	};
	~CThreadAnyFunction(){

	}
};

template <class T_return, class T_arg1>
class CThreadAnyFunction<T_return, T_arg1, nil, nil, nil, nil, nil, nil> 
	: public CThreadAnyFunction1<T_return, T_arg1 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1> function, T_arg1 arg1)
		: CThreadAnyFunction1(function, arg1)
	{

	};
};

template <class T_return, class T_arg1, class T_arg2>
class CThreadAnyFunction<T_return, T_arg1, T_arg2, nil, nil, nil, nil, nil> 
	: public CThreadAnyFunction2<T_return, T_arg1, T_arg2 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2> function, T_arg1 arg1, T_arg2 arg2)
		: CThreadAnyFunction2(function, arg1, arg2)
	{

	};
};
template <class T_return, class T_arg1, class T_arg2, class T_arg3>
class CThreadAnyFunction<T_return, T_arg1, T_arg2, T_arg3, nil, nil, nil, nil> 
	: public CThreadAnyFunction3<T_return, T_arg1, T_arg2, T_arg3 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3)
		: CThreadAnyFunction3(function, arg1, arg2, arg3)
	{

	};
};

template <class T_return, class T_arg1, class T_arg2, class T_arg3, class T_arg4>
class CThreadAnyFunction<T_return, T_arg1, T_arg2, T_arg3, T_arg4, nil, nil, nil> 
	: public CThreadAnyFunction4<T_return, T_arg1, T_arg2, T_arg3, T_arg4 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4)
		: CThreadAnyFunction4(function, arg1, arg2, arg3, arg4)
	{

	};
};

template <class T_return, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
class CThreadAnyFunction<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, nil, nil> 
	: public CThreadAnyFunction5<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5)
		: CThreadAnyFunction5(function, arg1, arg2, arg3, arg4, arg5)
	{

	};
};
template <class T_return, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
class CThreadAnyFunction<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, nil> 
	: public CThreadAnyFunction6<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6 >
{
public:
	CThreadAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3, T_arg4 arg4, T_arg5 arg5, T_arg6 arg6)
		: CThreadAnyFunction6(function, arg1, arg2, arg3, arg4, arg5, arg6)
	{

	};
};


}
