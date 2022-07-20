/**
 * Functions for generating psuedo-random CCIX PER error sections.
 * 
 * Author: Lawrence.Tang@arm.com
 **/

#include <stdlib.h>
#include "../../edk/BaseTypes.h"
#include "../gen-utils.h"
#include "gen-sections.h"

//Generates a single psuedo-random CCIX PER error section, saving the resulting address to the given
//location. Returns the size of the newly created section.
size_t generate_section_ccix_per(void** location)
{
    //Create a random length for the CCIX PER log.
    //The log attached here does not necessarily conform to the CCIX specification, and is simply random.
    int log_len = (rand() % 5 + 1) * 32;

    //Create random bytes.
    int size = 16 + log_len;
    UINT8* bytes = generate_random_bytes(size);
    
    //Set reserved areas to zero.
    UINT64* validation = (UINT64*)(bytes + 4);
    *validation &= 0b111; //Validation bits 3-63.
    *(bytes + 13) &= 0b11111; //CCIX port ID bits 5-7.
    UINT16* reserved = (UINT16*)(bytes + 14);
    *reserved = 0; //Reserved bytes 14-15.

    //Set expected values.
    UINT32* length = (UINT32*)bytes;
    *length = size;

    //Set return values, exit.
    *location = bytes;
    return size;
}