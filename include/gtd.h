#include "types.h"

#pragma once

class GDT {

    public:
    class SegmentDescriptor {
        
        private:
        // Low bytes of limit.
        uint16_t lLimit;

        // Low bytes of base.
        uint16_t lBase;

        // Mid byte of base.
        uint8_t mBase;

        // Access rights byte.
        uint8_t access;
        
        // Limit high 4 bits and 4 bits for flags.
        uint8_t hLimit;

        // High bytes of base.
        uint8_t hBase;

        public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags);
        ~SegmentDescriptor();

        // Getters.
        uint32_t Base();
        uint32_t Limit();

    } __attribute__((packed)); // GCC Compiler Directive - Pack the class so the compiler doesn't add fucking useless padding bytes.

    private:
    SegmentDescriptor null;
    SegmentDescriptor unused;
    SegmentDescriptor code;
    SegmentDescriptor data;

    public:
    GDT();
    ~GDT();

    uint16_t CodeSegment();
    uint16_t DataSegment();

};