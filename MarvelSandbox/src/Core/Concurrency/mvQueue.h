#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Core/mvMemory.h"

namespace Marvel {

    template<typename T>
    class mvQueue
    {

    public:

        struct node
        {
            Ref<T>         data;
            OwnedPtr<node> next;
        };

    public:

        mvQueue() : m_head(new node), m_tail(m_head.get()){}

        // copy assignment and constructor deleted
        mvQueue           (const mvQueue& other) = delete;
        mvQueue& operator=(const mvQueue& other) = delete;

        Ref<T> wait_and_pop();
        Ref<T> try_pop     ();
        void   wait_and_pop(T& value);
        bool   try_pop     (T& value);
        void   push        (T value);
        bool   empty       ();

    private:

        node*                        get_tail();
        OwnedPtr<node>               pop_head();
        OwnedPtr<node>               try_pop_head();
        OwnedPtr<node>               try_pop_head(T& value);
        OwnedPtr<node>               wait_pop_head();
        OwnedPtr<node>               wait_pop_head(T& value);
        std::unique_lock<std::mutex> wait_for_data();

    private:

        std::mutex              m_head_mutex;
        std::mutex              m_tail_mutex;
        OwnedPtr<node>          m_head;
        node*                   m_tail;
        std::condition_variable m_data_cond;

    };

}

#include "mvQueueImpl.hpp"