#include <string>
#include <cstdlib>
#include "sort.h"
#include "avltree.h"
#include "hash.h"
#include "list.h"

struct TestStruct
{
    std::string key1;
    std::string key2;
};

static std::string makeRandomString(int minL = 7, int maxL = 20)
{
    int length = rand() % maxL + minL;
    std::string s;
    s.reserve(length);
    const char dict[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
                        "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    const int dict_len = sizeof(dict);
    for (int i = 0; i < length; ++i) {
        s += dict[(rand() % dict_len)];
    }
    return s;
}

static void generate(TestStruct *pts)
{
    pts->key1 = makeRandomString();
    pts->key2 = makeRandomString();
}

int compareFunc(const TestStruct *pElem1, const TestStruct *pElem2)
{
    if (pElem1->key1 == pElem2->key1 && pElem1->key2 == pElem2->key2) {
        return 0;
    }
    if (pElem1->key1 < pElem2->key1) {
        return 1;
    }
    if (pElem1->key1 == pElem2->key1 && pElem1->key2 < pElem2->key2) {
        return 1;
    }
    return -1;
}

unsigned int hashFunc(const TestStruct* pElement)
{
    const int p1 = 7;
    const int p2 = 3;
    unsigned int hash = 0;
    size_t p_pow1 = 1;
    for (char i : pElement->key1)
    {
        hash += (i - 'A') * p_pow1;
        p_pow1 *= p1;
    }
    size_t p_pow2 = 1;
    for (char i : pElement->key2)
    {
        hash += (i - 'A') * p_pow2;
        p_pow2 *= p2;
    }
    return hash;
}

const int ELEMENTS_COUNT = 100;
typedef lab618::CDualLinkedList<TestStruct> TestList;

void TestListFunction()
{
    TestList list;
    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts;
        generate(&ts);
        list.pushBack(ts);
    }
    assert(list.getSize() == ELEMENTS_COUNT);

    size_t current_count = ELEMENTS_COUNT;
    size_t ik = 0;
    for (TestList::CIterator it = list.end(); it.isValid(); --it, ++ik)
    {
        TestStruct ts = *it;
        if (ik % 3 == 0)
        {
            list.erase(it); // удаляем каждый третий
            --current_count;
            assert(list.getSize() == current_count);
        }
    }
    assert(list.getSize() == current_count);
    for (TestList::CIterator it = list.end(); it.isValid(); --it)
    {
        TestStruct ts = *it;
        --current_count;
        list.eraseAndNext(it);
    }

}


void TestRate() {
    lab618::CAVLTree<TestStruct, compareFunc> avl;
    lab618::CHash<TestStruct, hashFunc, compareFunc> table(10000, 16);
    TestStruct** array1 = new TestStruct*[ELEMENTS_COUNT];
    TestStruct** array2 = new TestStruct*[ELEMENTS_COUNT];
    TestStruct** array3 = new TestStruct*[ELEMENTS_COUNT];
    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        array1[i] = new TestStruct();
        generate(array1[i]);
        array2[i] = new TestStruct();
        array2[i]->key1 = array1[i]->key1;
        array2[i]->key2 = array1[i]->key2;
        array3[i] = new TestStruct();
        array3[i]->key1 = array1[i]->key1;
        array3[i]->key2 = array1[i]->key2;
    }

    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        avl.add(array1[i]);
    }

    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        table.add(array2[i]);
    }


    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        bool deleted = avl.remove(*array1[i]);
        assert(deleted);
    }

    for (size_t i = 0; i < ELEMENTS_COUNT; ++i) {
        bool deleted = table.remove(*array2[i]);
        assert(deleted);
    }

    templates::mergeSort<TestStruct>(array3, ELEMENTS_COUNT, compareFunc);

}


int main() {
    TestRate();
}
