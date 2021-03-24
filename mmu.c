#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_PAGES 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB 16
#define MASK 255
#define OFFSET 8
#define CHARS 10

int main(int argc, char *argv[]) {

    if(argc != 4) {
        printf("Usage: <./executable> <memory size> <backing store> <address list>\n");
        exit(0);
    }

    char page_table[NUM_PAGES];
    memset(page_table, -1, sizeof(page_table));

    const char *backing = argv[2];
    const char *input = argv[3];
    const char *output;
    int num_frames;
    int size = atoi(argv[1]);

    if (size == 256) {
        output = "output256.csv";
        num_frames = 256;
    } else if(size == 128) {
        output = "output128.csv";
        num_frames = 128;
    } else {
        printf("Invalid memory size. Please choose either 128 or 256.\n");
        exit(0);
    }

    char main_mem[num_frames][FRAME_SIZE];

    FILE *backing_store = fopen(backing, "r");
    FILE *input_fp = fopen(input, "r");
    FILE *output_fp = fopen(output, "w");

    if(backing_store == NULL || input_fp == NULL) {
        printf("Failed to open file.\n");
        exit(0);
    }

    char logic[CHARS];
    int count = 0, page_fault = 0, free_frame = 0;

    while(fgets(logic, CHARS, input_fp) != NULL) {
        int logical = atoi(logic);
        int page = (logical >> OFFSET) & MASK;
        int offset = logical & MASK;
        int frame = 0, new_frame = 0;

        if(page_table[page] == -1) {

            char buffer[FRAME_SIZE];
            memset(buffer, 0, sizeof(buffer));

            if (fseek(backing_store, page * FRAME_SIZE, SEEK_SET) != 0)
                printf("error in fseek\n");

            if (fread(buffer, sizeof(char), FRAME_SIZE, backing_store) == 0)
                printf("error in fread\n");

            for(int i = 0; i < FRAME_SIZE; i++) {
                *((char*)main_mem + free_frame * FRAME_SIZE + i) = buffer[i];
            }

            new_frame = free_frame;
            page_table[page] = new_frame;

            free_frame++;
            page_fault++;
        }

        frame = page_table[page];
        count++;

        int physical = ((unsigned char)frame * FRAME_SIZE) + offset;
        int value = *((char*)main_mem + physical);
        fprintf(output_fp, "%d,%d,%d\n", logical, physical, value);
    }
    printf("%d, %d\n", page_fault, count);
    fprintf(output_fp, "Page Faults Rate, %.2f%%,\n", (page_fault / (count * 1.0)) * 100);

    fclose(input_fp);
    fclose(output_fp);
    fclose(backing_store);
    return(0);
}
