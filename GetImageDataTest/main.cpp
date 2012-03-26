#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;


static unsigned int sARGB[] = {
	0xff000000,
	0xff0000ff,
	0xff00ff00,
	0xff00ffff,
	0xffff0000,
	0xffff00ff,
	0xffffff00,
	0xffffffff,
	0x00000000,
	0x000000ff,
	0x0000ff00,
	0x0000ffff,
	0x00ff0000,
	0x00ff00ff,
	0x00ffff00,
	0x00ffffff,
};

static unsigned int sRGB[] = {
	0xff000000,
	0xff0000ff,
	0xff00ff00,
	0xff00ffff,
	0xffff0000,
	0xffff00ff,
	0xffffff00,
	0xffffffff,
};

void GetImageDataTest()
{
	// Image resources
	MAHandle image1 = 1; // ARGB_PNG
	MAHandle image2 = 2; // RGB_PNG

	// Pixels in each image
	int IMAGE16 = 16;
	int IMAGE8 = 8;

	// ARGB PNG image
	unsigned int buffer1[IMAGE16];
	MARect rect1 = {0, 0, IMAGE16, 1};
	maGetImageData(image1, buffer1, &rect1, IMAGE16);
	int success1 = 0 == memcmp(sARGB, buffer1, IMAGE16);

	// RGB PNG image
	unsigned int buffer2[IMAGE8];
	MARect rect2 = {0, 0, IMAGE8, 1};
	maGetImageData(image2, buffer2, &rect2, IMAGE8);
	int success2 = 0 == memcmp(sRGB, buffer2, IMAGE8);

	if (success1)
	{
		printf("Test 1 passed\n");
	}
	else
	{
		printf("Test 1 failed\n");
		printf(
			"%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x"
			"%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x",
			buffer2[0],buffer2[1],buffer2[2],buffer2[3],
			buffer2[4],buffer2[5],buffer2[6],buffer2[7],
			buffer2[8],buffer2[9],buffer2[10],buffer2[11],
			buffer2[12],buffer2[13],buffer2[14],buffer2[15]);
	}

	if (success2)
	{
		printf("Test 2 passed\n");
	}
	else
	{
		printf("Test 2 failed\n");
		printf("%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x",
			buffer2[0],buffer2[1],buffer2[2],buffer2[3],
			buffer2[4],buffer2[5],buffer2[6],buffer2[7]);
	}
}

void GetImageDataDisplayTest()
{
	MAHandle imageResource = 3; // MICKE_JPG

	// Do test with a bigger JPEG image
	maDrawImage(imageResource, 0, 0);
	maUpdateScreen();

	int extent = maGetImageSize(imageResource);
	int width = EXTENT_X(extent);
	int height = EXTENT_Y(extent);

	// Display whole image
	unsigned int buffer[width * height];
	MARect rect = {0, 0, width, height};
	maGetImageData(imageResource, buffer, &rect, width);

	MAHandle image = maCreatePlaceholder();
	maCreateImageRaw(image, buffer, extent, 0);
	maDrawImage(image, 0, height);
	maUpdateScreen();
	maDestroyPlaceholder(image);

	// Display part of image
	int w = width / 2;
	int h = height / 2;
	unsigned int buffer2[w * h];
	MARect rect2 = {w / 2, h / 2, w, h};
	maGetImageData(imageResource, buffer2, &rect2, w);

	image = maCreatePlaceholder();
	maCreateImageRaw(image, buffer2, EXTENT(w, h), 0);
	maDrawImage(image, 0, height * 2);
	maUpdateScreen();
	maDestroyPlaceholder(image);
}

void GetImageDataSpeedTest()
{
	MAHandle imageResource = 4; // MICKE_JPG

	int extent = maGetImageSize(imageResource);
	int width = EXTENT_X(extent);
	int height = EXTENT_Y(extent);

	unsigned int buffer[width * height];
	MARect rect = {0, 0, width, height};

	printf("Starting time test");

	int t1 = maGetMilliSecondCount();

	for (int i = 0; i < 1000; ++i)
	{
		maGetImageData(imageResource, buffer, &rect, width);
	}

	int t2 = maGetMilliSecondCount();

	printf("Time test took ms: %i", t2 - t1);
}

class MyMoblet : public Moblet
{
public:
	MyMoblet()
	{
		printf("Touch screen to run test\n");
		printf("Press zero or back to exit\n");
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}

		// Print the key character.
		printf("You typed: %c\n", keyCode);
	}

	void keyReleaseEvent(int keyCode, int nativeCode)
	{
	}

	void pointerPressEvent(MAPoint2d point)
	{
		printf("Running test\n");
		GetImageDataTest();
		GetImageDataSpeedTest();
		GetImageDataDisplayTest();
	}
};

extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
