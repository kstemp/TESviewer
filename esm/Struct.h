#pragma once

#include <variant>
#include <unordered_map>

using Value = std::variant<uint16_t, uint32_t, float>;

using ValueMap = std::unordered_map<std::string, Value>;

using StructValue = std::variant<Value, ValueMap>;

using StructValueMap = std::unordered_map<std::string, StructValue>;

namespace ESM {
	struct Struct {
		StructValueMap valueMap;

		const Value& operator [](const std::string& key) const {
			const auto it = valueMap.find(key);

			if (it == valueMap.end())
				throw;

			return std::get<Value>(it->second);
		}

		Value& operator [](const std::string& key) {
			StructValue& it = valueMap[key];

			return std::get<Value>(it);
		}

		const Value& operator ()(const std::string& key, const std::string& key2) const {
			const auto it = valueMap.find(key);

			if (it == valueMap.end())
				throw;

			const auto mp = std::get<ValueMap>(it->second);

			const auto it2 = mp.find(key2);

			return std::get<Value>(*it2);
		}

		Value& operator ()(const std::string& key, const std::string& key2) {
			StructValue& it = valueMap[key];

			//	if (it == valueMap.end())
				//	throw;
			size_t i = it.index();

			if (i == 0)
				it = ValueMap();

			ValueMap& mp = std::get<ValueMap>(it);

			Value& it2 = mp[key2];

			return it2;
		}
	};
}