#pragma once

#include <string>
#include <vector>

#include "Struct.h"

namespace ESM {
	struct Field {
		std::vector<char> buffer;
		uint16_t size;
		std::string name;

		Struct struct_;

		Field(const std::string& name, const uint16_t size) : name(name), size(size) {}

		template<typename T>
		T get() {
			// TODO check size etc.
			return *(T*)(&buffer[0]);
		}

		template<typename T>
		const T get() const {
			// TODO check size etc.
			return *(T*)(&buffer[0]);
		}

		std::string string() {
			return std::string(buffer.begin(), buffer.end());
		}

		const std::string string() const {
			return std::string(buffer.begin(), buffer.end());
		}
	};
}