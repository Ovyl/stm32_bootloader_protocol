/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file stm32_bl.h
 * @author Evan Stoddard
 * @brief Implementation of STM32 ROM Bootloader protocol
 */

#ifndef STM32_BL_H_
#define STM32_BL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/**
 * @brief Max transfer size of a read/write transaction
 *
 */
#define STM32_BL_MAX_TRANSFER_SIZE 256

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/**
 * @brief Typedef for interface write function pointer
 *
 */
typedef int32_t (*stm32_bl_write_func_t)(void *src, uint32_t len);

/**
 * @brief Typedef for interface read function pointer
 *
 */
typedef int32_t (*stm32_bl_read_func_t)(void *dst, uint32_t len);

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

/**
 * @brief Write data to memory
 *
 * @param addr Address to write to
 * @param src Source buffer to write
 * @param len Length of buffer (must be <= STM32_BL_MAX_TRANSFER_SIZE)
 * @return true Able to transfer to memory
 * @return false Unable to transfer to memory
 */
bool stm32_bl_write(uint32_t addr, void *src, uint32_t len);

/**
 * @brief Command bootloader to execute starting at address
 *
 * @param addr Address to start executing
 * @return true Successfully sent command
 * @return false Unsuccessfully sent command
 */
bool stm32_bl_go(uint32_t addr);

/**
 * @briefCommand bootloader to erase chip
 *
 * @return true Successfully erased chip
 * @return false Unable to erase chip
 */
bool stm32_bl_erase_chip(void);

/**
 * @brief Set low-level transfer write function pointer
 *
 * @param func Function pointer that will handle write transfer over communication protocol
 */
void stm32_bl_set_write_func(stm32_bl_write_func_t func);

/**
 * @brief Set low-level transfer read function pointer
 *
 * @param func Function pointer that will handle read transfer over communication protocol
 */
void stm32_bl_set_read_func(stm32_bl_read_func_t func);

#ifdef __cplusplus
}
#endif
#endif /* STM32_BL_H_ */