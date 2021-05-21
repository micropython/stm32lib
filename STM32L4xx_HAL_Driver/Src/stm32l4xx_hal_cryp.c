/**
  ******************************************************************************
  * @file    stm32l4xx_hal_cryp.c
  * @author  MCD Application Team
  * @brief   CRYP HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Cryptography (CRYP) peripheral:
  *           + Initialization and de-initialization functions
  *           + Processing functions using polling mode
  *           + Processing functions using interrupt mode
  *           + Processing functions using DMA mode
  *           + Peripheral State functions
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
      The CRYP HAL driver can be used as follows:

      (#)Initialize the CRYP low level resources by implementing the HAL_CRYP_MspInit():
         (++) Enable the CRYP interface clock using __HAL_RCC_AES_CLK_ENABLE()
         (++) In case of using interrupts (e.g. HAL_CRYP_AES_IT())
             (+++) Configure the CRYP interrupt priority using HAL_NVIC_SetPriority()
             (+++) Enable the AES IRQ handler using HAL_NVIC_EnableIRQ()
             (+++) In AES IRQ handler, call HAL_CRYP_IRQHandler()
         (++) In case of using DMA to control data transfer (e.g. HAL_CRYPEx_AES_DMA())
             (+++) Enable the DMA2 interface clock using
                 __HAL_RCC_DMA2_CLK_ENABLE()
             (+++) Configure and enable two DMA channels one for managing data transfer from
                 memory to peripheral (input channel) and another channel for managing data
                 transfer from peripheral to memory (output channel)
             (+++) Associate the initialized DMA handle to the CRYP DMA handle
                 using __HAL_LINKDMA()
             (+++) Configure the priority and enable the NVIC for the transfer complete
                 interrupt on the two DMA channels. The output channel should have higher
                 priority than the input channel.
                 Resort to HAL_NVIC_SetPriority() and HAL_NVIC_EnableIRQ()

      (#)Initialize the CRYP HAL using HAL_CRYP_Init(). This function configures:
         (++) The data type: 1-bit, 8-bit, 16-bit and 32-bit
         (++) The AES operating mode (encryption, key derivation and/or decryption)
         (++) The AES chaining mode (ECB, CBC, CTR, GCM, GMAC, CMAC when applicable, CCM when applicable)
         (++) The encryption/decryption key if so required
         (++) The initialization vector or nonce if applicable (not used in ECB mode).

      (#)Three processing (encryption/decryption) functions are available:
         (++) Polling mode: encryption and decryption APIs are blocking functions
              i.e. they process the data and wait till the processing is finished
         (++) Interrupt mode: encryption and decryption APIs are not blocking functions
              i.e. they process the data under interrupt
         (++) DMA mode: encryption and decryption APIs are not blocking functions
              i.e. the data transfer is ensured by DMA

       (#)Call HAL_CRYP_DeInit() to deinitialize the CRYP peripheral.

     *** Callback registration ***
     ===================================
     [..]
      (#) The compilation define  USE_HAL_CRYP_REGISTER_CALLBACKS when set to 1
          allows the user to configure dynamically the driver callbacks.
          Use function @ref HAL_CRYP_RegisterCallback() to register a user callback.

      (#) Function @ref HAL_CRYP_RegisterCallback() allows to register following callbacks:
            (+) InCpltCallback : callback for input DMA transfer completion.
            (+) OutCpltCallback : callback for output DMA transfer completion.
            (+) CompCpltCallback : callback for computation completion.
            (+) ErrorCallback : callback for error.
            (+) MspInitCallback    : CRYP MspInit.
            (+) MspDeInitCallback  : CRYP MspDeInit.
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.

      (#) Use function @ref HAL_CRYP_UnRegisterCallback() to reset a callback to the default
          weak (surcharged) function.
          @ref HAL_CRYP_UnRegisterCallback() takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (+) InCpltCallback : callback for input DMA transfer completion.
            (+) OutCpltCallback : callback for output DMA transfer completion.
            (+) CompCpltCallback : callback for computation completion.
            (+) ErrorCallback : callback for error.
            (+) MspInitCallback    : CRYP MspInit.
            (+) MspDeInitCallback  : CRYP MspDeInit.

      (#) By default, after the @ref HAL_CRYP_Init and if the state is HAL_CRYP_STATE_RESET
          all callbacks are reset to the corresponding legacy weak (surcharged) functions:
          examples @ref HAL_CRYP_InCpltCallback(), @ref HAL_CRYP_ErrorCallback()
          Exception done for MspInit and MspDeInit callbacks that are respectively
          reset to the legacy weak (surcharged) functions in the @ref HAL_CRYP_Init
          and @ref HAL_CRYP_DeInit only when these callbacks are null (not registered beforehand)
          If not, MspInit or MspDeInit are not null, the @ref HAL_CRYP_Init and @ref HAL_CRYP_DeInit
          keep and use the user MspInit/MspDeInit callbacks (registered beforehand).

          Callbacks can be registered/unregistered in READY state only.
          Exception done for MspInit/MspDeInit callbacks that can be registered/unregistered
          in READY or RESET state, thus registered (user) MspInit/DeInit callbacks can be used
          during the Init/DeInit.
          In that case first register the MspInit/MspDeInit user callbacks
          using @ref HAL_CRYP_RegisterCallback before calling @ref HAL_CRYP_DeInit
          or @ref HAL_