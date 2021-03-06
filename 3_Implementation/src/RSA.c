#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h> // include header 
#include <stddef.h>
#include "RSA.h"
#include "aes.h"
#include "AES_functions.h"



/**
 * @file RSA.c
 * @author adityasaggar2911@gmail.com
 * @brief This file defines the structures to hold private and public keys for RSA. and to randomly generate key and other related functions
 * @version 0.1
 * @date 2021-07-25
 * 
 * @copyright Copyright (c) 2021
 * 
 * 
 * */

/** 
 * @brief Encrypts input message based on RSA public key 
 * @param[in] message pointer to input message structure
 * @param[in] public key  pointer
 * @param[out] Encrypted message is now available in input message 
 * @return void 
 * */
void block_encrypt_RSA(message* input_message, public_key_RSA* pub_k)// before inputtinh to function typecast C & M to mpz_t 
{
    /* C = M^e mod n */
    mpz_powm(input_message->C, input_message->M, pub_k->e, pub_k->n); 
    return;
}

/** 
 * @brief Decrypts input cipher message based on RSA private key 
 * @param[in] message pointer to input message structure 
 * @param[in] private key  pointer
 * @param[out] Decrypted message is now available in input message 
 * @return void 
 * */
void block_decrypt_RSA(message* input_message,  private_key_RSA *pri_k)
{
    mpz_powm(input_message->M, input_message->C, pri_k->d, pri_k->n); 
    return;
}

/** 
 * @brief Generates randomized AES key
 * @param[in] AES Key array 
 * @param[out] Randomized Key Array
 * @return void 
 * */
void generate_AES_key(uint8_t key_array[16]) //sequence to be encrypted in this test
{
memset( key_array, 0, 16 );
	
	for (int i =0; i<16; i++)
	{
		key_array[i] = (uint8_t)rand()%256;

	}

}
/** 
 * @brief generates random 1024 bit prime numbers
 * @param[in] pointer to initialized mpz_t 
 * @param[out] mpz_t set to 1024 bit prime value
 * @return void 
 * */
void generate_large_prime(mpz_t* r_number)
{
    int rndBit=1023; 					// bound for mpz_randb
	
	gmp_randstate_t random_state; // random generator state object    	
	 
	gmp_randinit_default(random_state); // initilize random state with default algorithm 	
		
	//defining seed with unsigned long before beginning 
	unsigned long seed = (unsigned long)(time(NULL));	
	
	gmp_randseed_ui(random_state, seed);

	   
    mpz_urandomb(*r_number, random_state, rndBit);
    	
	int odd_check = mpz_odd_p (*r_number); 		// returns non zero if odd 
	if (odd_check==0) 							// if result is zero, that is term is even -> enter if condition 
	{
		unsigned long int one =1;
		mpz_add_ui ( *r_number, *r_number, one);  // now r_number is odd 
	}
	
	int reps= 40 ; // number of times to run primality test;
	mpz_t prime_1024;
	mpz_init (prime_1024);
	
	int primality_test = mpz_probab_prime_p (*r_number,  reps); //Return 2 if n is definitely prime, 	
													//return 1 if n is probably prime (without being certain), or return 0 if n is definitely non-prime.
		
	if(primality_test!=2)
	{
		mpz_nextprime (*r_number, *r_number);	
		//primality_test =  mpz_probab_prime_p (r_number,  reps);
	}	
	mpz_t tmp; mpz_init(tmp);
    mpz_t local_e; mpz_init(local_e); mpz_set_ui(local_e, 65537);

	mpz_mod(tmp, *r_number, local_e);        /* If p mod e == 1, gcd(phi, e) != 1 */
    while(!mpz_cmp_ui(tmp, 1))         
    {
        mpz_nextprime(*r_number, *r_number);    /* so choose the next prime */
        mpz_mod(tmp, *r_number, local_e);
    }
	

}
/** 
 * @brief Copy parameters from private key to public key 
 * @param[in] Public key pointer
 * @param[in] Private key pointer
 * @param[out] Initialized public key structure
 * @return void 
 * */

void generate_public_key (public_key_RSA* pub_k,private_key_RSA *pri_k)
{

mpz_set(pub_k->n, pri_k->n);
return;
} 

/** 
 * @brief Initializes private key values and sets them
 * @param[in] pointer to private key structure
 * @param[out] Initialized private key structure
 * @return void 
 * */
void generate_private_key (private_key_RSA *pri_k)
{   
    mpz_t phi; mpz_init(phi);
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);
 
    mpz_t * prime_p_ptr = &(pri_k->p);
    mpz_t * prime_q_ptr= &(pri_k->q);

    generate_large_prime (prime_p_ptr);
    generate_large_prime (prime_q_ptr);
    
    while (mpz_cmp(*prime_p_ptr, *prime_q_ptr) == 0){
        generate_large_prime (prime_q_ptr);
    }    

    mpz_mul(pri_k->n, pri_k->p, pri_k->q);

	mpz_sub_ui(tmp1, pri_k->p, 1);
    mpz_sub_ui(tmp2, pri_k->q, 1);
    mpz_mul(phi, tmp1, tmp2);

	if(mpz_invert(pri_k->d, pri_k->e, phi) == 0)
	{
		printf("Invert Failed");
	}
return;
}

/** 
 * @brief Initializes message structure
 * @param[in] message pointer to uninitialised message
 * @param[out] Initialized message structure
 * @return void 
 * */
void initialize_message(message * new_message)
{
    mpz_init(new_message->C);
    mpz_init(new_message->M);
    mpz_init(new_message->DC);
}

/** 
 * @brief Initializes RSA_parameters structure
 * @param[in] RSA_p pointer to RSA_parameters structure
 * @param[out] Initialized RSA_parameters structure
 * @return void 
 * */

void initialize_RSA(RSA_parameters *new_parameters)
{
    
    mpz_init(new_parameters->pri_k.p);
    mpz_init(new_parameters->pri_k.q);
    mpz_init(new_parameters->pri_k.n);
    mpz_init(new_parameters->pri_k.e);
    mpz_init(new_parameters->pri_k.d);
    mpz_set_ui(new_parameters->pri_k.e, 65537 );

    mpz_init(new_parameters->pub_k.n);
    mpz_init(new_parameters->pub_k.e);
    mpz_set_ui(new_parameters->pub_k.e, 65537 );

    


    generate_private_key (&(new_parameters->pri_k));
    generate_public_key (&(new_parameters->pub_k),&(new_parameters->pri_k));
    
    
	return;


}

/** 
 * @brief Encrypts the AES key using RSA public key
 * @param[in] AES key
 * @param[in] RSA parameters structure pointer
 * @param[in] message pointer 
 * @param[out] encrypted AES key in C in message
 * @return void 
 * */
void encrypt_AES_key(int8_t aes_key[16], RSA_parameters *RSA_initialization, message *input_message)
{
    int size_AES_key = 16*sizeof(uint8_t);
    mpz_import(input_message->M, size_AES_key, 1, sizeof(aes_key[0]), 0, 0, aes_key);   
    block_encrypt_RSA(input_message,&(RSA_initialization->pub_k));
    return; 
}


/** 
 * @brief Decrypts the AES key using RSA private key
 * @param[in] encrypted AES key in message pointer
 * @param[in] RSA parameters structure pointer
 * @param[in] message pointer to hold decrypted AES message
 * @param[out] decrypted AES key in C in out_message
 * @return void 
 * */
void decrypt_AES_key(RSA_parameters *RSA_initialization, message *input_message, message*output_message)
{
    *(output_message->C) = *(input_message->C);
    block_decrypt_RSA(output_message,&(RSA_initialization->pri_k));
    
    
    return; 
}


