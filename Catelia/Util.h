#pragma once

using namespace std;

#include <unordered_map>
#include <set>
#include <memory>

/*// like a shared pointer, but doesn't autodelete, used specifically so that I can remove the item from the data structure when it's deleted
template<typename T>
class RefCtr {
public:
	T* item;
	int refs = 0;
	RefCtr(T* item);
};

template<typename T>
inline RefCtr<T>::RefCtr(T* item)
{
	this->item = item;
}*/

template<typename T, typename U>
class MapCache {
public:
	shared_ptr<U> get(T key);
	U (*load)(T);
	int max_size;
	MapCache(int size);
	MapCache();
private:
	unordered_map<T, shared_ptr<U>> m;
	set<T> ru;
};

template<typename T, typename U>
inline shared_ptr<U> MapCache<T, U>::get(T key)
{
	ru.insert(key);
	if (m.size() > max_size) {
		m.erase(*ru.begin());
		ru.erase(*ru.begin());
	}
	if (m.count(key) == 0) {
		U val = load(key);
		m.insert({ key, make_shared<U>(move(val)) });
	}
	return m[key];  
}

template<typename T, typename U>
inline MapCache<T, U>::MapCache(int size)
{
	max_size = size;
	load = nullptr;
}

template<typename T, typename U>
inline MapCache<T, U>::MapCache()
{
	max_size = 50;
	load = nullptr;
}