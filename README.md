# Storage-Manager
Implementation of a storage manager capable of performing read/write to/from a file on disk in terms of blocks

A storage manager is a program module which is responsible for storing, retrieving and updating data in the database.
The storage manager deals with fixed size Pages or Blocks. The reading and writing page operations from a file are 
provided by the storage manager, additionally it also provides creating, opening, and closing files methods.
It is used to read blocks from files on the local disk and write it to memory and viceversa.

How To Compile and Run
*****************************
==> Enter 'make' command in the terminal (To build and run the default test cases).
==> Enter './test_assign1' command in the terminal (To run the defualt test cases).
==> Enter './sampletest' command in the terminal (To run the additional test cases).


Implementation Details
**************************
1. CreatePageFile
-----------------------
It creates a file with the given 'filename' and adds an empty Page or Block  at the beginning of the file, which is filled with '\0' bytes.
Each block in the file is assumed to be of size 4096 bytes.

The below Pseudocode explains the operations of CreatePageFile:	 

==> Create a file with the given file name
==> Open the file in write mode
 
==> Create a buffer (page) equivalent to PAGE_SIZE(4096 bytes) filled with '\0' characters
==> Write number of pages being added to the file

==> Write the buffer to the file

==> Close the file	

Note: EACH PAGE IS IMPLEMENTED AS ONE BLOCK OF MEMORY


2. OpenPageFile
----------------------
The given 'filename' is validated to check if it exists. If the file exists it is opened in editable mode.

The below Pseudocode explains the operations of OpenPageFile:	 

==> Check if the given file exists

==> If the file exists, Open the file in read and write mode

==> Update the file handle structure attributes: 
	number of total pages in the file ( written at the beginning of the file ), 
file name,
 current page position (for reading and writing),
 file pointer
.

3. ClosePageFile
----------------------
The given 'filename' is validated to check if it exists. If the file is open it is closed.

The below Pseudocode explains the operations of ClosePageFile:
==> Check if the given file is open

==> If yes, close the file

==> Reset the file handle structure attributes: 
		
	number of total pages in the file ( written at the beginning of the file ),
 file name,
 current page position (for reading and writing),
	 file pointer.


4. DestroyPageFile
------------------------
The specified file is deleted.

The below Pseudocode explains the operations of DestroyPageFile:
==> Check if the given file exists
 
==> If the file exists, check if the file is still open.
 
==> If the file is open, then close the file
 
==> Remove the file 
 
==> Reset the file handle structure attributes:
		
	number of total pages in the file ( written at the beginning of the file ),
 file name,
 current page position (for reading and writing),
 file pointer
.

5. ReadBlock
-----------------
==> The specified Block is validated and read if exists.

The below Pseudocode explains the operations of ReadBlock:
==> Check if the file is open

==> If yes, check if the page(block) to be read exists

==> If the page exists, calculate the offset of the block to be read

==> Set the file pointer to the correct offset (beginning of the page/block)

==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)

==> Update the file handle structure attributes:
		
	current page position (for reading and writing)

           	Set the file pointer back to the beginning of the page/block


6. GetBlockPosition
-------------------------
It returns the current Page or Block position in the file.

The below Pseudocode explains the operations of GetBlockPosition:
==> Check if the file is open

==> If yes, return the current page position


7. ReadFirstBlock 
-----------------------
It reads the first block in the file.

The below Pseudocode explains the operations of ReadFirstBlock:
==> Check if the file is open
 
==> If yes, check if the page(block) = 0 to be read exists
 
==> If the page exists, calculate the offset of the block to be read
 
==> Set the file pointer to the correct offset (beginning of the page/block)
 
==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
 
==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block


8. ReadPreviousBlock 
----------------------------
The previous block of the opened page is read.

The below Pseudocode explains the operations of ReadPreviousBlock:
==> Check if the file is open
 
==> If yes, check if the page(block) = (current page position - 1) to be read exists
 
==> If the page exists, calculate the offset of the block to be read
 
==> Set the file pointer to the correct offset (beginning of the page/block)
 
==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
 
==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block


9. ReadCurrentBlock
---------------------------
The current block of the opened page is read.

The below Pseudocode explains the operations of ReadCurrentBlock:
==> Check if the file is open
 
==> If yes, check if the page(block) = current page position to be read exists
 
==> If the page exists, calculate the offset of the block to be read
 
==> Set the file pointer to the correct offset (beginning of the page/block)
 
==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
 
==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block



10. ReadNextBlock 
-------------------------
The next block of the opened page is read.

The below Pseudocode explains the operations of ReadNextBlock:
==> Check if the file is open
 
==> If yes, check if the page(block) = (current page position + 1) to be read exists
 
==> If the page exists, calculate the offset of the block to be read
 
==> Set the file pointer to the correct offset (beginning of the page/block)
 
==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
 
==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block


11. ReadLastBlock
------------------------
The last block of the opened page is read.

The below Pseudocode explains the operations of ReadLastBlock:
==> Check if the file is open
 
==> If yes, check if the page(block) = (fHandle->totalNumPages - 1) exists
 
==> If the page exists, calculate the offset of the block to be read
 
==> Set the file pointer to the correct offset (beginning of the page/block)
 
==> Read a page/block from the file - offsets the file pointer (fHandle->mgmtInfo)
 
==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block


12. WriteBlock
-------------------
It is used to write a block of data to the opened file.

The below Pseudocode explains the operations of WriteBlock:
==> Check if the file is open
==> 
If yes, check if the page(block) to be written exists

==> If the page exists, calculate the offset of the block to be read
==> 
Set the file pointer to the correct offset (beginning of the page/block)

==> Write a page/block to the file - offsets the file pointer (fHandle->mgmtInfo)

==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block




13. WriteCurrentBlock
----------------------------
It is used to write a block of data to the current block or page.

The below Pseudocode explains the operations of WriteCurrentBlock:
==> Check if the file is open
==> If yes, check if the page(block) = (current page position) to be written exists

==> If the page exists, calculate the offset of the block to be read
==> 
Set the file pointer to the correct offset (beginning of the page/block)

==> Write a page/block to the file - offsets the file pointer (fHandle->mgmtInfo)

==> Update the file handle structure attributes:
		
	current page position (for reading and writing)
		
	Set the file pointer back to the beginning of the page/block


14. AppendEmptyBlock
-----------------------------
An empty block is appended to the end of an open file.

The below Pseudocode explains the operations of AppendEmptyBlock:
==> Check if the file is open

==> If yes, Create a buffer (page) equivalent to PAGE_SIZE(4096 bytes) filled with '\0' characters
==> 
Update the file handle structure attributes:
		
	totalNumPages
		
	current page position (for reading and writing)	

==> Write the updated information into the file - total number of pages in the file - at the beginning of the file

==> Write the empty buffer(page) created to the end of the file

==> Set the file pointer back to the beginning of the page/block


15. EnsureCapacity
-------------------------
The number of pages in a given file is increased to the given capacity, if it more than the current number of pages.

The below Pseudocode explains the operations of EnsureCapacity:
==> Check if the file is open
==> 
If yes, check if the new capacity is more than current capacity

==> If yes, append empty pages(blocks) ensuring the capacity is met



 Make File Operations
*************************
The make file can be run using the "make" command.
==> To run all the default testcases and additional testcases, please use the command -- make.
==> To run only the default testcases, please use the command -- make Assignment1.
==> To run only the additional testcases, please use the command -- make Testcases.


File Details
**************************
1. Storage_manager.c
----------------------
This file has the default testcases. The storage manager file contains the functions which defines the Create, Open, Read, Write and Destroy operations.

2. Storage_mgr.h
----------------------
This file provides the storage manager interface, which includes manipulating, reading and writing bloks to a page file.

3. test_assign1_1.c
-------------------------
This file contains the main function which runs all the test cases, which is used to test the storage manager application.

4. Storage_test.c 
-----------------
This file has the additional testcases. An user interface is provided for different file operations such as reading and writing first, current and previous block, etc.
