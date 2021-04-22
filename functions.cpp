#include <string>
#include <cstdlib>
#include "hash.h"
#include "list.h"

struct TestStruct
{
    std::string key;
    std::string value1;
    std::string value2;
};

static std::string makeRandomString(int minL = 7, int maxL = 14)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    const char dict[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int dict_len = sizeof(dict);
    for (int i = 0; i < length; ++i) {
        s += dict[(rand() % dict_len)];
    }
    return s;
}

static void generate(TestStruct *pts)
{
    pts->key = makeRandomString();
    pts->value1 = makeRandomString();
    pts->value2 = makeRandomString();
}

int compareFunc(const TestStruct *pElem1, const TestStruct *pElem2)
{
    if (pElem1->key < pElem2->key) {
        return 1;
    }
    if (pElem1->key == pElem2->key) {
        return 0;
    }
    return -1;
}

unsigned int hashFunc(const TestStruct* pElement)
{
    const int p = 7;
    unsigned int hash = 0;
    size_t p_pow = 1;
    for (char i : pElement->key)
    {
        hash += (i - 'A') * p_pow;
        p_pow *= p;
    }
    return hash;
}

const int ELEMENTS_COUNT = 1000;
typedef lab618::CSingleLinkedList<TestStruct> TestSingleList;
typedef lab618::CDualLinkedList<TestStruct> TestList;


void TestListFunction()
{
    // тест односвязного списка

    TestSingleList single_list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        single_list.pushBack(ts);
    }

    assert(single_list.getSize() == ELEMENTS_COUNT);

    for (TestSingleList::CIterator it = single_list.begin(); it.isValid(); ++it)
    {
        it.getLeaf();
        TestStruct ts = *it;
        single_list.erase(it);
    }

    assert(single_list.getSize() == 0);

    /// тест двусвязного списка

    TestList list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }

    assert(list.getSize() == ELEMENTS_COUNT);

    for (TestList::CIterator it = list.begin(); it.isValid(); ++it)
    {
        it.getLeaf();
        TestStruct ts = *it;
        list.erase(it);
    }
}


int main() {
    TestListFunction();
}
