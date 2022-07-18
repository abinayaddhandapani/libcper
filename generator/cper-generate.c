/**
 * A user-space application for generating psuedo-random specification compliant CPER records. 
 * 
 * Author: Lawrence.Tang@arm.com
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../edk/Cper.h"
#include "gen-utils.h"
#include "sections/gen-section-generic.h"
#include "sections/gen-section-ia32x64.h"

EFI_ERROR_SECTION_DESCRIPTOR* generate_section_descriptor(char* type, size_t* lengths, int index);
size_t generate_section(void** location, char* type);
void print_help();

int main(int argc, char* argv[])
{
    //If help requested, print help.
    if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        print_help();
        return 0;
    }

    //Ensure the minimum number of arguments.
    if (argc < 5)
    {
        printf("Insufficient number of arguments. See 'cper-generate --help' for command information.\n");
        return -1;
    }

    //Initialise randomiser.
    init_random();

    //Generate the sections. Type names start from argv[4].
    UINT16 num_sections = argc - 4;
    void* sections[num_sections];
    size_t section_lengths[num_sections];
    for (int i=0; i<num_sections; i++) 
    {
        section_lengths[i] = generate_section(sections + i, argv[4 + i]);
        if (section_lengths[i] == 0)
        {
            //Error encountered, exit.
            return -1;
        }
    }

    //Generate the header given the number of sections.
    EFI_COMMON_ERROR_RECORD_HEADER* header = 
        (EFI_COMMON_ERROR_RECORD_HEADER*)calloc(1, sizeof(EFI_COMMON_ERROR_RECORD_HEADER));
    header->SignatureStart = 0x52455043; //CPER
    header->SectionCount = num_sections;
    printf("%d sections\n", num_sections);
    header->SignatureEnd = 0xFFFFFFFF;
    header->Flags = 4; //HW_ERROR_FLAGS_SIMULATED

    //Generate the section descriptors given the number of sections.
    EFI_ERROR_SECTION_DESCRIPTOR* section_descriptors[num_sections];
    for (int i=0; i<num_sections; i++)
        section_descriptors[i] = generate_section_descriptor(argv[4 + i], section_lengths, i);

    //Calculate total length of structure, set in header.
    size_t total_len = sizeof(header);
    for (int i=0; i<num_sections; i++)
        total_len += section_lengths[i];
    total_len += num_sections * sizeof(EFI_ERROR_SECTION_DESCRIPTOR);
    header->RecordLength = (UINT32)total_len;

    //Open a file handle to write output.
    FILE* cper_file = fopen(argv[2], "w");
    if (cper_file == NULL) 
    {
        printf("Could not get a handle for output file '%s', file handle returned null.\n", argv[2]);
        return -1;
    }

    //Write to file in order, free all resources.
    fwrite(header, sizeof(EFI_COMMON_ERROR_RECORD_HEADER), 1, cper_file);
    fflush(cper_file);
    free(header);
    for (int i=0; i<num_sections; i++)
    {
        fwrite(section_descriptors[i], sizeof(EFI_ERROR_SECTION_DESCRIPTOR), 1, cper_file);
        fflush(cper_file);
        free(section_descriptors[i]);
    }
    for (int i=0; i<num_sections; i++) 
    {
        fwrite(sections[i], section_lengths[i], 1, cper_file);
        fflush(cper_file);
        free(sections[i]);
    }
    fclose(cper_file);
}

//Generates a single section descriptor for a section with the given properties.
EFI_ERROR_SECTION_DESCRIPTOR* generate_section_descriptor(char* type, size_t* lengths, int index)
{
    EFI_ERROR_SECTION_DESCRIPTOR* descriptor = 
        (EFI_ERROR_SECTION_DESCRIPTOR*)generate_random_bytes(sizeof(EFI_ERROR_SECTION_DESCRIPTOR));

    //Set reserved bits to zero.
    descriptor->SecValidMask &= 0b11;
    descriptor->Resv1 = 0;
    descriptor->SectionFlags &= 0xFF;

    //Set length, offset from base record.
    descriptor->SectionLength = (UINT32)lengths[index];
    descriptor->SectionOffset = sizeof(EFI_COMMON_ERROR_RECORD_HEADER);
    for (int i=0; i<index; i++)
        descriptor->SectionOffset += lengths[i];

    //Set section type GUID based on type name.
    if (strcmp(type, "generic") == 0)
        memcpy(&descriptor->SectionType, &gEfiProcessorGenericErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "ia32x64") == 0)
        memcpy(&descriptor->SectionType, &gEfiIa32X64ProcessorErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "ipf") == 0)
        memcpy(&descriptor->SectionType, &gEfiIpfProcessorErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "arm") == 0)
        memcpy(&descriptor->SectionType, &gEfiArmProcessorErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "memory") == 0)
        memcpy(&descriptor->SectionType, &gEfiPlatformMemoryErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "memory2") == 0)
        memcpy(&descriptor->SectionType, &gEfiPlatformMemoryError2SectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "pcie") == 0)
        memcpy(&descriptor->SectionType, &gEfiPcieErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "firmware") == 0)
        memcpy(&descriptor->SectionType, &gEfiFirmwareErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "pcibus") == 0)
        memcpy(&descriptor->SectionType, &gEfiPciBusErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "pcidev") == 0)
        memcpy(&descriptor->SectionType, &gEfiPciDevErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "dmargeneric") == 0)
        memcpy(&descriptor->SectionType, &gEfiDMArGenericErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "dmarvtd") == 0)
        memcpy(&descriptor->SectionType, &gEfiDirectedIoDMArErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "dmariommu") == 0)
        memcpy(&descriptor->SectionType, &gEfiIommuDMArErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "ccixper") == 0)
        memcpy(&descriptor->SectionType, &gEfiCcixPerLogErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "cxlprotocol") == 0)
        memcpy(&descriptor->SectionType, &gEfiCxlProtocolErrorSectionGuid, sizeof(EFI_GUID));
    else if (strcmp(type, "cxlcomponent") == 0)
    {
        //Choose between the different CXL component type GUIDs.
        int componentType = rand() % 5;
        switch (componentType)
        {
            case 0:
                memcpy(&descriptor->SectionType, &gEfiCxlGeneralMediaErrorSectionGuid, sizeof(EFI_GUID));
                break;
            case 1:
                memcpy(&descriptor->SectionType, &gEfiCxlDramEventErrorSectionGuid, sizeof(EFI_GUID));
                break;
            case 2:
                memcpy(&descriptor->SectionType, &gEfiCxlPhysicalSwitchErrorSectionGuid, sizeof(EFI_GUID));
                break;
            case 3:
                memcpy(&descriptor->SectionType, &gEfiCxlVirtualSwitchErrorSectionGuid, sizeof(EFI_GUID));
                break;
            default:
                memcpy(&descriptor->SectionType, &gEfiCxlMldPortErrorSectionGuid, sizeof(EFI_GUID));
                break;
        }
    }
    else if (strcmp(type, "unknown") != 0)
    {
        //Undefined section, show error.
        printf("Undefined section type '%s' provided. See 'cper-generate --help' for command information.\n", type);
        return 0;
    }

    return descriptor;
}

//Generates a single CPER section given the string type.
size_t generate_section(void** location, char* type)
{
    //The length of the section.
    size_t length = 0;

    //Switch on the type, generate accordingly.
    if (strcmp(type, "generic") == 0)
        length = generate_section_generic(location);
    else if (strcmp(type, "ia32x64") == 0)
        length = generate_section_ia32x64(location);
    // else if (strcmp(type, "ipf") == 0)
    //     length = generate_section_ipf(location);
    // else if (strcmp(type, "arm") == 0)
    //     length = generate_section_arm(location);
    // else if (strcmp(type, "memory") == 0)
    //     length = generate_section_memory(location);
    // else if (strcmp(type, "memory2") == 0)
    //     length = generate_section_memory2(location);
    // else if (strcmp(type, "pcie") == 0)
    //     length = generate_section_pcie(location);
    // else if (strcmp(type, "firmware") == 0)
    //     length = generate_section_firmware(location);
    // else if (strcmp(type, "pcibus") == 0)
    //     length = generate_section_pci_bus(location);
    // else if (strcmp(type, "pcidev") == 0)
    //     length = generate_section_pci_dev(location);
    // else if (strcmp(type, "dmargeneric") == 0)
    //     length = generate_section_dmar_generic(location);
    // else if (strcmp(type, "dmarvtd") == 0)
    //     length = generate_section_dmar_vtd(location);
    // else if (strcmp(type, "dmariommu") == 0)
    //     length = generate_section_dmar_iommu(location);
    // else if (strcmp(type, "ccixper") == 0)
    //     length = generate_section_ccix_per(location);
    // else if (strcmp(type, "cxlprotocol") == 0)
    //     length = generate_section_cxl_protocol(location);
    // else if (strcmp(type, "cxlcomponent") == 0)
    //     length = generate_section_cxl_component(location);
    // else if (strcmp(type, "unknown") == 0)
    //     length = generate_section_unknown(location);
    else 
    {
        //Undefined section, show error.
        printf("Undefined section type '%s' given to generate. See 'cper-generate --help' for command information.\n", type);
        return 0;
    }

    return length;
}

//Prints command help for this CPER generator.
void print_help()
{
    printf(":: --out cper.file --sections section1 [section2 section3 ...]\n");
    printf("\tGenerates a psuedo-random CPER file with the provided section types and outputs to the given file name.\n");
    printf("\tValid section type names are the following:\n");
    printf("\t\t- generic\n");
    printf("\t\t- ia32x64\n");
    printf("\t\t- ipf\n");
    printf("\t\t- arm\n");
    printf("\t\t- memory\n");
    printf("\t\t- memory2\n");
    printf("\t\t- pcie\n");
    printf("\t\t- firmware\n");
    printf("\t\t- pcibus\n");
    printf("\t\t- pcidev\n");
    printf("\t\t- dmargeneric\n");
    printf("\t\t- dmarvtd\n");
    printf("\t\t- dmariommu\n");
    printf("\t\t- ccixper\n");
    printf("\t\t- cxlprotocol\n");
    printf("\t\t- cxlcomponent\n");
    printf("\t\t- unknown\n");
    printf("\n:: --help\n");
    printf("\tDisplays help information to the console.\n");
}