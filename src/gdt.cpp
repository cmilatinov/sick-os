#include "gtd.h"

// SegmentDescriptor Class

GDT::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags){

    // Byte pointer to segment descriptor.
    uint8_t * ptr = reinterpret_cast<uint8_t *>(this);

    if(limit <= UINT16_MAX)

        // Use 16-bit address space for limit.
        ptr[6] = 0b01000000;

    else{

        // Use 32-bit address space for limit.
        // 12 least significant bits are implicitly set to 1.
        ptr[6] = 0b11000000;

        // Discard the 12 least significant bits.
        // Compensate by subtracting 1 from the shifted limit if those 12 bits aren't all ones.
        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        
    }

    // Encode the limit.
    reinterpret_cast<uint16_t *>(this)[0] = limit & 0xFFFF;
    ptr[6] |= (limit >> 16) & 0xF;

    // Encode the base.
    reinterpret_cast<uint16_t *>(this)[1] = base & 0xFFFF;
    ptr[4] = (base >> 16) & 0xFF;
    ptr[7] = (base >> 24) & 0xFF;

    // Encode the flags.
    ptr[5] = flags;
}

GDT::SegmentDescriptor::~SegmentDescriptor(){};

uint32_t GDT::SegmentDescriptor::Base(){

    // Byte pointer to segment descriptor.
    uint8_t * ptr = reinterpret_cast<uint8_t *>(this);

    // Decode the base.
    uint32_t result = ptr[7];
    result = (result << 8) | ptr[4];
    result = (result << 16) | reinterpret_cast<uint16_t *>(this)[1];

    // Return the result.
    return result;
}

uint32_t GDT::SegmentDescriptor::Limit(){
    
    // Byte pointer to segment descriptor.
    uint8_t * ptr = reinterpret_cast<uint8_t *>(this);

    // Decode the limit.
    uint32_t result = ptr[6] & 0xF;
    result = (result << 16) | reinterpret_cast<uint16_t *>(this)[0];

    // Add the implicit bits if necessary.
    if(ptr[6] & 0b10000000)
        result = (result << 12) | 0xFFF;

    // Return the result.
    return result;
}

// GDT Class

GDT::GDT(): 
    null(0, 0, 0),
    unused(0, 0, 0),
    code(0, 64 * 1024 * 1024, 0b10011010),
    data(0, 64 * 1024 * 1024, 0b10010010){

    // 16 Bit GDT size.
    uint64_t value = (sizeof(GDT) - 1) << 48;

    // 32 Bit pointer to the GDT.
    value |= reinterpret_cast<uint32_t>(this) << 16;

    // Load the GDT in assembly.
    asm volatile(
        "lgdt (%0)" : 
        : "p"(&value) );
};

GDT::~GDT(){};

uint16_t GDT::DataSegment(){
    return reinterpret_cast<BYTE*>(&data) - reinterpret_cast<BYTE*>(this);
}

uint16_t GDT::CodeSegment(){
    return reinterpret_cast<BYTE*>(&code) - reinterpret_cast<BYTE*>(this);
}