
#ifndef __QUEUE_H
#define __QUEUE_H

#include "stm32_includes.h"

#define QUEUE_TYPE   uint8_t

int queue_put(QUEUE_TYPE value);
int queue_get(QUEUE_TYPE *value);

int is_empty(void);
int is_full(void);

#endif


