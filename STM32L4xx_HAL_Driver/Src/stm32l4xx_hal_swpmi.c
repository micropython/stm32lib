/**
  ******************************************************************************
  * @file    stm32l4xx_hal_swpmi.c
  * @author  MCD Application Team
  * @brief   SWPMI HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Single Wire Protocol Master Interface (SWPMI).
  *           + Initialization and Configuration
  *           + Data transfers functions
  *           + DMA transfers management
  *           + Interrupts and flags management
  @verbatim
 ===============================================================================
                        ##### How to use this driver #####
 ===============================================================================
  [..]
     The SWPMI HAL driver can be used as follows:

    (#) Declare a SWPMI_HandleTypeDef handle structure (eg. SWPMI_HandleTypeDef hswpmi).

    (#) Initialize the SWPMI low level resources by implementing the HAL_SWPMI_MspInit() API:
        (##) Enable the SWPMIx interface clock with __HAL_RCC_SWPMIx_CLK_ENABLE().
        (##) SWPMI IO configuration:
            (+++) Enable the clock for the SWPMI GPIO.
            (+++) Configure these SWPMI pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_SWPMI_Transmit_IT()
             and HAL_SWPMI_Receive_IT() APIs):
            (+++) Configure the SWPMIx interrupt priority with HAL_NVIC_SetPriority().
            (+++) Enable the NVIC SWPMI IRQ handle with HAL_NVIC_EnableIRQ().

        (##) DMA Configuration if you need to use DMA process (HAL_SWPMI_Transmit_DMA()
             and HAL_SWPMI_Receive_DMA() APIs):
            (+++) Declare a DMA handle structure for the Tx/Rx channels.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required
                  Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx channels and requests.
            (+++) Associate the initialized DMA handle to the SWPMI DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete
                  interrupt on the DMA Tx/Rx channels.

    (#) Program the Bite Rate, Tx Buffering mode, Rx Buffering mode in the Init structure.

    (#) Enable the SWPMI peripheral by calling the HAL_SWPMI_Init() function.

  [..]
    Three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Send an amount of data in blocking mode using HAL_SWPMI_Transmit()
      (+) Receive an amount of data in blocking mode using HAL_SWPMI_Receive()

    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Send an amount of data in non-blocking mode using HAL_SWPMI_Transmit_IT()
      (+) At transmission end of transfer HAL_SWPMI_TxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_TxCpltCallback()
      (+) Receive an amount of data in non-blocking mode using HAL_SWPMI_Receive_IT()
      (+) At reception end of transfer HAL_SWPMI_RxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_RxCpltCallback()
      (+) In case of flag error, HAL_SWPMI_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_ErrorCallback()

    *** DMA mode IO operation ***
    =============================
    [..]
      (+) Send an amount of data in non-blocking mode (DMA) using HAL_SWPMI_Transmit_DMA()
      (+) At transmission end of transfer HAL_SWPMI_TxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_TxCpltCallback()
      (+) Receive an amount of data in non-blocking mode (DMA) using HAL_SWPMI_Receive_DMA()
      (+) At reception end of transfer HAL_SWPMI_RxCpltCallback() is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_RxCpltCallback()
      (+) In case of flag error, HAL_SWPMI_ErrorCallback() function is executed and user can
          add his own code by customization of function pointer HAL_SWPMI_ErrorCallback()
      (+) Stop the DMA Transfer using HAL_SWPMI_DMAStop()

    *** SWPMI HAL driver additional function list ***
    ===============================================
    [..]
      Below the list the others API available SWPMI HAL driver :

      (+) HAL_SWPMI_EnableLoopback(): Enable the loopback mode for test purpose only
      (+) HAL_SWPMI_DisableLoopback(): Disable the loopback mode

    *** SWPMI HAL driver macros list ***
    ==================================
    [..]
      Below the list of most used macros in SWPMI HAL driver :

      (+) __HAL_SWPMI_ENABLE(): Enable the SWPMI peripheral
      (+) __HAL_SWPMI_DISABLE(): Disable the SWPMI peripheral
      (+) __HAL_SWPMI_ENABLE_IT(): Enable the specified SWPMI interrupts
      (+) __HAL_SWPMI_DISABLE_IT(): Disable the specified SWPMI interrupts
      (+) __HAL_SWPMI_GET_IT_SOURCE(): Check if the specified SWPMI interrupt source is
          enabled or disabled
      (+) __HAL_SWPMI_GET_FLAG(): Check whether the specified SWPMI flag is set or not

    *** Callback registration ***
    =============================
    [..]
      The compilation define USE_HAL_SWPMI_REGISTER_CALLBACKS when set to 1
      allows the user to configure dynamically the driver callbacks.
    [..]
      Use function HAL_SWPMI_RegisterCallback() to register a user callback. It allows
      to register the following callbacks:
      (+) RxCpltCallback     : SWPMI receive complete.
      (+) RxHalfCpltCallback : SWPMI receive half complete.
      (+) TxCpltCallback     : SWPMI transmit complete.
      (+) TxHalfCpltCallback : SWPMI transmit half complete.
      (+) ErrorCallback      : SWPMI error.
      (+) MspInitCallback    : SWPMI MspInit.
      (+) MspDeInitCallback  : SWPMI MspDeInit.
    [..]
    This function takes as parameters the HAL peripheral handle, the callback ID
    and a pointer to the user callback function.
    [..]
    Use function HAL_SWPMI_UnRegisterCallback() to reset a callback to the default
    weak (surcharged) function.
    HAL_SWPMI_UnRegisterCallback() takes as parameters the HAL peripheral handle,
    and the callback ID.
    This function allows to reset following callbacks:
      (+) RxCpltCallback     : SWPMI receive complete.
      (+) RxHalfCpltCallback : SWPMI receive half complete.
      (+) TxCpltCallback     : SWPMI transmit complete.
      (+) TxHalfCpltCallback : SWPMI transmit half complete.
      (+) ErrorCallback      : SWPMI error.
      (+) MspInitCallback    : SWPMI MspInit.
      (+) MspDeInitCallback  : SWPMI MspDeInit.
    [..]
    By default, after the HAL_SWPMI_Init and if the state is HAL_SWPMI_STATE_RESET
    all callbacks are reset to the corresponding legacy weak (surcharged) functions:
    examples HAL_SWPMI_RxCpltCallback(), HAL_SWPMI_ErrorCallback().
    Exception done for MspInit and MspDeInit callbacks that are respectively
    reset to the legacy weak (surcharged) functions in the HAL_SWPMI_Init
    and HAL_SWPMI_DeInit only when these callbacks are null (not registered beforehand).
    If not, MspInit or MspDeInit are not null, the HAL_SWPMI_Init and HAL_SWPMI_DeInit
    keep and use the user MspInit/MspDeInit callbacks (registered beforehand).
    [..]
    Callbacks can be registered/unregistered in READY state only.
    Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
    in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
    during the Init/DeInit.
    In that case first register the MspInit/MspDeInit user callbacks
    using HAL_SWPMI_RegisterCallback before calling @ref HAL_SWPMI_DeInit
    or HAL_SWPMI_Init function.
    [..]
    When the compilation define USE_HAL_SWPMI_REGISTER_CALLBACKS is set to 0 or
    not defined, the callback registering feature is not available
    and weak (surcharged) callbacks are used.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

#if defined(SWPMI1)

/** @defgroup SWPMI SWPMI
  * @brief HAL SWPMI module driver
  * @{
  */
#ifdef HAL_SWPMI_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @addtogroup SWPMI_Private_Constants SWPMI Private Constants
  * @{
  */
#define SWPMI_TIMEOUT_VALUE                   22000U   /* End of transmission timeout */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SWPMI_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void SWPMI_DMATxHalfCplt(DMA_HandleTypeDef *hdma);
static void SWPMI_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SWPMI_DMARxHalfCplt(DMA_HandleTypeDef *hdma);
static void SWPMI_DMAError(DMA_HandleTypeDef *hdma);
static void SWPMI_DMAAbortOnError(DMA_HandleTypeDef *hdma);
static void SWPMI_Transmit_IT(SWPMI_HandleTypeDef *hswpmi);
static void SWPMI_EndTransmit_IT(SWPMI_HandleTypeDef *hswpmi);
static void SWPMI_Receive_IT(SWPMI_HandleTypeDef *hswpmi);
static void SWPMI_EndReceive_IT(SWPMI_HandleTypeDef *hswpmi);
static void SWPMI_EndTransmitReceive_IT(SWPMI_HandleTypeDef *hswpmi);
static HAL_StatusTypeDef SWPMI_WaitOnFlagSetUntilTimeout(SWPMI_HandleTypeDef *hswpmi, uint32_t Flag, uint32_t Tickstart, uint32_t Timeout);

/* Exported functions --------------------------------------------------------*/

/** @defgroup SWPMI_Exported_Functions SWPMI Exported Functions
  * @{
  */

/** @defgroup SWPMI_Exported_Group1 Initialization/de-initialization methods
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the SWPMI peripheral.
      (+) De-initialize the SWPMI peripheral.

@endverbatim
  * @{
  */

/**
  * @brief Initialize the SWPMI peripheral according to the specified parameters in the SWPMI_InitTypeDef.
  * @param hswpmi SWPMI handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SWPMI_Init(SWPMI_HandleTypeDef *hswpmi)
{
  HAL_StatusTypeDef status = HAL_OK;
  __IO uint32_t wait_loop_index = 0U;

  /* Check the SWPMI handle allocation */
  if(hswpmi == NULL)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Check the parameters */
    assert_param(IS_SWPMI_VOLTAGE_CLASS(hswpmi->Init.VoltageClass));
    assert_param(IS_SWPMI_BITRATE_VALUE(hswpmi->Init.BitRate));
    assert_param(IS_SWPMI_TX_BUFFERING_MODE(hswpmi->Init.TxBufferingMode));
    assert_param(IS_SWPMI_RX_BUFFERING_MODE(hswpmi->Init.RxBufferingMode));

    if(hswpmi->State == HAL_SWPMI_STATE_RESET)
    {
      /* Allocate lock resource and initialize it */
      hswpmi->Lock = HAL_UNLOCKED;

#if (USE_HAL_SWPMI_REGISTER_CALLBACKS == 1)
      /* Reset callback pointers to the weak predefined callbacks */
      hswpmi->RxCpltCallback     = HAL_SWPMI_RxCpltCallback;
      hswpmi->RxHalfCpltCallback = HAL_SWPMI_RxHalfCpltCallback;
      hswpmi->TxCpltCallback     = HAL_SWPMI_TxCpltCallback;
      hswpmi->TxHalfCpltCallback = HAL_SWPMI_TxHalfCpltCallback;
      hswpmi->ErrorCallback      = HAL_SWPMI_ErrorCallback;

      /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
      if(hswpmi->MspInitCallback == NULL)
      {
        hswpmi->MspInitCallback = HAL_SWPMI_MspInit;
      }
      hswpmi->MspInitCallback(hswpmi);
#else
      /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
      HAL_SWPMI_MspInit(hswpmi);
#endif
    }

    hswpmi->State = HAL_SWPMI_STATE_BUSY;

    /* Disable SWPMI interface */
    CLEAR_BIT(hswpmi->Instance->CR, SWPMI_CR_SWPACT);

    /* Clear all SWPMI interface flags */
    WRITE_REG(hswpmi->Instance->ICR, 0x019F);

    /* Apply Voltage class selection */
    MODIFY_REG(hswpmi->Instance->OR, SWPMI_OR_CLASS, hswpmi->Init.VoltageClass);

    /* If Voltage class B, apply 300 