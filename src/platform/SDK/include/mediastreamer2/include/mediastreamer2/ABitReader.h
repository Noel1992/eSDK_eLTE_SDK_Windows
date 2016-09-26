#ifndef abitreader_h
#define abitreader_h

#include "mediastreamer2/mscommon.h"
#include "mediastreamer2/msqueue.h"

#ifdef __cplusplus
extern "C"{
#endif

struct ABitReader {
    ABitReader(const uint8_t *data, size_t size);

    uint32_t getBits(size_t n);
    void skipBits(size_t n);

    void putBits(uint32_t x, size_t n);

    size_t numBitsLeft() const;

    const uint8_t *data() const;

private:
    const uint8_t *mData;
    size_t mSize;

    uint32_t mReservoir;  // left-aligned bits
    size_t mNumBitsLeft;

    void fillReservoir();
};

#ifdef __cplusplus
}
#endif

#endif