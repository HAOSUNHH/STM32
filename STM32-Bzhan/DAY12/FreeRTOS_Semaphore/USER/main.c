/****************************************************************

*****************************************************************/
#include "includes.h"

TaskHandle_t app_task_init_handle 			= NULL;
TaskHandle_t app_task_1_handle		= NULL;
TaskHandle_t app_task_2_handle			= NULL;
TaskHandle_t app_task_3_handle			= NULL;

/* 互斥型信号量句柄 */
SemaphoreHandle_t g_mutex_printf;
SemaphoreHandle_t g_mutex_oled;

/* 事件标志组句柄 */
EventGroupHandle_t g_event_group;

/* 消息队列句柄 */
QueueHandle_t g_queue_usart;
QueueHandle_t g_queue_led;
QueueHandle_t g_queue_beep;
QueueHandle_t g_queue_oled;
QueueHandle_t g_queue_kbd;

/* 二值信号量 */
SemaphoreHandle_t g_sem_led;
SemaphoreHandle_t g_sem_beep;
SemaphoreHandle_t g_sem_1;

/* 软件定时器句柄 */
static TimerHandle_t soft_timer_Handle = NULL;

/* 软件定时器 */
static void soft_timer_callback(TimerHandle_t pxTimer);


/* 全局变量 */
volatile uint32_t g_oled_display_flag = 1;
volatile uint32_t g_dht_get_what = FLAG_DHT_GET_NONE;
volatile uint32_t g_rtc_get_what = FLAG_RTC_GET_NONE;
volatile uint32_t g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
volatile uint32_t g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF; // 记录手势识别模块是否显示手势图片
volatile uint32_t g_gesture_valid = 0;								 // 记录手势识别模块是否连接成功
volatile uint32_t g_system_no_opreation_cnt = 0;
volatile uint32_t g_pass_man_what = FLAG_PASS_MAN_NONE;
volatile uint32_t g_mpu6050_run_flag=1;
volatile cpu_operating_mode_t g_cpu_operating_mode=CPU_RUN;


/* OLED互斥锁高度封装 */
#define OLED_SAFE(__CODE)                                \
	{                                                    \
                                                         \
		do                                               \
		{                                                \
			xSemaphoreTake(g_mutex_oled, portMAX_DELAY); \
			if (g_oled_display_flag)                     \
				__CODE;                                  \
			xSemaphoreGive(g_mutex_oled);                \
		} while (0)                                      \
	}

#define DEBUG_PRINTF_EN 1

void dgb_printf_safe(const char *format, ...)
{
#if DEBUG_PRINTF_EN

	va_list args;
	va_start(args, format);

	/* 获取互斥信号量 */
	xSemaphoreTake(g_mutex_printf, portMAX_DELAY);

	vprintf(format, args);

	/* 释放互斥信号量 */
	xSemaphoreGive(g_mutex_printf);

	va_end(args);
#else
	(void)0;
#endif
}
/**
 * @brief 主函数
 * @param void:无须传入参数
 * @retval 无
 */
int main(void)
{
	/* 设置系统中断优先级分组4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* 系统定时器中断频率为configTICK_RATE_HZ */
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

	/* 初始化串口1 */
	uart_init(115200);

	/* 创建 app_task_init任务 */
	xTaskCreate((TaskFunction_t)app_task_init,			/* 任务入口函数 */
				(const char *)"app_task_init",			/* 任务名字 */
				(uint16_t)512,							/* 任务栈大小 */
				(void *)NULL,							/* 任务入口函数参数 */
				(UBaseType_t)5,							/* 任务的优先级 */
				(TaskHandle_t *)&app_task_init_handle); /* 任务控制块指针 */

	/* 开启任务调度 */
	vTaskStartScheduler();

	while (1);
}

/* 任务列表 */
static const task_t task_tbl[] = {
	{app_task_1, 		"app_task_1", 		1024, NULL, 4, &app_task_1_handle},
	{app_task_2, 		"app_task_2", 		512, NULL, 3, &app_task_2_handle},
//	{app_task_3, 		"app_task_3", 		512, NULL, 5, &app_task_3_handle},
	{0, 0, 0, 0, 0, 0}
};

/**
 * @brief 集中初始化任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 */
static void app_task_init(void *pvParameters)
{
	uint32_t i = 0;

	printf("[app_task_init] create success\r\n");

	/* 创建互斥型信号量 */ 
	g_mutex_printf 	= xSemaphoreCreateMutex();
	g_mutex_oled 	= xSemaphoreCreateMutex();
	//g_sem_1 = xSemaphoreCreateMutex();

	/* 创建二值型信号量 */
	g_sem_led  = xSemaphoreCreateBinary();
	g_sem_beep = xSemaphoreCreateBinary();
	g_sem_1 = xSemaphoreCreateBinary();

	/* 创建事件标志组 */
	g_event_group = xEventGroupCreate();

	/* 创建消息队列 */
	g_queue_usart 	= xQueueCreate(QUEUE_USART_LEN, sizeof(struct usart_packet_t));
	g_queue_led  	= xQueueCreate(QUEUE_LED_LEN, 	sizeof(uint8_t));
	g_queue_beep  	= xQueueCreate(QUEUE_BEEP_LEN, 	sizeof(beep_t));
	g_queue_oled 	= xQueueCreate(QUEUE_OLED_LEN, 	sizeof(oled_t));
	g_queue_kbd 	= xQueueCreate(QUEUE_KBD_LEN, 	sizeof(uint8_t));

	/* 持续2秒 */
	vTaskDelay(2000);


#if IWDG_ENABLE
	/* 独立看门狗初始化 */
	iwdg_init();
#endif



	/* 创建用到的任务 */
	taskENTER_CRITICAL();
	i = 0;
	while (task_tbl[i].pxTaskCode)
	{
		xTaskCreate(task_tbl[i].pxTaskCode,		/* 任务入口函数 */
					task_tbl[i].pcName,			/* 任务名字 */
					task_tbl[i].usStackDepth,	/* 任务栈大小 */
					task_tbl[i].pvParameters,	/* 任务入口函数参数 */
					task_tbl[i].uxPriority,		/* 任务的优先级 */
					task_tbl[i].pxCreatedTask); /* 任务控制块指针 */
		i++;
	}
	taskEXIT_CRITICAL();

	/* 创建周期软件定时器 */
	soft_timer_Handle = xTimerCreate((const char *)"AutoReloadTimer",
									 (TickType_t)1000,	  /* 定时器周期 1000(tick) */
									 (UBaseType_t)pdTRUE, /* 周期模式 */
									 (void *)1,			  /* 为每个计时器分配一个索引的唯一ID */
									 (TimerCallbackFunction_t)soft_timer_callback);
	/* 开启周期软件定时器 */
	xTimerStart(soft_timer_Handle, 0);
	
	/* 删除任务自身 */
	vTaskDelete(NULL);

	printf("[app_task_init] nerver run here\r\n");
}

//循环延时
void delay(unsigned int ms)
{
	unsigned int i,j;

	for(i=0;i<ms;i++)
		for(j=0;j<5000;j++);
}

/**
  * @brief mpu6050任务
  * @param pvParameters:创建任务时传递的参数
  * @retval 无
  */
extern struct usart_packet_t g_usart_packet;

static void app_task_1(void *pvParameters)
{
	printf("app_task_1 start!\r\n");
	
	for(;;){
		//获取信号量
		if(xSemaphoreTake(g_sem_1,portMAX_DELAY)==pdTRUE){
			printf("############xSemaphoreTake success!\r\n");
		}
		
		//处理串口信息
		printf("command = %s\r\n",g_usart_packet.rx_buf);
	
		//释放信号量
//		if(xSemaphoreGive(g_sem_1)==pdTRUE){
//			printf("############xSemaphoreGive success!\r\n");
//		}
		
		//清空缓冲区
		memset(&g_usart_packet,0,sizeof g_usart_packet);
		
		vTaskDelay(300);
	}
}


/**
 * @brief 按键任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 */
static void app_task_2(void *pvParameters)
{
	u32 cnt = 0;
	printf("app_task_2 start!\r\n");
	
	for(;;){
		//获取信号量
//		if(xSemaphoreTake(g_sem_1,portMAX_DELAY)==pdTRUE){
//			printf("***********xSemaphoreTake success!\r\n");
//		}
		
		cnt++;
		printf("app_task_2 started = %d!\r\n",cnt);
		delay(20000);
		printf("app_task_2 finished = %d!\r\n",cnt);
		
		//释放信号量
//		if(xSemaphoreGive(g_sem_1)==pdTRUE){
//			printf("***********xSemaphoreGive success!\r\n");
//		}
		
		vTaskDelay(400);
	}
}

static void app_task_3(void *pvParameters)
{
	u32 cnt = 0;
	printf("app_task_3 start!\r\n");
	
	for(;;){
		cnt++;
		printf("app_task_3 started = %d!\r\n",cnt);
		delay(10000);
		printf("app_task_3 finished = %d!\r\n",cnt);
		
		vTaskDelay(500);
	}
}

/**
 * @brief 软件定时器任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 倒数管理、独立看门狗喂狗
 */
static void soft_timer_callback(TimerHandle_t pxTimer)
{

}


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	printf("[vApplicationMallocFailedHook] ....\r\n");
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API functi    on (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */

	/* 进入睡眠模式 */
	if(g_cpu_operating_mode == CPU_SLEEP)
		__wfi();
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void)pcTaskName;
	(void)pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	printf("[vApplicationStackOverflowHook] %s is StackOverflow\r\n", pcTaskName);
	for (;;);
}

void vApplicationTickHook(void)
{
}
