#include "BitmapLoader.h"
#include <fstream>
#include <iostream>
#include <stdint.h>

BitmapLoader::BitmapLoader(void) : m_fileHeader(NULL), m_infoHeader(NULL)
{
}

BitmapLoader::~BitmapLoader(void)
{
	if(m_fileHeader != NULL) delete m_fileHeader;
	if(m_infoHeader != NULL) delete m_infoHeader;
}

/**
 * Function: loadImage
 * Parameter(s): filename: Filename of bitmap image to open.
 * Returns: 1 on success and 0 on fail.
 * 
 * Populates structures with bitmap data and fills out columns of pixel data. Only one image
 * can be loaded in memory at once with this loader, so previous memory will be freed when 
 * a new image is loaded. 
 */
int BitmapLoader::loadImage(std::string filename)
{
	std::ifstream fileStream(filename.c_str(), std::ios::binary);

	if(!fileStream)
	{
		std::cerr << "No such file: " << filename << std::endl;
		return 0;
	}

	/* Free any previous memory used for the header structs */
	clearData();

	/* Fill the bitmap header struct with information from the file. */
	if(!fillHeader(fileStream)) 
	{
		fileStream.close();
		return 0;
	}

	/* Fill the bitmap information header struct with information from the file. */
	if(!fillInfoHeader(fileStream)) 
	{
		fileStream.close();
		return 0;
	}

	if(m_fileHeader == NULL || m_infoHeader == NULL)
	{
		/* One of the headers could not load */
		std::cerr << "Error loading information headers." << std::endl;
		return 0;
	}

	/* Verify that this file is a 24-bit bmp. */
	if(!verifyBitmapType())
	{
		fileStream.close();
		return 0;
	}

	dumpHeaderInfo();
	/* Read in the pixels */

}

/**
 * Function: fillHeader
 * Parameter(s): refInputStream: Open file stream of the bitmap file.
 * Returns: 1 on success and 0 on fail.
 * 
 * This fills the file header structure for this bitmap with data. This structure
 * has information about the file, but not the image itself.
 */
int BitmapLoader::fillHeader(std::ifstream& refInputStream)
{
	if(!refInputStream)
	{
		std::cerr << "Bad file stream." << std::endl;
		return 0;
	}

	// Read in the header data
	char* byteList = new char[sizeof(BmpFileHeader)];
	refInputStream.read(byteList, sizeof(BmpFileHeader));
	m_fileHeader = (BmpFileHeader*) byteList;

	return 1;
}

/**
 * Function: fillInfoHeader
 * Parameter(s): refInputStream: Open file stream of the bitmap file.
 * Returns: 1 on success and 0 on fail.
 * 
 * This fills the bitmap information structure with data from the file. This
 * structure has data concering the image properties (width, height, etc). 
 */
int BitmapLoader::fillInfoHeader(std::ifstream& refInputStream)
{
	if(!refInputStream)
	{
		std::cerr << "Bad file stream." << std::endl;
		return 0;
	}

	// Read in the info header data
	char* byteList = new char[sizeof(BmpInfoHeader)];
	refInputStream.read(byteList, sizeof(BmpInfoHeader));
	m_infoHeader = (BmpInfoHeader*) byteList;

	return 1;
}

/**
 * Function: verifyBitmapType
 * Parameter(s): None
 * Returns: 1 on success and 0 on fail.
 * 
 * This verifies that the loaded file is a bitmap by checking the headerField of the file's header. 
 * This should be 0x424D for a 24-bit bmp. 
 */
int BitmapLoader::verifyBitmapType() 
{
	if((m_fileHeader->headerField & 0x00FF) != BITMAP_TYPE_BYTE1 || 
		((m_fileHeader->headerField & 0xFF00) >> 8)  != BITMAP_TYPE_BYTE2)
	{
		std::cerr << "Wrong file type. Header Field received: " << std::hex << m_fileHeader->headerField;
		std::cerr << ", expected: " << ((BITMAP_TYPE_BYTE2 << 8) & BITMAP_TYPE_BYTE1) << std::dec << std::endl;
		return 0;
	}
	
	return 1;
}

/**
 * Function: clearData
 * Parameter(s): None
 * Returns: Nothing
 * 
 * Frees any memory associated with the header structures.
 */
void BitmapLoader::clearData() 
{
	if(m_fileHeader != NULL) delete m_fileHeader;
	if(m_infoHeader != NULL) delete m_infoHeader;
}

void BitmapLoader::dumpHeaderInfo()
{
	if(m_fileHeader == NULL) return;

	std::cout << "File Type: " << std::hex << m_fileHeader->headerField << std::dec << std::endl;
	std::cout << "File Size: " << m_fileHeader->size << std::endl;
	std::cout << "Offset to image information: " << m_fileHeader->offset << std::endl;

	if(m_infoHeader == NULL) return;

	std::cout << "Information Header Size: " << m_infoHeader->size << std::endl;
	std::cout << "Image Width: " << m_infoHeader->width << std::endl;
	std::cout << "Image Height: " << m_infoHeader->height << std::endl;
	std::cout << "Number of Bits Per Pixel: " << m_infoHeader->bitCount << std::endl;
	std::cout << "Image Size: " << m_infoHeader->imgSize << std::endl;
	std::cout << "Compression: " << m_infoHeader->compression << std::endl;
	std::cout << "Horizontal Resolution: " << m_infoHeader->xPixelsPerMeter << std::endl;
	std::cout << "Vertical Resolution: " << m_infoHeader->yPixelsPerMeter << std::endl;
	std::cout << "Number of important colours used: " << m_infoHeader->importantColours << std::endl;
	std::cout << "Number of colours in palette: " << m_infoHeader->coloursUsed << std::endl;
}