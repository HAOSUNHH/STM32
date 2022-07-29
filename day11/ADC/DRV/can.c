#include <can.h>
#include <stdio.h>

void can1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	//1.开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	//2.初始化PD0 PD1为CAN功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	
	//3.初始化CAN1 42M/4/(1+12+8) = 500K
	CAN_InitStruct.CAN_Prescaler = 4;//4分频
	CAN_InitStruct.CAN_BS1 = CAN_BS1_12tq;//12Tq
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;//8Tq
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;//1Tq
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;//正常模式
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;
	CAN_Init(CAN1,&CAN_InitStruct);
	
	//4.过滤器初始化
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//使能
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//使用FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;//掩码模式(列表模式)
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//过滤器长度
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//使用16位列表模式
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//使能
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//使用FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;//
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//过滤器长度
//	//提供4个标准数据帧的ID IDE=0 RTR=0 只接收 0x123 0x321 0x7ad 0x000
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123<<5;
//	CAN_FilterInitStruct.CAN_FilterIdLow = 0x321<<5;
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x7ad<<5;
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//32位列表模式
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//使能
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//使用FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;//
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;//过滤器长度
//	//提供2个扩展数据帧的ID IDE=1 RTR=0 只接收 0x7890abc 0xcba9876
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x7890abc>>13;//ID高16位
//	CAN_FilterInitStruct.CAN_FilterIdLow = ((0x7890abc<<3)&0xffff)|(0x1<<2);//ID的低13位放在高位，IDE=1
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xcba9876>>13;//ID高16位
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = ((0xcba9876<<3)&0xffff)|(0x1<<2);//ID的低13位放在高位，IDE=1
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//16位掩码模式
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//使能
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//使用FIFO0
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;//掩码模式(列表模式)
	CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//过滤器长度
	//对0x123 0x321进行掩码操作
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123<<5;;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x321<<5;;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xf0<<5;//0x*2*
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x70f<<5;//0x3*1
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//5.初始化CAN1的接收中断
	NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;//CAN接收中断通道0
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
	
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

CanTxMsg CAN1_TX_MSG;
CanRxMsg CAN1_RX_MSG;

//发送函数 0 - 成功 1 - 失败
u8 can1_send_message(u8 *data,u8 len,u32 message_id)
{
	u8 i,mailbox;
	u32 cnt = 0;
	
	if(len>8)
		return 1;
	
	//根据id值得范围选择标准帧/扩展帧
	if(message_id>0x7ff){
		CAN1_TX_MSG.IDE = CAN_Id_Extended;//扩展帧
	}
	else{
		CAN1_TX_MSG.IDE = CAN_Id_Standard;//标准帧
	}
	
	CAN1_TX_MSG.RTR = CAN_RTR_Data;
	CAN1_TX_MSG.DLC = len;
	CAN1_TX_MSG.ExtId = message_id;
	CAN1_TX_MSG.StdId = message_id;
	//放入数据
	for(i=0;i<len;i++){
		CAN1_TX_MSG.Data[i] = data[i];
	}
	
	//发送数据并等待发送成功
	mailbox = CAN_Transmit(CAN1,&CAN1_TX_MSG);
	do{
		cnt++;
	}while(CAN_TransmitStatus(CAN1,mailbox)!=CAN_TxStatus_Ok&&cnt<1000);

	if(cnt<1000)
		return 0;
	else
		return 1;
}


//CAN中断处理函数
void CAN1_RX0_IRQHandler(void)
{
	//CAN1收到数据
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)==SET){
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_RX_MSG);
		//存入缓冲区,通过串口打印
		//printf("%s\r\n",CAN1_RX_MSG.Data);
		//原路发回
		if(CAN1_RX_MSG.IDE==CAN_Id_Standard){
			can1_send_message(CAN1_RX_MSG.Data,CAN1_RX_MSG.DLC,CAN1_RX_MSG.StdId);
		}
		else{
			can1_send_message(CAN1_RX_MSG.Data,CAN1_RX_MSG.DLC,CAN1_RX_MSG.ExtId);
		}
		
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}
}

