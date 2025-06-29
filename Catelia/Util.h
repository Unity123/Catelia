#pragma once

using namespace std;

#include <unordered_map>
#include <queue>
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
	queue<T> ru;
};

template<typename T, typename U>
inline shared_ptr<U> MapCache<T, U>::get(T key)
{
	ru.push(key);
	if (m.size() > max_size) {
		m.erase(ru.front());
		ru.pop();
	}
	if (m.count(key) == 0) {
		U val = load(key);
		m.emplace(key, make_shared<U>(val));
	}
	return m[key];
}

template<typename T, typename U>
inline MapCache<T, U>::MapCache(int size)
{
	max_size = size;
}

template<typename T, typename U>
inline MapCache<T, U>::MapCache()
{
	max_size = 50;
	load = nullptr;
}