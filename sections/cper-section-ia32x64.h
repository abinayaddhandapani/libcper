#ifndef CPER_SECTION_IA32X64_H
#define CPER_SECTION_IA32X64_H

#include "json.h"
#include "../edk/Cper.h"

#define IA32X64_PROCESSOR_ERROR_VALID_BITFIELD_NAMES (const char*[]) \
    {"checkInfoValid", "targetAddressIdentifierValid", "requestorIdentifierValid", "responderIdentifierValid", \
    "instructionPointerValid"}
#define IA32X64_CHECK_INFO_VALID_BITFIELD_NAMES (const char*[]) \
    {"transactionTypeValid", "operationValid", "levelValid", "processorContextCorruptValid", "uncorrectedValid" \
    "preciseIPValid", "restartableIPValid", "overflowValid", "participationTypeValid", "timeOutValid" \
    "addressSpaceValid"}
#define IA32X64_CHECK_INFO_TRANSACTION_TYPES_KEYS (int []){0, 1, 2}
#define IA32X64_CHECK_INFO_TRANSACTION_TYPES_VALUES (const char*[]){"Instruction", "Data Access", "Generic"}
#define IA32X64_CHECK_INFO_OPERATION_TYPES_KEYS (int []){0, 1, 2, 3, 4, 5, 6, 7, 8}
#define IA32X64_CHECK_INFO_OPERATION_TYPES_VALUES (const char*[]){"Generic Error", "Generic Read", "Generic Write" \
    "Data Read", "Data Write", "Instruction Fetch", "Prefetch", "Eviction", "Snoop"}
#define IA32X64_BUS_CHECK_INFO_PARTICIPATION_TYPES_KEYS (int []){0, 1, 2, 3}
#define IA32X64_BUS_CHECK_INFO_PARTICIPATION_TYPES_VALUES (const char*[]){"Local processor originated request", \
    "Local processor responded to request", "Local processor observed", "Generic"}
#define IA32X64_BUS_CHECK_INFO_ADDRESS_SPACE_TYPES_KEYS (int []){0, 1, 2, 3}
#define IA32X64_BUS_CHECK_INFO_ADDRESS_SPACE_TYPES_VALUES (const char*[]){"Memory Access", "Reserved", "I/O", \
    "Other Transaction"}
#define IA32X64_MS_CHECK_INFO_ERROR_TYPES_KEYS (int []){0, 1, 2, 3, 4, 5}
#define IA32X64_MS_CHECK_INFO_ERROR_TYPES_VALUES (const char*[]){"No Error", "Unclassified", "Microcode ROM Parity Error", \
    "External Error", "FRC Error", "Internal Unclassified"}
#define IA32X64_REGISTER_CONTEXT_TYPES_KEYS (int []){0, 1, 2, 3, 4, 5, 6, 7}
#define IA32X64_REGISTER_CONTEXT_TYPES_VALUES (const char*[]){"Unclassified Data", "MSR Registers", \
    "32-bit Mode Execution Context", "64-bit Mode Execution Context", "FXSave Context", \
    "32-bit Mode Debug Registers", "64-bit Mode Debug Registers", "Memory Mapper Registers"}

json_object* cper_section_ia32x64_to_ir(void* section, EFI_ERROR_SECTION_DESCRIPTOR* descriptor);

#endif