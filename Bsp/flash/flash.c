

#include "stm32_includes.h"

uint32_t flash_end_addr = 0;
 
uint8_t bus_addr = 0;
 
void get_unique_id(uint32_t* id)
{
	*id = *(volatile uint32_t*)(UNIQUE_ID_BASE);
	*(id+1) = *(volatile uint32_t*)(UNIQUE_ID_BASE+4);
	*(id+2) = *(volatile uint32_t*)(UNIQUE_ID_BASE+8);
}


void update_flash_end_addr(void)
{
	uint32_t flash_size = *(volatile uint16_t*)(FLASH_SIZE_BASE);
	flash_size = flash_size << 10;
	flash_end_addr = 0x08000000 + flash_size - 1;
	debug(info, "flash_end_addr: 0x%x", flash_end_addr);
}



void update_bus_addr(void)
{
	bus_addr =  *(volatile uint16_t*)(flash_end_addr-1);
}


ip_flag_t get_ip_flag(void)
{
  return *(volatile uint8_t*)(IP_FLAG_FLASH_ADDR);
}

uint32_t get_ip_addr(void)
{
  return *(volatile uint32_t*)(IP_ADDR_FLASH_ADDR);
}

uint16_t get_ip_port(void)
{
  return *(volatile uint16_t*)(IP_PORT_FLASH_ADDR);
}

bool set_ip(ip4_addr_t ipaddr, uint16_t port)
{
        uint32_t SectorError = 0;
        uint32_t ip_addr = *((uint32_t*)(&ipaddr));
        
        FLASH_EraseInitTypeDef EraseInitStruct;
        /* Fill EraseInit structure*/
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector = FLASH_SECTOR_11;
        EraseInitStruct.NbSectors = 1;
        
        HAL_FLASH_Unlock();
              
        debug(info, "ipaddr in flash : 0x%x. ipaddr to set : 0x%x.", get_ip_addr(), ip_addr);
        debug(info, "port in flash : 0x%x. port to set : 0x%x.", get_ip_port(), port);

 
        if((get_ip_addr() != ip_addr) || (get_ip_port() != port))
        {
            if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
            { 
              /* 
                Error occurred while sector erase. 
                User can add here some code to deal with this error. 
                SectorError will contain the faulty sector and then to know the code error on this sector,
                user can call function 'HAL_FLASH_GetError()'
              */
              /*
                FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
              */
              debug(info, "flash erase error.");
              HAL_FLASH_Lock();
              return false;
            }
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, IP_ADDR_FLASH_ADDR, (uint64_t)ip_addr);
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, IP_PORT_FLASH_ADDR, (uint64_t)port);
        }
        else
        {
            HAL_FLASH_Lock();
            debug(info, "the same ip has been saved to flash! do nothing!");
            return true;
        }

              /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
         you have to make sure that these data are rewritten before they are accessed during code
         execution. If this cannot be done safely, it is recommended to flush the caches by setting the
         DCRST and ICRST bits in the FLASH_CR register. */
        __HAL_FLASH_DATA_CACHE_DISABLE();
        __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

        __HAL_FLASH_DATA_CACHE_RESET();
        __HAL_FLASH_INSTRUCTION_CACHE_RESET();

        __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
        __HAL_FLASH_DATA_CACHE_ENABLE();

	if ( get_ip_addr() != ip_addr )
	{
		debug(info, "set ip addr failed!");
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, IP_FLAG_FLASH_ADDR, IP_FLASH_INVALID);
                HAL_FLASH_Lock();
		return false;
	}
        if ( get_ip_port() != port )
        {
		debug(info, "set ip port failed!");
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, IP_FLAG_FLASH_ADDR, IP_FLASH_INVALID);
                HAL_FLASH_Lock();
		return false;
	}
        
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, IP_FLAG_FLASH_ADDR, IP_FLASH_VALID);
              /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
         you have to make sure that these data are rewritten before they are accessed during code
         execution. If this cannot be done safely, it is recommended to flush the caches by setting the
         DCRST and ICRST bits in the FLASH_CR register. */
        __HAL_FLASH_DATA_CACHE_DISABLE();
        __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

        __HAL_FLASH_DATA_CACHE_RESET();
        __HAL_FLASH_INSTRUCTION_CACHE_RESET();

        __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
        __HAL_FLASH_DATA_CACHE_ENABLE();

        HAL_FLASH_Lock();
        
        debug(info, "set ip port success!");
        char ip_str_buffer[20];
        ip4addr_ntoa_r(&ipaddr, ip_str_buffer, 20);
        debug(info, "numeric ip str after set : %s : %d", ip_str_buffer, port );
        debug(info, "please restart me!");       
	return true;
}


void set_ip_test(void)
{ 
  ip4_addr_t ipaddr;
  uint16_t port = 2000;
  uint32_t ip_addr;

  IP4_ADDR(&ipaddr, 192, 168, 0, 100);
  
  ip_addr = *((uint32_t*)(&ipaddr));
  debug(info, "numeric ip before set : %d : %d", ip_addr, port);
  
  set_ip(ipaddr, port);
  
  ip_addr = get_ip_addr();
  port = get_ip_port();
  debug(info, "numeric ip after set : %d : %d", ip_addr, port);
  
}

