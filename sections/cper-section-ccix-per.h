#ifndef CPER_SECTION_CCIX_PER_H
#define CPER_SECTION_CCIX_PER_H

#include <json.h>
#include "../edk/Cper.h"

#define CCIX_PER_ERROR_VALID_BITFIELD_NAMES (const char*[]) {"ccixSourceIDValid", "ccixPortIDValid", "ccixPERLogValid"}

///
/// CCIX PER Log Error Section
///
typedef struct {
    UINT32 Length;
    UINT64 ValidBits;
    UINT8 CcixSourceId;
    UINT8 CcixPortId;
    UINT16 Reserved;
} __attribute__((packed, aligned(1))) EFI_CCIX_PER_LOG_DATA;

json_object* cper_section_ccix_per_to_ir(void* section, EFI_ERROR_SECTION_DESCRIPTOR* descriptor);
void ir_section_ccix_per_to_cper(json_object* section, FILE* out);

#endif