#pragma once
#include"objectBase.h"
class ObjectFactoryBase
{
public:
	//������Ӧ�����͵Ķ���
	virtual std::unique_ptr<ObjectBase> CreateObject() = 0;

};

