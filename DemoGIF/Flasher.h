//------------------------------------------------------------------------
// Copyright(c) 2024 Dad Design.
// 
// Utility for flashing QSPIFlash memory from a PC
// Memory structure and transmission blocks
//-----------------------------------------------------------------------
#pragma once
#include <stdint.h>
#include <string.h>

namespace Dad {

#define QSPI_SIZE           8388608                         // Size of the QSPI flash memory 8M = 8 * 1024 * 1024
#define QSPI_PAGE_SIZE      4096                            // 4K per page
#define QSPI_PAGE_COUNT     2048                            // Number of QSPI pages for the flasher 2024 * 4096 4MB
#define QSPI_ADRESSE        0x90000000

#define MAX_ENTRY_NAME      40                              // Number of pages reserved at the beginning of QSPI Flash
#define DIR_FILE_COUNT      20                              // Number of entries in the Directory

    // Directory structure
    typedef struct stFile {
        char     Name[MAX_ENTRY_NAME];
        uint32_t Size;
        uint32_t DataAddress;
    } Directory[DIR_FILE_COUNT];

    // Structure of the memory area in QSPI used by the flasher
    typedef uint8_t Page[QSPI_PAGE_SIZE];
    struct stQSPI {
        Page Data[QSPI_PAGE_COUNT];               // Pages used by file data
    };
    static_assert(sizeof(stQSPI) <= QSPI_SIZE, "Memory used > QSPI Flash memory size");


class cQSPI{
	public:
	uint8_t* GetFilePtr(const char *pFileName){
		for(int8_t Index=0; Index < DIR_FILE_COUNT; Index++){
            if(0 == strcmp(Dir[Index].Name, pFileName)){
                return (uint8_t*) Dir[Index].DataAddress;
            }
        }
        return nullptr;
	}
    
    uint32_t GetFileSize(const char* pFileName) {
        for (int8_t Index = 0; Index < DIR_FILE_COUNT; Index++) {
            if (0 == strcmp(Dir[Index].Name, pFileName)) {
                return Dir[Index].Size;
            }
        }
        return 0;
    }
    
/**
 * Retrieves image information from a file stored in directory structure
 * @param pFileName: Name of the file to search for
 * @param ImgPtr: Pointer to store image data (currently unused)
 * @param NbFrame: Reference to store number of frames
 * @param Width: Reference to store image width
 * @param Height: Reference to store image height
 * @return true if file found and valid image format, false otherwise
 */
bool GetImgInformation(const char* pFileName, uint8_t* &ImgPtr, uint8_t &NbFrame, uint16_t &Width, uint16_t &Height) {
    // Input parameter validation
    if (!pFileName) {
        return false;
    }
    
    // Search through directory entries
    for (int8_t Index = 0; Index < DIR_FILE_COUNT; Index++) {
        if (0 == strcmp(Dir[Index].Name, pFileName)) {
            // Get image data pointer and size
            ImgPtr = (uint8_t*) Dir[Index].DataAddress;  // Fixed missing semicolon
            uint32_t Size = Dir[Index].Size;
            
            // Check minimum size to prevent buffer overflow
            if (Size < 16) {
                return false;
            }
            
            // Point to the last 16 bytes where magic signature should be
            uint8_t *pMagic = ImgPtr + Size - 16;
            
            // Check for "IMAG" magic signature (fixed assignment operators to comparison)
            if ((pMagic[0] == 'I') && (pMagic[1] == 'M') && (pMagic[2] == 'A') && (pMagic[3] == 'G')) {
                // Cast to uint32_t pointer to read metadata
                uint32_t *pData = (uint32_t*) &pMagic[4];
                
                // Read metadata values
                NbFrame = (uint8_t) pData[0];
                Width = (uint16_t) pData[1];
                Height = (uint16_t) pData[2];
                    
                    return true;
            }
            // Magic signature not found - invalid image format
            return false;
        }
    }
    // File not found in directory
    return false;
}

	protected :
	Dad::stFile Dir[DIR_FILE_COUNT];
	uint8_t		Data[QSPI_SIZE - sizeof(Dad::Directory)];
};
} //Dad