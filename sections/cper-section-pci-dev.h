#ifndef CPER_SECTION_PCI_DEV_H
#define CPER_SECTION_PCI_DEV_H

#include "json.h"
#include "../edk/Cper.h"

#define PCI_DEV_ERROR_VALID_BITFIELD_NAMES (const char*[]) {"errorStatusValid", "idInfoValid", "memoryNumberValid", \
    "ioNumberValid", "registerDataPairValid"}

///
/// PCI/PCI-X Device Error Section
///
typedef struct {
  UINT64 VendorId : 2;
  UINT64 DeviceId : 2;
  UINT64 ClassCode : 3;
  UINT64 FunctionNumber : 1;
  UINT64 DeviceNumber : 1;
  UINT64 BusNumber : 1;
  UINT64 SegmentNumber : 1;
  UINT64 Reserved : 5;
} EFI_PCI_PCIX_DEVICE_ID_INFO;

typedef struct {
  UINT64                      ValidFields;
  EFI_GENERIC_ERROR_STATUS    ErrorStatus;
  EFI_PCI_PCIX_DEVICE_ID_INFO IdInfo;
  UINT32                      MemoryNumber;
  UINT32                      IoNumber;
} EFI_PCI_PCIX_DEVICE_ERROR_DATA;

json_object* cper_section_pci_dev_to_ir(void* section, EFI_ERROR_SECTION_DESCRIPTOR* descriptor);

#endif