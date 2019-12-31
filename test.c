#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define MEMORY_SIZE 100000

int main() {
	unsigned long result_1_vir[MEMORY_SIZE];
	unsigned long result_2_vir[MEMORY_SIZE];
	unsigned long result_3_vir[MEMORY_SIZE];
	unsigned long result_vir_temp[MEMORY_SIZE];
	int result_vir_temp_index = 0;
	unsigned long result_1_phy[MEMORY_SIZE];
	unsigned long result_2_phy[MEMORY_SIZE];
	unsigned long result_3_phy[MEMORY_SIZE];
	int exit_status;
	FILE *pResult_1_vir;
	FILE *pResult_2_vir;
	FILE *pResult_3_vir;
	FILE *pResult_1_phy;
	FILE *pResult_2_phy;
	FILE *pResult_3_phy;
	int result_1_all_page = 0;
	int result_2_all_page = 0;
	int result_3_all_page = 0;
	float result_1_phy_assigned = 0;
	float result_2_phy_assigned = 0;
	float result_3_phy_assigned = 0;
	float result_1_percentage;
	float result_2_percentage;
	float result_3_percentage;

	if(fork()) {
		// executed by the parent process
		syscall(351, result_1_vir, result_1_phy);		//location 1
		wait(&exit_status);
		// Code to write the final results in array result_1[] to a file, such as result_1.txt for further analysis.
		pResult_1_vir = fopen("result_1_vir.txt", "w");
		pResult_1_phy = fopen("result_1_phy.txt", "w");
		// pResult_2_phy = fopen("result_1_phy.txt", "w+");
		// pResult_3_phy = fopen("result_1_phy.txt", "w+");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_1_vir[i] == 0) {
				break;
			}
			fprintf(pResult_1_vir, "0x%lx\n", result_1_vir[i]);
			if(i % 2 == 1) {
				result_1_all_page += (result_1_vir[i] - result_1_vir[i-1]) / 0x1000;
				for(int j = 0; j <= (result_1_vir[i] - result_1_vir[i-1]) / 0x1000 - 1; j++) {
					result_vir_temp[j+result_vir_temp_index] = result_1_vir[i-1] + 0x1000 * j;
				}
				result_vir_temp_index += (result_1_vir[i] - result_1_vir[i-1]) / 0x1000;
			}
		}
		for(int i = 0; i <= result_1_all_page - 1; i++) {
			if(result_1_phy[i] != 0) {
				result_1_phy_assigned += 1;
			}
			fprintf(pResult_1_phy, "0x%lx\n", result_1_phy[i]);
		}
		fclose(pResult_1_phy);
		result_1_percentage = (result_1_phy_assigned / result_1_all_page) * 100;
		printf("This is result 1.\nThere are %i pages.\nThere are %.0f physical memory assigned.\nSo the percentage is %.0f / %i = %f%%\n", result_1_all_page, result_1_phy_assigned, result_1_phy_assigned, result_1_all_page, result_1_percentage);
		if(result_vir_temp_index == result_1_all_page) {
			printf("Correct.\n");
		}
		else {
			printf("Something goes wrong.\n");
		}
		unsigned long phy_temp_1;
		unsigned long phy_temp_2;
		unsigned long phy_temp_3;
		pResult_1_phy = fopen("result_1_phy.txt", "r");
		pResult_2_phy = fopen("result_2_phy.txt", "r");
		pResult_3_phy = fopen("result_3_phy.txt", "r");
		for(int i = 0; i <= result_1_all_page - 1; i++) {
			fscanf(pResult_1_phy, "0x%lx\n", &phy_temp_1);
			fscanf(pResult_2_phy, "0x%lx\n", &phy_temp_2);
			fscanf(pResult_3_phy, "0x%lx\n", &phy_temp_3);
			if(phy_temp_1 == phy_temp_2 && phy_temp_1 == phy_temp_3 && phy_temp_1 != 0) {
				printf("At virtual address 0x%lx 1 is assigned the same as physical address of 2 and 3 0x%lx.\n", result_vir_temp[i], phy_temp_1);
			}
			else if(phy_temp_1 == phy_temp_2 && phy_temp_1 != 0) {
				printf("At virtual address 0x%lx, 1, 2, same: 0x%lx.\n", result_vir_temp[i], phy_temp_1);
			}
			else if(phy_temp_1 == phy_temp_3 && phy_temp_1 != 0) {
				printf("At virtual address 0x%lx, 1, 3, same: 0x%lx.\n", result_vir_temp[i], phy_temp_1);
			}
			else if(phy_temp_2 == phy_temp_3 && phy_temp_2 != 0) {
				printf("At virtual address 0x%lx, 2, 3, same: 0x%lx.\n", result_vir_temp[i], phy_temp_2);
			}
		}
		fclose(pResult_1_vir);
		fclose(pResult_1_phy);
		fclose(pResult_2_phy);
		fclose(pResult_3_phy);
	}
	else {
		//executed by the child process
		syscall(351, result_2_vir, result_2_phy);		//location 2
		result_1_vir[0]=123;		//This should trigger copy-on-right, if the above system call does not trigger the COW.
		syscall(351, result_3_vir, result_3_phy);		//location 3
		// Code to write the final results in array result_2[] to a file, such as result_2.txt for further analysis.
		pResult_2_vir = fopen("result_2_vir.txt", "w");
		pResult_2_phy = fopen("result_2_phy.txt", "w");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_2_vir[i] == 0) {
				break;
			}
			fprintf(pResult_2_vir, "0x%lx\n", result_2_vir[i]);
			if(i % 2 == 1) {
				result_2_all_page += (result_2_vir[i] - result_2_vir[i-1]) / 0x1000;
			}
		}
		for(int i = 0; i <= result_2_all_page - 1; i++) {
			if(result_2_phy[i] != 0) {
				result_2_phy_assigned += 1;
			}
			fprintf(pResult_2_phy, "0x%lx\n", result_2_phy[i]);
		}
		result_2_percentage = (result_2_phy_assigned / result_2_all_page) * 100;
		printf("This is result 2.\nThere are %i pages.\nThere are %.0f physical memory assigned.\nSo the percentage is %.0f / %i = %f%%\n", result_2_all_page, result_2_phy_assigned, result_2_phy_assigned, result_2_all_page, result_2_percentage);

		fclose(pResult_2_vir);
		fclose(pResult_2_phy);

		// Code to write the final results in array result_3[] to a file, such as result_3.txt for further analysis.
		pResult_3_vir = fopen("result_3_vir.txt", "w");
		pResult_3_phy = fopen("result_3_phy.txt", "w");
		for(int i = 0; i <= MEMORY_SIZE - 1; i++) {
			if(result_3_vir[i] == 0) {
				break;
			}
			fprintf(pResult_3_vir, "0x%lx\n", result_3_vir[i]);
			if(i % 2 == 1) {
				result_3_all_page += (result_3_vir[i] - result_3_vir[i-1]) / 0x1000;
			}
		}
		for(int i = 0; i <= result_3_all_page - 1; i++) {
			if(result_3_phy[i] != 0) {
				result_3_phy_assigned += 1;
			}
			fprintf(pResult_3_phy, "0x%lx\n", result_3_phy[i]);
		}
		result_3_percentage = (result_3_phy_assigned / result_3_all_page) * 100;
		printf("This is result 3.\nThere are %i pages.\nThere are %.0f physical memory assigned.\nSo the percentage is %.0f / %i = %f%%\n", result_3_all_page, result_3_phy_assigned, result_3_phy_assigned, result_3_all_page, result_3_percentage);
		fclose(pResult_3_vir);
		fclose(pResult_3_phy);
	}

	return 0;
}
