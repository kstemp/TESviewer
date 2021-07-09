#include "NiTypes.h"

std::string readSizedString(BinaryStreamReader& bsr) {

    return bsr.readString(bsr.readVar<uint>());

}