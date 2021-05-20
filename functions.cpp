#include <string>
#include <cstdlib>
#include <chrono>
#include <fstream>
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

const int ELEMENTS_COUNT = 1000;


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

    for (int i = 0; i < ELEMENTS_COUNT; ++i)
    {
        TestStruct ts = list.popBack();
    }

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

}

TestStruct* binFind(TestStruct **ppArray, const TestStruct& pElement, int len) {
    int l = -1;
    int r = len;
    while (l < r - 1) {
        int m = (l + r) / 2;
        int compare = compareFunc(ppArray[m], &pElement);
        if (compare > 0) {
            l = m;
        }
        else if (compare < 0) {
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
int INCREASE = 30000;


void TestRate() {
    std::ofstream fout("время.csv");
    fout << "кол-во эл-ов,кол-во эл-ов x2,"
            "hash table добавление,hash table поиск сущ,hash table поиск несущ,hash table удаление по одному,"
            "avl добавление,avl поиск сущ,avl поиск несущ,avl удаление по одному,"
            "sort сортировка,sort поиск сущ,sort поиск несущ,"
            "hash table удаление всего,avl удаление всего,sort удаление всего\n";
    for (int i = ELEMENTS_COUNT1; i <= ELEMENTS_COUNT2; i += INCREASE) {
        std::string s;
        s += std::to_string(i) + "," + std::to_string(i*2) + ",";
        lab618::CAVLTree<TestStruct, compareFunc> avl;
        lab618::CHash<TestStruct, hashFunc, compareFunc> table(i, 10000);
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
        s += std::to_string(hash_add.count()) + ",";

        // find yes
        auto begin_hash_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = table.find(*array1[j]);
            assert(el != nullptr);
            assert(compareFunc(el, array1[j]) == 0);
        }
        auto end_hash_find_yes = std::chrono::steady_clock::now();
        auto hash_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_find_yes - begin_hash_find_yes);
        s += std::to_string(hash_find_yes.count()) + ",";

        // find no
        auto begin_hash_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < 2*i; ++j) {
            TestStruct* el = table.find(*array3[j]);
            assert(el == nullptr);
        }
        auto end_hash_find_no = std::chrono::steady_clock::now();
        auto hash_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_find_no - begin_hash_find_no);
        s += std::to_string(hash_find_no.count()) + ",";

        // del
        auto begin_hash_del = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            bool d = table.remove(*array1[j]);
            assert(d == true);
        }
        auto end_hash_del = std::chrono::steady_clock::now();
        auto hash_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_del - begin_hash_del);
        s += std::to_string(hash_del.count()) + ",";


        //// avl
        // add
        auto begin_avl_add = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            bool added = avl.add(array1[j]);
            assert(added);
        }
        auto end_avl_add = std::chrono::steady_clock::now();
        auto avl_add = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_add - begin_avl_add);
        s += std::to_string(avl_add.count()) + ",";

        // find yes
        auto begin_avl_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = avl.find(*array1[j]);
            assert(el != nullptr);
            assert(compareFunc(el, array1[j]) == 0);
        }
        auto end_avl_find_yes = std::chrono::steady_clock::now();
        auto avl_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_find_yes - begin_avl_find_yes);
        s += std::to_string(avl_find_yes.count()) + ",";

        // find no
        auto begin_avl_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < 2*i; ++j) {
            TestStruct* el = avl.find(*array3[j]);
            assert(el == nullptr);
        }
        auto end_avl_find_no = std::chrono::steady_clock::now();
        auto avl_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_find_no - begin_avl_find_no);
        s += std::to_string(avl_find_no.count()) + ",";

        // del
        auto begin_avl_del = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            bool d = avl.remove(*array1[j]);
            assert(d == true);
        }
        auto end_avl_del = std::chrono::steady_clock::now();
        auto avl_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_del - begin_avl_del);
        s += std::to_string(avl_del.count()) + ",";


        //// sort
        // sort
        auto begin_sort = std::chrono::steady_clock::now();
        templates::mergeSort<TestStruct>(array1, i, compareFunc);
        auto end_sort = std::chrono::steady_clock::now();
        auto sort = std::chrono::duration_cast<std::chrono::milliseconds>(end_sort - begin_sort);
        s += std::to_string(sort.count()) + ",";

        // find yes
        auto begin_sort_find_yes = std::chrono::steady_clock::now();
        for (size_t j = 0; j < i; ++j) {
            TestStruct* el = binFind(array1, *array2[j], i);
            assert(el != nullptr);
            assert(compareFunc(el, array2[j]) == 0);
        }
        auto end_sort_find_yes = std::chrono::steady_clock::now();
        auto sort_find_yes = std::chrono::duration_cast<std::chrono::milliseconds>(end_sort_find_yes - begin_sort_find_yes);
        s += std::to_string(sort_find_yes.count()) + ",";

        // find no
        auto begin_sort_find_no = std::chrono::steady_clock::now();
        for (size_t j = 0; j < 2*i; ++j) {
            TestStruct* el = binFind(array1, *array3[j], i);
            assert(el == nullptr);
        }
        auto end_sort_find_no = std::chrono::steady_clock::now();
        auto sort_find_no = std::chrono::duration_cast<std::chrono::milliseconds>(end_sort_find_no - begin_sort_find_no);
        s += std::to_string(sort_find_no.count()) + ",";

        for (size_t j = 0; j < i; ++j) {
            bool hashAdded = table.add(array2[j]);
            bool avlAdded = avl.add(array2[j]);
            assert(hashAdded);
            assert(avlAdded);
        }

        auto begin_hash_clear = std::chrono::steady_clock::now();
        table.clear();
        auto end_hash_clear = std::chrono::steady_clock::now();
        auto hash_clear = std::chrono::duration_cast<std::chrono::milliseconds>(end_hash_clear - begin_hash_clear);
        s += std::to_string(hash_clear.count()) + ",";

        auto begin_avl_clear = std::chrono::steady_clock::now();
        avl.clear();
        auto end_avl_clear = std::chrono::steady_clock::now();
        auto avl_clear = std::chrono::duration_cast<std::chrono::milliseconds>(end_avl_clear - begin_avl_clear);
        s += std::to_string(avl_clear.count()) + ",";

        auto begin_array_del = std::chrono::steady_clock::now();
        delete[] array1;
        auto end_array_del = std::chrono::steady_clock::now();
        auto array_del = std::chrono::duration_cast<std::chrono::milliseconds>(end_array_del - begin_array_del);
        s += std::to_string(array_del.count()) + "\n";
        fout << s;
        if (i == 100000) {
            INCREASE = 300000;
        }
    }
    fout.close();
}


int main() {
    TestRate();
    //TestListFunction();
}
