/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file stm32_bl.h
 * @author Evan Stoddard
 * @brief 
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

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/**
 * @brief 
 * 
 */
typedef int32_t (*stm32_bl_write_func_t)(void *src, uint32_t len);

/**
 * @brief 
 * 
 */
typedef int32_t (*stm32_bl_read_func_t)(void *dst, uint32_t len);

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param addr Address to write to
 * @param src 
 * @param len 
 * @return true 
 * @return false 
 */
bool stm32_bl_write(uint32_t addr, void *src, uint32_t len);

/**
 * @brief 
 * 
 * @param func 
 */
void stm32_bl_set_write_func(stm32_bl_write_func_t func);

/**
 * @brief 
 * 
 * @param func 
 */
void stm32_bl_set_read_func(stm32_bl_read_func_t func);

#ifdef __cplusplus
}
#endif
#endif /* STM32_BL_H_ */