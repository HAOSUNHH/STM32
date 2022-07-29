/****************************************************************

*****************************************************************/
#include "includes.h"

TaskHandle_t app_task_init_handle 			= NULL;
TaskHandle_t app_task_1_handle		= NULL;
TaskHandle_t app_task_2_handle			= NULL;
TaskHandle_t app_task_3_handle			= NULL;

/* �������ź������ */
SemaphoreHandle_t g_mutex_printf;
SemaphoreHandle_t g_mutex_oled;

/* �¼���־���� */
EventGroupHandle_t g_event_group;

/* ��Ϣ���о�� */
QueueHandle_t g_queue_usart;
QueueHandle_t g_queue_led;
QueueHandle_t g_queue_beep;
QueueHandle_t g_queue_oled;
QueueHandle_t g_queue_kbd;

/* ��ֵ�ź��� */
SemaphoreHandle_t g_sem_led;
SemaphoreHandle_t g_sem_beep;
SemaphoreHandle_t g_sem_1;

/* �����ʱ����� */
static TimerHandle_t soft_timer_Handle = NULL;

/* �����ʱ�� */
static void soft_timer_callback(TimerHandle_t pxTimer);


/* ȫ�ֱ��� */
volatile uint32_t g_oled_display_flag = 1;
volatile uint32_t g_dht_get_what = FLAG_DHT_GET_NONE;
volatile uint32_t g_rtc_get_what = FLAG_RTC_GET_NONE;
volatile uint32_t g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
volatile uint32_t g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF; // ��¼����ʶ��ģ���Ƿ���ʾ����ͼƬ
volatile uint32_t g_gesture_valid = 0;								 // ��¼����ʶ��ģ���Ƿ����ӳɹ�
volatile uint32_t g_system_no_opreation_cnt = 0;
volatile uint32_t g_pass_man_what = FLAG_PASS_MAN_NONE;
volatile uint32_t g_mpu6050_run_flag=1;
volatile cpu_operating_mode_t g_cpu_operating_mode=CPU_RUN;


/* OLED�������߶ȷ�װ */
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

	/* ��ȡ�����ź��� */
	xSemaphoreTake(g_mutex_printf, portMAX_DELAY);

	vprintf(format, args);

	/* �ͷŻ����ź��� */
	xSemaphoreGive(g_mutex_printf);

	va_end(args);
#else
	(void)0;
#endif
}
/**
 * @brief ������
 * @param void:���봫�����
 * @retval ��
 */
int main(void)
{
	/* ����ϵͳ�ж����ȼ�����4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* ϵͳ��ʱ���ж�Ƶ��ΪconfigTICK_RATE_HZ */
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

	/* ��ʼ������1 */
	uart_init(115200);

	/* ���� app_task_init���� */
	xTaskCreate((TaskFunction_t)app_task_init,			/* ������ں��� */
				(const char *)"app_task_init",			/* �������� */
				(uint16_t)512,							/* ����ջ��С */
				(void *)NULL,							/* ������ں������� */
				(UBaseType_t)5,							/* ��������ȼ� */
				(TaskHandle_t *)&app_task_init_handle); /* ������ƿ�ָ�� */

	/* ����������� */
	vTaskStartScheduler();

	while (1);
}

/* �����б� */
static const task_t task_tbl[] = {
	{app_task_1, 		"app_task_1", 		1024, NULL, 4, &app_task_1_handle},
	{app_task_2, 		"app_task_2", 		512, NULL, 3, &app_task_2_handle},
//	{app_task_3, 		"app_task_3", 		512, NULL, 5, &app_task_3_handle},
	{0, 0, 0, 0, 0, 0}
};

/**
 * @brief ���г�ʼ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 */
static void app_task_init(void *pvParameters)
{
	uint32_t i = 0;

	printf("[app_task_init] create success\r\n");

	/* �����������ź��� */ 
	g_mutex_printf 	= xSemaphoreCreateMutex();
	g_mutex_oled 	= xSemaphoreCreateMutex();
	//g_sem_1 = xSemaphoreCreateMutex();

	/* ������ֵ���ź��� */
	g_sem_led  = xSemaphoreCreateBinary();
	g_sem_beep = xSemaphoreCreateBinary();
	g_sem_1 = xSemaphoreCreateBinary();

	/* �����¼���־�� */
	g_event_group = xEventGroupCreate();

	/* ������Ϣ���� */
	g_queue_usart 	= xQueueCreate(QUEUE_USART_LEN, sizeof(struct usart_packet_t));
	g_queue_led  	= xQueueCreate(QUEUE_LED_LEN, 	sizeof(uint8_t));
	g_queue_beep  	= xQueueCreate(QUEUE_BEEP_LEN, 	sizeof(beep_t));
	g_queue_oled 	= xQueueCreate(QUEUE_OLED_LEN, 	sizeof(oled_t));
	g_queue_kbd 	= xQueueCreate(QUEUE_KBD_LEN, 	sizeof(uint8_t));

	/* ����2�� */
	vTaskDelay(2000);


#if IWDG_ENABLE
	/* �������Ź���ʼ�� */
	iwdg_init();
#endif



	/* �����õ������� */
	taskENTER_CRITICAL();
	i = 0;
	while (task_tbl[i].pxTaskCode)
	{
		xTaskCreate(task_tbl[i].pxTaskCode,		/* ������ں��� */
					task_tbl[i].pcName,			/* �������� */
					task_tbl[i].usStackDepth,	/* ����ջ��С */
					task_tbl[i].pvParameters,	/* ������ں������� */
					task_tbl[i].uxPriority,		/* ��������ȼ� */
					task_tbl[i].pxCreatedTask); /* ������ƿ�ָ�� */
		i++;
	}
	taskEXIT_CRITICAL();

	/* �������������ʱ�� */
	soft_timer_Handle = xTimerCreate((const char *)"AutoReloadTimer",
									 (TickType_t)1000,	  /* ��ʱ������ 1000(tick) */
									 (UBaseType_t)pdTRUE, /* ����ģʽ */
									 (void *)1,			  /* Ϊÿ����ʱ������һ��������ΨһID */
									 (TimerCallbackFunction_t)soft_timer_callback);
	/* �������������ʱ�� */
	xTimerStart(soft_timer_Handle, 0);
	
	/* ɾ���������� */
	vTaskDelete(NULL);

	printf("[app_task_init] nerver run here\r\n");
}

//ѭ����ʱ
void delay(unsigned int ms)
{
	unsigned int i,j;

	for(i=0;i<ms;i++)
		for(j=0;j<5000;j++);
}

/**
  * @brief mpu6050����
  * @param pvParameters:��������ʱ���ݵĲ���
  * @retval ��
  */
extern struct usart_packet_t g_usart_packet;

static void app_task_1(void *pvParameters)
{
	printf("app_task_1 start!\r\n");
	
	for(;;){
		//��ȡ�ź���
		if(xSemaphoreTake(g_sem_1,portMAX_DELAY)==pdTRUE){
			printf("############xSemaphoreTake success!\r\n");
		}
		
		//��������Ϣ
		printf("command = %s\r\n",g_usart_packet.rx_buf);
	
		//�ͷ��ź���
//		if(xSemaphoreGive(g_sem_1)==pdTRUE){
//			printf("############xSemaphoreGive success!\r\n");
//		}
		
		//��ջ�����
		memset(&g_usart_packet,0,sizeof g_usart_packet);
		
		vTaskDelay(300);
	}
}


/**
 * @brief ��������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 */
static void app_task_2(void *pvParameters)
{
	u32 cnt = 0;
	printf("app_task_2 start!\r\n");
	
	for(;;){
		//��ȡ�ź���
//		if(xSemaphoreTake(g_sem_1,portMAX_DELAY)==pdTRUE){
//			printf("***********xSemaphoreTake success!\r\n");
//		}
		
		cnt++;
		printf("app_task_2 started = %d!\r\n",cnt);
		delay(20000);
		printf("app_task_2 finished = %d!\r\n",cnt);
		
		//�ͷ��ź���
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
 * @brief �����ʱ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ���������������Ź�ι��
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

	/* ����˯��ģʽ */
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
