#ifndef _FLASH_PROGRAMMER_H_
#define _FLASH_PROGRAMMER_H_

#include <stdint.h>
#include <stdbool.h>

void spi_flash_init(void);
uint32_t spi_flash_read_id(void);
bool spi_flash_sector_erase(uint32_t flash_addr);
bool spi_flash_page_program(uint32_t flash_addr, const uint8_t *data, uint32_t length);

#endif // _FLASH_PROGRAMMER_H_
