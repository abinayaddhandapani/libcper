#ifndef CPER_SECTION_PCIE_H
#define CPER_SECTION_PCIE_H

#include "json.h"
#include "../edk/Cper.h"

#define PCIE_ERROR_VALID_BITFIELD_NAMES (const char*[]) {"portTypeValid", "versionValid", "commandStatusValid", \
    "deviceIDValid", "deviceSerialNumberValid", "bridgeControlStatusValid", "capabilityStructureStatusValid", \
    "aerInfoValid"}
#define PCIE_ERROR_PORT_TYPES_KEYS (int []){0, 1, 4, 5, 6, 7, 8, 9, 10}
#define PCIE_ERROR_PORT_TYPES_VALUES (const char*[]){"PCI Express End Point", "Legacy PCI End Point Device", \
    "Root Port", "Upstream Switch Port", "Downstream Switch Port", "PCI Express to PCI/PCI-X Bridge", \
    "PCI/PCI-X Bridge to PCI Express Bridge", "Root Complex Integrated Endpoint Device", "Root Complex Event Collector"}

json_object* cper_section_pcie_to_ir(void* section, EFI_ERROR_SECTION_DESCRIPTOR* descriptor);
void ir_section_pcie_to_cper(json_object* section, FILE* out);

#endif