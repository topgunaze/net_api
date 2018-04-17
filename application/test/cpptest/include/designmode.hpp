#ifndef __DESIGNMODE_HPP__
#define __DESIGNMODE_HPP__

#include <iostream>

class ireader
{
public:
	virtual void reader() = 0;
};

class book:public ireader
{
	void reader(void)
	{
		std::cout<<"read book"<<std::endl;
	}
};

class newspaper:public ireader
{
	void reader(void)
	{
		std::cout<<"read newspaper"<<std::endl;
	}
};

class people
{
	public:
		void tell(ireader *p_ir)
		{
			p_ir->reader();
		}

		void test(void)
		{
			std::cout<<"this is a test"<<std::endl;
		}
};

//���������  
class Product1
{
public:
	virtual void show() = 0;  
};

//���������   
class Product_A : public Product1
{
public:
	void show()
	{
		std::cout << "Product_A" <<std::endl;
	}
};
 
class Product_B : public Product1
{
public:
	void show()
	{
		std::cout << "Product_B" << std::endl;
	}
};

//simple factory 
class Factory_simp
{
public:
	Product1* Create(int i)
	{
		switch (i)
		{
			case 1:
				return new Product_A;
				break;
				
			case 2:
				return new Product_B;
				break;
				
			default:
				break;
		}

		return NULL;
	}
};

//factory patterns
class Factory_pattern
{
public:
	virtual Product1* create() = 0;
};
 
class Factory_A : public Factory_pattern
{
public:
	Product1* create()
	{
		return new Product_A;
	}
};
 
class Factory_B : public Factory_pattern
{
public:
	Product1* create()
	{
		return new Product_B;
	}
};
 
//���������  
class Product2  
{  
public:  
	virtual void show() = 0;  
};  
   
//���������  
class Product_A2 :public Product2  
{  
public:  
	void show(){ std::cout << "product A2" << std::endl; }  
};  

class Product_B2 :public Product2  
{  
public:  
 void show(){ std::cout << "product B2" << std::endl; }  
};  
   
class Factory_abstruct  
{  
 public:  
     virtual Product1 *creat1() = 0;  
     virtual Product2 *creat2() = 0;  
};  
   
class FactoryA :public Factory_abstruct
{  
 public:  
     Product1 *creat1(){ return new Product_A; }  
     Product2 *creat2(){ return new Product_A2; }  
};  
   
class FactoryB: public Factory_abstruct
{  
 public:  
     Product1 *creat1(){ return new Product_B; }  
     Product2 *creat2(){ return new Product_B2; }  
};

//delegate
class applicationprotocal
{
public:
	applicationprotocal(){}
	virtual ~applicationprotocal(){}

	virtual bool applicationlanch() = 0;
};


class application : public applicationprotocal
{
public:
	application();
	virtual ~application(){};

	void run(void);

	virtual bool applicationlanch(){return true;};
	
	static application *shardapplication()
	{
		return p_shardapplication;
	}

private:
	static application *p_shardapplication;
};

class appdelegate :private application
{
public:
	appdelegate(){}
	~appdelegate(){}

	virtual bool applicationlanch();
};

void simple_factory_test();
void simple_delegate_test();
   
#endif