#ifndef TEMPLATES_LIST_2021_02_11
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
                m_pCurrent = nullptr;
                m_pBegin = p;
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
            clear();
        }

        void pushBack(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr);
            if (nullptr == m_pEnd) {
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
            if (nullptr == m_pBegin) {
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
            if (nullptr == newBegin) {
                m_pEnd = nullptr;
            }
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid()) {
                return;
            }
            leaf* p = it.getLeaf();
            leaf* pPrev = nullptr;
            if (p == m_pBegin) {
                m_pBegin = p->pnext;
                it.setLeafPreBegin(m_pBegin);
            }
            else {
                pPrev = m_pBegin;
                while (nullptr != pPrev) {
                    if (pPrev->pnext == p) {
                        break;
                    }
                    pPrev = pPrev->pnext;
                }
                it.setLeaf(pPrev);
            }
            if (nullptr != pPrev) {
                pPrev->pnext = p->pnext;
            }
            if (!p->pnext) {
                m_pEnd = pPrev;
            }
            p->pnext = nullptr;
            delete p;
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
                m_pCurrent = nullptr;
                m_pBegin = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf* p)
            {
                m_pCurrent = nullptr;
                m_pEnd = p;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
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
            clear();
        };

        void pushBack(T& data)
        {
            leaf *newLeaf = new leaf(data, m_pEnd, nullptr);
            if (nullptr == m_pEnd) {
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
            m_pEnd->pnext = nullptr;
            if (nullptr == newEnd) {
                m_pBegin = nullptr;
            }
            return tmp;
        }

        void pushFront(T& data)
        {
            leaf* newLeaf = new leaf(data, nullptr, m_pBegin);
            if (nullptr == m_pBegin) {
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
            m_pBegin->pprev = nullptr;
            if (nullptr == newBegin) {
                m_pEnd = nullptr;
            }
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator& it)
        {
            if (!it.isValid()) {
                return;
            }
            leaf* p = it.getLeaf();
            leaf* pPrev = nullptr;
            if (p == m_pBegin) {
                m_pBegin = p->pnext;
                it.setLeafPreBegin(m_pBegin);
            }
            else {
                pPrev = m_pBegin;
                while (nullptr != pPrev) {
                    if (pPrev->pnext == p) {
                        break;
                    }
                    pPrev = pPrev->pnext;
                }
                it.setLeaf(pPrev);
            }
            if (nullptr != pPrev) {
                pPrev->pnext = p->pnext;
            }
            if (!p->pnext) {
                m_pEnd = pPrev;
            }
            p->pnext = nullptr;
            delete p;
        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator& it)
        {
            if (!it.isValid()) {
                return;
            }
            leaf* p = it.getLeaf();
            leaf* pNext = nullptr;
            if (p == m_pEnd) {
                m_pEnd = p->pprev;
                it.setLeafPostEnd(m_pEnd);
            }
            else {
                pNext = m_pEnd;
                while (nullptr != pNext) {
                    if (pNext->pprev == p) {
                        break;
                    }
                    pNext = pNext->pprev;
                }
                it.setLeaf(pNext);
            }
            if (nullptr != pNext) {
                pNext->pprev = p->pprev;
            }
            if (!p->pprev) {
                m_pBegin = pNext;
            }
            p->pprev = nullptr;
            delete p;
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
            return CIterator(m_pEnd);
        }

    private:
        leaf* m_pBegin, *m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2021_02_11
