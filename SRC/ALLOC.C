#include "include.h"
#include "gtalk.h"

#define DAVE_FUDGE_FACTOR 32768l
// #define DAVE_FUDGE_FACTOR 0

#undef DEBUG
#undef MEM_DEBUG

/* Memory and file allocation unit */

int delete_file_entry(int entry_num);

mem_entry  far mem_array[MAX_MEM_HANDLES];
file_entry far file_array[MAX_FILE_HANDLES];

int           mem_handles = 0;
int           file_handles = 0;
unsigned int  ems_page_frame;
void far     *location_of_ems;

int find_memory_pointer(void *memory_pointer, int who)
{
#ifdef DEBUG
    char s[80];
#endif
    mem_entry *cur_entry = mem_array;
    int count = 0;
    int flag = 0;

    while ((count<mem_handles) && (!flag))
    {
     if (cur_entry->memory_pointer == memory_pointer)
     {
       if ((cur_entry->in_ems) && (cur_entry->ems_for_task==who))
         flag = 1;
       else
         if (!(cur_entry->in_ems)) flag = 1;
     }
     if (!flag)
     {
        count++;
        cur_entry++;
     }
    }
#ifdef DEBUG
    if (tasking)
    {
      sprintf(s,"Find Memory Pointer %p entry %d flag %d",memory_pointer,count,flag);
      print_str_cr_to(s,0);
    }
#endif
    if (flag) return (count);
     else return (-1);
};

int insert_memory_entry_at(int insert_at, void *memory_pointer, int task_id,
                      unsigned int size, char keep_open,
                      const char *description, int empty,
                      char in_ems, char dont_combine, char ems_for_task)
{
#ifdef DEBUG
    char s[80];
#endif
    int count;
    mem_entry *cur_entry;

    if (mem_handles==MAX_MEM_HANDLES) return (-1);

    for (count=mem_handles;count>insert_at;count--)
     mem_array[count] = mem_array[count-1];

    cur_entry = &mem_array[insert_at];

    cur_entry->memory_pointer = memory_pointer;
    cur_entry->task_id = task_id;
    cur_entry->paragraphs = size;
    cur_entry->kept_open = keep_open;
    cur_entry->empty = empty;
    cur_entry->in_ems = in_ems;
    cur_entry->dont_combine = dont_combine;
    cur_entry->ems_for_task = ems_for_task;
    strncpy(cur_entry->allocby,description,DESCRIPTION_LENGTH-1);
    cur_entry->allocby[DESCRIPTION_LENGTH-1] = 0;
#ifdef DEBUG
    if (tasking)
    {
      sprintf(s,"Add memory entry %p entry %c",memory_pointer,mem_handles);
      print_str_cr_to(s,0);
    }
#endif
    mem_handles++;
    return (insert_at);
};

int delete_memory_entry(int entry_num)
{
#ifdef DEBUG
    char s[80];
#endif
    int count;

    if (entry_num >= mem_handles) return (0);

    for (count=entry_num;count<(mem_handles-1);count++)
     mem_array[count] = mem_array[count+1];
#ifdef DEBUG
    if (tasking)
    {
      sprintf(s,"Deleting entry %d",entry_num);
      print_str_cr_to(s,0);
    }
#endif
    mem_handles--;
    return (1);
};

int find_smallest_available_memory(unsigned int size, char use_ems, int who)
{
  int smallest = -1;
  unsigned int smallest_size = 0x7FFF;
  int current = 0;
  int cur_block_in_ems = 0;
  char flag;
  char is_smallest;
  mem_entry *cur_entry = mem_array;

  while (current < mem_handles)
  {
    if (cur_entry->empty)
    {
      if ((cur_entry->paragraphs>=size))
      {
        flag = 0;
        is_smallest = (cur_entry->paragraphs<smallest_size);
        if ((cur_entry->in_ems) && (cur_entry->ems_for_task == who)
            && (use_ems))
        {
          flag = ((!cur_block_in_ems) || (cur_block_in_ems && is_smallest));
        } else
        {
          flag = ((!cur_entry->in_ems) && (!cur_block_in_ems) && is_smallest);
        }
        if (flag)
        {
          smallest_size = cur_entry->paragraphs;
          smallest = current;
          cur_block_in_ems = cur_entry->in_ems;
        }
      }
    }
    current++;
    cur_entry++;
  }
  return (smallest);
}

void merge_blocks(int entry)
{

#ifdef DEBUG
print_str_cr_to("Merge Blocks",0);
#endif

  if (entry>0)
  {
   if ((mem_array[entry-1].empty) && (!mem_array[entry-1].dont_combine))
   {
     mem_array[entry].paragraphs += mem_array[entry-1].paragraphs;
     mem_array[entry].memory_pointer = mem_array[entry-1].memory_pointer;
     entry--;
     delete_memory_entry(entry);
   }
  }

 if (entry<(mem_handles-1))
 {
  if (!mem_array[entry].dont_combine)
  {
   if (mem_array[entry+1].empty)
   {
     mem_array[entry+1].paragraphs += mem_array[entry].paragraphs;
     mem_array[entry+1].memory_pointer = mem_array[entry].memory_pointer;
     delete_memory_entry(entry);
   }
  }
 }

}

unsigned long int look_for_ems(void)
{
  int cur_task;
  void *test_ems = getvect(0x67);
  void *word_ems;
  struct task_struct near *ctask = tasks;
  int map;
  unsigned int size;
  char map_error;
  unsigned long int total_memory = 0;

  word_ems = MK_FP(FP_SEG(test_ems),0x0A);
  if (memcmp(word_ems,"EMMXXXX0",8))
  {
    printf("EMS Memory not found\n");
    return 0;
  }

  _AH = 0x46;
  geninterrupt(0x67);
  if ((_AH) || (_AL<0x40))
  {
    printf("Must have EMS 4.0 or greater to use EMS\n");
    return 0;
  }

  _AH = 0x41;
  geninterrupt(0x67);
  if (_AH) return 0;  // No EMS available
                      // Get Page Frame, if the ERROR Code (_AH)
                      // is NON Zero, then we had a problem
                      // getting the page frame
  ems_page_frame = _BX;

  location_of_ems = (void far *) (((unsigned long int) ems_page_frame) << 16);

  printf("Found EMS at page frame: %04X\n",ems_page_frame);

  for (cur_task=0;cur_task<MAX_THREADS;cur_task++)
  {
    _AH = 0x43;
    _BX = 4;
    geninterrupt(0x67);
    if (!(_AH))
    {
      ctask->ems_handle = _DX;
      ctask->is_ems = 1;
      ctask->mapped_pages = 0;
      size = 0;
      map_error = 0;
      map = 0;
      while ((!map_error) && (map<4))
      {
        _AH = 0x44;
        _DX = ctask->ems_handle;
        _AL = map;
        _BX = map;
        geninterrupt(0x67);
        if (_AH)
        {
          printf("EMS page mapping failed\n");
          ctask->is_ems = 0;
          map_error = 1;
        } else
        {
          size += 1024;
          ctask->mapped_pages++;
        }
        map++;
      }
      if (size == 0) ctask->is_ems = 0;
      if (ctask->is_ems)
      {
         insert_memory_entry_at(mem_handles,(char *)location_of_ems,
                      -1,size,1,"EMPTY",1,1,1,cur_task);
         total_memory += (((unsigned long int)size) << 4);
      }
    }
    ctask++;
  }
  return (total_memory);
}

void deallocate_ems(void)
{
  int cur_task;
  struct task_struct near *ctask = tasks;
  unsigned char error;

  for (cur_task=0;cur_task<MAX_THREADS;cur_task++)
  {
    if (ctask->is_ems)
    {
      _AH = 0x45;
      _DX = ctask->ems_handle;
      geninterrupt(0x67);
      error = _AH;
      if (error)
      {
        printf("Can't deallocate EMS handle error: %d\n",error);
      }
    }
    ctask++;
   }
}

void grab_all_available_memory(int use_ems)
{
  unsigned long int precore = farcoreleft();
  unsigned long int core_left = precore - DAVE_FUDGE_FACTOR;
  unsigned int paragraphs = (unsigned int) ((core_left + 15l) >> 4);
  void *total_memory_pointer;

#ifdef MEM_DEBUG
  int result=0;
  struct farheapinfo test;
  unsigned *far_seg;

  printf("Found: %lu\n",precore);
#endif

#ifdef MEM_DEBUG
  test.ptr=NULL;
  do
   {
     result=farheapwalk(&test);
     printf("FarHeapSize %lu  Used: %d Ptr:%lX Res:%d\n",test.size,test.in_use,test.ptr,result);
   } while ((result!=5) && (result!=1));
   printf("Result: %d\n",result);

  printf("Total Memory (_dos_allocmem) Rtn: %u  SegPtr: %u\n",_dos_allocmem(0xFFFFu,far_seg),(unsigned int)*far_seg);
#endif

  total_memory_pointer = farmalloc((unsigned long int)core_left);

#ifdef MEM_DEBUG
  printf("Allocated: %lX\n",total_memory_pointer);

  test.ptr=NULL;
  do
   {
     result=farheapwalk(&test);
     printf("FarHeapSize %lu  Used: %d Ptr:%lX Res:%d\n",test.size,test.in_use,test.ptr,result);
   } while ((result!=5) && (result!=1));
   printf("Result: %d\n",result);
#endif

  if (!total_memory_pointer)
  {
    printf("Could not allocate memory!\n");
    g_exit(1);
  }
  if (!tasking)
    {
     printf("CoreLeft Begin: %ld asking for %ld CoreLeft Now: %ld \n",precore,core_left,farcoreleft());
     printf("We have [%ud] 16 byte paragraphs\n",paragraphs);
    }
  mem_handles = 0;
  insert_memory_entry_at(0,(char *)total_memory_pointer,
                      -1,paragraphs,1,"EMPTY",1,0,1,-1);

  sys_toggles.total_starting_memory=core_left;
  sys_toggles.total_dos_starting_memory=farcoreleft();

  if (use_ems)
    sys_toggles.total_starting_memory += look_for_ems();

#ifdef MEM_DEBUG
  g_exit(1);
#endif
}

void *allocate_some_memory(unsigned long int length,
 const char *description, int have_owner, char use_ems, int who)
{
  unsigned int paragraphs = (unsigned int) ((length + 15) >> 4);
  int available_block = find_smallest_available_memory(paragraphs,
     use_ems,who);
  mem_entry *cur_entry;
  void *old_pointer;
  char dont_combine = 0;
  char in_ems;
  char ems_for_task;

  if (available_block==-1) return (NULL);
  if (mem_handles==MAX_MEM_HANDLES) return (NULL);
  cur_entry = &mem_array[available_block];
  old_pointer = cur_entry->memory_pointer;
  cur_entry->memory_pointer = (void *)
   ((unsigned long int)(cur_entry->memory_pointer) +
   ((unsigned long int)paragraphs << 16));
  cur_entry->paragraphs -= paragraphs;
  in_ems = cur_entry->in_ems;
  ems_for_task = cur_entry->ems_for_task;
  if (!cur_entry->paragraphs)
  {
    if (cur_entry->dont_combine) dont_combine = 1;
    delete_memory_entry(available_block);
  }
  insert_memory_entry_at(available_block,old_pointer,
   ((tasking && have_owner) ? tswitch : -1),
   paragraphs,!(tasking && have_owner),description,0,
      in_ems,dont_combine,ems_for_task);
  return(old_pointer);
}

int free_some_memory(void *pointer_to_free, int who)
{
  int freed_memory_entry = find_memory_pointer(pointer_to_free,who);
  mem_entry *cur_entry;

  if (freed_memory_entry==-1) return (0);
  cur_entry = &mem_array[freed_memory_entry];

  if (cur_entry->empty) return;
  cur_entry->task_id = -1;
  cur_entry->empty = 1;
  cur_entry->kept_open = 0;
  strcpy(cur_entry->allocby,"EMPTY");

  merge_blocks(freed_memory_entry);
  return (1);
}

void *g_malloc_main_only(unsigned long int memory,
        const char *description)
{
#ifdef DEBUG
    char s[80];
#endif
    int not_lock_dos;
    void *memory_pointer;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    memory_pointer = allocate_some_memory(memory,description,1,0,tswitch);
#ifdef DEBUG
    if (tasking)
    {
      sprintf(s,"Mallocing memory pointer %p",memory_pointer);
      print_str_cr_to(s,0);
    }
#endif
    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (memory_pointer);
};


void *g_malloc(unsigned long int memory, const char *description)
{
#ifdef DEBUG
    char s[80];
#endif
    int not_lock_dos;
    void *memory_pointer;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    memory_pointer = allocate_some_memory(memory,description,1,1,tswitch);
#ifdef DEBUG
    if (tasking)
    {
      sprintf(s,"Mallocing memory pointer %p",memory_pointer);
      print_str_cr_to(s,0);
    }
#endif
    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (memory_pointer);
};

void *g_malloc_no_owner(unsigned long int memory, const char *description,
         int for_task)
{
#ifdef DEBUG
    char s[80];
#endif

    int not_lock_dos;
    void *memory_pointer;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    memory_pointer = allocate_some_memory(memory,description,0,1,for_task);
#ifdef DEBUG
    sprintf(s,"Mallocing memory pointer %p",memory_pointer);
#endif
    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (memory_pointer);
}

int g_free_from_who(void *memory_pointer, int who)
{
#ifdef DEBUG
    char s[80];
#endif

    int not_lock_dos;
    int result;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    result=!free_some_memory(memory_pointer,who);

#ifdef DEBUG
      sprintf(s,"Freeing pointer %p",memory_pointer);
#endif

    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (result);
};


int g_free(void *memory_pointer)
{
#ifdef DEBUG
    char s[80];
#endif

    int not_lock_dos;
    int result;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    result=!free_some_memory(memory_pointer,tswitch);

#ifdef DEBUG
      sprintf(s,"Freeing pointer %p",memory_pointer);
#endif

    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (result);
};

int g_transfer(void *memory_pointer,int new_task_id)
{
    int not_lock_dos;
    int mem_point;
    int response = 0;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    mem_point = find_memory_pointer(memory_pointer,tswitch);
    if (mem_point != -1)
     if ((mem_array[mem_point].task_id == tswitch) &&
         (!mem_array[mem_point].in_ems))
      {
        mem_array[mem_point].task_id = new_task_id;
        response = 1;
      };

    if (tasking)
    {
     if (not_lock_dos) unlock_dos();
    }
    return (response);
};

int g_keep_memory(void *memory_pointer,int keep_it)
{
    int not_lock_dos;
    int mem_point;
    int response = 0;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    mem_point = find_memory_pointer(memory_pointer,tswitch);
    if (mem_point != -1)
     {
       mem_array[mem_point].kept_open = keep_it;
       response = 1;
     };

    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (response);
};

#ifdef REALLOC_DEFINED
void *g_realloc(void *memory_pointer,size_t new_size)
{
    int not_lock_dos = !islocked(DOS_SEM);
    void *new_pointer;
    int mem_point;

    if (not_lock_dos) lock_dos();
    mem_point = find_memory_pointer(memory_pointer);
    if (mem_point == -1)
     {
       if (not_lock_dos) unlock_dos();
       return NULL;
     };
    new_pointer = realloc(memory_pointer,new_size);
    if (new_pointer)
      {
        mem_array[mem_point].size = new_size;
        mem_array[mem_point].memory_pointer = new_pointer;
      };
    if (not_lock_dos) unlock_dos();
    return (new_pointer);
};

void *g_calloc(size_t num, size_t size, const char *description)
{
    int not_lock_dos = !islocked(DOS_SEM);
    void *memory_pointer;

    if (not_lock_dos) lock_dos();
    memory_pointer = calloc(num,size);
    if (memory_pointer)
     add_memory_entry(memory_pointer,tswitch,(num*size),0,description);
    if (not_lock_dos) unlock_dos();
    return (memory_pointer);
};
#endif

void g_free_all_handles(int task_id) /* Should only be called in tasker */
{
    mem_entry *cur_entry = &mem_array[mem_handles];
    file_entry *cur_file_entry = &file_array[file_handles];

    int not_lock_dos = !islocked(DOS_SEM);
    int current = mem_handles;

    if (not_lock_dos) lock_dos();

    while (current > 0)
     {
       cur_entry--;
       current--;
       if ((cur_entry->task_id == task_id) && (!cur_entry->kept_open) &&
           (!(cur_entry->empty)))
        {
          free_some_memory(cur_entry->memory_pointer,
                           cur_entry->ems_for_task);
        };
     };

    current=file_handles;
    while (current > 0)
     {
       current--;
       cur_file_entry--;
       if ((cur_file_entry->task_id == task_id) && (!cur_file_entry->kept_open))
        {
          fclose(cur_file_entry->file_pointer);
          delete_file_entry(current);
        };
     };
    if (not_lock_dos) unlock_dos();
};

int g_owns_memory(void *memory_pointer)
{
    int not_lock_dos;
    int mem_point;

    if (tasking)
    {
      not_lock_dos = !islocked(DOS_SEM);
      if (not_lock_dos) lock_dos();
    }
    mem_point = find_memory_pointer(memory_pointer,tswitch);
    if (tasking)
    {
      if (not_lock_dos) unlock_dos();
    }
    return (mem_point);
};

int find_file_pointer(FILE *file_pointer)
{
    file_entry *cur_entry = file_array;
    int count = 0;
    int flag = 0;

    while ((count<file_handles) && (!flag))
     if (cur_entry->file_pointer == file_pointer)
      flag = 1;
      else
      {
        count++;
        cur_entry++;
      };
    if (flag) return (count);
     else return (-1);
};

int add_file_entry(FILE *file_pointer,int task_id,
                      const char *filename, char keep_open,
                      const char *description)
{
    file_entry *cur_entry = &file_array[file_handles];

    if (file_handles==MAX_FILE_HANDLES) return (-1);

    cur_entry->file_pointer = file_pointer;
    cur_entry->task_id = task_id;
    strncpy(cur_entry->filename,filename,FILENAME_LENGTH-1);
    cur_entry->filename[FILENAME_LENGTH - 1] = 0;
    cur_entry->kept_open = keep_open;
    strncpy(cur_entry->allocby,description,DESCRIPTION_LENGTH-1);
    cur_entry->allocby[DESCRIPTION_LENGTH - 1] = 0;
    file_handles++;
    return (file_handles - 1);
};

int delete_file_entry(int entry_num)
{
    int count;

    if (entry_num >= file_handles) return (0);

    for (count=entry_num;count<(file_handles-1);count++)
     file_array[count] = file_array[count+1];
    file_handles--;
    return (1);
};

FILE *g_fopen(const char *filename, const char *mode, const char *description)
{
    int not_lock_dos = !islocked(DOS_SEM);
    void *file_pointer;

    if (not_lock_dos) lock_dos();
    file_pointer = fopen(filename,mode);
    if (file_pointer)
     if (add_file_entry(file_pointer,tswitch,filename,0,description)==-1)
      {
        fclose(file_pointer);
        file_pointer = NULL;
      };
    if (not_lock_dos) unlock_dos();
    return (file_pointer);
};

int g_fclose(FILE *file_pointer)
{
    int not_lock_dos = !islocked(DOS_SEM);
    int file_point;
    int result=0;

    if (not_lock_dos) lock_dos();
    file_point = find_file_pointer(file_pointer);
    if (file_point != -1)
     {
      result=fclose(file_pointer);
      delete_file_entry(file_point);
     };
    if (not_lock_dos) unlock_dos();
    return (result);
};

int g_ftransfer(FILE *file_pointer, int new_task_id)
{

    int not_lock_dos = !islocked(DOS_SEM);
    int file_point;
    int response = 0;

    if (not_lock_dos) lock_dos();
    file_point = find_file_pointer(file_pointer);
    if (file_point != -1)
     if (file_array[file_point].task_id == tswitch)
      {
        file_array[file_point].task_id = new_task_id;
        response = 1;
      };

    if (not_lock_dos) unlock_dos();
    return (response);
};

int g_flush(FILE *file_pointer)
{
    int not_lock_dos = !islocked(DOS_SEM);
    int file_point;
    int result=0;

    if (not_lock_dos) lock_dos();
    file_point = find_file_pointer(file_pointer);
    if (file_point != -1)
     {
      result=fflush(file_pointer);
     };
    if (not_lock_dos) unlock_dos();
    return (result);
};

int g_owns_file(void *file_pointer)
{
    int not_lock_dos = !islocked(DOS_SEM);
    int file_point;

    if (not_lock_dos) lock_dos();
    file_point = find_file_pointer(file_pointer);
    if (not_lock_dos) unlock_dos();
    return (file_point);
};

