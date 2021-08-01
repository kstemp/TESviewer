#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

struct BinaryStreamReader {
	BinaryStreamReader(const std::vector<char>& buffer) : buffer(buffer) {}

	BinaryStreamReader(std::string fileName) {
		std::ifstream is;
		is.open(fileName, std::ios::binary | std::ios::ate);

		fileSize = is.tellg();
		is.seekg(0, std::ios::beg);

		buffer.resize(fileSize / sizeof(char));

		is.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	}

	template<typename T>
	BinaryStreamReader& operator >> (T& t) {
		readIntoVar(t);
		return *this;
	}

	const std::streampos tellg() const {
		return pos;
	}

	void skip(const std::streamsize count) {
		pos += count;
	}

	template<typename T>
	T readVar() {
		T out;
		operator>>(out);
		return out;
	}

	std::string readString(std::size_t len) {
		std::string out(len, ' ');
		readIntoVar(out[0], len);
		return out;
	}

	void expect(const std::string& expected) {
		if (fileSize - pos < expected.length())
			throw std::runtime_error("\"" + expected + "\" expected at position " + std::to_string((size_t)pos - expected.length()) + ", but end-of-file reached");

		const std::string actual = readString(expected.length());
		if (actual != expected)
			throw std::runtime_error("\"" + expected + "\" expected at position " + std::to_string((size_t)pos - expected.length()) + ", but got \"" + actual + "\".");
	}

	template<typename T>
	inline void readIntoVar(T& t, size_t size = sizeof(T)) {
		memcpy(&t, &buffer[pos], size);
		pos += size;
	}

	inline void readIntoVectorBuf(std::vector<char>& buf, size_t dataSize) {
		buf.resize(dataSize);
		readIntoVar(buf[0], dataSize);
	}

	std::streampos pos = 0;
	std::streampos fileSize;

	std::vector<char> buffer;
};

struct BinaryStreamWriter {
	BinaryStreamWriter(const std::string& fileName) {
		os.open(fileName, std::ios::binary);
	}

	template<typename T>
	BinaryStreamWriter& operator << (T t) {
		os.write(reinterpret_cast<const char*>(&t), sizeof(T));
		return *this;
	}

	BinaryStreamWriter& operator << (const std::string t) {
		os.write(t.c_str(), t.size());
		return *this;
	}

	template<typename T>
	void writeField(const std::string& name, T val) {
		this->operator<<(name);
		uint16_t siz = sizeof(val);
		this->operator<<(siz);
		this->operator<<(val);
	}

	void writeField(const std::string& name, const std::string& val) {
		this->operator<<(name);
		uint16_t siz = val.length();
		this->operator<<(siz);
		this->operator<<(val);
	}

	std::ofstream os;
};