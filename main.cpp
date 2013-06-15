#include "BitmapLoader.h"
#include "ImageLoader.h"

using namespace std;

int main() 
{
	ImageLoader* loader = new BitmapLoader();
	loader->loadImage("test_bitmap.bmp");

	system("PAUSE");
	delete loader;
	return 0;
}