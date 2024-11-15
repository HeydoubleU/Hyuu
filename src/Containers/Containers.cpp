#include "Containers.h"
#include <functional>
#include <Amino/Cpp/ClassDefine.h>
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::Containers::Map::HashMap);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::Containers::Set::HashSet);


void Hyuu::Containers::Map::map_set(HashMap& map, const Amino::ulong_t& hash, const Amino::Any& value) {
	//map.m_map.insert(std::make_pair(hash, value));
	map.m_map[hash] = value;

}


bool Hyuu::Containers::Map::map_get(const HashMap& map, const Amino::ulong_t& hash, Amino::Any& value)
{
	auto item = map.m_map.find(hash);
	if (item != map.m_map.end()) {
		value = item->second;
		return true;
	}
	else {
		return false;
	}
}


bool Hyuu::Containers::Map::map_remove(HashMap& map, const Amino::ulong_t& hash) {
	auto result = map.m_map.erase(hash);
	return result > 0;
}


bool Hyuu::Containers::Map::map_remove(HashMap& map, const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash) {
	const auto& hash_ref = *hash;
	auto result = 0;
	for (const auto& h : hash_ref) {
		result += map.m_map.erase(h);
	}
	return result > 0;
}


bool Hyuu::Containers::Map::map_pop(HashMap& map, const Amino::ulong_t& hash, Amino::Any& value) {
	auto item = map.m_map.find(hash);
	if (item != map.m_map.end()) {
		value = item->second;
		map.m_map.erase(item);
		return true;
	}
	else {
		return false;
	}
}


void Hyuu::Containers::Map::map_hashes(const HashMap& map, Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes) {
	auto hash_array = Amino::Array<Amino::ulong_t>();
	for (const auto& item : map.m_map) {
		hash_array.push_back(item.first);
	}

	hashes = Amino::newClassPtr<Amino::Array<Amino::ulong_t>>(std::move(hash_array));

}


void Hyuu::Containers::Map::map_values(const HashMap& map, Amino::Ptr<Amino::Array<Amino::Any>>& values) {
	auto value_array = Amino::Array<Amino::Any>();
	for (const auto& item : map.m_map) {
		value_array.push_back(item.second);
	}

	values = Amino::newClassPtr<Amino::Array<Amino::Any>>(std::move(value_array));
}


void Hyuu::Containers::Map::map_set_items(HashMap& map, const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes, const Amino::Ptr<Amino::Array<Amino::Any>>& values) {
	auto min_size = std::min(hashes->size(), values->size());
	map.m_map.reserve(min_size);
	for (int i = 0; i < min_size; ++i) {
		const auto& hash = hashes->at(i);
		const auto& value = values->at(i);
		map.m_map[hash] = value;
	}
}


bool Hyuu::Containers::Map::map_get_first(const HashMap& map, Amino::ulong_t& hash, Amino::Any& value) {
	if (map.m_map.empty()) {
		return false;
	}
	const auto& it = map.m_map.begin();
	hash = it->first;
	value = it->second;
	return true;
}


// Sets functions ===============================================================

void Hyuu::Containers::Set::set_add(HashSet& set, const Amino::ulong_t& hash) {
	set.m_set.insert(hash);
}

void Hyuu::Containers::Set::set_remove(HashSet& set, const Amino::ulong_t& hash) {
	set.m_set.erase(hash);
}


void Hyuu::Containers::Set::set_add(HashSet& set, const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash) {
	const auto& hash_ref = *hash;
	for (const auto& h : hash_ref) {
		set.m_set.insert(h);
	}
}

void Hyuu::Containers::Set::set_remove(HashSet& set, const Amino::Ptr<Amino::Array<Amino::ulong_t>>& hash) {
	const auto& hash_ref = *hash;
	for (const auto& h : hash_ref) {
		set.m_set.erase(h);
	}
}

void Hyuu::Containers::Set::set_hashes(const HashSet& set, Amino::Ptr<Amino::Array<Amino::ulong_t>>& hashes) {
	auto hash_array = Amino::Array<Amino::ulong_t>();
	for (const auto& item : set.m_set) {
		hash_array.push_back(item);
	}

	hashes = Amino::newClassPtr<Amino::Array<Amino::ulong_t>>(std::move(hash_array));
}


// General container functions ===============================================================

void Hyuu::Containers::container_size(const Map::HashMap& container, Amino::long_t& size) {
	size = container.m_map.size();
}

void Hyuu::Containers::container_size(const Set::HashSet& container, Amino::long_t& size) {
	size = container.m_set.size();
}

bool Hyuu::Containers::container_contains(const Map::HashMap& container, const Amino::ulong_t& hash) {
	return container.m_map.find(hash) != container.m_map.end();
}

bool Hyuu::Containers::container_contains(const Set::HashSet& container, const Amino::ulong_t& hash) {
	return container.m_set.find(hash) != container.m_set.end();
}

bool Hyuu::Containers::container_empty(const Map::HashMap& container) {
	return container.m_map.empty();
}

bool Hyuu::Containers::container_empty(const Set::HashSet& container) {
	return container.m_set.empty();
}


// Hash functions ===============================================================

Amino::ulong_t Hyuu::Containers::hash_value(const Amino::ulong_t& value) {
	return value;
}


Amino::ulong_t Hyuu::Containers::hash_value(const Amino::long_t& value) {
	return std::hash< Amino::long_t>()(value);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Amino::float_t& value) {
	return std::hash<float>()(value);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Amino::String& value) {
	return std::hash<std::string>()(value.asChar());
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::float2& value) {
	auto hash_x = std::hash<float>()(value.x);
	auto hash_y = std::hash<float>()(value.y);
	return hash_x ^ (hash_y << 1);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::float3& value) {
	auto hash_x = std::hash<float>()(value.x);
	auto hash_y = std::hash<float>()(value.y);
	auto hash_z = std::hash<float>()(value.z);
	return hash_x ^ (hash_y << 1) ^ (hash_z << 2);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::float4& value) {
	auto hash_x = std::hash<float>()(value.x);
	auto hash_y = std::hash<float>()(value.y);
	auto hash_z = std::hash<float>()(value.z);
	auto hash_w = std::hash<float>()(value.w);
	return hash_x ^ (hash_y << 1) ^ (hash_z << 2) ^ (hash_w << 3);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::long2& value) {
	auto hash_x = std::hash<long>()(value.x);
	auto hash_y = std::hash<long>()(value.y);
	return hash_x ^ (hash_y << 1);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::long3& value) {
	auto hash_x = std::hash<long>()(value.x);
	auto hash_y = std::hash<long>()(value.y);
	auto hash_z = std::hash<long>()(value.z);
	return hash_x ^ (hash_y << 1) ^ (hash_z << 2);
}


Amino::ulong_t Hyuu::Containers::hash_value(const Bifrost::Math::long4& value) {
	auto hash_x = std::hash<long>()(value.x);
	auto hash_y = std::hash<long>()(value.y);
	auto hash_z = std::hash<long>()(value.z);
	auto hash_w = std::hash<long>()(value.w);
	return hash_x ^ (hash_y << 1) ^ (hash_z << 2) ^ (hash_w << 3);
}