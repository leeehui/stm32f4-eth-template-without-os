
#ifndef __QUEUE_H
#define __QUEUE_H

#include "stm32_includes.h"

#define QUEUE_TYPE   cmd_data_t

void insert(QUEUE_TYPE value);
void delete(void);
QUEUE_TYPE first(void);
int is_empty(void);
int is_full(void);

#endif


