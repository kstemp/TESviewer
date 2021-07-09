#pragma once

#include <memory>

#include "Header.h"
#include "NiObject.h"
#include "BSTriShape.h"
#include "BSFadeNode.h"
#include "BSLightingShaderProperty.h"
#include "BSShaderTextureSet.h"

struct NiFile {

    NiHeader header;

    std::vector<std::unique_ptr<NiObject>> objects;

    const std::string _fileName;

    NiFile(const std::string& fileName) : _fileName(fileName) {}

    void parse() {

        BinaryStreamReader bsr(_fileName);

        header.parse(bsr);

        objects.resize(header.numBlocks);
        for (int i = 0; i < header.numBlocks; ++i) {

            std::streampos posBefore = bsr.tellg();

            objects[i] = createBlockByName(header.blockTypes[header.blockTypeIndices[i]]);
            if (objects[i])
                objects[i]->parse(bsr);

            // TODO this is mostly a debug check - verify that the respective ::parse() method 
            // went through the entire size of the i-th block as specified by blockSize[i]
            if (bsr.tellg() - posBefore != header.blockSize[i]){
               // throw;
               bsr.skip(header.blockSize[i] - (bsr.tellg() - posBefore));
            }

        }

    }

    ~NiFile() {
#ifdef _DEBUG
        OutputDebugStringA(std::string("DESTROYING NIFILE").c_str());
#endif
    }

    std::unique_ptr<NiObject> createBlockByName(const std::string& name) {

        if (name == "BSTriShape")
            return std::make_unique<BSTriShape>();
        else if (name == "BSShaderTextureSet")
            return std::make_unique<BSShaderTextureSet>();
        else if (name == "BSFadeNode")
            return std::make_unique<BSFadeNode>();
        else if (name == "BSLightingShaderProperty")
            return std::make_unique<BSLightingShaderProperty>();
        else if (name == "NiNode")
            return std::make_unique<NiNode>();

        return nullptr;

    }

    int getParentNodeIndex(const int blockIndex) const {
        
        // TODO check if blockIndex is within bounds

        for (int i = 0; i < objects.size(); ++i) {
            auto node = dynamic_cast<NiNode*>(objects[i].get());
            if (node)
                for (int j = 0; j < node->numChildren; ++j)
                    if (blockIndex == node->children[j])
                        return i;
        }

        return -1;

    }

    Vector3 getAVObjectCompoundTranslation(const int objIndex) const {

        auto AVObject = dynamic_cast<NiAVObject*>(objects[objIndex].get());

        Vector3 translation = AVObject->translation;

        int parent = getParentNodeIndex(objIndex);
        while (parent >= 0) {
            auto node = dynamic_cast<NiNode*>(objects[parent].get());
            translation += node->translation;

            parent = getParentNodeIndex(parent);
        }

        return translation;

    }

};