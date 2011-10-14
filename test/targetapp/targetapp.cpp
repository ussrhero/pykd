#include "stdafx.h"

#include <intrin.h>

#include <iostream>
#include <string>

#include "utils.h"

#pragma pack( push, 4 )

const ULONG g_constNumValue = 0x5555;
const bool g_constBoolValue = true;

UCHAR g_ucharValue = 1;
USHORT g_ushortValue = 2;
ULONG g_ulongValue = 4;
ULONGLONG g_ulonglongValue = 8;

std::string g_string;

struct structWithBits {
    ULONG m_bit0_4  : 5;
    ULONG m_bit5    : 1;
    ULONG m_bit6_7  : 2;
};
structWithBits g_structWithBits = {0};

union unionTest {
    ULONG m_value;
    structWithBits m_bits;
};

class classBase {
public:
    int m_baseField;
    void baseMethod() const {}
    virtual void virtFunc() =  0;
    virtual void virtFunc2() =  0;
};

struct structTest {
    ULONG m_field0;
    ULONGLONG m_field1;
    bool m_field2;
    USHORT m_field3;
};

structTest      g_structTest = {  0, 500, true, 1 };

class classChild : public classBase {
public:
    int m_childField;
    int m_childField2;
    structTest m_childField3;
    void childMethod() const {}
    virtual void virtFunc() {}
    virtual void virtFunc2() {}
};

struct struct2 {
    structTest m_struct;
    unionTest m_union;
    int m_field;
};

void FuncWithName0()
{
    classChild _classChild;
    _classChild.baseMethod();

    reinterpret_cast<classChild *>(&_classChild)->virtFunc2();
    std::cout << _classChild.m_childField2;
    std::cout << g_constNumValue;
    std::cout << g_constBoolValue;
    std::cout << g_ucharValue;
    std::cout << g_ushortValue;
    std::cout << g_ulongValue;
    std::cout << g_ulonglongValue;

    std::cout << g_structTest.m_field0;
}

void FuncWithName1(int a)
{
    unionTest _unionTest[2] = {0};
    _unionTest[1].m_value = 0;
    structTest _structTest;
    _structTest.m_field1 = a;
    struct2 _struct2;
    RtlZeroMemory(&_struct2, sizeof(_struct2));

    std::cout << _unionTest[0].m_value;
    std::cout << _structTest.m_field1;
    std::cout << _struct2.m_struct.m_field1;
    std::cout << g_string;
}

#pragma pack( pop )

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        // Let test scripts to execute
        __debugbreak();
        __debugbreak();
        __debugbreak();
        __debugbreak();
        FuncWithName0();
        FuncWithName1(2);
    }
    catch(std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "Unknown error" << std::endl;
        return 1;
    }
    return 0;
}

