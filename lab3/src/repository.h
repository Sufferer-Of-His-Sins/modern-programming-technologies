#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>

template <class T>
class Repository
{
private:
    std::vector<T> items;

public:
    void Add(T item){ items.push_back(item);}
    void Remove(int index){ items.erase(items.begin() + index);}
    void Update(int index, T item){ items[index] = item;}
    int Size(){ return static_cast<int>(items.size());}
    std::vector<T> GetAll(){ return items;}
    void Clear(){ items.clear();}
};

#endif
