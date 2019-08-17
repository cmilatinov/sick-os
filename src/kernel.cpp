#include "lib.h"
#include "gtd.h"

extern "C" void kernelMain(void * mbStruct, uint32_t magic){

	GDT gdt;
	
	printf("=========================== This is a 32-bit SickOS! ===========================");
	printf();

	printf("Magic Number - ");
	printh(magic);
	printf();

	printf("Multiboot Structure Pointer - ");
	printp(mbStruct);
	printf();

	printf("Random Integer In Decimal - ");
	printi(234564345);
	printf();

	printf("Random Integer In Hexadecimal - ");
	printh(234564345);
	printf();

	printf("Random Integer In Binary - ");
	printb(234564345, 2, false);
	printf();

}