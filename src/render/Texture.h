#pragma once

#define DDSKTX_IMPLEMENT
#include <lib\dds-ktx\dds-ktx.h>

#include <qopenglfunctions.h>

unsigned int loadDDSTexture(QOpenGLFunctions* fs, const std::string& fileName) {

    // TODO honestly not sure if this is a good idea. We essentially copy from vector to vector here, and gain nothing, since
    // we'd read the entire file anyway, so BSR acts as a pointless wrapper here.
    BinaryStreamReader bsr(fileName);

    std::vector<char> buffer;
    bsr.readIntoVectorBuf(buffer, bsr.fileSize);

    ddsktx_texture_info tc = { 0 };

    if (!ddsktx_parse(&tc, buffer.data(), buffer.size(), NULL))
        throw "Failed to parse DDS texture data.";

    unsigned int texture;

    fs->glGenTextures(1, &texture);
    fs->glBindTexture(GL_TEXTURE_2D, texture);

    fs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    fs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    fs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    fs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //for (int mip = 0; mip < tc.num_mips; ++mip) {
    ddsktx_sub_data sub_data;
    ddsktx_get_sub(&tc, &sub_data, buffer.data(), buffer.size(), 0, 0, 0/*mip*/);

    if (ddsktx_format_compressed(tc.format)) {

        GLenum internalFormat;
        switch (tc.format) {
        case ddsktx_format::DDSKTX_FORMAT_BC1:
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case ddsktx_format::DDSKTX_FORMAT_BC3:
            internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            throw "Unsupported!";
        }

        fs->glCompressedTexImage2D(
            GL_TEXTURE_2D, 
            /*mip*/ 0, 
            /*tc.format*/  internalFormat, 
            sub_data.width, 
            sub_data.height, 
            0, 
            sub_data.size_bytes, 
            sub_data.buff
        );

    } else {
        //glTexImage2D();
        throw "IMPLEMENT THIS!";
       // break;
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        throw "TODO error handling!!";
    }

    // }
        // TODO we should load mipmaps directly from DDS file, as outlined above
       // glGenerateMipmap(GL_TEXTURE_2D);

        // TODO add texture HEREEEEEEEEEEEEEEE!

    return texture;

}
