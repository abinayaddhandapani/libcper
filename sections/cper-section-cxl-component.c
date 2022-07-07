/**
 * Describes functions for converting CXL component error CPER sections from binary and JSON format
 * into an intermediate format.
 * 
 * Author: Lawrence.Tang@arm.com
 **/
#include <stdio.h>
#include "json.h"
#include "b64.h"
#include "../edk/Cper.h"
#include "../cper-utils.h"
#include "cper-section-cxl-component.h"

//Converts a single CXL component error CPER section into JSON IR.
json_object* cper_section_cxl_component_to_ir(void* section, EFI_ERROR_SECTION_DESCRIPTOR* descriptor)
{
    EFI_CXL_COMPONENT_EVENT_HEADER* cxl_error = (EFI_CXL_COMPONENT_EVENT_HEADER*)section;
    json_object* section_ir = json_object_new_object();

    //Length (bytes) for the entire structure.
    json_object_object_add(section_ir, "length", json_object_new_uint64(cxl_error->Length));
    
    //Validation bits.
    json_object* validation = bitfield_to_ir(cxl_error->ValidBits, 3, CXL_COMPONENT_ERROR_VALID_BITFIELD_NAMES);
    json_object_object_add(section_ir, "validationBits", validation);

    //Device ID.
    json_object* device_id = json_object_new_object();
    json_object_object_add(device_id, "vendorID", json_object_new_int(cxl_error->DeviceId.VendorId));
    json_object_object_add(device_id, "deviceID", json_object_new_int(cxl_error->DeviceId.DeviceId));
    json_object_object_add(device_id, "functionNumber", json_object_new_int(cxl_error->DeviceId.FunctionNumber));
    json_object_object_add(device_id, "deviceNumber", json_object_new_int(cxl_error->DeviceId.DeviceNumber));
    json_object_object_add(device_id, "busNumber", json_object_new_int(cxl_error->DeviceId.BusNumber));
    json_object_object_add(device_id, "segmentNumber", json_object_new_int(cxl_error->DeviceId.SegmentNumber));
    json_object_object_add(device_id, "slotNumber", json_object_new_int(cxl_error->DeviceId.SlotNumber));
    json_object_object_add(section_ir, "deviceID", device_id);

    //Device serial.
    json_object_object_add(section_ir, "deviceSerial", json_object_new_uint64(cxl_error->DeviceSerial));

    //The specification for this is defined within the CXL Specification Section 8.2.9.1.
    unsigned char* cur_pos = (unsigned char*)(cxl_error + 1);
    int remaining_len = section - (void*)cur_pos + cxl_error->Length;
    if (remaining_len > 0)
    {
        json_object* event_log = json_object_new_object();
        char* encoded = b64_encode(cur_pos, remaining_len);
        json_object_object_add(event_log, "data", json_object_new_string(encoded));
        free(encoded);
        json_object_object_add(section_ir, "cxlComponentEventLog", event_log);
    }

    return section_ir;
}