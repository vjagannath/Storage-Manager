all: Assignment1 Testcases

Assignment1:
	gcc -o test_assign1 test_assign1_1.c dberror.c storage_manager.c -I. -std=c99
	./test_assign1
	
Testcases:
	gcc -o sampletest storage_test.c dberror.c storage_manager.c -I. -std=c99