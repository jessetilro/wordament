#ifndef BOX
#define BOX

void* box_int(int i);
int unbox_int(void* d);
void destroy_int(void* d);
int remove_int(void* d);
void* copy_int(void* d);

void* box_char(char c);
char unbox_char(void* d);
void destroy_char(void* d);
char remove_char(void* d);
void* copy_char(void* d);

#endif
