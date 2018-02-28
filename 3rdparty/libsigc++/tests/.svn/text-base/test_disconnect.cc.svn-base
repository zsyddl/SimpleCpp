// -*- c++ -*-
/* Copyright 2002, The libsigc++ Development Team
 *  Assigned to public domain.  Use as you wish without restriction.
 */

#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>
#include <iostream>

#include <stdarg.h>

//The Tru64 compiler seems to need this to avoid an unresolved symbol
//See bug #161503
#include <new>
SIGC_USING_STD(new)

SIGC_USING_STD(cout)
SIGC_USING_STD(endl)
//获得 成员函数的地址
/*
static void *memFun(void *pObj, ...)
{
	void *pValue;

	va_list arg_ptr;
	va_start(arg_ptr, pObj);
	pValue = va_arg(arg_ptr, void*);
	va_end(arg_ptr);

	return pValue;
}
//获取普通函数的地址
static void *ptrFun(void *pFun)
{
	return pFun;
}*/

int foo(int i)    { std::cout << "foo: " << i << std::endl; return 1;}
int bar(double i) { std::cout << "bar: " << i << std::endl; return 1;}

struct A : public sigc::trackable 
{
  int foo(int i) { std::cout << "A::foo: " << i << std::endl; return 1;}
};
class CA{
public:
	CA(){};
	~CA(){};

	void Fun(){
		printf("%s\n", __FUNCTION__);
	}
	void Fun1(int)
	{
		printf("%s\n", __FUNCTION__);
	}
	void Fun2(int, int)
	{
		printf("%s\n", __FUNCTION__);
	}
	void Fun3(int, int, int)
	{
		printf("%s\n", __FUNCTION__);
	}
	void Fun4(int, int, int,int)
	{
		printf("%s\n", __FUNCTION__);
	}
};

void good_bye_world() { std::cout << "Good bye world!" << std::endl; }

struct B : public sigc::trackable
{
  B()
    {
      sig.connect(sigc::mem_fun(this, &B::destroy));
      sig.connect(sigc::mem_fun(this, &B::boom));
      sig.connect(sigc::ptr_fun(&good_bye_world));
    }

  void destroy()     // Calling destroy() during signal emission seems weird!
    { delete this; } // However, if this works, anything will work!

  void boom()
    { std::cout << "boom!" << std::endl; }

  void emit()
    { sig.emit(); }

  sigc::signal<void> sig;
};
using namespace sigc;
int main()
{
  sigc::signal<int,int> sig;
  sigc::signal<int,int>::iterator confoo, conbar;
  sigc::connection cona;  // connection objects are safe to use beyond the life time of a signal.

  {
    A a;
    sig.connect(sigc::mem_fun1(&a, &A::foo));
    confoo = sig.connect(sigc::ptr_fun1(&foo));
    conbar = sig.connect(sigc::ptr_fun1(&bar));
    std::cout << "sig is connected to A::foo, foo, bar (size=" << sig.size() << "): " << std::endl;
    sig(1);
  }                     // auto disconnection! iterators stay valid after disconnections.

  std::cout << "sig is connected to foo, bar (size=" << sig.size() << "): " << std::endl;
  sig(2);

  A a;                  // iterators stay valid after further connections.
  cona = sig.slots().insert(conbar, sigc::mem_fun1(&a, &A::foo));
  //sig.disconnect(&a, &A::foo);
  
  std::cout << "sig is connected to foo, A::foo, bar (size=" << sig.size() << "): " << std::endl;
  sig.disconnect(memFun(&a, &A::foo), &a );

  std::cout << "sig is connected to foo, A::foo, bar (size=" << sig.size() << "): " << std::endl;

  sig(3);

  //conbar->disconnect(); // manual disconnection
  sig.disconnect(&bar);
  std::cout << "sig is connected to foo, A::foo (size=" << sig.size() << "): " << std::endl;
  sig(4);

  confoo->disconnect(); // manual disconnection
  std::cout << "sig is connected to A::foo (size=" << sig.size() << "): " << std::endl;
  sig(5);

  cona.disconnect();    // manual disconnection
  std::cout << "sig is empty (size=" << sig.size() << "): " << std::endl;
  sig(6);

  cona.disconnect();    // already disconnected -> legal with connection objects, however, nothing happens ...

  std::cout << "deleting a signal during emission..." << std::endl;
  B* b = new B;
  b->emit();
  //////////////////////////////////////////////////////////////////////////
  printf("test fun address\n");

	sigc::signal<void> sig0;
	sigc::signal<void, int> sig1;
	sigc::signal<void, int, int> sig2;
	sigc::signal<void, int, int, int> sig3;
	sigc::signal<void, int, int, int, int> sig4;
	CA a1;
	sig0.connect(mem_fun(&a1, &CA::Fun));
	sig1.connect(mem_fun(&a1, &CA::Fun1));
	sig2.connect(mem_fun(&a1, &CA::Fun2));
	sig3.connect(mem_fun(&a1, &CA::Fun3));
	sig4.connect(mem_fun(&a1, &CA::Fun4));

	sig0.emit();
	sig1.emit(1);
	sig2.emit(1, 1);
	sig3.emit(1, 1, 1);
	sig4.emit(1, 1, 1, 1);

	printf("0, %d\n", sig0.size());
	printf("1, %d\n", sig1.size());
	printf("2, %d\n", sig2.size());
	printf("3, %d\n", sig3.size());
	printf("4, %d\n", sig4.size());

	sig0.disconnect(memFun(&a1, &CA::Fun), &a1);
	sig1.disconnect(memFun(&a1, &CA::Fun1), &a1);
	sig2.disconnect(memFun(&a1, &CA::Fun2), &a1);
	sig3.disconnect(memFun(&a1, &CA::Fun3), &a1);
	sig4.disconnect(memFun(&a1, &CA::Fun4), &a1);
	//sig0.emit();
	//sig1.emit(1);
	//sig2.emit(1, 1);
	printf("disconnectted\n");
	printf("0, %d\n", sig0.size());
	printf("1, %d\n", sig1.size());
	printf("2, %d\n", sig2.size());
	printf("3, %d\n", sig3.size());
	printf("4, %d\n", sig4.size());
/*
  CA a1;
  void *pMemFun = memFun(&a1, &CA::Fun);

  void *pPtrFun = ptrFun(&foo);

  printf("%d, %d, %d\n", pMemFun, &CA::Fun,pPtrFun);
  */
  getchar();
}
