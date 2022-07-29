#include <can.h>
#include <stdio.h>

void can1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	CAN_InitTypeDef CAN_InitStruct;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	//1.����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	//2.��ʼ��PD0 PD1ΪCAN����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	
	//3.��ʼ��CAN1 42M/4/(1+12+8) = 500K
	CAN_InitStruct.CAN_Prescaler = 4;//4��Ƶ
	CAN_InitStruct.CAN_BS1 = CAN_BS1_12tq;//12Tq
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;//8Tq
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;//1Tq
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;//����ģʽ
	CAN_InitStruct.CAN_ABOM = DISABLE;
	CAN_InitStruct.CAN_AWUM = DISABLE;
	CAN_InitStruct.CAN_NART = DISABLE;
	CAN_InitStruct.CAN_RFLM = DISABLE;
	CAN_InitStruct.CAN_TTCM = DISABLE;
	CAN_InitStruct.CAN_TXFP = DISABLE;
	CAN_Init(CAN1,&CAN_InitStruct);
	
	//4.��������ʼ��
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//ʹ��
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//ʹ��FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;//����ģʽ(�б�ģʽ)
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//ʹ�ù�����0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//����������
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000;
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//ʹ��16λ�б�ģʽ
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//ʹ��
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//ʹ��FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;//
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//ʹ�ù�����0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//����������
//	//�ṩ4����׼����֡��ID IDE=0 RTR=0 ֻ���� 0x123 0x321 0x7ad 0x000
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123<<5;
//	CAN_FilterInitStruct.CAN_FilterIdLow = 0x321<<5;
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x7ad<<5;
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//32λ�б�ģʽ
//	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//ʹ��
//	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//ʹ��FIFO0
//	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;//
//	CAN_FilterInitStruct.CAN_FilterNumber = 0;//ʹ�ù�����0
//	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;//����������
//	//�ṩ2����չ����֡��ID IDE=1 RTR=0 ֻ���� 0x7890abc 0xcba9876
//	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x7890abc>>13;//ID��16λ
//	CAN_FilterInitStruct.CAN_FilterIdLow = ((0x7890abc<<3)&0xffff)|(0x1<<2);//ID�ĵ�13λ���ڸ�λ��IDE=1
//	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xcba9876>>13;//ID��16λ
//	CAN_FilterInitStruct.CAN_FilterMaskIdLow = ((0xcba9876<<3)&0xffff)|(0x1<<2);//ID�ĵ�13λ���ڸ�λ��IDE=1
//	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//16λ����ģʽ
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;//ʹ��
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//ʹ��FIFO0
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;//����ģʽ(�б�ģʽ)
	CAN_FilterInitStruct.CAN_FilterNumber = 0;//ʹ�ù�����0
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;//����������
	//��0x123 0x321�����������
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123<<5;;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x321<<5;;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xf0<<5;//0x*2*
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x70f<<5;//0x3*1
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	//5.��ʼ��CAN1�Ľ����ж�
	NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;//CAN�����ж�ͨ��0
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x1;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
	
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

CanTxMsg CAN1_TX_MSG;
CanRxMsg CAN1_RX_MSG;

//���ͺ��� 0 - �ɹ� 1 - ʧ��
u8 can1_send_message(u8 *data,u8 len,u32 message_id)
{
	u8 i,mailbox;
	u32 cnt = 0;
	
	if(len>8)
		return 1;
	
	//����idֵ�÷�Χѡ���׼֡/��չ֡
	if(message_id>0x7ff){
		CAN1_TX_MSG.IDE = CAN_Id_Extended;//��չ֡
	}
	else{
		CAN1_TX_MSG.IDE = CAN_Id_Standard;//��׼֡
	}
	
	CAN1_TX_MSG.RTR = CAN_RTR_Data;
	CAN1_TX_MSG.DLC = len;
	CAN1_TX_MSG.ExtId = message_id;
	CAN1_TX_MSG.StdId = message_id;
	//��������
	for(i=0;i<len;i++){
		CAN1_TX_MSG.Data[i] = data[i];
	}
	
	//�������ݲ��ȴ����ͳɹ�
	mailbox = CAN_Transmit(CAN1,&CAN1_TX_MSG);
	do{
		cnt++;
	}while(CAN_TransmitStatus(CAN1,mailbox)!=CAN_TxStatus_Ok&&cnt<1000);

	if(cnt<1000)
		return 0;
	else
		return 1;
}


//CAN�жϴ�����
void CAN1_RX0_IRQHandler(void)
{
	//CAN1�յ�����
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)==SET){
		CAN_Receive(CAN1,CAN_FIFO0,&CAN1_RX_MSG);
		//���뻺����,ͨ�����ڴ�ӡ
		//printf("%s\r\n",CAN1_RX_MSG.Data);
		//ԭ·����
		if(CAN1_RX_MSG.IDE==CAN_Id_Standard){
			can1_send_message(CAN1_RX_MSG.Data,CAN1_RX_MSG.DLC,CAN1_RX_MSG.StdId);
		}
		else{
			can1_send_message(CAN1_RX_MSG.Data,CAN1_RX_MSG.DLC,CAN1_RX_MSG.ExtId);
		}
		
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}
}

