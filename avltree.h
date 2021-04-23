#ifndef AVL_HEAD_H_2021_04_01
#define AVL_HEAD_H_2021_04_01

#include "mm.h"

namespace lab618
{
    template <class T, int(*Compare)(const T *pElement, const T* pElement2) >
    class CAVLTree
    {
    private:
        struct leaf
        {
            T* pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;
        };

    public:
        class CException
        {
        public:
            CException()
            {
            }
        };

    public:
        CAVLTree() : m_pRoot(nullptr), m_Memory(16, true){}

        virtual ~CAVLTree()
        {
            clear();
        }

        bool add(T* pElement)
        {
            leaf* fLeaf = findLeaf(pElement);
            if (nullptr != fLeaf) {
                return false;
            }
            leaf* newLeaf = m_Memory.newObject();
            if (nullptr == newLeaf)
            {
                throw CException();
            }
            newLeaf->pData = pElement;
            newLeaf->balanceFactor = 0;
            newLeaf->pLeft = nullptr;
            newLeaf->pRight = nullptr;
            bool balanced = false;
            m_pRoot = Insert(m_pRoot, newLeaf, balanced);
            return true;
        }

        bool update(T* pElement)
        {
            leaf* elLeaf = findLeaf(pElement);
            if (nullptr != elLeaf) {
                elLeaf->pData = pElement;
                return true;
            }
            leaf* newLeaf = m_Memory.newObject();
            if (nullptr == newLeaf)
            {
                throw CException();
            }
            newLeaf->pData = pElement;
            newLeaf->balanceFactor = 0;
            newLeaf->pLeft = nullptr;
            newLeaf->pRight = nullptr;
            bool balanced = false;
            m_pRoot = Insert(m_pRoot, newLeaf, balanced);
            return false;
        }

        T* find(const T& pElement)
        {
            leaf* elLeaf = findLeaf(pElement);
            if (nullptr != elLeaf) {
                return elLeaf->pData;
            }
            return nullptr;
        }

        bool remove(const T& element) {
            bool removed = false;
            bool balanced = false;
            m_pRoot = findAndDelete(m_pRoot, element, balanced, removed);
            return removed;
        }

        void clear()
        {
            m_pRoot = nullptr;
            m_Memory.clear();
        }

    private:
        leaf *findLeaf(const T* pElement)
        {
            if (m_pRoot == nullptr) {
                return nullptr;
            }
            leaf* curLeaf = m_pRoot;
            int comp = Compare(curLeaf->pData, pElement);
            while(comp != 0) {
                if (comp > 0 && curLeaf->pRight != nullptr) {
                    curLeaf = curLeaf->pRight;
                    comp = Compare(curLeaf->pData, pElement);
                }
                else if (comp < 0 && curLeaf->pLeft != nullptr) {
                    curLeaf = curLeaf->pLeft;
                    comp = Compare(curLeaf->pData, pElement);
                }
                else {
                    return nullptr;
                }
            }
            return curLeaf;
        }

        leaf* findMinLeaf(leaf* curLeaf) {
            while (nullptr != curLeaf->pLeft) {
                curLeaf = curLeaf->pLeft;
            }
            return curLeaf;
        }

        leaf* deleteMinLeaf(leaf* parentLeaf, bool &balanced) {
            if (nullptr == parentLeaf->pLeft) {
                return parentLeaf->pRight;
            }
            parentLeaf->pLeft = deleteMinLeaf(parentLeaf->pLeft, balanced);
            if (!balanced) {
                --parentLeaf->balanceFactor;
            }
            if (parentLeaf->balanceFactor == 1 || parentLeaf->balanceFactor == -1 || balanced) {
                balanced = true;
                return parentLeaf;
            }
            if (parentLeaf->balanceFactor == -2 || parentLeaf->balanceFactor == 2) {
                leaf* newParentLeaf = balance(parentLeaf);
                if (newParentLeaf->balanceFactor == 1 || newParentLeaf->balanceFactor == -1) {
                    balanced = true;
                }
                return newParentLeaf;
            }
            return parentLeaf;
        }

        leaf* balance(leaf* curLeaf) {
            if (2 == curLeaf->balanceFactor) {
                if (curLeaf->pLeft->balanceFactor < 0) {
                    if (curLeaf->pLeft->pRight->balanceFactor == 1) {
                        curLeaf->balanceFactor = -1;
                        curLeaf->pLeft->balanceFactor = 0;
                    }
                    else if (curLeaf->pLeft->pRight->balanceFactor == 0) {
                        curLeaf->balanceFactor = 0;
                        curLeaf->pLeft->balanceFactor = 0;
                    }
                    else {
                        curLeaf->balanceFactor = 0;
                        curLeaf->pLeft->balanceFactor = 1;
                    }
                    curLeaf->pLeft->pRight->balanceFactor = 0;
                    return BigRight(curLeaf);
                }
                if (curLeaf->pLeft->balanceFactor == 1) {
                    curLeaf->balanceFactor = 0;
                    curLeaf->pLeft->balanceFactor = 0;
                }
                else {
                    curLeaf->balanceFactor = 1;
                    curLeaf->pLeft->balanceFactor = -1;
                }
                return LittleRight(curLeaf);
            }
            else if (-2 == curLeaf->balanceFactor) {
                if (curLeaf->pRight->balanceFactor > 0) {
                    if (curLeaf->pRight->pLeft->balanceFactor == 1) {
                        curLeaf->balanceFactor = 0;
                        curLeaf->pRight->balanceFactor = -1;
                    }
                    else if (curLeaf->pRight->pLeft->balanceFactor == 0) {
                        curLeaf->balanceFactor = 0;
                        curLeaf->pRight->balanceFactor = 0;
                    }
                    else {
                        curLeaf->balanceFactor = 1;
                        curLeaf->pRight->balanceFactor = 0;
                    }
                    curLeaf->pRight->pLeft->balanceFactor = 0;
                    return BigLeft(curLeaf);
                }
                if (curLeaf->pRight->balanceFactor == -1) {
                    curLeaf->balanceFactor = 0;
                    curLeaf->pRight->balanceFactor = 0;
                }
                else {
                    curLeaf->balanceFactor = -1;
                    curLeaf->pRight->balanceFactor = 1;
                }
                return LittleLeft(curLeaf);
            }
            return curLeaf;
        }



        leaf* findAndDelete(leaf* parentLeaf, const T& element, bool &balanced, bool& removed) {
            if (parentLeaf == nullptr) {
                balanced = true;
                return parentLeaf;
            }
            if (Compare(parentLeaf->pData, &element) != 0) {
                if (Compare(parentLeaf->pData, &element) < 0) {
                    parentLeaf->pLeft = findAndDelete(parentLeaf->pLeft, element, balanced, removed);
                    if (removed && !balanced) {
                        --parentLeaf->balanceFactor;
                    }
                }
                else if (Compare(parentLeaf->pData, &element) > 0) {
                    parentLeaf->pRight = findAndDelete(parentLeaf->pRight, element, balanced, removed);
                    if (removed && !balanced) {
                        ++parentLeaf->balanceFactor;
                    }
                }
                if (parentLeaf->balanceFactor == 1 || parentLeaf->balanceFactor == -1 || balanced || !removed) {
                    balanced = true;
                    return parentLeaf;
                }
                if (parentLeaf->balanceFactor == -2 || parentLeaf->balanceFactor == 2) {
                    leaf* newParentLeaf = balance(parentLeaf);
                    if (newParentLeaf->balanceFactor == 1 || newParentLeaf->balanceFactor == -1) {
                        balanced = true;
                    }
                    return newParentLeaf;
                }
                return parentLeaf;
            }
            else {
                removed = true;
                leaf* leftChild = parentLeaf->pLeft;
                leaf* rightChild = parentLeaf->pRight;
                size_t b = parentLeaf->balanceFactor;
                m_Memory.deleteObject(parentLeaf);
                if (nullptr == rightChild) {
                    return leftChild;
                }
                leaf* minLeaf = findMinLeaf(rightChild);
                minLeaf->pRight = deleteMinLeaf(rightChild, balanced);
                minLeaf->pLeft = leftChild;
                minLeaf->balanceFactor = b;
                if (!balanced) {
                    ++minLeaf->balanceFactor;
                }
                if (minLeaf->balanceFactor == 1 || minLeaf->balanceFactor == -1 || balanced) {
                    balanced = true;
                    return minLeaf;
                }
                if (minLeaf->balanceFactor == -2 || minLeaf->balanceFactor == 2) {
                    leaf* newParentLeaf = balance(minLeaf);
                    if (newParentLeaf->balanceFactor == 1 || newParentLeaf->balanceFactor == -1) {
                        balanced = true;
                    }
                    return newParentLeaf;
                }
                return minLeaf;
            }
        }

        leaf* Insert(leaf* parentLeaf, leaf* curLeaf, bool &balanced) {
            if (!parentLeaf) {
                return curLeaf;
            }
            if (Compare(parentLeaf->pData, curLeaf->pData) < 0) {
                parentLeaf->pLeft = Insert(parentLeaf->pLeft, curLeaf, balanced);
                if (!balanced) {
                    ++parentLeaf->balanceFactor;
                }
            }
            else {
                parentLeaf->pRight = Insert(parentLeaf->pRight, curLeaf, balanced);
                if (!balanced) {
                    --parentLeaf->balanceFactor;
                }
            }
            if (parentLeaf->balanceFactor == 0 || balanced) {
                balanced = true;
                return parentLeaf;
            }
            if (parentLeaf->balanceFactor == -2 || parentLeaf->balanceFactor == 2) {
                leaf* newParentLeaf = balance(parentLeaf);
                if (newParentLeaf->balanceFactor == 0) {
                    balanced = true;
                }
                return newParentLeaf;
            }
            return parentLeaf;
        }

        leaf* BigRight(leaf* curLeaf) {
            curLeaf->pLeft = LittleLeft(curLeaf->pLeft);
            return LittleRight(curLeaf);
        }

        leaf* BigLeft(leaf* curLeaf) {
            curLeaf->pRight = LittleRight(curLeaf->pRight);
            return LittleLeft(curLeaf);
        }

        leaf* LittleRight(leaf* curLeaf) {
            leaf* p = curLeaf->pLeft;
            curLeaf->pLeft = p->pRight;
            p->pRight = curLeaf;
            return p;
        }

        leaf* LittleLeft(leaf* curLeaf) {
            leaf* p = curLeaf->pRight;
            curLeaf->pRight = p->pLeft;
            p->pLeft = curLeaf;
            return p;
        }

        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };
}; // namespace templates

#endif // #define AVL_HEAD_H_2021_04_01
