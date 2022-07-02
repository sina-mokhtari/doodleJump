/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "globals.h"
#include "init.h"
#include "keypad.h"
#include "7segment.h"
#include "lcd.h"
#include "adc.h"
#include "game.h"
#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t tsts;
int myFlag = 1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

PCD_HandleTypeDef hpcd_USB_FS;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 200 * 4,
        .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for uartTxTsk */
osThreadId_t uartTxTskHandle;
const osThreadAttr_t uartTxTsk_attributes = {
        .name = "uartTxTsk",
        .stack_size = 200 * 4,
        .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for getVolumeTsk */
osThreadId_t getVolumeTskHandle;
const osThreadAttr_t getVolumeTsk_attributes = {
        .name = "getVolumeTsk",
        .stack_size = 200 * 4,
        .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for updateLcdTsk */
osThreadId_t updateLcdTskHandle;
const osThreadAttr_t updateLcdTsk_attributes = {
        .name = "updateLcdTsk",
        .stack_size = 800 * 4,
        .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for update7SegTsk */
osThreadId_t update7SegTskHandle;
const osThreadAttr_t update7SegTsk_attributes = {
        .name = "update7SegTsk",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for playMelodyTsk */
osThreadId_t playMelodyTskHandle;
const osThreadAttr_t playMelodyTsk_attributes = {
        .name = "playMelodyTsk",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for melodyNameQu */
osMessageQueueId_t melodyNameQuHandle;
const osMessageQueueAttr_t melodyNameQu_attributes = {
        .name = "melodyNameQu"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
        .name = "uartMutex"
};
/* Definitions for keypadSem */
osSemaphoreId_t keypadSemHandle;
const osSemaphoreAttr_t keypadSem_attributes = {
        .name = "keypadSem"
};
/* Definitions for volumeSem */
osSemaphoreId_t volumeSemHandle;
const osSemaphoreAttr_t volumeSem_attributes = {
        .name = "volumeSem"
};
/* Definitions for uartDmaSem */
osSemaphoreId_t uartDmaSemHandle;
const osSemaphoreAttr_t uartDmaSem_attributes = {
        .name = "uartDmaSem"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);

static void MX_USB_PCD_Init(void);

static void MX_USART2_UART_Init(void);

static void MX_ADC1_Init(void);

static void MX_TIM8_Init(void);

void StartDefaultTask(void *argument);

void StartUartTxTsk(void *argument);

void StartGetVolumeTsk(void *argument);

void StartUpdateLcdTsk(void *argument);

void StartUpdate7SegTsk(void *argument);

void StartPlayMelodyTsk(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USB_PCD_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    MX_TIM8_Init();
    /* USER CODE BEGIN 2 */
    programInit();

    //HAL_UART_Transmit_DMA(&huart2, (uint8_t *) abc, strlen(abc));
    //HAL_UART_Transmit(&huart2, (uint8_t *) abc, strlen(abc),100);
    /* USER CODE END 2 */

    /* Init scheduler */
    osKernelInitialize();
    /* Create the mutex(es) */
    /* creation of uartMutex */
    uartMutexHandle = osMutexNew(&uartMutex_attributes);

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* creation of keypadSem */
    keypadSemHandle = osSemaphoreNew(1, 0, &keypadSem_attributes);

    /* creation of volumeSem */
    volumeSemHandle = osSemaphoreNew(1, 0, &volumeSem_attributes);

    /* creation of uartDmaSem */
    uartDmaSemHandle = osSemaphoreNew(1, 1, &uartDmaSem_attributes);

    /* USER CODE BEGIN RTOS_SEMAPHORES */

    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* creation of melodyNameQu */
    melodyNameQuHandle = osMessageQueueNew(10, 4, &melodyNameQu_attributes);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */



    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of uartTxTsk */
    uartTxTskHandle = osThreadNew(StartUartTxTsk, NULL, &uartTxTsk_attributes);

    /* creation of getVolumeTsk */
    getVolumeTskHandle = osThreadNew(StartGetVolumeTsk, NULL, &getVolumeTsk_attributes);

    /* creation of updateLcdTsk */
    updateLcdTskHandle = osThreadNew(StartUpdateLcdTsk, NULL, &updateLcdTsk_attributes);

    /* creation of update7SegTsk */
    update7SegTskHandle = osThreadNew(StartUpdate7SegTsk, NULL, &update7SegTsk_attributes);

    /* creation of playMelodyTsk */
    playMelodyTskHandle = osThreadNew(StartPlayMelodyTsk, NULL, &playMelodyTsk_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    char abc[100];
    sprintf(abc, "dma testtt...\n");

    while (1) {
        while (myFlag == 0);
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *) abc, strlen(abc));
        myFlag = 0;
        HAL_Delay(500);
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_USART2
                                         | RCC_PERIPHCLK_TIM8;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
    PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void) {

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    /** Configure the ADC multi-mode
    */
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK) {
        Error_Handler();
    }

    /** Configure Regular Channel
    */
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void) {

    /* USER CODE BEGIN TIM8_Init 0 */

    /* USER CODE END TIM8_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    /* USER CODE BEGIN TIM8_Init 1 */

    /* USER CODE END TIM8_Init 1 */
    htim8.Instance = TIM8;
    htim8.Init.Prescaler = 0;
    htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim8.Init.Period = 65535;
    htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;
    htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim8) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 0;
    sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
    sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter = 0;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM8_Init 2 */

    /* USER CODE END TIM8_Init 2 */
    HAL_TIM_MspPostInit(&htim8);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void) {

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 600000;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void) {

    /* USER CODE BEGIN USB_Init 0 */

    /* USER CODE END USB_Init 0 */

    /* USER CODE BEGIN USB_Init 1 */

    /* USER CODE END USB_Init 1 */
    hpcd_USB_FS.Instance = USB;
    hpcd_USB_FS.Init.dev_endpoints = 8;
    hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
    hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd_USB_FS.Init.low_power_enable = DISABLE;
    hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
    if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USB_Init 2 */

    /* USER CODE END USB_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin | LD4_Pin | LD3_Pin | LD5_Pin
                             | LD7_Pin | LD9_Pin | LD10_Pin | LD8_Pin
                             | LD6_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12
                             | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_4 | GPIO_PIN_5
                             | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

    /*Configure GPIO pins : PE2 PE6 PE7 PE0
                             PE1 */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_0
                          | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                             LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                             LD6_Pin */
    GPIO_InitStruct.Pin = CS_I2C_SPI_Pin | LD4_Pin | LD3_Pin | LD5_Pin
                          | LD7_Pin | LD9_Pin | LD10_Pin | LD8_Pin
                          | LD6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : MEMS_INT3_Pin MEMS_INT4_Pin */
    GPIO_InitStruct.Pin = MEMS_INT3_Pin | MEMS_INT4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : PC13 PC1 PC2 PC3
                             PC7 PC8 PC9 PC10
                             PC11 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
                          | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                          | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PF9 PF10 PF2 PF4
                             PF6 */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_2 | GPIO_PIN_4
                          | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : PA1 PA8 PA9 PA10
                             PA15 */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                          | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : PC4 PC5 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PB0 PB1 PB2 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PB10 PB11 PB12 PB13
                             PB14 PB15 PB4 PB5
                             PB8 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
                          | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_4 | GPIO_PIN_5
                          | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PD8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PD9 PD10 PD11 PD12
                             PD13 PD14 PD4 PD5
                             PD6 PD7 */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12
                          | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_4 | GPIO_PIN_5
                          | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : PD15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
    GPIO_InitStruct.Pin = I2C1_SCL_Pin | I2C1_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */
/*
void StartUartTxTsk(void *argument)
{

char uartStr[100];
for (;;) {
osSemaphoreAcquire(keypadSemHandle, osWaitForever);

HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);

sprintf(uartStr, "keypad: %lu\n", keypadNum);

//osMutexAcquire(uartMutexHandle, osWaitForever);
// HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
tsts = osSemaphoreGetCount(uartDmaSemHandle);
//osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

//HAL_UART_Transmit_DMA(&huart2, (uint8_t *) uartStr, strlen(uartStr));

}

}
 //---------

 void StartUpdateLcdTsk(void *argument)
{


lcdInit();
for (;;) {
lcdUpdate();
osDelay(1);
}

}

 //--------------

 void StartUpdate7SegTsk(void *argument)
{

for (;;) {
update7Segment(score, difficulty);
osDelay(1);
}

}



*/
uint32_t lastGpioExti = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (lastGpioExti + 300 > osKernelGetTickCount())
        return;

    handleKeypad(GPIO_Pin);
    osSemaphoreRelease(keypadSemHandle);
    lastGpioExti = osKernelGetTickCount();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) { // volume
        //HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);
        osSemaphoreRelease(volumeSemHandle);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        osSemaphoreRelease(uartDmaSemHandle);
        //myFlag =1;
    }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
    /* USER CODE BEGIN 5 */
    /* Infinite loop */
    char defaultStr[100];
    //buzzerChangeTone(1000,1000);
    //sprintf(defaultStr, "from default\n");


    for (;;) {


        //osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);
        //HAL_UART_Transmit_DMA(&huart2, (uint8_t *) defaultStr, strlen(defaultStr));
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);

        osDelay(1000);
    }
    osThreadTerminate(NULL);
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartUartTxTsk */
/**
* @brief Function implementing the uartTxTsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTxTsk */
void StartUartTxTsk(void *argument) {
    /* USER CODE BEGIN StartUartTxTsk */
    char uartStr[100];
    /* Infinite loop */
    for (;;) {
        osSemaphoreAcquire(keypadSemHandle, osWaitForever);
        if (keypadNum == 16) {
            srand(osKernelGetTickCount());
            osThreadResume(updateLcdTskHandle);
        } else if (keypadNum == 5) {
            doodlerMoveLeft();
        } else if (keypadNum == 7) {
            doodlerMoveRight();
        }
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);

        sprintf(uartStr, "keypad: %lu\n", keypadNum);

        osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *) uartStr, strlen(uartStr));
    }
    /* USER CODE END StartUartTxTsk */
}

/* USER CODE BEGIN Header_StartGetVolumeTsk */
/**
* @brief Function implementing the getVolumeTsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGetVolumeTsk */
void StartGetVolumeTsk(void *argument) {
    /* USER CODE BEGIN StartGetVolumeTsk */
    char adcStr[100];
    adcInit();
    /* Infinite loop */
    for (;;) {

        osSemaphoreAcquire(volumeSemHandle, osWaitForever);
        handleVolume(&hadc1);
        //sprintf(adcStr, "volume: %lu\n", difficulty);

        //osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);
        // HAL_UART_Transmit_DMA(&huart2, (uint8_t *) adcStr, strlen(adcStr));

        osDelay(200);

        HAL_ADC_Start_IT(&hadc1);
    }
    /* USER CODE END StartGetVolumeTsk */
}

/* USER CODE BEGIN Header_StartUpdateLcdTsk */
/**
* @brief Function implementing the updateLcdTsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdateLcdTsk */
void StartUpdateLcdTsk(void *argument) {
    /* USER CODE BEGIN StartUpdateLcdTsk */
    osThreadSuspend(updateLcdTskHandle);
    char lcdStr[100];
    int writeCount = 0;
    int maxCount = 0;
    lcdInit();
    charactersInit();
//    lcdInitFirst();
    lcdUpdate();
    gameStart();
    uint32_t startTime;
    uint32_t duration;
    long int delay;
    /* Infinite loop */
    for (;;) {
        startTime = osKernelGetTickCount();
        writeCount = handleGame();
        if (writeCount > maxCount)
            maxCount = writeCount;
        duration = osKernelGetTickCount() - startTime;
        sprintf(lcdStr, "lcd Max Write Count: %d | duration %lu\n", writeCount,
                duration);//, duration - writeCount * 12);

        osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *) lcdStr, strlen(lcdStr));
        delay =  480 - writeCount * 12;
        osDelay(delay > 0 ? delay : 0);
    }
    /* USER CODE END StartUpdateLcdTsk */
}

/* USER CODE BEGIN Header_StartUpdate7SegTsk */
/**
* @brief Function implementing the update7SegTsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUpdate7SegTsk */
void StartUpdate7SegTsk(void *argument) {
    /* USER CODE BEGIN StartUpdate7SegTsk */
    /* Infinite loop */
    for (;;) {
        update7Segment(getScore(), difficulty);
        osDelay(1);
    }
    /* USER CODE END StartUpdate7SegTsk */
}

/* USER CODE BEGIN Header_StartPlayMelodyTsk */
/**
* @brief Function implementing the playMelodyTsk thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPlayMelodyTsk */
void StartPlayMelodyTsk(void *argument) {
    /* USER CODE BEGIN StartPlayMelodyTsk */
    /* Infinite loop */
    osStatus_t status;
    melodyName melodyToPlay;
    for (;;) {
        status = osMessageQueueGet(melodyNameQuHandle, &melodyToPlay, NULL, osWaitForever);
        if (status == osOK) {
            buzzerMelodyPlay(melodyToPlay);
        }

    }
    /* USER CODE END StartPlayMelodyTsk */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
