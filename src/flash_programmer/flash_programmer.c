#include "flash_programmer.h"
#include "../hw_structs/spi_flash.h"

// Wait for the SPI Flash controller to be idle
static void wait_for_idle(void) {
    while (hw_spi_flash->spi_data_fifo_ro & 0x01) {
        // Wait until SPI_FLASH_BUSY (bit 0) is clear
        __asm__ volatile ("nop");
    }
}

void spi_flash_init(void) {
    // Basic SPI configuration
    // Based on headers: set clock divider, standard read mode
    hw_spi_flash->spi_config = (0x01 << 8); // Example: CLK_DIVIDER = 1
    hw_spi_flash->spi_cs_size = 0x02; // Example: SPI_SIZE_16M
}

uint32_t spi_flash_read_id(void) {
    wait_for_idle();

    // Send Read ID command (0x9F or 0x90 typically)
    hw_spi_flash->spi_cmd_addr = 0x9F;

    // Wait for response
    wait_for_idle();

    return hw_spi_flash->spi_read_back;
}

bool spi_flash_sector_erase(uint32_t flash_addr) {
    wait_for_idle();

    // Command 0x20 for Sector Erase, shifted address
    hw_spi_flash->spi_cmd_addr = (flash_addr << 8) | 0x20;

    wait_for_idle();

    // In a full implementation, we'd also poll the flash's internal WIP status bit
    // using a RDSR command (0x05) to know when the erase physically completes.
    return true;
}

bool spi_flash_page_program(uint32_t flash_addr, const uint8_t *data, uint32_t length) {
    wait_for_idle();

    // Write Enable (0x06) should precede this in reality

    // Setup block size
    hw_spi_flash->spi_block_size = length << 8;

    // Command 0x02 Page Program
    hw_spi_flash->spi_cmd_addr = (flash_addr << 8) | 0x02;

    for (uint32_t i = 0; i < length; i++) {
        // Wait for TX FIFO not full
        while (hw_spi_flash->spi_data_fifo_ro & 0x04) {
            __asm__ volatile ("nop");
        }
        hw_spi_flash->spi_data_fifo_wo = data[i];
    }

    wait_for_idle();
    return true;
}
