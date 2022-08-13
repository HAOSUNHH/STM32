#include <stm32f4xx.h>
#include <rtc.h>
#include <beep.h>
#include <led.h>

void rtc_init(void)
{
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x8888){
		RTC_InitTypeDef RTC_InitStruct;
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
		
		//1.使能PWR时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		
		//2.使能RTC寄存器的写操作
		PWR_BackupAccessCmd(ENABLE);
		
		//3.使能LSE，并作为RTC时钟源
		RCC_LSEConfig(RCC_LSE_ON);
		//等待LSE就绪
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);
		//选择LSE作为RTC时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		//4.使能RTC时钟
		RCC_RTCCLKCmd(ENABLE);
		
		//5.关闭RTC写保护,等待就绪
		RTC_WaitForSynchro();
		
		//6.初始化RTC,配置分频系数和小时格式  1Hz
		RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;//24小时格式
		RTC_InitStruct.RTC_AsynchPrediv = 0x7f;//异步分频系数 128
		RTC_InitStruct.RTC_SynchPrediv = 0xff;//同步分频系数 256
		RTC_Init(&RTC_InitStruct);
		
		//7.设置日期和时间 BCD码
		RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
		RTC_TimeStruct.RTC_Hours = 0x09;//时
		RTC_TimeStruct.RTC_Minutes = 0x41;//分
		RTC_TimeStruct.RTC_Seconds = 0x50;//秒
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
		
		RTC_DateStruct.RTC_Year = 0x22;
		RTC_DateStruct.RTC_Month = RTC_Month_May;
		RTC_DateStruct.RTC_Date = 0x14;
		RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Saturday;
		RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
		
		//8.写备份寄存器
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
	}
}

//闹钟初始化
void rtc_alarm_init(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//4.关闭闹钟
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//5.设置闹钟参数
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;//上午
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x10;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x42;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0x0;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//日期
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;//14号
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_Hours|RTC_AlarmMask_DateWeekDay;//RTC_AlarmMask_None;//不屏蔽
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
	
	//6.清除中断标志
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
	
	//7.使能闹钟A中断
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//8.初始化外部中断17
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;//外部中断17
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);
	
	//9.初始化NVIC
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;//RTC闹钟中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	//10.开启闹钟
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

//RTC闹钟中断处理函数
void RTC_Alarm_IRQHandler(void)
{
	//闹钟A中断
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET){
		//BEEP = 1;
		D1 = 0;
		//清除闹钟中断标志
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	
    
    //清除外部中断标志
    EXTI_ClearITPendingBit(EXTI_Line17);
}
