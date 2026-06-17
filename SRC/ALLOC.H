


/* Gtalk */
/* Copyright (C) 1993, by David W Jeske, and Daniel L Marks */
/* Copying or distributing this source code without written  */
/* permission of David W Jeske and Daniel L Marks is strictly forbidden */

/* File handle and memory allocation stuff */

#define MAX_MEM_HANDLES 100
#define MAX_FILE_HANDLES 40
#define DESCRIPTION_LENGTH 10
#define FILENAME_LENGTH 25

typedef struct mem_alloc
{
        void    *memory_pointer;
        int     task_id;
        int     empty;
        unsigned int paragraphs;
        char    kept_open;
        char    allocby[DESCRIPTION_LENGTH];
        char    ems_for_task;
        char    in_ems;
        char    dont_combine;
} mem_entry;

typedef struct file_alloc
{
        FILE    *file_pointer;
        int     task_id;
        unsigned int size;
        char    filename[FILENAME_LENGTH];
        char    kept_open;
        char    allocby[DESCRIPTION_LENGTH];
} file_entry;

void grab_all_available_memory(int use_ems);
void deallocate_ems(void);
void *g_malloc_main_only(unsigned long int memory, const char *description);
void *g_malloc(unsigned long int memory, const char *description);
int g_keep_memory(void *memory_pointer,int keep_it);
void *g_malloc_no_owner(unsigned long int memory, const char *description,
       int for_who);
int g_free_from_who(void *memory_pointer, int who);
int g_free(void *memory_pointer);
int g_transfer(void *memory_pointer,int new_task_id);
#ifdef REALLOC_DEFINED
void *g_realloc(void *memory_pointer,size_t new_size);
void *g_calloc(size_t num, size_t size, const char *description);
#endif
int g_owns_memory(void *memory_pointer);
void g_free_all_handles(int task_id);

FILE *g_fopen(const char *filename, const char *mode, const char *description);
int g_fclose(FILE *file_ptr);
int g_ftransfer(FILE *file_ptr, int new_task_id);
int g_flush(FILE *file_ptr);
int g_owns_file(void *file_ptr);

extern mem_entry far mem_array[MAX_MEM_HANDLES];
extern file_entry far file_array[MAX_FILE_HANDLES];
extern mem_handles;
extern file_handles;
