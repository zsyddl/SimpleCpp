#pragma once

#include "sigc++/sigc++.h"
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>


using namespace sigc;

template<class T_Return >
class CAnyFunction0
{
public:
	CAnyFunction0(sigc::slot<T_Return> function)
		: m_pFunction(function)
	{
		
	};
	virtual ~CAnyFunction0(void){
		
	};
	
	virtual int Run(){
		int nRet = 0;
		m_pFunction();
		return nRet;
	};
protected:
	sigc::slot<T_Return> m_pFunction;;
};
// 
 template<class T_Return, class T_arg1>
 class CAnyFunction1
 {
 public:
 	CAnyFunction1(sigc::slot<T_Return, T_arg1> function, T_arg1 arg1)
 		: m_pFunction(function)
		, m_arg1(arg1)
 	{
 
 	};
 	virtual ~CAnyFunction1(void){
 
 	};
 
 	virtual int Run(){
 		int nRet = 0;
 		m_pFunction(m_arg1);
 
 		 return nRet;
 	};
 protected:
 	sigc::slot<T_Return, T_arg1> m_pFunction;;
 	T_arg1	m_arg1;
 };
 template<class T_Return, class T_arg1, class T_arg2>
 class CAnyFunction2
 {
 public:
	 CAnyFunction2(sigc::slot<T_Return, T_arg1, T_arg2> function, T_arg1 arg1, T_arg2 arg2)
		 : m_pFunction(function)
		 , m_arg1(arg1)
		 , m_arg2(arg2)
	 {

	 };
	 virtual ~CAnyFunction2(void){

	 };

	 virtual int Run(){
		 int nRet = 0;
		 m_pFunction(m_arg1, m_arg2);

		 return nRet;
	 };
 protected:
	 sigc::slot<T_Return, T_arg1, T_arg2> m_pFunction;;
	 T_arg1	m_arg1;
	 T_arg2 m_arg2;
 };


 template<class T_Return, class T_arg1, class T_arg2, class T_arg3>
 class CAnyFunction3
 {
 public:
	 CAnyFunction3(sigc::slot<T_Return, T_arg1, T_arg2, T_arg3> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3)
		 : m_pFunction(function)
		 , m_arg1(arg1)
		 , m_arg2(arg2)
		 , m_arg3(arg3)
	 {

	 };
	 virtual ~CAnyFunction3(void){

	 };

	 virtual int Run(){
		 int nRet = 0;
		 m_pFunction(m_arg1, m_arg2, m_arg3);

		 return nRet;
	 };
 protected:
	 sigc::slot<T_Return, T_arg1, T_arg2, T_arg3> m_pFunction;;
	 T_arg1	m_arg1;
	 T_arg2 m_arg2;
	 T_arg3 m_arg3;
 };
 
 template<class T_return, class T_arg1 = nil, class T_arg2 = nil, class T_arg3 = nil, class T_arg4 = nil, class T_arg5 = nil, class T_arg6 = nil, class T_arg7 = nil>
 //template <class T_return, class T_arg1 /* =nil*/>
 class CAnyFunction
	 : public CAnyFunction3<T_return, T_arg1, T_arg2, T_arg3>
 {
 public:
	 CAnyFunction(sigc::slot<T_return, T_arg1, T_arg2, T_arg3> function, T_arg1 arg1, T_arg2 arg2, T_arg3 arg3)
		 : CAnyFunction3(function, arg1, arg2, arg3)
	 {

	 };
	 virtual ~CAnyFunction(void){};
 };
 template <class T_return>
 class CAnyFunction <T_return, nil, nil, nil, nil, nil, nil, nil> 
	 : public CAnyFunction0<T_return>
 {
 public:
	 CAnyFunction(sigc::slot<T_return> function)
		 : CAnyFunction0(function)
	 {

	 };
	 virtual ~CAnyFunction(void){};


 };

  //template<class T_return, class T_arg1 = nil, class T_arg2 = nil, class T_arg3 = nil, class T_arg4 = nil, class T_arg5 = nil, class T_arg6 = nil, class T_arg7 = nil>
   template <class T_return, class T_arg1 >
   class CAnyFunction<T_return, T_arg1, nil, nil, nil, nil, nil, nil>
  	 : public CAnyFunction1<T_return, T_arg1>
   {
   public:
  	 CAnyFunction(sigc::slot<T_return, T_arg1> function, T_arg1 arg1)
  		 : CAnyFunction1(function, arg1)
  	 {
  
  	 };
  	 virtual ~CAnyFunction(void){};
   };

   template <class T_return, class T_arg1, class T_arg2 >
   class CAnyFunction<T_return, T_arg1, T_arg2, nil, nil, nil, nil, nil>
	   : public CAnyFunction2<T_return, T_arg1, T_arg2>
   {
   public:
	   CAnyFunction(sigc::slot<T_return, T_arg1, T_arg2> function, T_arg1 arg1, T_arg2 arg2)
		   : CAnyFunction2(function, arg1, arg2)
	   {

	   };
	   virtual ~CAnyFunction(void){};
   };

// 
//  template <class T_return, class T_arg1>
//  class CAnyFunction <T_return, T_arg1> 
// 	 : public CAnyFunction1<T_return, T_arg1>
//  {
//  public:
// 	 CAnyFunction(sigc::slot<T_return> function, T_arg1 arg1)
// 		 : CAnyFunction1(function, arg1)
// 	 {
// 
// 	 };
// 	 virtual ~CAnyFunction(void){};
// 
// 
//  };
//  

//
//template<class T_Return, class T_Argc0>
//class CAnyFunction<T_Return, T_Argc0>
//{
//public:
//	CAnyFunction(sigc::slot<T_Return, T_Argc0> function);
//	virtual ~CAnyFunction(void);
//
//	virtual int Run();
//protected:
//	sigc::slot<T_Return, T_Argc0> m_pFunction;;
//	T_Argc0 m_argc0;
//};
//
//
//template<class T_Return, class T_Argc0>
//CAnyFunction<T_Return, T_Argc0>::CAnyFunction(sigc::slot<T_Return, T_Argc0> function)
//: m_pFunction(function)
//{
//}
//template<class T_Return, class T_Argc0>
//CAnyFunction<T_Return, T_Argc0>::~CAnyFunction(void)
//{
//}
//template<class T_Return, class T_Argc0>
//int CAnyFunction<T_Return>::Run()
//{
//	return m_pFunction(m_argc0);
//}