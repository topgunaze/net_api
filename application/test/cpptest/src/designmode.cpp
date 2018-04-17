#include "designmode.hpp"
#include <stdlib.h>

using namespace std;


application* application::p_shardapplication = NULL;

application::application()
{
	p_shardapplication = this;
}

void application::run()
{
	applicationlanch();
}

#if 1
bool appdelegate::applicationlanch()
{
	cout<<"this is a appdelegate "<<this<<endl;
	
	return true;
}
#endif

void simple_factory_test()
{
	//forbide: virtual func
	//newspaper *p_paper = NULL;
	//book      *p_bk = NULL;
	newspaper paper;
	book      bk;
	people    p;

	p.tell(&bk);
	p.tell(&paper);

#if 0
	people    *p = (people*)2;
	p->test();
#endif

	cout<<"factory simple"<<endl;
	//static Factory
	Factory_simp *factory = new Factory_simp();
	factory->Create(1)->show();
	factory->Create(2)->show();

	cout<<"factory pattern"<<endl;
	//定义一个用于创建对象的接口，让子类决定实例化哪一个类
	Factory_A* factoryA = new Factory_A;
	Factory_B* factoryB = new Factory_B();
 
	factoryA->create()->show();
	factoryB->create()->show();

	cout<<"factory abstruct"<<endl;

	 //提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。  
     FactoryA *factory1 = new FactoryA();  
     factory1->creat1()->show();  
     factory1->creat2()->show();  
   
     FactoryB *factory2 = new FactoryB();  
     factory2->creat1()->show();  
     factory2->creat2()->show();

	system("pause");
}

void simple_delegate_test()
{
	appdelegate delegate;

	application::shardapplication()->run();

/*
第一种情况:protected与private继承
这两种继承方式时，出现编译错误: `A' is an inaccessible base of `B'。
A *b=new B这种方式相当于将A当作一个接口，而这两种继承方式下B并没有从A中继承到public成员，
也就等于是没有利用到A这个接口，所以编译器拒绝将派生类的指针或引用赋 给A的指针，因为没有意义。
*/
	//application *p_app = new appdelegate;
	//p_app->run();
	//application app;
	//app.run();

}

