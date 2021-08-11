#pragma once

#include <iomanip>
#include <sstream>
#include <fstream>

#include <QtZlib\zlib.h>

#include <esm\Record.h>

template<typename T>
QString NumToHexStr(const T& num) {
	std::stringstream sstream;
	sstream << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << num << std::dec;
	return QString::fromStdString(sstream.str());
}

inline bool decompressVectorBuf(
	std::vector<char>& decompressedBuf,
	const uint32_t decompressedSize,
	const std::vector<char>& compressedBuf,
	const uint32_t compressedSize) {
	decompressedBuf.resize(decompressedSize);

	int uncompressValue = uncompress(
		(z_Bytef*)decompressedBuf.data(),
		(z_uLongf*)&decompressedSize,
		(z_Bytef*)compressedBuf.data(),
		(z_uLong)compressedSize);

	return (uncompressValue == Z_OK);
}