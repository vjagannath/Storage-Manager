#include <stdio.h>
#include <stdlib.h>
#include "storage_mgr.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define total_length 256

SM_FileHandle test_case;

void main(){
SM_PageHandle page_size;
page_size = (char*)calloc(PAGE_SIZE, sizeof(char));
int option, position, number, page_count;
char name_file[total_length],character;

while(1)
{
	printf("\n**********************\n");
	printf("Storage Manager\n"); 
	printf("\n**********************\n"); 
	printf("\n1. Create File\n"); 
	printf("2. Open File\n"); 
	printf("3. Read File\n");
	printf("4. Write File\n");
	printf("5. Block Position\n");
	printf("6. Append Block\n");
	printf("7. File Capacity\n");
	printf("8. Close\n");
	printf("9. Destroy\n");
	printf("10. Exit\n");
	printf("Enter your choice:");
	scanf("%d", &option);
	
	switch(option)
	{
		case 1: 
			printf("Please enter a file name: ");
			scanf("%s", name_file);
			if(createPageFile (name_file) == RC_OK){
				printf("\nFile has been created !!!\n");
			}else{
				printf("\nCould not create file!!!\n");
			}
			break;

		case 2:
			printf("Enter file name to be opened : ");
			scanf("%s", name_file);
			if(openPageFile(name_file, &test_case) == RC_OK){
				printf("\nFile has been opened !!!\n");
			}else{
				printf("\nFile is open or missing !!!\n");
			}
			break;
			
		case 3:
			printf("\n**********************\n");
			printf("Read Operation\n"); 
			printf("\n**********************\n");
			printf("1. Read from a specific Block\n");
			printf("2. Read from the Current Block\n");
			printf("3. Read from the First Block\n");
			printf("4. Read from the Last Block\n");
			printf("5. Read from the Previous Block\n");
			printf("6. Read from the Next Block\n");
			printf("Please enter your choice: ");
			scanf("%d", &option);
			if(option == 1)
			{
				printf("\nPlease enter the page number:");
				scanf("%d", &number);
				if(readBlock(number, &test_case, page_size) == RC_OK)
				{
					printf("\nThe file contains the below data: \n%s\n", page_size);
					printf("\n");
				}
				else
				{
				printf("\nPlease open the file or enter a valid page number\n");
				}
			}
			else if(option == 2)
			{
				if(readCurrentBlock (&test_case, page_size) == RC_FILE_NOT_FOUND)
				{
					printf("\nPlease open the file!!!\n");
				}
				else
				{
					printf("\nThe file contains the below data: \n%s\n\n", page_size);
					printf("\n");
				}
			}
			else if(option == 3)
			{
				if(readFirstBlock (&test_case, page_size) == RC_FILE_NOT_FOUND)
				{
					printf("\nPlease open the file!!!\n");
				}
				else
				{
				printf("\nThe file contains the below data: \n%s\n\n", page_size);
				printf("\n");
				}
			}
			else if(option == 4)
			{
				if(readLastBlock (&test_case, page_size) == RC_FILE_NOT_FOUND)
				{
					printf("\nPlease open the file!!!\n");
				}else
				{
					printf("\nThe file contains the below data: \n%s\n\n", page_size);
					printf("\n");
				}
			}
			else if(option == 5)
			{
				if(readPreviousBlock (&test_case, page_size) == RC_FILE_NOT_FOUND)
				{
					printf("\nPlease open the file!!!\n");
				}
				else
				{
					printf("\nThe file contains the below data: \n%s\n\n", page_size);
					printf("\n");
				}
			}
			else if(option == 6)
			{
				if(readNextBlock (&test_case, page_size) == RC_FILE_NOT_FOUND)
				{
					printf("\nPlease open the file!!!\n");
				}
				else
				{
					printf("\nThe file contains the below data: \n%s\n\n", page_size);
					printf("\n");
				}
			}
			else
			{
				printf("Error!!");
			}
			break;
		
		case 4:
			printf("\n**********************\n");
			printf("Write Operation\n"); 
			printf("\n**********************\n");
			printf("1. Write to the Current Block\n");
			printf("2. Write to a specific block\n");
			printf("Please enter your choice: ");
			scanf("%d", &option);
			if(option == 1)
			{
				printf("Please enter a character: ");
				scanf(" %c", &character);
				memset(page_size, character, PAGE_SIZE);
				if(writeCurrentBlock (&test_case, page_size) == RC_OK)
				{
					printf ("\nThe entered character is written to the current block !!!\n");
				}
				else
				{
					printf("\nPlease open the file!!!\n");
				}
			}
			else if(option == 2)
			{
				printf("Please enter the page number: ");
				scanf("%d", &number);
				printf("Please enter a character: ");
				scanf(" %c", &character);
				memset(page_size, character, PAGE_SIZE);
				if (writeBlock (number, &test_case, page_size) == RC_OK)
				{
					printf("\nThe entered character is written to the block !!!\n");
				}
				else
				{
				printf("\nPlease open the file!!!\n");
				}
			}
			break;
			
		case 5: 
			position = getBlockPos (&test_case);
			if (position == -1)
			{
				printf("\nPlease open the file!!!\n");				
			}
			else
			{
				printf("\nPage position is: %d\n", position);
			}
			break;
			
		case 6:
			if(appendEmptyBlock (&test_case) == RC_FILE_NOT_FOUND)
			{
				printf("\nPlease open the file!!!\n");
			}
			else
			{
				printf("\nEmpty block is appended to the file!!!\n");
			}
			break;
			
		case 7:
			printf("Enter the number of pages:");
			scanf("%d", &page_count);
			if(ensureCapacity (page_count, &test_case) == RC_FILE_NOT_FOUND)
			{
				printf("\nPlease open the file!!!\n");
			}
			else
			{
				printf ("\nPage count is: %d", page_count);
			}
			break;
			
						
		case 8:
			if(closePageFile(&test_case) == RC_FILE_NOT_FOUND)
			{
				printf("\nPlease open the file!!!\n");
			}
			else
			{
				printf("\nFile closed successfully !!!\n");
			}
			break;
			
		case 9: 
			printf("Enter file name to be deleted : ");
			scanf("%s", name_file);
			RC status = destroyPageFile(name_file);
			if(status == RC_WRITE_FAILED)
			{
				printf("\nFile not deleted !!!\n");
			}
			else if(status == RC_FILE_NOT_FOUND)
			{
				printf("\nFile not found !!! Please enter a valid file name!!!\n");
			}
			else
			{
				printf("\nFile destroyed successfully !!!\n");
			}
			break;

		case 10:
			exit(0);

		default:
			printf("\nPlease enter a valid choice !!!\n");
	}//case
}//while

}
