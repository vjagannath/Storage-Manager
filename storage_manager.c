#include <stdio.h>
#include <stdlib.h>
#include "storage_mgr.h"
#include <sys/stat.h>

#define CLEAN_FILE_HANDLE(fHandle) \
do                                 \
{								   \
	fHandle->fileName = NULL;	   \
	fHandle->totalNumPages = 0;    \
	fHandle->curPagePos = 0;       \
	fHandle->mgmtInfo = NULL;      \
} while (0)                        \

char *RC_message = NULL;
FILE* openFileHandle;

void initStorageManager ()
{
	openFileHandle = NULL;
}

RC createPageFile (char *fileName)
{
	/* Pseudocode:	 
	 * Create a file with the given file name
	 * Open the file in write mode 
	 * Create a buffer (page) equivalent to PAGE_SIZE(4096 bytes) filled with '\0' characters
	 * Write number of pages being added to the file
	 * Write the buffer to the file
	 * Close the file	 * 
	 * Note: EACH PAGE IS IMPLEMENTED AS ONE BLOCK OF MEMORY
	 */
	
	FILE *fd = fopen(fileName, "w");
	if (fd == NULL)
	{
		RC_message = "could not create file";
		return RC_FILE_NOT_FOUND;
	}

	int numberOfPages = 1;
	fwrite(&numberOfPages, sizeof(int), 1, fd);

	char *emptyPage = (char *) malloc(PAGE_SIZE);
	for(int byteIndex = 0; byteIndex < PAGE_SIZE; byteIndex++)
	{
		*(emptyPage + byteIndex) = '\0';
	}		
	fwrite(emptyPage, sizeof(char), PAGE_SIZE, fd);

	free(emptyPage);
	fclose(fd);

	return RC_OK;
}


RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
	/* Pseudocode:
	 * Check if the given file exists
	 * If the file exists, Open the file in read and write mode
	 * Update the file handle structure attributes:
	 *		number of total pages in the file ( written at the beginning of the file ), 
	 *		file name,
	 *		current page position (for reading and writing),
	 *		file pointer
	 */

	FILE *fd = fopen(fileName, "r+");
	if(fd != NULL)
	{
		int numberOfPages;
		fread(&numberOfPages, sizeof(int), 1, fd);

		fHandle->fileName = fileName;
		fHandle->totalNumPages = numberOfPages;
		fHandle->curPagePos = 0;
		fHandle->mgmtInfo = fd;
		openFileHandle = fd;

		return RC_OK;
	}
	
	RC_message = "could not open page file";
	return RC_FILE_NOT_FOUND;
}


RC closePageFile (SM_FileHandle *fHandle)
{
	/* Pseudocode:
	* Check if the given file is open
	* If yes, close the file
	* Reset the file handle structure attributes: 
	*		number of total pages in the file ( written at the beginning of the file ),
	*		file name,
	*		current page position (for reading and writing),
	*		file pointer
	*/

	if (fHandle->fileName != NULL)
	{
		fclose(fHandle->mgmtInfo);
		CLEAN_FILE_HANDLE(fHandle);
		openFileHandle = NULL;
		return RC_OK;
	}	
	
	RC_message = "error closing file";
	return RC_FILE_NOT_FOUND;
}


RC destroyPageFile (char *fileName)
{
	/* Pseudocode:
	* Check if the given file exists
	* If the file exists, check if the file is still open.
	* If the file is open, then close the file
	* Remove the file 
	* Reset the file handle structure attributes:
	*		number of total pages in the file ( written at the beginning of the file ),
	*		file name,
	*		current page position (for reading and writing),
	*		file pointer
	*/

	struct stat buffer;
	if (stat(fileName, &buffer) != 0)
	{
		return RC_FILE_NOT_FOUND;
	}
	if(openFileHandle)
	{
		fclose(openFileHandle);
		openFileHandle = NULL;
	}
	if (remove(fileName) == RC_OK)
	{	
		return 	RC_OK;
	}

	RC_message = "could not delete file";
	return RC_FILE_NOT_FOUND;
}


RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) to be read exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	if (fHandle->fileName == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	if (pageNum >= 0 && pageNum < fHandle->totalNumPages)
	{		
		long int fileOffset = pageNum * PAGE_SIZE + sizeof(int);
		fseek(fHandle->mgmtInfo, fileOffset, SEEK_SET);
		fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
		
		fHandle->curPagePos = pageNum;
		fseek(fHandle->mgmtInfo, fileOffset, SEEK_SET);

		return RC_OK;
	}

	return RC_READ_NON_EXISTING_PAGE;
}


int getBlockPos (SM_FileHandle *fHandle)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, return the current page position
	*/

	if (fHandle->fileName == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	return fHandle->curPagePos;
}


RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) = 0 to be read exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = readBlock(0, fHandle, memPage);
	return status;
}


RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) = (current page position - 1) to be read exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = readBlock(fHandle->curPagePos - 1, fHandle, memPage);
	return status;
}


RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) = (current page position) to be read exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = readBlock(fHandle->curPagePos,fHandle, memPage);
	return status;
}


RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) = (current page position + 1) to be read exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = readBlock(fHandle->curPagePos + 1, fHandle, memPage);	
	return status;
}


RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the last page(block) = (fHandle->totalNumPages - 1) exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
	return status;
}


RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) to be written exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Write a page/block to the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	if (fHandle->fileName == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}			

	if (pageNum >= 0 && pageNum < fHandle->totalNumPages)
	{
		if (memPage == NULL)
		{
			return RC_WRITE_FAILED;
		}

		long int fileOffset = pageNum * PAGE_SIZE + sizeof(int);
		fseek(fHandle->mgmtInfo, fileOffset, SEEK_SET);
		fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

		fHandle->curPagePos = pageNum;
		fseek(fHandle->mgmtInfo, fileOffset, SEEK_SET);

		return RC_OK;
	}
	return RC_FILE_NOT_FOUND;
}


RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{	
	/* Pseudocode:
	* Check if the file is open
	* If yes, check if the page(block) = (current page position) to be written exists
	* If the page exists, calculate the offset of the block to be read
	* Set the file pointer to the correct offset (beginning of the page/block)
	* Write a page/block to the file - offsets the file pointer (fHandle->mgmtInfo)
	* Update the file handle structure attributes:
	*		current page position (for reading and writing)
	*		Set the file pointer back to the beginning of the page/block
	*/

	int status = writeBlock(fHandle->curPagePos, fHandle, memPage);
	return status;
}


RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	/* Pseudocode:
	* Check if the file is open
	* If yes, Create a buffer (page) equivalent to PAGE_SIZE(4096 bytes) filled with '\0' characters
	* Update the file handle structure attributes:
	*		totalNumPages
	*		current page position (for reading and writing)		
	* Write the updated information into the file - total number of pages in the file - at the beginning of the file
	* Write the empty buffer(page) created to the end of the file
	* Set the file pointer back to the beginning of the page/block
	*/

	if (fHandle->fileName == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	fHandle->totalNumPages++;
	fseek(fHandle->mgmtInfo, 0, SEEK_SET);
	fwrite(&fHandle->totalNumPages, sizeof(int), 1, fHandle->mgmtInfo);

	fHandle->curPagePos = fHandle->totalNumPages - 1;
	char *emptyPage = (char *)malloc(PAGE_SIZE);
	for (int byteIndex = 0; byteIndex < PAGE_SIZE; byteIndex++)
	{
		*(emptyPage + byteIndex) = '\0';
	}
	fseek (fHandle->mgmtInfo, 0, SEEK_END);
	fwrite(emptyPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

	long int fileOffset = (fHandle->totalNumPages - 1) * PAGE_SIZE + sizeof(int);
	fseek(fHandle->mgmtInfo, fileOffset, SEEK_SET);

	free(emptyPage);

	return RC_OK;
}


RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
	/* Pseudocode:e
	* Check if the file is open
	* If yes, check if the new capacity is more than current capacity
	* If yes, append empty pages(blocks) ensuring the capacity is met
	*/

	if (fHandle->fileName == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	int numberOfAdditionalPages = numberOfPages - fHandle->totalNumPages;
	while (numberOfAdditionalPages)
	{
		appendEmptyBlock(fHandle);
		numberOfAdditionalPages--;
	}
	return RC_OK;
}
