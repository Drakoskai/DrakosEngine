#pragma once

#include "Utility.h"
#include "Texture.h"


unsigned char* LoadTarga(char*, int&, int&);

INLINE unsigned char* LoadTarga(char* filename, int& height, int& width) {
	FILE* filePtr;
	TargaHeader targaFileHeader;

	// Open the targa file for reading in binary.
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) {
		//throw
	}

	// Read in the file header.
	uint32_t count = static_cast<uint32_t>(fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr));

	// Get the important information from the header.
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	int bpp = static_cast<int>(targaFileHeader.bpp);

	// Check that it is 32 bit and not 24 bit.
	if (bpp != 32) {
		//throw
	}

	// Calculate the size of the 32 bit image data.
	int imageSize = width * height * 4;

	// Allocate memory for the targa image data.
	unsigned char * targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, filePtr));
	if (count != imageSize) {
		//throw
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0) {
		//throw
	}

	// Allocate memory for the targa destination data.
	unsigned char* targaData = new unsigned char[imageSize];
	if (!targaData) {
		//throw
	}

	// Initialize the index into the targa destination data array.
	int index = 0;

	// Initialize the index into the targa image data.
	int k = (width * height * 4) - (width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			targaData[index + 0] = targaImage[k + 2];  // Red.
			targaData[index + 1] = targaImage[k + 1];  // Green.
			targaData[index + 2] = targaImage[k + 0];  // Blue
			targaData[index + 3] = targaImage[k + 3];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;

	return targaData;
}
