#pragma once
#include <string>

class ImageLoader
{
public:
	ImageLoader(void);
	virtual ~ImageLoader(void);

	virtual int loadImage(std::string filename) = 0;

};

