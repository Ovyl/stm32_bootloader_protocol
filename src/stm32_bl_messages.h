/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file stm32_bl_messages.h
 * @author Evan Stoddard
 * @brief Messages defined by the STM32 ROM Bootloader
 */

#ifndef STM32_BL_MESSAGES_H_
#define STM32_BL_MESSAGES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Commands
 *****************************************************************************/

#define STM32_BL_CMD_GET        0x00U
#define STM32_BL_CMD_GET_VER    0x01U
#define STM32_BL_CMD_GET_ID     0x02U
#define STM32_BL_CMD_READ_MEM   0x11U
#define STM32_BL_CMD_GO         0x21U
#define STM32_BL_CMD_WRITE_MEM  0x31U
#define STM32_BL_CMD_ERASE      0x43U

/*****************************************************************************
 * Responses
 *****************************************************************************/

#define STM32_BL_RES_ACK        0x79
#define STM32_BL_RES_NACK       0x1F
#define STM32_BL_RES_BUSY       0x76

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/


/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* STM32_BL_MESSAGES_H_ */