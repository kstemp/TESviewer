#pragma once

#include <string>

namespace ESM {
	enum class ValueType {
		Float,
		uInt32,
		uInt16,
		String
	};

	struct Value {
		const ValueType type() const {
			return _type;
		}

		~Value() {
			_cleanup();
		}

		Value& operator =(const float val) {
			_cleanup();

			_type = ValueType::Float;
			_val.float_ = val;
		}

		Value& operator =(const uint32_t val) {
			_cleanup();

			_type = ValueType::uInt32;
			_val.uint32 = val;
		}

		Value& operator =(const std::string val) {
			_cleanup();

			_type = ValueType::String;
			_val.data_ = new std::string(val);
		}

		float Float(const float def = 0.f) const {
			return _type == ValueType::Float ? _val.float_ : def;
		}

		float Uint32(const uint32_t def = 0) const {
			return _type == ValueType::uInt32 ? _val.uint32 : def;
		}

		std::string String(const std::string def = "") const {
			return _type == ValueType::String ? *(std::string*)(_val.data_) : def;
		}

	private:

		ValueType _type;

		union Val {
			uint16_t uint16;
			uint32_t uint32;
			float float_;
			void* data_;
		};

		Val _val;

		void _cleanup() {
			if (_type == ValueType::String)
				delete (std::string*)(_val.data_);
		}
	};
}