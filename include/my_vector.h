#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>
#include <cstring>
#include <exception>
#include <iterator>
#include <utility>
#include <algorithm>
#include <initializer_list>

class my_vector_out_of_range final : std::exception
{
public:
    const char* what() const noexcept override
    {
        return "my_vector out of range";
    }
};

template <typename T>
class my_vector
{
    template <typename U>
    class Iterator
    {
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = U;
        using pointer = U*;
        using reference = U&;

        explicit Iterator(pointer ptr)
            : m_ptr(ptr)
        {
        }

        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        Iterator& operator++()
        {
            ++m_ptr;
            return *this;
        }

        Iterator operator++(int)
        {
            return Iterator(m_ptr++);
        }

        Iterator& operator--()
        {
            --m_ptr;
            return *this;
        }

        Iterator operator--(int)
        {
            return Iterator(m_ptr--);
        }

        Iterator& operator+=(difference_type offset)
        {
            m_ptr += offset;
            return *this;
        }

        Iterator operator+(difference_type offset) const
        {
            return Iterator(m_ptr + offset);
        }

        Iterator& operator-=(difference_type offset)
        {
            m_ptr -= offset;
            return *this;
        }

        Iterator operator-(difference_type offset) const
        {
            return Iterator(m_ptr - offset);
        }

        difference_type operator-(const Iterator& other) const
        {
            return m_ptr - other.m_ptr;
        }

        reference operator[](difference_type index) const
        {
            return *(m_ptr + index);
        }

        bool operator==(const Iterator& other) const
        {
            return m_ptr == other.m_ptr;
        }

        auto operator<=>(const Iterator& other) const = default;

        explicit operator pointer() const
        {
            return m_ptr;
        }

        operator Iterator<const value_type>() const
        {
            return Iterator<const value_type>(m_ptr);
        }

    private:
        pointer m_ptr;
    };

    template <typename U>
    class ReverseIterator
    {
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = U;
        using pointer = U*;
        using reference = U&;

        explicit ReverseIterator(pointer ptr)
            : m_ptr(ptr)
        {
        }

        reference operator*() const
        {
            return *m_ptr;
        }

        pointer operator->() const
        {
            return m_ptr;
        }

        ReverseIterator& operator++()
        {
            --m_ptr;
            return *this;
        }

        ReverseIterator operator++(int)
        {
            return ReverseIterator(m_ptr--);
        }

        ReverseIterator& operator--() {
            ++m_ptr;
            return *this;
        }

        ReverseIterator operator--(int)
        {
            return ReverseIterator(m_ptr++);
        }

        ReverseIterator& operator+=(difference_type offset)
        {
            m_ptr -= offset;
            return *this;
        }

        ReverseIterator operator+(difference_type offset) const
        {
            return ReverseIterator(m_ptr - offset);
        }

        ReverseIterator& operator-=(difference_type offset)
        {
            m_ptr += offset;
            return *this;
        }

        ReverseIterator operator-(difference_type offset) const
        {
            return ReverseIterator(m_ptr + offset);
        }

        difference_type operator-(const ReverseIterator& other) const
        {
            return other.m_ptr - m_ptr;
        }

        reference operator[](difference_type index) const
        {
            return *(m_ptr - index);
        }

        bool operator==(const ReverseIterator& other) const
        {
            return m_ptr == other.m_ptr;
        }

        auto operator<=>(const ReverseIterator& other) const
        {
            return other <=> *this;
        }

        explicit operator pointer() const
        {
            return m_ptr;
        }

        operator ReverseIterator<const value_type>() const
        {
            return ReverseIterator<const value_type>(m_ptr);
        }

    private:
        pointer m_ptr;
    };

public:
    using value_type = T;

    using iterator = Iterator<value_type>;
    using const_iterator = Iterator<const value_type>;
    using reverse_iterator = ReverseIterator<value_type>;
    using const_reverse_iterator = ReverseIterator<const value_type>;

    my_vector() = default;

    my_vector(const my_vector& other)
    {
        insert(end(), other.cbegin(), other.cend());
    }

    my_vector(my_vector&& other) noexcept :
        m_capacity{ other.m_capacity },
        m_size{ other.m_size },
        m_data{ other.m_data }
    {
        other.m_capacity = 0;
        other.m_size = 0;
        other.m_data = nullptr;
    }

    my_vector(std::initializer_list<value_type> initializerList)
    {
        insert(end(), initializerList.begin(), initializerList.end());
    }

    template<class InputIt>
    my_vector(InputIt first, InputIt last)
    {
        insert(end(), first, last);
    }

    my_vector& operator=(const my_vector& other)
    {
        if (this != &other)
        {
            my_vector tmp(other);
            swap(tmp);
        }

        return *this;
    }

    my_vector& operator=(my_vector&& other) noexcept
    {
        if (this != &other)
        {
            my_vector tmp(std::move(other));
            swap(tmp);
        }

        return *this;
    }

    my_vector& operator=(std::initializer_list<value_type> initializerList)
    {
        my_vector tmp(std::move(initializerList));
        swap(tmp);

        return *this;
    }

    value_type& at(std::size_t i)
    {
        if (i < size())
        {
            return m_data[i];
        }
        throw my_vector_out_of_range{};
    }

    const value_type& at(std::size_t i) const
    {
        if (i < size())
        {
            return m_data[i];
        }
        throw my_vector_out_of_range{};
    }

    value_type& operator[](std::size_t i)
    {
        return m_data[i];
    }

    const value_type& operator[](std::size_t i) const
    {
        return m_data[i];
    }

    value_type& front()
    {
        return m_data[0];
    }

    const value_type& front() const
    {
        return m_data[0];
    }

    value_type& back()
    {
        return m_data[size() - 1];
    }

    const value_type& back() const
    {
        return m_data[size() - 1];
    }

    value_type* data() noexcept
    {
        return m_data;
    }

    const value_type* data() const noexcept
    {
        return m_data;
    }

    bool is_empty() const noexcept
    {
        return m_size == 0;
    }

    std::size_t size() const noexcept
    {
        return m_size;
    }

    std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

    void swap(my_vector& other) noexcept
    {
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_size, other.m_size);
        std::swap(m_data, other.m_data);
    }

    iterator begin()
    {
        return iterator(m_data);
    }

    iterator end()
    {
        return iterator(m_data + size());
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(m_data + size() - 1);
    }

    reverse_iterator rend()
    {
        return reverse_iterator(m_data - 1);
    }

    const_iterator cbegin() const
    {
        return const_iterator(m_data);
    }

    const_iterator cend() const
    {
        return const_iterator(m_data + size());
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(m_data + size() - 1);
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(m_data - 1);
    }

    template <typename U>
    bool operator==(const my_vector<U>& other) const noexcept
    {
        if (size() != other.size())
        {
            return false;
        }

        for (std::size_t i = 0; i < size(); ++i)
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename U>
    auto operator<=>(const my_vector<U>& other) const
    {
        return std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend());
    }

    void reserve(std::size_t newCapacity)
    {
        if (m_capacity < newCapacity)
        {
            reallocate(newCapacity);
        }
    }

    void shrink_to_fit()
    {
        reallocate(m_size);
    }

    void push_back(const value_type& elem)
    {
        if (m_size == m_capacity)
        {
            value_type elemCopy = elem;
            if (m_capacity != 0)
            {
                reallocate(m_capacity << 1);
            }
            else
            {
                reallocate(1);
            }

            new(m_data + m_size++) value_type(std::move(elemCopy));
        }
        else
        {
            new(m_data + m_size++) value_type(elem);
        }
    }

    void push_back(value_type&& elem)
    {
        if (m_size == m_capacity)
        {
            value_type elemCopy = elem;
            if (m_capacity != 0)
            {
                reallocate(m_capacity << 1);
            }
            else
            {
                reallocate(1);
            }

            new(m_data + m_size++) value_type(std::move(elem));
        }
        else
        {
            new(m_data + m_size++) value_type(elem);
        }
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        if (m_size == m_capacity)
        {
            if (m_capacity != 0)
            {
                reallocate(m_capacity << 1);
            }
            else
            {
                reallocate(1);
            }
        }

        new(m_data + m_size++) value_type(std::forward<value_type>(args)...);
    }

    void pop_back()
    {
        m_data[--m_size].~value_type();
        while (m_capacity != 0 && m_size == m_capacity >> 2)
        {
            reallocate(m_capacity >> 1);
        }
    }

    iterator insert(const_iterator pos, value_type&& elem)
    {
        std::size_t numPos = pos - cbegin();

        if (m_size == m_capacity)
        {
            if (m_capacity != 0)
            {
                reallocate(m_capacity << 1);
            }
            else
            {
                reallocate(1);
            }
        }

        ++m_size;
        for (int i = m_size - 1; i > numPos; --i)
        {
            if (i == m_size - 1)
            {
                new(m_data + i) value_type(std::forward<value_type>(m_data[i - 1]));
            }
            else
            {
                m_data[i] = std::move(m_data[i - 1]);
            }
        }

        if (numPos == m_size - 1)
        {
            new(m_data + numPos) value_type(std::forward<value_type>(elem));
        }
        else
        {
            m_data[numPos] = std::forward<value_type>(elem);
        }

        return iterator(m_data + numPos);
    }

    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        std::size_t numPos = pos - cbegin();
        const std::size_t elemsCount = std::distance(first, last);

        while (m_size + elemsCount > m_capacity)
        {
            if (m_capacity != 0)
            {
                reallocate(m_capacity << 1);
            }
            else
            {
                reallocate(1);
            }
        }

        m_size += elemsCount;
        for (int i = m_size - 1; i >= numPos + elemsCount; --i)
        {
            if (i >= m_size - elemsCount)
            {
                new(m_data + i) value_type(std::forward<value_type>(m_data[i - elemsCount]));
            }
            else
            {
                m_data[i] = std::move(m_data[i - elemsCount]);
            }
        }

        std::size_t currPos = numPos;
        while (first != last)
        {
            if (currPos >= m_size - elemsCount)
            {
                new(m_data + currPos++) value_type(*first++);
            }
            else
            {
                m_data[currPos++] = *first++;
            }
        }

        return iterator(m_data + numPos);
    }

    iterator erase(const_iterator pos)
    {
        std::size_t numPos = pos - cbegin();

        m_data[numPos].~value_type();

        --m_size;
        for (std::size_t i = numPos; i < m_size; ++i)
        {
            m_data[i] = std::move(m_data[i + 1]);
        }

        while (m_capacity != 0 && m_size == m_capacity >> 2)
        {
            reallocate(m_capacity >> 1);
        }

        return iterator(m_data + numPos);
    }

    iterator erase(iterator first, iterator last)
    {
        std::size_t intervalSize = last - first;
        std::size_t numPos = first - begin();

        const_iterator endRange = last;
        while (first != end() - intervalSize)
        {
            first->~value_type();
            if (last != end())
            {
                *first = *last;
                ++last;
            }
            ++first;
        }

        m_size -= intervalSize;

        while (m_capacity != 0 && m_size <= m_capacity >> 2)
        {
            reallocate(m_capacity >> 1);
        }

        return iterator(m_data + numPos);
    }

    void clear()
    {
        erase(begin(), end());
        reallocate(0);
    }

    void resize(std::size_t count)
    {
        if (count < m_size)
        {
            erase(begin() + count, end());
        }
        else
        {
            while (count > m_capacity)
            {
                if (m_capacity != 0)
                {
                    reallocate(m_capacity << 1);
                }
                else
                {
                    reallocate(1);
                }
            }
            for (std::size_t i = m_size; i < count; ++i)
            {
                new(m_data + i) value_type{};
            }
            m_size = count;
        }
    }

    void resize(std::size_t count, const value_type& value)
    {
        if (count < size())
        {
            erase(begin() + count, end());
        }
        else
        {
            while (count > m_capacity)
            {
                if (m_capacity != 0)
                {
                    reallocate(m_capacity << 1);
                }
                else
                {
                    reallocate(1);
                }
            }
            for (std::size_t i = m_size; i < count; ++i)
            {
                new(m_data + i) value_type(value);
            }
            m_size = count;
        }
    }

private:
    void reallocate(std::size_t newCapacity)
    {
        m_capacity = newCapacity;
        auto newBuffer = static_cast<value_type*>(std::malloc(sizeof(value_type) * newCapacity));
        for (std::size_t i = 0; i < std::min(size(), capacity()); ++i)
        {
            newBuffer[i] = std::move(m_data[i]);
        }
        free(m_data);
        m_data = newBuffer;
    }

    std::size_t m_capacity = 0;
    std::size_t m_size = 0;
    value_type* m_data = nullptr;
};

#endif
