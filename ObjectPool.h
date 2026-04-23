#pragma once
#include <unordered_set>
#include <vector>
#include <cstddef>

template <class T>
class ObjectPool
{
    std::vector<T *> freeObjects;
    std::unordered_set<T *> takenObjects;

public:
    ObjectPool() // O(1)
    {
    }
    ~ObjectPool() // O(N)
    {
        clear();
    }
    void reserve(size_t n) // O(n)
    {
        if (capacity() >= n)
        {
            return;
        }
        int missingPlacesNumber = n - capacity();
        freeObjects.reserve(freeObjects.capacity() + missingPlacesNumber);
    }
    void reserveObjects(size_t n) // O(n)
    {
        if (size() >= n)
        {
            return;
        }
        int missingObjectsNumber = n - size();
        freeObjects.reserve(freeObjects.size() + missingObjectsNumber);
        while (missingObjectsNumber--)
        {
            freeObjects.push_back(new T());
        }
    }
    template <typename... Args>
    T *get(Args &&...args) // ~ O(1)
    {
        T *ptr = nullptr;
        if (freeObjects.empty())
        {
            ptr = new T(std::forward<Args>(args)...);
        }
        else
        {
            ptr = freeObjects.back();
            freeObjects.pop_back();
            if constexpr (sizeof...(Args) > 0)
            {
                ptr->~T();
                new (ptr) T(std::forward<Args>(args)...);
            }
        }
        takenObjects.insert(ptr);
        return ptr;
    }
    void release(T *ptr) // ~ O(1)
    {
        auto it = takenObjects.find(ptr);
        if (it == takenObjects.end())
        {
            return;
        }
        takenObjects.erase(it);
        freeObjects.push_back(ptr);
    }
    void clear() // O(N)
    {
        for (T *ptr : freeObjects)
        {
            delete ptr;
        }
        for (T *ptr : takenObjects)
        {
            delete ptr;
        }
        freeObjects.clear();
        takenObjects.clear();
    }
    size_t size() const // O(1)
    {
        return freeObjects.size() + takenObjects.size();
    }
    size_t capacity() const // O(1)
    {
        return freeObjects.capacity() + takenObjects.size();
    }
};
