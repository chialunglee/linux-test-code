#include <stdio.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/wait.h>
#define MEMORY_SIZE 100000

int main() {
	unsigned long result_1[MEMORY_SIZE];
	unsigned long result_2[MEMORY_SIZE];
	unsigned long result_3[MEMORY_SIZE];
	int exit_status;
	FILE *pResult_1;
	FILE *pResult_2;
	FILE *pResult_3;
	unsigned long result_1_all_page[MEMORY_SIZE];
	unsigned long result_2_all_page[MEMORY_SIZE];
	unsigned long result_3_all_page[MEMORY_SIZE];
	int all_page_index = 0;
	int all_page_temp;

	if(fork()) {
		// executed by the parent process
		syscall(351, result_1);		//location 1
		wait(&exit_status);
		// Code to write the final results in array result_1[] to a file, such as result_1.txt for further analysis.
		pResult_1 = fopen("result_1.txt", "w");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_1[i] == 0) {
				break;
			}
			fprintf(pResult_1, "0x%lx\n", result_1[i]);
			if(i % 2 == 1) {
				all_page_temp = result_1[i-1];
				do {
					result_1_all_page[all_page_index] = all_page_temp;
					all_page_temp += 0x1000;
					all_page_index += 1;
				}while(all_page_temp < result_1[i]);
			}
		}
		fclose(pResult_1);

		printf("This is result_1_all_page:\n");
		for(int i = 0; i <= MEMORY_SIZE; i++) {
			if(result_1_all_page[i] == 0) {
				break;
			}
			printf("0x%lx\n", result_1_all_page[i]);
		}
	}
	else {
		//executed by the child process
		syscall(351, result_2);		//location 2
		result_1[0]=123;		//This should trigger copy-on-right, if the above system call does not trigger the COW.
		syscall(351, result_3);		//location 3
		// Code to write the final results in array result_2[] to a file, such as result_2.txt for further analysis.
		pResult_2 = fopen("result_2.txt", "w");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_2[i] == 0) {
				break;
			}
			fprintf(pResult_2, "0x%lx\n", result_2[i]);
			/*if(i % 2 == 1) {
				all_page_temp = result_2[i-1];
				do {
					result_1_all_page[all_page_index] = all_page_temp;
					all_page_temp += 0x1000;
					all_page_index += 1;
				}while(all_page_temp <= result_2[i]);
			}*/
		}
		fclose(pResult_2);

		/*printf("This is result_2_all_page:\n");
		for(int i = 0; i <= MEMORY_SIZE; i++) {
			if(result_2_all_page[i] == 0) {
				break;
			}
			printf("0x%lx\n", result_2_all_page[i]);
		}*/
		// Code to write the final results in array result_3[] to a file, such as result_3.txt for further analysis.
		pResult_3 = fopen("result_3.txt", "w");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_3[i] == 0) {
				break;
			}
			fprintf(pResult_3, "0x%lx\n", result_3[i]);
			/*if(i % 2 == 1) {
				all_page_temp = result_3[i-1];
				do {
					result_1_all_page[all_page_index] = all_page_temp;
					all_page_temp += 0x1000;
					all_page_index += 1;
				}while(all_page_temp <= result_3[i]);
			}*/
		}
		fclose(pResult_3);

		/*printf("This is result_3_all_page:\n");
		for(int i = 0; i <= MEMORY_SIZE; i++) {
			if(result_3_all_page[i] == 0) {
				break;
			}
			printf("0x%lx\n", result_3_all_page[i]);
		}*/
	}

	return 0;
}
