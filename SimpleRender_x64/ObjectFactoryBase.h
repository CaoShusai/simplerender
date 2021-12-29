#pragma once
#include"objectBase.h"
class ObjectFactoryBase
{
public:
	//创建对应的类型的对象
	virtual std::unique_ptr<ObjectBase> CreateObject() = 0;

};

