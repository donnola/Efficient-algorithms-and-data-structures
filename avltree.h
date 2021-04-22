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

//        bool remove(const T& element)
//        {
//            leaf* curLeaf = m_pRoot;
//            int comp = Compare(curLeaf->pData, element);
//            while(comp != 0) {
//                if (comp > 0 && nullptr != curLeaf->pRight) {
//                    curLeaf = curLeaf->pRight;
//                    comp = Compare(curLeaf->pData, element);
//                }
//                else if (comp < 0 && nullptr != curLeaf->pLeft) {
//
//                    curLeaf = curLeaf->pLeft;
//                    comp = Compare(curLeaf->pData, element);
//                }
//                else {
//                    return false;
//                }
//            }
//            leaf* leftLeaf = curLeaf->pLeft;
//            leaf* rightLeaf = curLeaf->pRight;
//            leaf* minLeaf;
//            if (nullptr == rightLeaf && nullptr == leftLeaf) {
//                DeleteBalance(curLeaf->pParent, curLeaf);
//                if (curLeaf->pParent->pLeft == curLeaf) {
//                    curLeaf->pParent->pLeft = nullptr;
//                }
//                else {
//                    curLeaf->pParent->pRight = nullptr;
//                }
//                m_Memory.deleteObject(curLeaf);
//                return true;
//            }
//            else if (nullptr == rightLeaf) {
//                minLeaf = leftLeaf;
//            }
//            else {
//                minLeaf = FindMinLeaf(rightLeaf);
//            }
//            curLeaf->pData = minLeaf->pData;
//            if (nullptr != minLeaf->pRight) {
//                minLeaf->pData = minLeaf->pRight->pData;
//                DeleteBalance(minLeaf, minLeaf->pRight);
//                minLeaf->pRight = nullptr;
//                m_Memory.deleteObject(minLeaf->pRight);
//            }
//            else {
//                DeleteBalance(minLeaf->pParent, minLeaf);
//                if (minLeaf->pParent->pLeft == minLeaf) {
//                    minLeaf->pParent->pLeft = nullptr;
//                }
//                else {
//                    minLeaf->pParent->pRight = nullptr;
//                }
//                m_Memory.deleteObject(minLeaf);
//            }
//            return true;
//        }

        void clear()
        {
            m_Memory.clear();
            m_pRoot = nullptr;
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

        leaf* FindMinLeaf(leaf* curLeaf) {
            while (nullptr != curLeaf->pLeft) {
                curLeaf = curLeaf->pLeft;
            }
            return curLeaf;
        }

        leaf* Balance(leaf* curLeaf) {
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
                leaf* newParentLeaf = Balance(parentLeaf);
                if (newParentLeaf->balanceFactor == 0) {
                    balanced = true;
                }
                return newParentLeaf;
            }
            return parentLeaf;
        }


//        void DeleteBalance(leaf* parentLeaf, leaf* curLeaf) {
//            if (nullptr != parentLeaf) {
//                if (parentLeaf->pLeft == curLeaf) {
//                    parentLeaf->balanceFactor -= 1;
//                } else {
//                    parentLeaf->balanceFactor += 1;
//                }
//                if (2 == parentLeaf->balanceFactor || -2 == parentLeaf->balanceFactor) {
//                    parentLeaf = Balance(parentLeaf);
//                }
//                if (m_pRoot == parentLeaf || 0 == parentLeaf->balanceFactor) {
//                    return;
//                }
//                DeleteBalance(parentLeaf->pParent, parentLeaf);
//            }
//        }

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

        size_t size = 0;
        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };
}; // namespace templates

#endif // #define AVL_HEAD_H_2021_04_01
