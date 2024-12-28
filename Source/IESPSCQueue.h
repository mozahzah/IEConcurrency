// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include <atomic>
#include <memory>
#include <thread>

template <typename T>
class IESPSCQueue
{
public:
    using ValueType = T;
    IESPSCQueue(size_t Size) :
        m_Capacity(Size),
        m_Data(static_cast<T*>(std::malloc(sizeof(T)* Size))) {}
    IESPSCQueue(const IESPSCQueue&) = delete;
    IESPSCQueue& operator=(const IESPSCQueue&) = delete;
    ~IESPSCQueue()
    {
        while (!IsEmpty())
        {
            Pop();
        }

        std::free(m_Data);
    }

    bool Push(const T& Value)
    {
        if (m_Num.load(std::memory_order_relaxed) >= m_Capacity)
        {
            return false;
        }

        new (m_Data + (m_BackIndex)) T(Value);
        m_BackIndex = (m_BackIndex + 1) % m_Capacity;
        m_Num.fetch_add(1, std::memory_order_release);
        return true;
    }

    std::optional<T> Pop()
    {
        if (m_Num.load(std::memory_order_acquire) == 0)
        {
            return std::nullopt;
        }

        std::optional<T> Element(std::move(m_Data[m_FrontIndex]));
        m_Data[m_FrontIndex].~T();
        m_FrontIndex = (m_FrontIndex + 1) % m_Capacity;
        m_Num.fetch_sub(1, std::memory_order_release);
        return Element;
    }

    bool IsEmpty() const
    {
        return m_Num.load(std::memory_order_relaxed) == 0;
    }

private:
    const size_t m_Capacity;
    T* const m_Data;

    size_t m_FrontIndex = 0;
    size_t m_BackIndex = 0;
    std::atomic<size_t> m_Num{ 0 };
};