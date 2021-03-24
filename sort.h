#ifndef SORT_HEAD_H_2021_02_25
#define SORT_HEAD_H_2021_02_25

namespace templates
{
    /** Определение типа функции сравнения элементов
    \return
    \li > 0  - pElem1 меньше pElem2
    \li 0   - элементы pElem1 и pElem2 равны
    \li < 0 - pElem1 больше pElem2
    */
    typedef  int (CompareSortType)(const void *pElem1, const void *pElem2);

    void merge(void **ppArray, int left, int mid, int right, CompareSortType pCompareFunc)
    {
        int it1 = 0;
        int it2 = 0;
        void *result[right - left];
        while (left + it1 < mid && mid + it2 < right)
        {
            if (pCompareFunc(ppArray[left + it1], ppArray[mid + it2]) > 0)
            {
                result[it1 + it2] = ppArray[left + it1];
                it1 += 1;
            }
            else
            {
                result[it1 + it2] = ppArray[mid + it2];
                it2 += 1;
            }
        }

        while (left + it1 < mid)
        {
            result[it1 + it2] = ppArray[left + it1];
            it1 += 1;
        }

        while (mid + it2 < right)
        {
            result[it1 + it2] = ppArray[mid + it2];
            it2 += 1;
        }
        for (int i = 0; i < right - left; ++i)
        {
            ppArray[left + i] = result[i];
        }
    }

    void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc, int left = 0, int right = 0)
    {
        if (1 == length)
        {
            return;
        }
        if (0 == right)
        {
            right = length;
        }
        int mid = (left + right) / 2;
        mergeSort(ppArray, mid - left, pCompareFunc, left, mid);
        mergeSort(ppArray, right - mid, pCompareFunc, mid, right);
        merge(ppArray, left, mid, right, pCompareFunc);
    }

    template <class T>
    void mergeSort(T **ppArray, int length, int (pCompareFunc)(const T *pElem1, const T *pElem2))
    {
        mergeSort(reinterpret_cast<void**>(ppArray), length, (CompareSortType*)pCompareFunc);
    }

}; // namespace templates

#endif // #define SORT_HEAD_H_2021_02_25

