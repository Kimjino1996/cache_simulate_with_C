#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

typedef struct cache {

	int valid;
	int tag;
	int *data;

}cache;

void setIndex(int address, int *index, int *tag, int *offset);
int setaddress(int index, int tag, int offset);
int to_hexa(int data);

float total_mem_access;
int access_count;
int miss;
float mem_access_time;

int cache_size;
int block_size;

int  main(int ac, char* av[])
{
	
	FILE *f;
	char fname[] = "mem_access2.txt";
	char buf[BUFSIZ];
	char key;
	int address;
	int data;
	int *memory;
	int clock_rate;
	int mem_la_cycle;
	float clock_cycle;
	int clock_cnt = 0;
	int set_size;

	int index;
	int s_index;
	int tag;
	int offset;


	char text[100];
	int* time;

	int i;
	int j;
	int k;


	FILE *f2;
	clock_t start_point,end_point;
	
	cache* c_memory;
	cache* print_cmem;
	cache* r_cmem;
	cache** set;

	
	f2 = fopen(av[1], "r");
		
		fscanf(f2, "%s", text);
			fscanf(f2, "%d", &clock_rate);
		fscanf(f2, "%s", text);
		
			fscanf(f2, "%d", &mem_la_cycle);
		fscanf(f2, "%s", text);
		
			fscanf(f2, "%d", &cache_size);
		
		fscanf(f2, "%s", text);
			fscanf(f2, "%d", &block_size);
		fscanf(f2, "%s", text);
			fscanf(f2, "%d", &set_size);
		
		
	
	printf("clock_rate(Ghz): %d\n", clock_rate);
	printf("mem_access_latency(cycles): %d\n", mem_la_cycle);
	printf("L1_size(Bytes): %d\n", cache_size);
	printf("L1_line_size(Bytes): %d\n", block_size);
	fclose(f2);
	start_point=clock();	
	if (cache_size != 0 && block_size != 0) {
		c_memory = (cache*)malloc(sizeof(cache)*cache_size / block_size);

		for (i = 0; i<cache_size / block_size; i++) {
			c_memory[i].data = (int*)malloc(sizeof(int)*block_size / 4);
			for (j = 0; j < block_size / 4; j++) {
				c_memory[i].data[j] = 0;
			}
			c_memory[i].valid = 0;
			c_memory[i].tag = 0;
		}
	}

	total_mem_access = 0.0;

	access_count = 0;
	miss = 0;

	clock_cycle = 1 / (float)clock_rate;
	mem_access_time = clock_cycle * (float)mem_la_cycle;

	f = fopen(av[2], "r");

	memory = (int*)calloc(sizeof(int), 1024 * 1024 * 16);
	i = 0;


	if (set_size == 1) {
		
		if (block_size == 0 || cache_size == 0) {

			while (1) {
				if (fscanf(f, "%c ", &key)>0) {

					if (key == 'R') {
						fscanf(f, "%d", &address);
						address = address / 4;
						access_count++;
						total_mem_access = total_mem_access + mem_access_time;

					}

					else if (key == 'W') {
						fscanf(f, "%d", &address);
						address = address / 4;
						fscanf(f, "%d", &data);
						memory[address] = data;
						access_count++;
						total_mem_access = total_mem_access + mem_access_time;
					}
				}
				else
					break;
			}
			
		}


		else if (block_size == 4)
		{
			while (1) {
				if (fscanf(f, "%c ", &key)>0) {
					if (key == 'R') {
						fscanf(f, "%d", &address);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						
						if (c_memory[index].valid == 0) {
							miss++;
							clock_cnt += 2;
							total_mem_access = total_mem_access + mem_access_time;
							c_memory[index].valid = 1;
							c_memory[index].tag = tag;
							c_memory[index].data[0] = memory[setaddress(index, tag, offset) / 4];
							
						}
						else {
							if (c_memory[index].tag != tag) {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);

								memory[setaddress(index, c_memory[index].tag, offset) / 4] = c_memory[index].data[0];
								
								c_memory[index].tag = tag;
								
								c_memory[index].data[0] = memory[setaddress(index, tag, offset) / 4];
								

							}
							else {

								clock_cnt += 2;
							}
						}

					}

					else if (key == 'W') {
						fscanf(f, "%d", &address);
						fscanf(f, "%d", &data);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						
						if (c_memory[index].valid == 0) {
							clock_cnt += 2;
							miss++;
							c_memory[index].valid = 1;
							c_memory[index].tag = tag;
							c_memory[index].data[offset / 4] = data;
						}
						else {
							if (c_memory[index].tag != tag) {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time);

								memory[setaddress(index, c_memory[index].tag, offset) / 4] = c_memory[index].data[0];
								
								c_memory[index].tag = tag;

								c_memory[index].data[0] = memory[setaddress(index, tag, offset) / 4];
								
								c_memory[index].data[0] = data;
							}
							else {
								
								c_memory[index].data[0] = data;
								clock_cnt += 2;
							}


						}
					}
				}
				else
					break;
			}
		}
		else {
			while (1) {
				if (fscanf(f, "%c ", &key) > 0) {
					if (key == 'R') {
						fscanf(f, "%d", &address);
						access_count++;

						setIndex(address, &index, &tag, &offset);

						if (c_memory[index].valid == 0) {
							miss++;
							clock_cnt += 2;
							total_mem_access = total_mem_access + mem_access_time;
							c_memory[index].valid = 1;
							c_memory[index].tag = tag;
							for (i = 0; i < (block_size / 4); i++) {
								c_memory[index].data[i] = memory[setaddress(index, tag, i * 4) / 4];

							}
						}

						else {
							if (c_memory[index].tag != tag) {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);
								for (i = 0; i < (block_size / 4); i++) {
									memory[setaddress(index, c_memory[index].tag, i * 4) / 4] = c_memory[index].data[i];
								}

								c_memory[index].tag = tag;
								for (i = 0; i < (block_size / 4); i++) {

									c_memory[index].data[i] = memory[setaddress(index, tag, i * 4) / 4];

								}

							}
							else {

								clock_cnt += 2;
							}
						}

					}

					else if (key == 'W') {
						fscanf(f, "%d", &address);
						fscanf(f, "%d", &data);
						access_count++;
						setIndex(address, &index, &tag, &offset);

						if (c_memory[index].valid == 0) {
							miss++;
							clock_cnt += 2;
							total_mem_access = total_mem_access + mem_access_time;

							c_memory[index].valid = 1;
							c_memory[index].tag = tag;
							for (i = 0; i < (block_size / 4); i++) {

								c_memory[index].data[i] = memory[setaddress(index, tag, i * 4) / 4];

							}
							c_memory[index].data[offset / 4] = data;
						}
						else {
							if (c_memory[index].tag != tag) {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);
								for (i = 0; i < (block_size / 4); i++) {
									memory[setaddress(index, c_memory[index].tag, i * 4) / 4] = c_memory[index].data[i];
								}

								c_memory[index].tag = tag;
								for (i = 0; i < (block_size / 4); i++) {

									c_memory[index].data[i] = memory[setaddress(index, tag, i * 4) / 4];

								}
								c_memory[index].data[offset / 4] = data;
							}
							else {
								c_memory[index].data[offset / 4] = data;
								clock_cnt += 2;
							}


						}
					}
				}
				else {
					
					
					break;
				}
				
			}

		}
		
		printf("Average memory access latency without cache: %.1f ns\n", (float)mem_la_cycle / (float)clock_rate);
		
		if (block_size == 0 || cache_size == 0)return;
		for (i = 0; i<cache_size / block_size; i++) {

		
			printf("cache index %d:", i);
			printf("valid = ");
			to_hexa(c_memory[i].valid);
			printf("tag = ");
			to_hexa(c_memory[i].tag);
			printf("data = ");
			for (j = 0; j < block_size / 4; j++) {
				to_hexa(c_memory[i].data[j]);
			
				printf("  ");
			}
			printf("\n");
		}
	}

	else {


		set = (cache**)malloc(sizeof(cache*)*(cache_size / block_size / set_size));
		time = (int *)malloc(sizeof(int)*(cache_size / block_size / set_size));
		for (i = 0; i < (cache_size / block_size / set_size); i++) {
			set[i] = (cache*)malloc(sizeof(cache)*set_size);
			time[i] = 0;
			for (j = 0; j < set_size; j++) {
				set[i][j].valid = 0;
				set[i][j].tag = 0;
				set[i][j].data = (int*)malloc(sizeof(int)*block_size / 4);
				for (k = 0; k < (block_size / 4); k++) {
					set[i][j].data[k] = 0;
				}

			}
		}
		if (block_size == 4)
		{
			while (1) {
				if (fscanf(f, "%c ", &key)>0) {
					if (key == 'R') {
						fscanf(f, "%d", &address);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						s_index = index / 4;

						for (i = 0; i < set_size; i++) {
							if (set[s_index][i].valid == 0|| set[s_index][i].tag == tag) {
								break;
							}

						}
						if (i != set_size) {  
							if (set[s_index][i].tag == tag) {
								clock_cnt += 2;
							}
							else {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + mem_access_time;
								set[s_index][time[s_index]].valid = 1;
								set[s_index][time[s_index]].tag = tag;
								for (j = 0; j < (block_size / 4); j++) {
									set[s_index][time[s_index]].data[j] = memory[setaddress(index, tag, j * 4) / 4];
								}
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							}

						}
						else { 
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);

								for (k = 0; k < (block_size / 4); k++) {
									memory[setaddress(index, set[s_index][time[s_index]].tag, k * 4) / 4] = set[s_index][time[s_index]].data[k];
								}
								set[s_index][time[s_index]].tag = tag;
								for (k = 0; k < (block_size / 4); k++) {
									set[s_index][time[s_index]].data[k] = memory[setaddress(index, tag, k * 4) / 4];
								}
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							
							
						}
					}

					else if (key == 'W') {
						fscanf(f, "%d", &address);
						fscanf(f, "%d", &data);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						s_index = index / 4;
						for (i = 0; i < set_size; i++) {
							if (set[s_index][i].valid == 0 || set[s_index][i].tag == tag) {
								break;
							}
						}
						if (i != set_size) {  
							if (set[s_index][i].tag == tag) {
								clock_cnt += 2;
								set[s_index][time[s_index]].data[offset / 4] = data;
							}
							else {
								clock_cnt += 2;
								miss++;
								set[s_index][time[s_index]].valid = 1;
								set[s_index][time[s_index]].tag = tag;
								set[s_index][time[s_index]].data[offset / 4] = data;
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							}
						}

						else { 
									
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time);

								for (k = 0; k < (block_size / 4); k++) {
									memory[setaddress(index, set[s_index][time[s_index]].tag, k * 4) / 4] = set[s_index][time[s_index]].data[k];
								}
								set[s_index][time[s_index]].tag = tag;
								for (k = 0; k < (block_size / 4); k++) {
									set[s_index][time[s_index]].data[k] = memory[setaddress(index, tag, k * 4) / 4];
								}
								set[s_index][time[s_index]].data[offset / 4] = data;
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;

						}
					}
				}
				else
					break;
			}
		}
		else {
			while (1) {
				if (fscanf(f, "%c ", &key)>0) {
					if (key == 'R') {
						fscanf(f, "%d", &address);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						s_index = index / 4;

						for (i = 0; i < set_size; i++) {
							if (set[s_index][i].valid == 0 || set[s_index][i].tag == tag) {
								break;
							}

						}
						if (i != set_size) {  
							if (set[s_index][i].valid == 0) {
								clock_cnt += 2;
							}
							else {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + mem_access_time;
								set[s_index][time[s_index]].valid = 1;
								set[s_index][time[s_index]].tag = tag;
								for (j = 0; j < (block_size / 4); j++) {
									set[s_index][time[s_index]].data[j] = memory[setaddress(index, tag, j * 4) / 4];
								}
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							}

						}
						else { 

							miss++;
							clock_cnt += 2;
							total_mem_access = total_mem_access + (mem_access_time * 2);

							for (k = 0; k < (block_size / 4); k++) {
								memory[setaddress(index, set[s_index][time[s_index]].tag, k * 4) / 4] = set[s_index][time[s_index]].data[k];
							}
							set[s_index][time[s_index]].tag = tag;
							for (k = 0; k < (block_size / 4); k++) {
								set[s_index][time[s_index]].data[k] = memory[setaddress(index, tag, k * 4) / 4];
							}
							time[s_index] = time[s_index] + 1;
							time[s_index] = time[s_index] % set_size;


						}
					}

					else if (key == 'W') {
						fscanf(f, "%d", &address);
						fscanf(f, "%d", &data);
						access_count++;
						setIndex(address, &index, &tag, &offset);
						s_index = index / 4;
						for (i = 0; i < set_size; i++) {
							if (set[s_index][i].valid == 0 || set[s_index][i].tag == tag) {
								break;
							}
						}
						if (i != set_size) {   
							if (set[s_index][i].valid == 0) {
								set[s_index][time[s_index]].data[offset / 4] = data;
								clock_cnt += 2;
							}
							
							else {
								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);
								set[s_index][time[s_index]].valid = 1;
								set[s_index][time[s_index]].tag = tag;
								for (k = 0; k < (block_size / 4); k++) {
									set[s_index][time[s_index]].data[k] = memory[setaddress(index, tag, k * 4) / 4];
								}
								set[s_index][time[s_index]].data[offset / 4] = data;
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							}
						}

						else { 


								miss++;
								clock_cnt += 2;
								total_mem_access = total_mem_access + (mem_access_time * 2);

								for (k = 0; k < (block_size / 4); k++) {
									memory[setaddress(index, set[s_index][time[s_index]].tag, k * 4) / 4] = set[s_index][time[s_index]].data[k];
								}
								set[s_index][time[s_index]].tag = tag;
								for (k = 0; k < (block_size / 4); k++) {
									set[s_index][time[s_index]].data[k] = memory[setaddress(index, tag, k * 4) / 4];
								}
								set[s_index][time[s_index]].data[offset / 4] = data;
								time[s_index] = time[s_index] + 1;
								time[s_index] = time[s_index] % set_size;
							
						}
					}
				}
				else {

					break;
				}
			}

		}

		printf("Average memory access latency without cache: %.1f ns\n", (float)mem_la_cycle / (float)clock_rate);
		for (i = 0; i<cache_size / block_size/set_size; i++) {


			printf("cache index %d:", i);
			printf("data = ");
			for (j = 0; j < set_size; j++) {
				for (k = 0; k < block_size/4; k++) {
					to_hexa(set[i][j].data[k]);
					printf("\t");
				}
				
			}
			printf("\n");
		}

	}
	
	end_point=clock();	

	printf("Total program run time: %.5f\n", ((float)end_point - (float)start_point)/CLOCKS_PER_SEC);
	printf("L1 total access : %d\n", access_count);
	printf("L1 total miss_count: %d\n", miss);
	printf("L1 miss rate: %.2f%% \n", ((float)miss / (float)access_count)*100.0);
	printf("Average memory acess latency: %.2f ns\n", ((total_mem_access + ((float)clock_cnt*(1/(float)clock_rate))) / (float)access_count));

	free(c_memory);
	free(memory);

}
int to_hexa(int data) {
	int i;
	int cur;
	int word = 'A';
	for (i = 0; i < 8; i++) {
		cur = data >> (28 - (4 * i));
		cur = cur % 16;

		if (cur >= 10) {
			printf("%c", word + cur - 10);
		}
		else {
			printf("%d", cur);
		}
	}

}


void setIndex(int address, int *index, int *tag, int *offset) {
	int b_offset;
	int index_offset;

	*offset = address & (block_size - 1);
	b_offset = byteoffset(block_size);
	address = address >> b_offset;// byteoffset

	*index = address & ((cache_size / block_size) - 1);
	index_offset = byteoffset((cache_size / block_size));
	address = address >> index_offset;

	tagbit(32 - index_offset - b_offset);
	*tag = address & (tagbit(32 - index_offset - b_offset) - 1); //4194304
}
int setaddress(int index, int tag, int offset) {
	int to_address_tag;
	int to_address_index;
	int to_address_offset;
	int address;
	to_address_offset = offset;
	to_address_tag = tag << (byteoffset((cache_size / block_size))+ byteoffset(block_size));
	to_address_index = index << byteoffset(block_size);
	address = to_address_offset + to_address_tag + to_address_index;
	return address;


}


int byteoffset(int block) {

	int i = 0;
	while (1) {
		block = block / 2;
		i++;
		if (block == 1)break;
	}
	return i;
}
int tagbit(int data) {

	int i;
	int total = 1;
	for (i = 0; i < data; i++) {
		total *= 2;
	}
	return total;
}
