#include <string>
#include <cstdlib>
#include <chrono>
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


//typedef lab618::CDualLinkedList<TestStruct> TestList;
//
//void TestListFunction()
//{
//    TestList list;
//    for (int i = 0; i < ELEMENTS_COUNT; ++i)
//    {
//        TestStruct ts;
//        generate(&ts);
//        list.pushBack(ts);
//    }
//    assert(list.getSize() == ELEMENTS_COUNT);
//
//    size_t current_count = ELEMENTS_COUNT;
//    size_t ik = 0;
//    for (TestList::CIterator it = list.end(); it.isValid(); --it, ++ik)
//    {
//        TestStruct ts = *it;
//        if (ik % 3 == 0)
//        {
//            list.erase(it); // удаляем каждый третий
//            --current_count;
//            assert(list.getSize() == current_count);
//        }
//    }
//    assert(list.getSize() == current_count);
//    for (TestList::CIterator it = list.end(); it.isValid(); --it)
//    {
//        TestStruct ts = *it;
//        --current_count;
//        list.eraseAndNext(it);
//    }
//
//}

TestStruct* binFind(TestStruct **ppArray, const TestStruct& pElement, int len) {
    int l = -1;
    int r = len;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (compareFunc(ppArray[m], &pElement) > 0) {
            l = m;
        }
        else if (compareFunc(ppArray[m], &pElement) < 0) {
            r = m;
        }
        else {
            return ppArray[m];
        }
    }
    return nullptr;
}


const int ELEMENTS_COUNT1 = 10000;
const int ELEMENTS_COUNT2 = 1000000;


void TestRate() {
    for (int i = ELEMENTS_COUNT1; i <= ELEMENTS_COUNT2; i += 1000) {
        lab618::CAVLTree<TestStruct, compareFunc> avl;
        lab618::CHash<TestStruct, hashFunc, compareFunc> table(1000, 16);
        TestStruct** array1 = new TestStruct*[i];
        TestStruct** array2 = new TestStruct*[i];
        TestStruct** array3 = new TestStruct*[2 * i];
        for (size_t j = 0; j < i; ++j) {
            array1[j] = new TestStruct();
            generate(array1[j]);
            array2[j] = new TestStruct();
            array2[j]->key1 = array1[j]->key1;
            array2[j]->key2 = array1[j]->key2;
        }
        for (size_t j = 0; j < 2 * i; ++j) {
            array3[j] = new TestStruct();
            generate(array3[j]);
        }

        //// hash
        // add
        auto begin_hash_add = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            bool added = table.add(array1[j]);
            assert(added);
        }
        auto end_hash_add = std::chrono::steady_clock::now();
        auto hash_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_add - begin_hash_add);
        //std::cout << hash_add.count() << std::endl;

        // find yes
        auto begin_hash_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = table.find(*array1[j]);
            assert(el != nullptr);
            assert(compareFunc(el, array1[j]) == 0);
        }
        auto end_hash_find_yes = std::chrono::steady_clock::now();
        auto hash_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_find_yes - begin_hash_find_yes);
        //std::cout << hash_find_yes.count() << std::endl;

        // find no
        auto begin_hash_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = table.find(*array3[j]);
            assert(el == nullptr);
        }
        auto end_hash_find_no = std::chrono::steady_clock::now();
        auto hash_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_find_no - begin_hash_find_no);
        //std::cout << hash_find_no.count() << std::endl;


        //// avl
        // add
        auto begin_avl_add = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            bool added = avl.add(array1[j]);
            assert(added);
        }
        auto end_avl_add = std::chrono::steady_clock::now();
        auto avl_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_add - begin_avl_add);
        //std::cout << avl_add.count() << std::endl;

        // find yes
        auto begin_avl_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = avl.find(*array1[j]);
            assert(el != nullptr);
            assert(compareFunc(el, array1[j]) == 0);
        }
        auto end_avl_find_yes = std::chrono::steady_clock::now();
        auto avl_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_find_yes - begin_avl_find_yes);
        //std::cout << avl_find_yes.count() << std::endl;

        // find no
        auto begin_avl_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = avl.find(*array3[j]);
            assert(el == nullptr);
        }
        auto end_avl_find_no = std::chrono::steady_clock::now();
        auto avl_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_find_no - begin_avl_find_no);
        //std::cout << avl_find_no.count() << std::endl;


        //// sort
        // sort
        auto begin_sort = std::chrono::steady_clock::now();
        templates::mergeSort<TestStruct>(array1, i, compareFunc);
        auto end_sort = std::chrono::steady_clock::now();
        auto sort = std::chrono::duration_cast<std::chrono::milliseconds>(end_sort - begin_sort);
        //std::cout << sort.count() << std::endl;

        // find yes
        auto begin_sort_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = binFind(array1, *array2[j], i);
            assert(el != nullptr);
            assert(compareFunc(el, array2[j]) == 0);
        }
        auto end_sort_find_yes = std::chrono::steady_clock::now();
        auto sort_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(begin_sort_find_yes - end_sort_find_yes);
        //std::cout << sort_find_yes.count() << std::endl;

        auto begin_sort_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = binFind(array1, *array3[j], i);
            assert(el == nullptr);
        }
        auto end_sort_find_no = std::chrono::steady_clock::now();
        auto sort_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_sort_find_no - begin_sort_find_no);
        //std::cout << sort_find_no.count() << std::endl;
    }
}


int main() {
    TestRate();
}
