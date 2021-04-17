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
            leaf *pParent;
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
        CAVLTree() : m_pRoot(nullptr)
        {
            m_Memory(16, true);
        }

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
            Insert(newLeaf);
            InsertBalance(newLeaf->pParent, newLeaf);
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
            Insert(newLeaf);
            InsertBalance(newLeaf->pParent, newLeaf);
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

        bool remove(const T& element)
        {
            leaf* curLeaf = m_pRoot;
            int comp = Compare(curLeaf->pData, element);
            while(comp != 0) {
                if (comp > 0 && nullptr != curLeaf->pRight) {
                    curLeaf = curLeaf->pRight;
                    comp = Compare(curLeaf->pData, element);
                }
                else if (comp < 0 && nullptr != curLeaf->pLeft) {

                    curLeaf = curLeaf->pLeft;
                    comp = Compare(curLeaf->pData, element);
                }
                else {
                    return false;
                }
            }
            leaf* leftLeaf = curLeaf->pLeft;
            leaf* rightLeaf = curLeaf->pRight;
            leaf* minLeaf;
            if (nullptr == rightLeaf && nullptr == leftLeaf) {
                DeleteBalance(curLeaf->pParent, curLeaf);
                if (curLeaf->pParent->pLeft == curLeaf) {
                    curLeaf->pParent->pLeft = nullptr;
                }
                else {
                    curLeaf->pParent->pRight = nullptr;
                }
                m_Memory.deleteObject(curLeaf);
                return true;
            }
            else if (nullptr == rightLeaf) {
                minLeaf = leftLeaf;
            }
            else {
                minLeaf = FindMinLeaf(rightLeaf);
            }
            curLeaf->pData = minLeaf->pData;
            if (nullptr != minLeaf->pRight) {
                minLeaf->pData = minLeaf->pRight->pData;
                DeleteBalance(minLeaf, minLeaf->pRight);
                minLeaf->pRight = nullptr;
                m_Memory.deleteObject(minLeaf->pRight);
            }
            else {
                DeleteBalance(minLeaf->pParent, minLeaf);
                if (minLeaf->pParent->pLeft == minLeaf) {
                    minLeaf->pParent->pLeft = nullptr;
                }
                else {
                    minLeaf->pParent->pRight = nullptr;
                }
                m_Memory.deleteObject(minLeaf);
            }
            return true;
        }

        void clear()
        {
            m_Memory.clear();
            m_pRoot = nullptr;
        }

    private:
        leaf *findLeaf(const T* pElement)
        {
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

        void Insert(leaf* curLeaf) {
            if (nullptr == m_pRoot) {
                m_pRoot = curLeaf;
                curLeaf->pParent = nullptr;
                return;
            }
            leaf* parentLeaf = m_pRoot;
            while (true) {
                if (Compare(parentLeaf->pData, curLeaf->pData) < 0) {
                    if (nullptr == parentLeaf->pLeft) {
                        parentLeaf->pLeft = curLeaf;
                        curLeaf->pParent = parentLeaf;
                        return;
                    }
                    parentLeaf = parentLeaf->pLeft;
                }
                else {
                    if (nullptr == parentLeaf->pRight) {
                        parentLeaf->pRight = curLeaf;
                        curLeaf->pParent = parentLeaf;
                        return;
                    }
                    parentLeaf = parentLeaf->pRight;
                }
            }
        }

        leaf* Balance(leaf* parentLeaf) {
            if (2 == parentLeaf->balanceFactor) {
                if (parentLeaf->pLeft->balanceFactor < 0) {
                    parentLeaf->pLeft = LittleLeft(parentLeaf->pLeft);
                }
                if (nullptr == parentLeaf->pParent) {
                    m_pRoot = LittleRight(parentLeaf);
                    return m_pRoot;
                }
                if (parentLeaf->pParent->pLeft == parentLeaf) {
                    parentLeaf->pParent->pLeft = LittleRight(parentLeaf);
                    return parentLeaf->pParent->pLeft;
                }
                if (parentLeaf->pParent->pRight == parentLeaf) {
                    parentLeaf->pParent->pRight = LittleRight(parentLeaf);
                    return parentLeaf->pParent->pRight;
                }
            }
            else if (-2 == parentLeaf->balanceFactor) {
                if (parentLeaf->pRight->balanceFactor > 0) {
                    parentLeaf->pRight = LittleRight(parentLeaf->pRight);
                }
                if (nullptr == parentLeaf->pParent) {
                    m_pRoot = LittleLeft(parentLeaf);
                    return m_pRoot;
                }
                if (parentLeaf->pParent->pLeft == parentLeaf) {
                    parentLeaf->pParent->pLeft = LittleLeft(parentLeaf);
                    return parentLeaf->pParent->pLeft;
                }
                if (parentLeaf->pParent->pRight == parentLeaf) {
                    parentLeaf->pParent->pRight = LittleLeft(parentLeaf);
                    return parentLeaf->pParent->pRight;
                }
            }
            if (0 == parentLeaf->balanceFactor) {
                parentLeaf;
            }
        }

        void DeleteBalance(leaf* parentLeaf, leaf* curLeaf) {
            if (nullptr != parentLeaf) {
                if (parentLeaf->pLeft == curLeaf) {
                    parentLeaf->balanceFactor -= 1;
                } else {
                    parentLeaf->balanceFactor += 1;
                }
                parentLeaf = Balance(parentLeaf);
                if (0 == parentLeaf->balanceFactor || m_pRoot == parentLeaf) {
                    return;
                }
                DeleteBalance(parentLeaf->pParent, parentLeaf);
            }
        }

        void InsertBalance(leaf* parentLeaf, leaf* curLeaf) {
            if (nullptr != parentLeaf) {
                if (parentLeaf->pLeft == curLeaf) {
                    parentLeaf->balanceFactor += 1;
                }
                else {
                    parentLeaf->balanceFactor -= 1;
                }
                parentLeaf = Balance(parentLeaf);
                if (0 == parentLeaf->balanceFactor || m_pRoot == parentLeaf) {
                    return;
                }
                InsertBalance(parentLeaf->pParent, parentLeaf);
            }
        }

        leaf* LittleRight(leaf* curLeaf) {
            curLeaf->pLeft->pParent = curLeaf->pParent;
            curLeaf->pParent = curLeaf->pLeft;
            curLeaf->pLeft->pRight->pParent = curLeaf;
            leaf* p = curLeaf->pLeft;
            curLeaf->pLeft = p->pRight;
            p->pRight = curLeaf;
            if (p->balanceFactor == 1) {
                curLeaf->balanceFactor = 0;
                p->balanceFactor = 0;
            }
            if (p->balanceFactor == 0) {
                curLeaf->balanceFactor = 1;
                p->balanceFactor = -1;
            }
            return p;
        }

        leaf* LittleLeft(leaf* curLeaf) {
            curLeaf->pRight->pParent = curLeaf->pParent;
            curLeaf->pParent = curLeaf->pRight;
            curLeaf->pRight->pLeft->pParent = curLeaf;
            leaf* p = curLeaf->pRight;
            curLeaf->pRight = p->pLeft;
            p->pLeft = curLeaf;
            if (p->balanceFactor == -1) {
                curLeaf->balanceFactor = 0;
                p->balanceFactor = 0;
            }
            if (p->balanceFactor == 0) {
                curLeaf->balanceFactor = -1;
                p->balanceFactor = 1;
            }
            return p;
        }

        leaf* m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };
}; // namespace templates

#endif // #define AVL_HEAD_H_2021_04_01
