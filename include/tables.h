#pragma once
#include <iostream>
#include <utility> 
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

template<class T>
class TableInterface{
public:
	virtual bool insert(size_t key, const T& data) = 0;
	virtual bool erase(size_t key) = 0;
	virtual size_t getSize() = 0;
	virtual bool isEmpty() = 0;
	virtual void clear() = 0;
	virtual pair<size_t, T>* find(size_t key) = 0;
	virtual  const pair<size_t, T>* find(size_t key) const = 0;
};


template<class T, class CellType = pair<size_t, T>>
class TableByArray : public TableInterface<T>{
protected:
	vector<CellType> data;
	size_t size = 0;
public:

	size_t getSize() override { return size; }
	bool isEmpty()  override { return size == 0; }
	void clear() override
	{
		data.clear();
		size = 0;
	}
	auto begin() { return data.begin(); }
	auto end() { return data.end(); }
	auto begin() const { return data.begin(); }
	auto end() const { return data.end(); }
};

template<class T>
class UnorderedTable : public TableByArray<T>{
private:
	using TableByArray<T>::data;
	using TableByArray<T>::size;
public:
	using TableByArray<T>::clear;
	using TableByArray<T>::isEmpty;
	UnorderedTable() = default;
	~UnorderedTable() { clear(); }
	const pair<size_t, T>* find(size_t key) const override {
		for (auto it = data.begin(); it != data.end(); ++it){
			if (it->first == key)
			{
				return &(*it);
			}
		}
		return nullptr;
	}

	pair<size_t, T>* find(size_t key)  override {
		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it->first == key)
			{
				return &(*it);
			}
		}
		return nullptr;
	}
	bool insert(size_t key, const T& value) override {
		bool is_exist = false;
		for (auto it = data.begin(); it != data.end(); ++it)
			if (it->first == key) {
				is_exist = true;
				break;
			}
		if (!is_exist) {
			data.push_back(make_pair(key, value));
			++size;
			return true;
		}
		else { return false; }
	}

	bool erase(size_t key) override	{
		for (size_t i = 0; i < size; i++)
			if (data[i].first == key) {
				swap(data[i], data[size - 1]);
				data.pop_back();
				--size;
				return true;
			}
		return false;
	}

};

template<class T>
class OrderedTable : public TableByArray<T> {
private:
	using TableByArray<T>::data;
	using TableByArray<T>::size;
	typename vector<pair<size_t, T>>::iterator safe_lower_bound(size_t key) {
		return lower_bound(data.begin(), data.end(), key,
			[](const auto& elem, size_t k) { return elem.first < k; });
	}
	typename vector<pair<size_t, T>>::const_iterator safe_lower_bound(size_t key) const {
		return lower_bound(data.begin(), data.end(), key,
			[](const auto& elem, size_t k) { return elem.first < k; });
	}

public:
	OrderedTable() = default;
	~OrderedTable() { clear(); }

	const pair<size_t, T>* find(size_t key) const override {
		auto it = safe_lower_bound(key);
		if (it != data.end() && it->first == key) {
			return &(*it);
		}
		else {
			return nullptr;
		}
	}

	pair<size_t, T>* find(size_t key) override {
		auto it = safe_lower_bound(key);
		if (it != data.end() && it->first == key) {
			return &(*it);
		}
		else {
			return nullptr;
		}
	}

	bool insert(size_t key, const T& value) override {
		auto it = safe_lower_bound(key);
		if (it != data.end() && it->first == key) {
			return false;
		}
		data.insert(it, make_pair(key, value));
		++size;
		return true;
	}

	bool erase(size_t key) override {
		auto it = safe_lower_bound(key);
		if (it == data.end() || it->first != key) {
			return false;
		}
		data.erase(it);
		--size;
		return true;
	}
};


template<class T>
class HashTable : public TableByArray<T, std::list<std::pair<size_t, T>>> {
private:
	size_t hashFunction(size_t key) const {
		if (data.empty()) return 0;
		const size_t prime = 0x9e3779b97f4a7c15ULL;
		key ^= key >> 33;
		key *= prime;
		key ^= key >> 33;
		return key % data.size();
	}
	using TableByArray<T, list<pair<size_t, T>>>::data;
	using TableByArray<T, list<pair<size_t, T>>>::size;
	size_t data_size = 0;


public:
	HashTable(size_t sz = 128) : data_size(sz) {
		data.resize(data_size);
	};
	~HashTable() { clear(); }

	const pair<size_t, T>* find(size_t key) const override {
		auto idx = hashFunction(key);
		if (data[idx].size() != 0)
			for (auto it = data[idx].begin(); it != data[idx].end(); ++it) {
				if (it->first == key) { return &(*it); }
			}
		return nullptr;
	}
	pair<size_t, T>* find(size_t key) override {
		auto idx = hashFunction(key);
		if (data[idx].size() != 0)
			for (auto it = data[idx].begin(); it != data[idx].end(); ++it)	{
				if (it->first == key) { return &(*it); }
			}
		return nullptr;
	}
	bool erase(size_t key) override	{
		auto idx = hashFunction(key);
		for (auto it = data[idx].begin(); it != data[idx].end(); ++it) {
			if (it->first == key) {
				data[idx].erase(it);
				--size;
				return true;
			}
		}
		return false;
	}

	bool insert(size_t key, const T& value) override {
		auto idx = hashFunction(key);
		for (auto& item : data[idx]) {
			if (item.first == key) {
				return false;
			}
		}
		data[idx].push_back(make_pair(key, value));
		++size;
		return true;
	}
};