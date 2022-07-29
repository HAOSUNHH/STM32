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
		
		//1.ʹ��PWRʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		
		//2.ʹ��RTC�Ĵ�����д����
		PWR_BackupAccessCmd(ENABLE);
		
		//3.ʹ��LSE������ΪRTCʱ��Դ
		RCC_LSEConfig(RCC_LSE_ON);
		//�ȴ�LSE����
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=SET);
		//ѡ��LSE��ΪRTCʱ��Դ
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		//4.ʹ��RTCʱ��
		RCC_RTCCLKCmd(ENABLE);
		
		//5.�ر�RTCд����,�ȴ�����
		RTC_WaitForSynchro();
		
		//6.��ʼ��RTC,���÷�Ƶϵ����Сʱ��ʽ  1Hz
		RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;//24Сʱ��ʽ
		RTC_InitStruct.RTC_AsynchPrediv = 0x7f;//�첽��Ƶϵ�� 128
		RTC_InitStruct.RTC_SynchPrediv = 0xff;//ͬ����Ƶϵ�� 256
		RTC_Init(&RTC_InitStruct);
		
		//7.�������ں�ʱ�� BCD��
		RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
		RTC_TimeStruct.RTC_Hours = 0x09;//ʱ
		RTC_TimeStruct.RTC_Minutes = 0x41;//��
		RTC_TimeStruct.RTC_Seconds = 0x50;//��
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
		
		RTC_DateStruct.RTC_Year = 0x22;
		RTC_DateStruct.RTC_Month = RTC_Month_May;
		RTC_DateStruct.RTC_Date = 0x14;
		RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Saturday;
		RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);
		
		//8.д���ݼĴ���
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
	}
}

//���ӳ�ʼ��
void rtc_alarm_init(void)
{
	RTC_AlarmTypeDef RTC_AlarmStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//4.�ر�����
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	//5.�������Ӳ���
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;//����
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x10;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x42;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0x0;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//����
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;//14��
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_Hours|RTC_AlarmMask_DateWeekDay;//RTC_AlarmMask_None;//������
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStruct);
	
	//6.����жϱ�־
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
	
	//7.ʹ������A�ж�
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	//8.��ʼ���ⲿ�ж�17
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;//�ⲿ�ж�17
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);
	
	//9.��ʼ��NVIC
	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;//RTC�����ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x2;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x2;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	//10.��������
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

//RTC�����жϴ�����
void RTC_Alarm_IRQHandler(void)
{
	//����A�ж�
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET){
		//BEEP = 1;
		D1 = 0;
		//��������жϱ�־
		RTC_ClearITPendingBit(RTC_IT_ALRA);
	}
	
    
    //����ⲿ�жϱ�־
    EXTI_ClearITPendingBit(EXTI_Line17);
}
