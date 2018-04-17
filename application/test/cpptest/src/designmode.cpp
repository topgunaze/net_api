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
	//����һ�����ڴ�������Ľӿڣ����������ʵ������һ����
	Factory_A* factoryA = new Factory_A;
	Factory_B* factoryB = new Factory_B();
 
	factoryA->create()->show();
	factoryB->create()->show();

	cout<<"factory abstruct"<<endl;

	 //�ṩһ������һϵ����ػ��໥��������Ľӿڣ�������ָ�����Ǿ�����ࡣ  
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
��һ�����:protected��private�̳�
�����ּ̳з�ʽʱ�����ֱ������: `A' is an inaccessible base of `B'��
A *b=new B���ַ�ʽ�൱�ڽ�A����һ���ӿڣ��������ּ̳з�ʽ��B��û�д�A�м̳е�public��Ա��
Ҳ�͵�����û�����õ�A����ӿڣ����Ա������ܾ����������ָ������ø� ��A��ָ�룬��Ϊû�����塣
*/
	//application *p_app = new appdelegate;
	//p_app->run();
	//application app;
	//app.run();

}

