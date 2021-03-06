#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef _AES_FUNCTIONS_H_
#define _AES_FUNCTIONS_H_


//#include "pkcs7_padding.c" // create object file and link statically
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "pkcs7_padding.h"

typedef struct 
{
    uint8_t  AES_KEY[16];
    uint8_t AES_IV[16];

}AES_parameters;

void generate_key(uint8_t key_array[16]);
void generate_iv (uint8_t iv_array[16]);
void initialize_AES_parameters(AES_parameters* new_parameters);
void print_hex_array (uint8_t * input_ptr, int buffer_size);


#endif 
