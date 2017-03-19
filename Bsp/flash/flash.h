#ifndef BSP_FLASH__
#define BSP_FLASH__

#include "stm32_includes.h"



//CM4
#define UNIQUE_ID_BASE 0x1FFF7A10
#define FLASH_SIZE_BASE 0x1FFF7A22




/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */


#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_11   /* Start @ of user Flash area */



#define IP_ADDR_OFFSET 0   //FLASH_USER_START_ADDR   ->   FLASH_USER_START_ADDR+3
#define IP_PORT_OFFSET 4   //FLASH_USER_START_ADDR + 4    ->   FLASH_USER_START_ADDR + 5
#define IP_FLAG_OFFSET 6


#define IP_ADDR_FLASH_ADDR (FLASH_USER_START_ADDR + IP_ADDR_OFFSET)
#define IP_PORT_FLASH_ADDR (FLASH_USER_START_ADDR + IP_PORT_OFFSET)
#define IP_FLAG_FLASH_ADDR (FLASH_USER_START_ADDR + IP_FLAG_OFFSET)


typedef enum {
	IP_FLASH_VALID = 0x01,
	IP_FLASH_INVALID = 0xFF
} ip_flag_t;




void get_unique_id(uint32_t* id);

void update_flash_end_addr(void);

bool set_bus_addr(uint16_t addr);

void update_bus_addr(void);

void flash_test(void);

#endif

