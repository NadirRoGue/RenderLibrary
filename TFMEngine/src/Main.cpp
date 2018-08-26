
#include "CPU/memory/PoolAttribute.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include <typeinfo>
#include <typeindex>
#include "Transform.h"

class Component
{
public:
	bool originalVar;
	virtual void print()
	{
		std::cout << "Component" << std::endl;
	}
};

class Derived : public Component
{
public:
	int myVar;
	double myOtherVar;
	void print()
	{
		std::cout << "Derived" << std::endl;
	}
};

class AnotherDerived : public Component
{
public:
	void print()
	{
		std::cout << "Another Derived" << std::endl;
	}
};

class AbstractStage
{
public:
	std::vector<Component*> elements;
	virtual std::type_index getAssociatedElementType() = 0;
	
	void printType()
	{
		std::cout << typeid(*this).name() << std::endl;
	}
};

template<class T>
class Stage : public AbstractStage
{
public:
	std::type_index getAssociatedElementType()
	{
		return typeid(T);
	}

	void runStage()
	{
		for (auto elem : elements)
		{
			processElement(static_cast<T*>(elem));
		}
	}

	virtual void processElement(T * element) = 0;
};

class DerivedStage : public Stage<Derived>
{
public:
	void processElement(Derived * derived)
	{
		std::cout << "This is a beautiful derived object" << std::endl;
		derived->print();
		std::cout << derived->myVar << std::endl;
	}
};

class StageBroker
{
public:
	std::map<std::type_index, AbstractStage*> broker;

	void registerStage(AbstractStage * stage)
	{
		broker[stage->getAssociatedElementType()] = stage;
	}

	void registerElement(Component * c)
	{
		std::type_index cls(typeid(*c));
		std::map<std::type_index, AbstractStage*>::iterator it = broker.find(cls);
		if (it != broker.end())
		{
			it->second->elements.push_back(c);
		}
	}
};

void test(AbstractStage * stage)
{
	stage->printType();
}

int main(int argc, void ** arg)
{
	StageBroker sb;
	DerivedStage dstage;

	test(&dstage);

	if (dynamic_cast<AbstractStage*>(&dstage) != NULL)
	{
		std::cout << "WORKS" << std::endl;
	}

	sb.registerStage(&dstage);

	Component c;
	Derived d;
	d.myVar = 5454;
	AnotherDerived ad;

	sb.registerElement(&c);
	sb.registerElement(&d);
	sb.registerElement(&ad);

	dstage.runStage();

	/*
	std::type_index ti(typeid(RenderLib::Transform));
	std::cout << ti.name() << std::endl;
	std::cout << sizeof(RenderLib::Transform) << std::endl;
	*/
	/*
	float testData[6] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f};

	Eigen::Vector3f testList[5] = 
	{
		Eigen::Vector3f (5.0f, 1.3f, 789.258f),
		Eigen::Vector3f (6.0f, 1.3f, 789.258f),
		Eigen::Vector3f (7.0f, 1.3f, 789.258f),
		Eigen::Vector3f (8.0f, 1.3f, 789.258f),
		Eigen::Vector3f (9.0f, 1.3f, 789.258f)
	};

	CPU::Memory::MemoryPool pool(1024 * 1024);
	CPU::Memory::MemoryBlock * block = pool.append(testList, sizeof(testList), false);

	CPU::Memory::PoolAttribute<Eigen::Vector3f, 1> vectorTest(block);

	Eigen::Vector3f retrieved = vectorTest[2];
	std::cout << retrieved.x() << ", " << retrieved.y() << ", " << retrieved.z() << std::endl;

	
	CPU::Memory::PoolAttribute<float, 1> test(block);

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << test[i] << std::endl;
	}

	test[1] = 99.99f;

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << test[i] << std::endl;
	}
	*/
	/*
	float secondTest[3] = { 5.0f, 3.0f, 2.0f };
	Eigen::Vector3f testVector (1.0f, 1.0f, 1.0f);
	memcpy(&testVector[0], testData, sizeof(float) * 3);

	std::cout << testVector.x() << ", " << testVector.y() << ", " << testVector.z() << std::endl;
	*/
	return 0;
}