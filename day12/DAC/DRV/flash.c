#include <flash.h>
#include <stdio.h>

void flash_test(void)
{
	FLASH_Status status;

	//1.解除写保护
	FLASH_Unlock();
	
	//2.清除标志
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
					FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	//3.擦除扇区5
	status = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3);
	if(status!=FLASH_COMPLETE){
		printf("erase sector 5 error!\r\n");
	}
	
	//4.写数据
	status = FLASH_ProgramWord(SECTOR_5_ADDRESS,0x55555555);
	if(status!=FLASH_COMPLETE){
		printf("write sector 5 error!\r\n");
	}

	//5.恢复写保护
	FLASH_Lock();
}

void flash_write(u8 temp,u8 humi)
{
	FLASH_Status status;

	//1.解除写保护
	FLASH_Unlock();
	
	//2.清除标志
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
					FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	//3.擦除扇区5
	status = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3);
	if(status!=FLASH_COMPLETE){
		printf("erase sector 5 error!\r\n");
	}
	
	//4.写数据
	status = FLASH_ProgramByte(SECTOR_5_ADDRESS,temp);
	if(status!=FLASH_COMPLETE){
		printf("write temp error!\r\n");
	}
	
	status = FLASH_ProgramByte(SECTOR_5_ADDRESS+1,humi);
	if(status!=FLASH_COMPLETE){
		printf("write humi error!\r\n");
	}

	//5.恢复写保护
	FLASH_Lock();
}
