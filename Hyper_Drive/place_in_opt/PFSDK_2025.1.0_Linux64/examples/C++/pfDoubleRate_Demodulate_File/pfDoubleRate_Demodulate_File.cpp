/*
******************************************************************************
* @attention
*
*<h2><center>&copy; COPYRIGHT(c) 2021 Photonfocus AG</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
* 3. Neither the name of Photonfocus nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pfDoubleRate.h"

#define BUFFER_SIZE 1024


int main(int argc, char **argv)
{
	unsigned char *modImage, *demodImage;
	int Height, modWidth, demodWidth, filesize;
	char filename[BUFFER_SIZE];
	FILE *pFile;
		
	modWidth = 0;
	demodWidth = 0;
	Height = 0;
	filesize = 0;

	//Read DR1 image
	if(argc == 2){
		strcpy(filename, argv[1]);
	}
	else{
		strcpy(filename, "image.dr1");
		printf("--------------------------------------------------------------------------\n");
		printf("This program shows, how to use pfDoubleRate Demodulation DLL\n(pfDoubleRate.dll)\n");
		printf("\n\bUsage:\n");
		printf("pfDoubleRateExample.exe <dr1 filename>\n");
		printf("pfDoubleRateExample.exe image.dr1\n");
		printf("\n\nDefault DR1 image file name is: image.dr1");
		printf("\n");
	}

	if((pFile = fopen(filename, "rb")) == NULL){
		printf("Could not read input image!!!\n");
		return 0;
	}
	//obtain file size:
	fseek(pFile , 0 , SEEK_END);
	filesize = ftell(pFile);
	rewind(pFile);

	//alloc buffer and read from file
	modImage = (unsigned char*)malloc(filesize);
	fread(modImage, 1, filesize, pFile);
	fclose(pFile);


	//DEMODULATE
	//get width of deModulated image
	pfDoubleRate_GetDeModulatedWidth(modImage, &demodWidth);
	pfDoubleRate_GetModulatedWidth(demodWidth, &modWidth);
	Height = filesize / modWidth;
	demodImage = (unsigned char*)malloc(demodWidth * Height);

	//demodulate image
	int demod_result = pfDoubleRate_DeModulateImage(demodImage, modImage, demodWidth, Height, modWidth);
	if(demod_result == PFDOUBLERATE_SUCCESS) {
		printf("Size of demodulated image: %d %d\n", demodWidth, Height);
		//write the demodulated image as RAW image format
		strcpy(filename, "image.raw");
		if((pFile = fopen(filename, "wb")) == NULL){
			printf("Could not write output image!!!\n");
			return 0;
		}
		fwrite(demodImage, 1, demodWidth*Height, pFile);
		fclose(pFile);
		printf("Image written to %s\n", filename);
	}

	//clean up
	if(modImage != NULL){
		free(modImage);
		modImage = NULL;
	}
	if(demodImage != NULL){
		free(demodImage);
		demodImage = NULL;
	}
	
	return 0;
}
