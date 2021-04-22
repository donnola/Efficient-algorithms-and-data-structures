#include <string>
#include <cstdlib>
#include "hash.h"

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

int main() {
    lab618::CHash<TestStruct, hashFunc, compareFunc> h(300, 3);
    TestStruct* el1 = new TestStruct();
    TestStruct* el2 = new TestStruct();
    TestStruct* el3 = new TestStruct();
    generate(el1);
    generate(el2);
    generate(el3);
    h.add(el1);
    h.add(el2);
    h.add(el3);
    TestStruct del_el;
    del_el.key = el1->key;
    bool res = h.find(del_el);
    std::cout << res;
}
