//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASEV1_1_LIST_HPP
#define PROJECT_BASEV1_1_LIST_HPP
#include "../h/MemoryAllocator.hpp"
#include "../lib/mem.h"

template<typename T>
class List{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:

    void *operator new(size_t n) {
        MemoryAllocator &m = m.getInstance();
        return m.kmem_alloc(n);
        //return __mem_alloc(n);
    }

    void operator delete(void *p) {
        MemoryAllocator &m = m.getInstance();
        m.kmem_free(p);
        //__mem_free(p);
    }

    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        MemoryAllocator& m = m.getInstance();
        Elem* elem = (Elem*)m.kmem_alloc(sizeof(Elem));
        //Elem* elem = (Elem*) __mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = head;
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        MemoryAllocator& m = m.getInstance();
        Elem* elem = (Elem*)m.kmem_alloc(sizeof(Elem));
        //Elem* elem = (Elem*) __mem_alloc(sizeof(Elem));
        elem->data = data;
        elem->next = 0;
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        MemoryAllocator& m = m.getInstance();
        m.kmem_free(elem);
        //__mem_free(elem);
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
};

#endif //PROJECT_BASEV1_1_LIST_HPP
