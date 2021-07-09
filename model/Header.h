#pragma once

#include <vector>
#include <string>

#include "..\io\BinaryIO.h"
#include "NiTypes.h"

struct BSStreamHeader {

    ulittle32 bsVersion;
    std::string author;
    std::string processScript;
    std::string exportScript;

    void parse(BinaryStreamReader& bsr) {

        bsr >> bsVersion;
        author = bsr.readString(bsr.readVar<byte>());
        processScript = bsr.readString(bsr.readVar<byte>()); // TODO readSizedString?
        exportScript = bsr.readString(bsr.readVar<byte>());

    }

};

struct NiHeader {

    NiVersion version;

    ulittle32 numBlocks;

    BSStreamHeader bethesdaHeader;

    ushort numBlockTypes;

    std::vector<std::string> blockTypes;
    std::vector<BlockTypeIndex> blockTypeIndices;
    std::vector<uint> blockSize;

    uint numStrings;
    uint maxStringLen;
    std::vector<std::string> strings;

    uint numGroups;

    void parse(BinaryStreamReader& bsr) {

        const std::string NIFHEADER = "Gamebryo File Format, Version ";

        std::string header = bsr.readString(NIFHEADER.length());

        if (header.compare(NIFHEADER) != 0)
            throw;

        // TODO we know the version number starts with 2 digits.
        version.parse(bsr);

        // skip over 0x0A newline char, Version, endian type and user version
        bsr.skip(1 + 4 + 1 + 4);

        bsr >> numBlocks;

        bethesdaHeader.parse(bsr);

        bsr >> numBlockTypes;

        blockTypes.resize(numBlockTypes);
        for (int i = 0; i < numBlockTypes; ++i)
            blockTypes[i] = readSizedString(bsr);

        blockTypeIndices.resize(numBlocks);
        for (int i = 0; i < numBlocks; ++i)
            bsr >> blockTypeIndices[i];

        blockSize.resize(numBlocks);
        for (int i = 0; i < numBlocks; ++i)
            bsr >> blockSize[i];

        bsr >> numStrings;
        bsr >> maxStringLen;

        for (int i = 0; i < numStrings; ++i)
            strings.push_back(readSizedString(bsr));

        bsr >> numGroups;

        for (int i = 0; i < numGroups; ++i) { /* TODO implement */ }


    }

};