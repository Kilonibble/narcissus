#pragma once
#include "imageloader.h"

// Header field type for a bitmap
#define BITMAP_TYPE_BYTE1 0x42
#define BITMAP_TYPE_BYTE2 0x4D

#pragma pack(2)

/* Structure to be filled with information from the bitmap
   header. */
typedef struct
{
    unsigned short headerField;
    unsigned int   size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int   offset;
} BmpFileHeader;

#pragma pack()

typedef struct                       
{
    unsigned int   size;           /* Header size */
    int            width;          /* Bitmap Width */
    int            height;         /* Bitmap Height */
    unsigned short planes;         /* Colour plane numbers */
    unsigned short bitCount;       /* # bits per pixel */
    unsigned int   compression;    /* Compression type */
    unsigned int   imgSize;      /* Image data size */
    int            xPixelsPerMeter;  /* X pixels/meter */
    int            yPixelsPerMeter;  /* Y pixels/meter */
    unsigned int   coloursUsed;        /* Number of colours */
    unsigned int   importantColours;   /* Number of important colours */
} BmpInfoHeader;

class BitmapLoader : public ImageLoader
{
public:
	BitmapLoader(void);
	~BitmapLoader(void);

	int loadImage(std::string filename);
	void dumpHeaderInfo();

private:
	BmpFileHeader* m_fileHeader; 
	BmpInfoHeader* m_infoHeader;

	// Helper functions
	int fillHeader(std::ifstream& refInputStream);
	int fillInfoHeader(std::ifstream& refInputStream);
	int verifyBitmapType();
	void clearData();
};

