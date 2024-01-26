/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file stm32_bl.c
 * @author Evan Stoddard
 * @brief Implementation of STM32 ROM Bootloader protocol
 */

#include "stm32_bl.h"
#include "stm32_bl_messages.h"
#include <string.h>

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/**
 * @brief Bootloader protocol instance typedef
 * 
 */
typedef struct stm32_bl_inst_t {
    stm32_bl_write_func_t write_func;
    stm32_bl_read_func_t read_func;
} stm32_bl_inst_t;


/*****************************************************************************
 * Variables
 *****************************************************************************/

/**
 * @brief Instance
 * 
 */
static stm32_bl_inst_t inst;

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/**
 * @brief Calculate XOR'd checksum
 * 
 * @param src 
 * @param len 
 * @return uint8_t 
 */
uint8_t stm32_bl_calculate_checksum(void *src, uint32_t len);

/**
 * @brief Write a command and wait for ack
 * 
 * @param command Command to write
 * @return true Successfully wrote command
 * @return false Unsuccessfully wrote command
 */
bool stm32_bl_write_command(uint8_t command);

/**
 * @brief Wait for ACK byte from bootloader
 * 
 * @return true Received ACK
 * @return false Didn't receive ACK
 */
bool stm32_bl_wait_for_ack(void);

/*****************************************************************************
 * Functions
 *****************************************************************************/

bool stm32_bl_write(uint32_t addr, void *src, uint32_t len) {
    if (len > STM32_BL_MAX_TRANSFER_SIZE)
        return false;
    
    // Buffer for address (needs to be big endian)
    // Size is max transfer size plus one byte for size
    // and another byte for checksum
    uint8_t buffer[STM32_BL_MAX_TRANSFER_SIZE + 2] = {0};

    // Please see: https://sourceforge.net/projects/stm32flash/
    // I don't fully understand everything around this aligned length...
    uint32_t aligned_len = (len + 3) & ~3;

    // Setup buffer
    buffer[0] = (uint8_t)(aligned_len - 1);
    buffer[1 + len] = (uint8_t)(aligned_len - 1);
    buffer[1 + len] ^= stm32_bl_calculate_checksum(src, len);
    memcpy(&buffer[1], src, len);
    
    // Issue command
    if (!stm32_bl_write_command(STM32_BL_CMD_WRITE_MEM))
        return false;
    
    // Convert address to big endian and get checksum
    uint8_t addr_buffer[5] = { 0 };
    addr_buffer[0] = addr >> 24;
    addr_buffer[1] = (addr >> 16) & 0xFF;
    addr_buffer[2] = (addr >> 8) & 0xFF;
    addr_buffer[3] = addr & 0xFF;
    addr_buffer[4] = stm32_bl_calculate_checksum(addr_buffer, sizeof(uint32_t));
    
    // Write address and wait for ack
    if (!inst.write_func(addr_buffer, sizeof(addr_buffer)))
        return false;
        
    if (!stm32_bl_wait_for_ack())
        return false;

    // Write data and wait for ack
    if (!inst.write_func(buffer, aligned_len + 2))
        return false;
    
    return stm32_bl_wait_for_ack();
}

void stm32_bl_set_write_func(stm32_bl_write_func_t func) {
    inst.write_func = func;
}

void stm32_bl_set_read_func(stm32_bl_read_func_t func) {
    inst.read_func = func;
}

uint8_t stm32_bl_calculate_checksum(void *src, uint32_t len) {
    uint8_t checksum = 0;
    
    for (uint32_t i = 0; i < len; i++) {
        checksum ^= ((uint8_t *)src)[i];
    }
    
    return checksum;
}

bool stm32_bl_write_command(uint8_t command) {
    if (!inst.write_func)
        return false;
    
    // Buffer with command as first byte and XOR'd compliment
    uint8_t buf[] = {command, 0xFF ^ command};
    
    // Call write function
    if (!inst.write_func(buf, sizeof(buf)))
        return false;
    
    // Return ack status
    return stm32_bl_wait_for_ack();
}

bool stm32_bl_wait_for_ack(void) {
    if (!inst.read_func)
        return false;
    
    // Loop in case device returns that it's busy
    while(true) {
        // Attempt to read ack value from bootloader
        uint8_t ack_val = 0;
        if (!inst.read_func(&ack_val, 1))
            return false;
        
        // Handle value
        switch(ack_val) {
            case STM32_BL_RES_ACK:
                return true;
                break;
            case STM32_BL_RES_BUSY:
                continue;
                break;
            default:
                return false;
                break;
        }
    }
    
    return false;
}