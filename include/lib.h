#include "types.h"

#pragma once

BYTE * const VIDEO_MEMORY = (BYTE *) 0xb8000;

uint32_t pointer = 0;

void printf(){
	uint32_t nextLine = 0;

	// Increment to the next multiple of 80.
	while(nextLine <= pointer)
		nextLine += 80;
	pointer = nextLine;
}

void printf(const char * str){
	
	// Copy the string into video memory.
	int i = 0;
	for(; str[i]; i++)
		VIDEO_MEMORY[(pointer + i) * 2] = str[i];
	
	// Increment pointer.
	pointer += i;
}

void printb(uint32_t integer, uint8_t base, bool upper){
	
	// 50 characters should be more than enough.
	char str[50];
	int i = 0;

	// Create char sequence.
	while(integer != 0){
		int digit = integer % base;
		integer /= base;
		str[i] = digit < 10 ? (char) (48 + digit) : upper ? (char) (65 + (digit % 10)) : (char) (97 + (digit % 10));
		i++;
	}
	
	// Reverse the sequence as the first character represents the zeroth power.
	for(int j = 0; j < i / 2; j++){
        int opposite = i - j - 1;
		str[j] ^= str[opposite];
		str[opposite] ^= str[j];
		str[j] ^= str[opposite];
	}

	// Terminating character.
	str[i] = '\0';

	// Print the string.
	printf(str);
}

void printi(uint32_t integer){

	// Print the integer in base 10.
	printb(integer, 10, false);
}

void printh(uint32_t hex){

	// Prepend '0x' and print the integer in base 16.
	printf("0x");
	printb(hex, 16, false);
}

void printp(void * ptr){

	// Print the pointer in hex.
	printh((uint32_t)ptr);
}