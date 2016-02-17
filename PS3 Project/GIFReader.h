#ifndef GIFREADER_H
#define GIFREADER_H

#include <cell/codec.h>
#include <sys/spu_initialize.h>

//#define SPU_THREAD_ENABLE		CELL_GIFDEC_SPU_THREAD_ENABLE
#define SPU_THREAD_ENABLE		CELL_GIFDEC_SPU_THREAD_DISABLE
#define ALPHA_TRANSPARENT_VALUE	0x00
#define ALPHA_OPAQUE_VALUE		0xFF

#define EMSG0(x, ...)	printf(x, __func__, ##__VA_ARGS__)
#define EMSG(...)		EMSG0("* %s: ERROR! " __VA_ARGS__)
#define EINFO(x)		EMSG("exit. (ret=0x%08X, file=%s:%d)\n", (unsigned int)x, __FILE__, __LINE__)
#define DP0(x, ...)		printf(x, __func__, ##__VA_ARGS__)
#define DP(...)			DP0("* %s: " __VA_ARGS__)

class GIFReader
{
public:
	bool failed;
	int ret;
	char* errMessage;
	CellGifDecThreadInParam threadInParam;
	CellGifDecThreadOutParam threadOutParam;
	CellGifDecMainHandle mainHandle;
	CellGifDecInfo info;
	CellGifDecSubHandle subHandle;
	CellGifDecOpnInfo openInfo;
	CellGifDecSrc src;
	CellGifDecInParam inParam;
	CellGifDecOutParam outParam;
	int bufferSize;
	uint8_t* data;
	CellGifDecDataCtrlParam dataCtrlParam;
	CellGifDecDataOutInfo dataOutInfo;	

	/*E memory allocation callback. */
	static void *gifDecCbControlMalloc(uint32_t size, void *cbArg){
		return malloc(size);
	}

	/*E memory deallocation callback. */
	static int32_t gifDecCbControlFree(void *ptr, void *cbArg){
		free(ptr);
		return 0;
	}

	GIFReader()
	{

		failed = false;
		//stage 1: load PRX module
		ret = cellSysmoduleLoadModule(CELL_SYSMODULE_GIFDEC);
		if(ret != CELL_OK)
		{
			failed = true;
			errMessage = "failed to load PRX module (stage 1)1";
		}

		//stage 2: create a decoder
		if (!failed)
		{
			threadInParam.spuThreadEnable = SPU_THREAD_ENABLE;
			threadInParam.ppuThreadPriority = 512;
			threadInParam.spuThreadPriority = 200;
			threadInParam.cbCtrlMallocFunc = gifDecCbControlMalloc;
			threadInParam.cbCtrlMallocArg = NULL;
			threadInParam.cbCtrlFreeFunc = gifDecCbControlFree;
			threadInParam.cbCtrlFreeArg = NULL;
			ret = cellGifDecCreate(&mainHandle, &threadInParam, &threadOutParam);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to create a decoder (stage 2)";
			}//if
		}//if (stage 2)
		//finalise
		if (failed) printErr();
	}//constructor

	~GIFReader()
	{
		failed = false;

		//stage 10: destroy decoder
		ret = cellGifDecDestroy(mainHandle);
		if(ret != CELL_OK)
		{
				failed = true;
				errMessage = "failed to destroy decoder (stage 10)";
		}//if stage 10

		//unload PRX module (stage 11)
		if (!failed)
		{
			ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_GIFDEC);
			if(ret < CELL_OK)
			{
				failed = true;
				errMessage = "failed to unload PRX module (stage 11)";
			}
		}//if stage 11
		if (failed) printErr();
	}//destructor

	void loadFile(const char* _filename)
	{
		/*printf("Attempting to read texture file: ");
		printf(_filename);
		printf("\n");*/

		failed = false;


		//stage 3: open a stream
		if (!failed)
		{
			src.srcSelect = CELL_GIFDEC_FILE;
			src.fileName = _filename;
			src.fileOffset = 0;
			src.fileSize = 0;
			src.streamPtr = NULL;
			src.streamSize = 0;
			src.spuThreadEnable = SPU_THREAD_ENABLE;
			ret = cellGifDecOpen(mainHandle, &subHandle, &src, &openInfo);
			//DP("opening file stage 3: ret = 0x%08x\n", ret);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to open stream (stage 3)";
			}
		}//if (stage 3)

		//stage 4: read GIF header
		if (!failed)
		{
			ret = cellGifDecReadHeader(mainHandle, subHandle, &info);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to read GIF header (stage 4)";
			}//if
		}//if (stage 4)

		//stage 5: parameters for decoding
		if (!failed)
		{
			inParam.commandPtr = NULL;
			inParam.colorSpace = CELL_GIFDEC_RGBA;
			inParam.outputColorAlpha1 = ALPHA_TRANSPARENT_VALUE;
			inParam.outputColorAlpha2 = ALPHA_OPAQUE_VALUE;
			inParam.reserved[0] = 0;
			inParam.reserved[1] = 0;
			ret = cellGifDecSetParameter(mainHandle, subHandle, &inParam, &outParam);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to capture decode parameters (stage 5)";
			}//if
		}//if (stage 5)

		//stage 6: allocate memory
		if (!failed)
		{
			bufferSize = outParam.outputWidthByte * outParam.outputHeight;
			//printf("need %i memory\n", bufferSize);
			data = new uint8_t[bufferSize];
			dataCtrlParam.outputBytesPerLine = outParam.outputWidthByte;
			/*if (outParam.outputColorSpace == CELL_GIFDEC_RGBA) printf("RGBA format\n");
			if (outParam.outputColorSpace == CELL_GIFDEC_ARGB) printf("ARGB format\n");*/
		}//if stage 6

		//stage 7: read data
		if (!failed)
		{
			do
			{
				ret = cellGifDecDecodeData(mainHandle, subHandle, data, &dataCtrlParam, &dataOutInfo);
				if(ret != CELL_OK)
				{
					failed = true;
					errMessage = "failed to read data (stage 7)";
				}
			} while ((dataOutInfo.recordType != CELL_GIFDEC_RECORD_TYPE_IMAGE_DESC) && (dataOutInfo.recordType != CELL_GIFDEC_RECORD_TYPE_TERMINATE));
		}//if stage 7

		//stage 8 - check we got an image
		if ((!failed) && (dataOutInfo.recordType != CELL_GIFDEC_RECORD_TYPE_IMAGE_DESC))
		{
				failed = true;
				errMessage = "no image found (stage 8)";
		}//if stage 8

		//cleanup - stage 9
		if (!failed)
		{
			ret = cellGifDecClose(mainHandle, subHandle);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to close stream (stage 9)";
			}//if
		}//if cleanup

		//load into current texture object
		if (!failed)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, outParam.outputWidth,
				outParam.outputHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
			glGenerateMipmapOES(GL_TEXTURE_2D);
			delete data;
		}//if load

		//finalise
		if (failed) printErr();

	}//loadFile()

	void printErr()
	{
			printf("!!!!!!!!!!!GIF reader failed at ");
			printf(errMessage);
			printf("\n");
	}
};//class

#endif