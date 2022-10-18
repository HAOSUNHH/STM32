#include <flash.h>
#include <stdio.h>

void flash_test(void)
{
	FLASH_Status status;

	//1.���д����
	FLASH_Unlock();
	
	//2.�����־
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
					FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	//3.��������5
	status = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3);
	if(status!=FLASH_COMPLETE){
		printf("erase sector 5 error!\r\n");
	}
	
	//4.д����
	status = FLASH_ProgramWord(SECTOR_5_ADDRESS,0x55555555);
	if(status!=FLASH_COMPLETE){
		printf("write sector 5 error!\r\n");
	}

	//5.�ָ�д����
	FLASH_Lock();
}

void flash_write(u8 temp,u8 humi)
{
	FLASH_Status status;

	//1.���д����
	FLASH_Unlock();
	
	//2.�����־
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
					FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	//3.��������5
	status = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3);
	if(status!=FLASH_COMPLETE){
		printf("erase sector 5 error!\r\n");
	}
	
	//4.д����
	status = FLASH_ProgramByte(SECTOR_5_ADDRESS,temp);
	if(status!=FLASH_COMPLETE){
		printf("write temp error!\r\n");
	}
	
	status = FLASH_ProgramByte(SECTOR_5_ADDRESS+1,humi);
	if(status!=FLASH_COMPLETE){
		printf("write humi error!\r\n");
	}

	//5.�ָ�д����
	FLASH_Lock();
}
