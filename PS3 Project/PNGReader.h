#ifndef __PNG_READER__
#define __PNG_READER__

#include <sys/spu_initialize.h>
#include <cell/codec/pngdec.h>

/*E Set the argument for malloc callback function. */
typedef struct {
	uint32_t mallocCallCounts;
	uint32_t freeCallCounts;
} CtrlCbArg;

/*E PNG Decoder needs those parameters. */
typedef struct{
	CellPngDecMainHandle		mainHandle;
	CellPngDecSubHandle			subHandle;
	CtrlCbArg					ctrlCbArg;
}SPngDecCtlInfo;

class PNGReader
{
public:
	int ret;
	//MyBuffs	dispBuffs;
	SPngDecCtlInfo myPngDecCtlInfo;
	bool failed;
	char* errMessage;
	CellPngDecThreadInParam 	threadInParam;
	CellPngDecThreadOutParam 	threadOutParam;
	CellPngDecSrc 			pngdecSrc; 
	CellPngDecOpnInfo		pngdecOpenInfo;
	CellPngDecInfo				pngdecInfo;
	CellPngDecInParam 			pngdecInParam;
	CellPngDecOutParam			pngdecOutParam;
	int bufferSize;
	uint8_t* data;
	CellPngDecDataCtrlParam		pngdecCtrlParam;
	CellPngDecDataOutInfo 		pngdecDataOutInfo;

	/*E Set the malloc callback function. */
	static void *pngDecCbControlMalloc(uint32_t size, void *cbCtrlCbArg)
	{
		CtrlCbArg *arg;
		arg = (CtrlCbArg *)cbCtrlCbArg;
		arg->mallocCallCounts++;
		return malloc(size);
	}//pngDecCbControlMalloc

	/*E Set the free callback function. */
	static int32_t pngDecCbControlFree(void *ptr, void *cbCtrlCbArg)
	{
		CtrlCbArg *arg;
		arg = (CtrlCbArg *)cbCtrlCbArg;
		arg->freeCallCounts++;
		free(ptr);
		return 0;
	}//pngDecCbControlFree

	PNGReader()
	{
		failed = false;

		//stage 1: load PRX module
		ret = cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
		if (ret != CELL_OK)
		{
			failed = true;
			errMessage = "failed to load PRX module (stage 1)";
		}//if

		if (!failed)
		{
			//stage 2: create the decoder
			/*E Set the spu thread disable/enable. */
			myPngDecCtlInfo.ctrlCbArg.mallocCallCounts	= 0;
			myPngDecCtlInfo.ctrlCbArg.freeCallCounts	= 0;
			threadInParam.spuThreadEnable	= CELL_PNGDEC_SPU_THREAD_DISABLE;
			threadInParam.ppuThreadPriority	= 512;
			threadInParam.spuThreadPriority	= 200;
			threadInParam.cbCtrlMallocFunc	= pngDecCbControlMalloc;
			threadInParam.cbCtrlMallocArg	= &myPngDecCtlInfo.ctrlCbArg;
			threadInParam.cbCtrlFreeFunc	= pngDecCbControlFree;
			threadInParam.cbCtrlFreeArg		= &myPngDecCtlInfo.ctrlCbArg;
			
			/*E Create the PNG Decoder. */
			ret = cellPngDecCreate( &myPngDecCtlInfo.mainHandle, &threadInParam, &threadOutParam );
			if (ret != CELL_OK)
			{
				failed=true;
				errMessage = "failed to create PNG decoder (stage 2)";
			}//if ret
		}//if !failed
		if (failed) printErr();
	}//constructor

	void loadFile(const char* _filename)
	{
		/*printf("Attempting to read PNG file: ");
		printf(_filename);
		printf("\n");*/
		failed = false;

		//stage 3: open a stream
		
		/*E Set the stream source. */
		pngdecSrc.srcSelect		= CELL_PNGDEC_FILE;
		pngdecSrc.fileName		= _filename;
		pngdecSrc.fileOffset	= 0;
		pngdecSrc.fileSize		= 0;
		pngdecSrc.streamPtr		= NULL;
		pngdecSrc.streamSize	= 0;
		
		/*E Set the spu thread disable/enable. */
		pngdecSrc.spuThreadEnable  = CELL_PNGDEC_SPU_THREAD_DISABLE;
		
		/*E Open the stream. */
		ret = cellPngDecOpen( myPngDecCtlInfo.mainHandle, &myPngDecCtlInfo.subHandle, &pngdecSrc, &pngdecOpenInfo);
		if (ret != CELL_OK)
		{
			failed = true;
			errMessage = "Unable to open PNG stream (stage 3)";
		}//if ret
		
		//set read PNG header (stage 4)
		if (!failed)
		{
			ret = cellPngDecReadHeader(myPngDecCtlInfo.mainHandle, myPngDecCtlInfo.subHandle, &pngdecInfo);
			if (ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to read PNG header\n";
			}
		}//stage 4

		//set decode parameters (stage 5)
		if (!failed)
		{
			/*E Set the parameter for PNG Decoder. */
			pngdecInParam.commandPtr		= NULL;
			pngdecInParam.outputMode		= CELL_PNGDEC_TOP_TO_BOTTOM;
			pngdecInParam.outputColorSpace	= CELL_PNGDEC_RGBA;
			pngdecInParam.outputBitDepth	= 8;
			pngdecInParam.outputPackFlag	= CELL_PNGDEC_1BYTE_PER_1PIXEL;
			
			if(( pngdecInfo.colorSpace 	== CELL_PNGDEC_GRAYSCALE_ALPHA )
				||(pngdecInfo.colorSpace	== CELL_PNGDEC_RGBA)
				||(pngdecInfo.chunkInformation & 0x10))
			{
				pngdecInParam.outputAlphaSelect = CELL_PNGDEC_STREAM_ALPHA;
			}//if
			else
			{
				pngdecInParam.outputAlphaSelect = CELL_PNGDEC_FIX_ALPHA;
			}//else
			pngdecInParam.outputColorAlpha  = 0xff;
			
			ret = cellPngDecSetParameter(myPngDecCtlInfo.mainHandle, myPngDecCtlInfo.subHandle, &pngdecInParam, &pngdecOutParam);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to set decode parameters (stage 5)";
			}//if
		}//if (!failed) stage 5
		
		//allocate memory (stage 6)
		if (!failed)
		{

			bufferSize = pngdecOutParam.outputWidthByte * pngdecOutParam.outputHeight;
			pngdecCtrlParam.outputBytesPerLine 	= pngdecOutParam.outputWidthByte;
			data = new uint8_t[bufferSize];
		}//if (1failed) stage 6

		//read data (stage 7)
		if (!failed)
		{
			ret = cellPngDecDecodeData(myPngDecCtlInfo.mainHandle, myPngDecCtlInfo.subHandle, data, &pngdecCtrlParam, &pngdecDataOutInfo);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to read PNG image data (stage 7)";
			}
		}//if (!failed) stage 7

		//close input stream (stage 8)
		if (!failed)
		{
			ret = cellPngDecClose( myPngDecCtlInfo.mainHandle, myPngDecCtlInfo.subHandle);
			if (ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to close decode stream (stage 8)";
			}//if
		}//if (!failed) stage 8

		//load into current texture object (stage 9)
		if (!failed)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pngdecOutParam.outputWidth,
				pngdecOutParam.outputHeight, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data);
			glGenerateMipmapOES(GL_TEXTURE_2D);
			delete data;
		}//if (!failed) stage 9

		//finalise
		if (failed) printErr();

	}//loadFile()

	~PNGReader()
	{
		failed = false;

		//destroy decoder (stage 10)
		ret = cellPngDecDestroy(myPngDecCtlInfo.mainHandle);
		if(ret != CELL_OK)
		{
			failed = true;
			errMessage = "failed to destroy GIF decoder (stage 10)";
		}//if

		//unload PRX module
		if (!failed)
		{
			ret = cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
			if(ret != CELL_OK)
			{
				failed = true;
				errMessage = "failed to unload PRX module (stage 11)";
			}
		}//if (!failed) stage 11


	}//destructor

	void printErr()
	{
			printf("!!!!!!!!!!!PNG reader failed at ");
			printf(errMessage);
			printf("\n");
	}//printErr()
};//class

#endif