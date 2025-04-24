#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <cstddef>
#include <exception>
#include <iterator>
#include <algorithm>

class my_array_out_of_range final : std::exception
{
public:
    const char* what() const noexcept override
    {
        return "my_array out of range";
    }
};

template <typename T, std::size_t N>
class my_array
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

        explicit ReverseIterator(pointer ptr) :
            m_ptr(ptr)
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

    value_type& at(std::size_t i)
    {
        if (i < N)
        {
            return m_data[i];
        }
        throw my_array_out_of_range{};
    }

    const value_type& at(std::size_t i) const
    {
        if (i < N)
        {
            return m_data[i];
        }
        throw my_array_out_of_range{};
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

    constexpr const value_type& front() const
    {
        return m_data[0];
    }

    value_type& back()
    {
        return m_data[N - 1];
    }

    constexpr const value_type& back() const
    {
        return m_data[N - 1];
    }

    value_type* data() noexcept
    {
        return m_data;
    }

    constexpr const value_type* data() const noexcept
    {
        return m_data;
    }

    constexpr bool is_empty() const noexcept
    {
        return N == 0;
    }

    constexpr std::size_t size() const noexcept
    {
        return N;
    }

    void fill(const value_type& value)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            m_data[i] = value;
        }
    }

    void swap(my_array& other) noexcept
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            std::swap(m_data[i], other.m_data[i]);
        }
    }

    iterator begin()
    {
        return iterator(m_data);
    }

    iterator end()
    {
        return iterator(m_data + N);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(m_data + N - 1);
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
        return const_iterator(m_data + N);
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(m_data + N - 1);
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(m_data - 1);
    }

    template <typename U, std::size_t OtherN>
    bool operator==(const my_array<U, OtherN>& other) const noexcept
    {
        if constexpr (N != OtherN)
        {
            return false;
        }

        for (std::size_t i = 0; i < N; ++i)
        {
            if (m_data[i] != other.m_data[i])
            {
                return false;
            }
        }

        return true;
    }

    template <typename U, std::size_t OtherN>
    auto operator<=>(const my_array<U, OtherN>& other) const
    {
        return std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend());
    }

    value_type m_data[N];
};

#endif
