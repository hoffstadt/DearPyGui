#pragma once

namespace Marvel {

    template<typename T>
    using node = typename mvQueue<T>::node;

    //-----------------------------------------------------------------------------
    // Public Methods
    //-----------------------------------------------------------------------------
    template<typename T>
    Ref<T> mvQueue<T>::wait_and_pop()
    {
        OwnedPtr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    template<typename T>
    void mvQueue<T>::wait_and_pop(T& value)
    {
        OwnedPtr<node> const old_head = wait_pop_head(value);
    }

    template<typename T>
    Ref<T> mvQueue<T>::try_pop()
    {
        OwnedPtr<node> const old_head = try_pop_head();
        return old_head ? old_head->data : Ref<T>();
    }

    template<typename T>
    bool mvQueue<T>::try_pop(T& value)
    {
        OwnedPtr<node> const old_head = try_pop_head(value);
        if (old_head)
            return true;
        return false;
    }

    template<typename T>
    void mvQueue<T>::push(T value)
    {

        Ref<T> new_data = CreateRef<T>(std::move(value));
        OwnedPtr<node> p(new node);

        // scoped in order to unlock tail mutex before notifying other threads
        {
            std::lock_guard<std::mutex> tail_lock(m_tail_mutex);
            m_tail->data = new_data;
            node* const new_tail = p.get();
            m_tail->next = std::move(p);
            m_tail = new_tail;
        }

        m_data_cond.notify_one();
    }

    template<typename T>
    bool mvQueue<T>::empty()
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        return (m_head == get_tail());
    }

    //-----------------------------------------------------------------------------
    // Private Methods
    //-----------------------------------------------------------------------------
    template<typename T>
    node<T>* mvQueue<T>::get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(m_tail_mutex);
        return m_tail;
    }

    template<typename T>
    OwnedPtr<node<T>> mvQueue<T>::pop_head()
    {
        OwnedPtr<node> old_head = std::move(m_head);
        m_head = std::move(old_head->next);
        return old_head;
    }

    template<typename T>
    OwnedPtr<node<T>> mvQueue<T>::try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        if (m_head.get() == get_tail())
            return OwnedPtr<node>();
        return pop_head();
    }

    template<typename T>
    OwnedPtr<node<T>> mvQueue<T>::try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(m_head_mutex);
        if (m_head.get() == get_tail())
            return OwnedPtr<node>();

        value = std::move(*m_head->data);
        return pop_head();
    }

    template<typename T>
    std::unique_lock<std::mutex> mvQueue<T>::wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(m_head_mutex);
        m_data_cond.wait(head_lock, [&] {return m_head != get_tail(); });
        return std::move(head_lock);
    }

    template<typename T>
    OwnedPtr<node<T>> mvQueue<T>::wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }

    template<typename T>
    OwnedPtr<node<T>> mvQueue<T>::wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*m_head->data);
        return pop_head();
    }

}