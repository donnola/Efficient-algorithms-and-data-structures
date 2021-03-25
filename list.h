﻿#ifndef TEMPLATES_LIST_2021_02_11
#define TEMPLATES_LIST_2021_02_11

namespace lab618
{
    template<class T>
    class CSingleLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext;
            leaf(T& _data, leaf * _pnext): data(_data), pnext(_pnext)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator(): m_pCurrent(0), m_pBegin(0)
            {
            }

            CIterator(leaf *p): m_pBegin(0), m_pCurrent(p)
            {
            }

            CIterator(const CIterator &src): m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin)
            {
            }

            ~CIterator() = default;

            CIterator& operator = (const CIterator&  src)
            {
                if (*this != src) {
                    this->m_pCurrent = src.m_pCurrent;
                    this->m_pBegin = src.m_pBegin;
                }
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return it.m_pCurrent != m_pCurrent || it.m_pBegin != m_pBegin;
            }

            void operator++()
            {
                if (m_pCurrent == nullptr) {
                    if (m_pBegin != nullptr) {
                        m_pCurrent = m_pBegin;
                        m_pBegin = nullptr;
                    }
                    return;
                }
                m_pCurrent = m_pCurrent->pnext;
            }

            T& getData()
            {
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf* p)
            {
                if (m_pBegin == p) {
                    return;
                }
                leaf* tmpLeaf = m_pBegin;
                while (tmpLeaf->pnext != p) {
                    tmpLeaf = tmpLeaf->pnext;
                }
                m_pCurrent = tmpLeaf;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
        };

    public:

        CSingleLinkedList(): m_pBegin(nullptr), m_pEnd(nullptr)
        {
        }

        virtual ~CSingleLinkedList()
        {
            while (m_pBegin != nullptr) {
                leaf *curDel = m_pBegin;
                m_pBegin = m_pBegin->pnext;
                delete curDel;
            }
        }

        void pushBack(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr);
            if (getSize() == 0) {
                m_pBegin = newLeaf;
            }
            else {
                m_pEnd->pnext = newLeaf;
            }
            m_pEnd = newLeaf;
        }

        void pushFront(T& data)
        {
            leaf *newLeaf = new leaf(data, m_pBegin);
            if (getSize() == 0) {
                m_pEnd = newLeaf;
            }
            m_pBegin = newLeaf;
        }

        T popFront()
        {
            T tmp = m_pBegin->data;
            leaf* newBegin = m_pBegin->pnext;
            delete m_pBegin;
            m_pBegin = newBegin;
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* delLeaf = it.getLeaf();
            it.setLeafPreBegin(delLeaf);
            if (delLeaf == m_pBegin) {
                m_pBegin = delLeaf->pnext;
                it = CIterator(m_pBegin);
                it.setLeaf(nullptr);
            }
            else {
                leaf* currLeaf = it.getLeaf();
                currLeaf->pnext = delLeaf->pnext;
            }
            delete delLeaf;
        }

        int getSize()
        {
            leaf* curLeaf = m_pBegin;
            int size = 0;
            while (curLeaf != nullptr) {
                ++size;
                curLeaf = curLeaf->pnext;
            }
            return size;
        }

        void clear()
        {
            while (m_pBegin != nullptr) {
                leaf* curDel = m_pBegin;
                m_pBegin = m_pBegin->pnext;
                delete curDel;
            }
            m_pEnd = nullptr;
            m_pBegin = nullptr;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList
    {
    private:
        struct leaf
        {
            T data;
            leaf * pnext, *pprev;
            leaf(T& _data, leaf * _pprev, leaf * _pnext): data(_data), pnext(_pnext), pprev(_pprev)
            {
            }
        };
    public:
        class CIterator
        {
        public:
            CIterator(): m_pCurrent(0), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(leaf *p): m_pCurrent(p), m_pBegin(0), m_pEnd(0)
            {
            }

            CIterator(const CIterator &src) : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent), m_pEnd(src.m_pEnd)
            {
            }

            ~CIterator() = default;

            CIterator& operator = (const CIterator&  src)
            {
                if (*this != src) {
                    this->m_pCurrent = src.m_pCurrent;
                    this->m_pBegin = src.m_pBegin;
                    this->m_pEnd = src.m_pEnd;
                }
                return *this;
            }

            bool operator != (const CIterator&  it) const
            {
                return it.m_pCurrent != m_pCurrent;
            }

            void operator++()
            {
                if (m_pCurrent == nullptr) {
                    if (m_pBegin != nullptr) {
                        m_pCurrent = m_pBegin;
                        m_pBegin = nullptr;
                    }
                    return;
                }
                m_pCurrent = m_pCurrent->pnext;
            }

            void operator--()
            {
                if (m_pCurrent == nullptr) {
                    if (m_pEnd != nullptr) {
                        m_pCurrent = m_pEnd;
                        m_pEnd = nullptr;
                    }
                    return;
                }
                m_pCurrent = m_pCurrent->pprev;
            }

            T& getData()
            {
                return m_pCurrent->data;
            }

            T& operator* ()
            {
                return m_pCurrent->data;
            }

            leaf* getLeaf()
            {
                return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf* p)
            {
                m_pCurrent = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf* p)
            {
                if (m_pBegin == p) {
                    return;
                }
                m_pCurrent = p->pprev;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                if (m_pEnd == p) {
                    return;
                }
                m_pCurrent = p->pnext;
            }

            bool isValid() {
                return m_pBegin != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf* m_pBegin;
            // храним текущее положение
            leaf* m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf* m_pEnd;
        };

    public:

        CDualLinkedList(): m_pBegin(nullptr), m_pEnd(nullptr)
        {
        };

        virtual ~CDualLinkedList()
        {
            while (m_pBegin != nullptr) {
                leaf* curDel = m_pBegin;
                m_pBegin = m_pBegin->pnext;
                delete curDel;
            }
        };

        void pushBack(T& data)
        {
            leaf *newLeaf = new leaf(data, m_pEnd, nullptr);
            if (getSize() == 0) {
                m_pBegin = newLeaf;
            }
            else {
                m_pEnd->pnext = newLeaf;
            }
            m_pEnd = newLeaf;
        }

        T popBack()
        {
            T tmp = m_pEnd->data;
            leaf* newEnd = m_pEnd->pprev;
            delete m_pEnd;
            m_pEnd = newEnd;
            return tmp;
        }

        void pushFront(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr, m_pBegin);
            if (getSize() == 0) {
                m_pEnd = newLeaf;
            }
            else {
                m_pBegin->pprev = newLeaf;
            }
            m_pBegin = newLeaf;
        }

        T popFront()
        {
            T tmp = m_pBegin->data;
            leaf* newBegin = m_pBegin->pnext;
            delete m_pBegin;
            m_pBegin = newBegin;
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            leaf* delLeaf = it.getLeaf();
            it.setLeafPreBegin(delLeaf);
            if (delLeaf == m_pBegin) {
                m_pBegin = delLeaf->pnext;
                m_pBegin->pprev = nullptr;
                it = CIterator(m_pBegin);
                it.setLeaf(nullptr);
            }
            else {
                leaf* currLeaf = it.getLeaf();
                currLeaf->pnext = delLeaf->pnext;
                delLeaf->pnext->pprev = currLeaf;
            }
            delete delLeaf;
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            leaf* delLeaf = it.getLeaf();
            it.setLeafPostEnd(delLeaf);
            if (delLeaf == m_pEnd) {
                m_pEnd = delLeaf->pprev;
                m_pEnd->pnext = nullptr;
                it = CIterator(m_pEnd);
                it.setLeaf(nullptr);
            }
            else {
                leaf* currLeaf = it.getLeaf();
                currLeaf->pprev = delLeaf->pprev;
                delLeaf->pprev->pnext = currLeaf;
            }
            delete delLeaf;
        }

        int getSize()
        {
            leaf* curLeaf = m_pBegin;
            int size = 0;
            while (curLeaf != nullptr) {
                ++size;
                curLeaf = curLeaf->pnext;
            }
            return size;
        }

        void clear()
        {
            while (m_pBegin != nullptr) {
                leaf* curDel = m_pBegin;
                m_pBegin = m_pBegin->pnext;
                delete curDel;
            }
            m_pEnd = nullptr;
            m_pBegin = nullptr;
        }

        CIterator begin()
        {
            return CIterator(m_pBegin);
        }

        CIterator end()
        {
            CIterator end = CIterator(m_pBegin);
            end.setLeaf(m_pEnd);
            return end;
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11