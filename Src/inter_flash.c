/**
  ******************************************************************************
  * @file           : cpuFlash.c
  * @brief          : cpu内部flash
  ******************************************************************************
  * @attention
  *
  * Copyright: https://blog.csdn.net/qq_30155503
  * All rights reserved.
  *
  ******************************************************************************
  */


/*****************************************************************
* 包含头文件
******************************************************************/

#include "inter_flash.h"
#include "tool.h"
#include "usb_data.h"
#include <math.h>




_UpdatePara UpdatePara;


/****************************************************************
* Func 	:
* Desc	:	读取CPU内部flash
* Input	:
* Output:
* Return:
*****************************************************************/
int32_t cpuflash_read(uint32_t unStartAddr, uint8_t *pData, uint16_t usSize)
{

	if(pData == NULL)
		return -1;

	memcpy(pData, (int8_t *)unStartAddr, usSize);

	return 0;
}

/****************************************************************
* Func 	:
* Desc	:	写入CPU内部flash (要先erase才能写)
* Input	:
* Output:
* Return:
*****************************************************************/
int32_t cpuflash_write(uint32_t unStartAddr, uint8_t *pData, uint16_t usSize)
{
	int32_t  cwIndex1= 0;
  int32_t 	cwIndex2 = 0;
	uint16_t nowSize=0;
  int32_t 	nRet = 0;

  uint64_t 	usTempALL = 0;
	
	uint32_t  lowD=0;
	uint32_t highD=0;

	
	 nowSize=usSize;
   if(nowSize%8 != 0)
   {
        nowSize += (8-usSize%8);
    }

	  HAL_FLASH_Unlock();		// unlock
    for(cwIndex1=0; cwIndex1<nowSize/8; cwIndex1++)
    {
			usTempALL=0;
			lowD=0;
			highD=0;
			for(cwIndex2=0;cwIndex2<8;cwIndex2++)
			{
				if(cwIndex1*8+cwIndex2>=usSize)
				{
					break;
				}
				
				switch(cwIndex2)
				{
					case 0:
						lowD+=pData[cwIndex1*8+cwIndex2];
						break;
				  case 1:
						lowD+=pData[cwIndex1*8+cwIndex2]*0x100;
						break;
					case 2:
						lowD+=pData[cwIndex1*8+cwIndex2]*0x10000;
						break;
					case 3:
						lowD+=pData[cwIndex1*8+cwIndex2]*0x1000000;
						break;
					case 4:
						highD+=pData[cwIndex1*8+cwIndex2];
						break;
					case 5:
						highD+=pData[cwIndex1*8+cwIndex2]*0x100;
						break;
					case 6:
						highD+=pData[cwIndex1*8+cwIndex2]*0x10000;
						break;
					case 7:
						highD+=pData[cwIndex1*8+cwIndex2]*0x1000000;
						break;
				}
			  /*if(j==0)
				{
					usTempALL=pData[i*8+j];
				}
				else
				{
						usTempALL+=(uint64_t)((((uint64_t)pData[i*8+j])<<(j*8))&0xffffffffffffffff);
				}*/
			}
		
			usTempALL=(uint64_t)(lowD+highD*0x100000000);
			//MyPrintf("write:%08x-%08X",(uint32_t)((usTempALL>>32)&0xffffffff),(uint32_t)(usTempALL&0xffffffff));
			nRet = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, unStartAddr, usTempALL);					//add by lilei-20021028
			if(nRet != HAL_OK)
			{
						HAL_FLASH_Lock();		// lock
						MyPrintf("ERROR: %08X: program failed\n",unStartAddr);
						return -1;
			}
			unStartAddr += 8;
			//pData += 8;
	 }
   HAL_FLASH_Lock();		// lock

	return 0;
}
/*********************************************
* Func 	:
* Desc	:	擦除CPU内部flash(整页)
* Input	:
* Output:
* Return:
*****************************************************************/
int32_t cpuflash_erase(uint32_t unStartAddr, uint32_t unEndAddr)
{
	FLASH_EraseInitTypeDef	stEraseInit;
	uint32_t		ucPageErr = 0;
	int32_t		nRet = 0;

  uint32_t FirstPage = 0; 
	uint32_t NbOfPages = 0;
	uint32_t BankNumber = 0;
	
	
	HAL_FLASH_Unlock();		// unlock
   FirstPage = GetPage(unStartAddr);
	BankNumber = GetBank(unStartAddr);
	BankNumber=1;
	MyPrintf("firstpage:%d\r\n",FirstPage);
	MyPrintf("BankNumber:%d\r\n",BankNumber);
	
	//for(unTempAddr=ADDR_FLASH_PAGE_64; unTempAddr<=unEndAddr; unTempAddr+=FLASH_PAGE_SIZE)
	//{
		stEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	  stEraseInit.Banks       = BankNumber;
     stEraseInit.Page        = FirstPage;
		stEraseInit.NbPages=1;
		nRet = HAL_FLASHEx_Erase(&stEraseInit, &ucPageErr);
		if(nRet != HAL_OK)
		{
			HAL_FLASH_Lock();
			MyPrintf("Erase error-%08X\r\n",FLASH_PAGE_SIZE*FirstPage);
			return -1;
		}
		else
		{
			MyPrintf("Erase pagesaddress-%08X\r\n",FirstPage*FLASH_PAGE_SIZE);
		}
		//GPIO_feedDog();
	//}

    HAL_FLASH_Lock();		// lock

	return 0;
}

/****************************************************************
* Func 	:
* Desc	:	使能读保护函数
* Input	:
* Output:
* Return:
*****************************************************************/
void cpuflash_enableReadProtect(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);
  if(OBInit.RDPLevel == OB_RDP_LEVEL_0)
  {
  	MyPrintf("%s: ------------ set ----------\n", __FUNCTION__);
    OBInit.OptionType = OPTIONBYTE_RDP;
    OBInit.RDPLevel = OB_RDP_LEVEL_1;
    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();
    HAL_FLASHEx_OBProgram(&OBInit);
    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();
	//HAL_FLASH_OB_Launch();
  }
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();

}

/****************************************************************
* Func 	:
* Desc	:	失能读保护函数
* Input	:
* Output:
* Return:
*****************************************************************/
void cpuflash_disableReadProtect(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);
  if(OBInit.RDPLevel == OB_RDP_LEVEL_1)
  {
	  MyPrintf("%s: ------------ set ----------\n", __FUNCTION__);
    OBInit.OptionType = OPTIONBYTE_RDP;
    OBInit.RDPLevel = OB_RDP_LEVEL_0;
    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();
    HAL_FLASHEx_OBProgram(&OBInit);
    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();
	//HAL_FLASH_OB_Launch();
  }
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();

}

/****************************************************************
* Func 	:
* Desc	:	WriteFlag=1   update flag       writeFlag=2  update SN length=12
* Input	:
* Output:
* Return:
*****************************************************************/
int cpuflash_WriteData(uint8_t WriteFlag,uint8_t *snBuf)
{

	/* 开启读保护-在bootloader开启即可 */
	//cpuflash_enableReadProtect();
	//memset((uint8_t *)&UpdatePara,0x00,sizeof(_UpdatePara));
	
	 _UpdatePara *UpdateparaWPoint=NULL;
	 uint16_t snIndex=0;
	if(WriteFlag==1)
	{
		UpdatePara.UpdateFlag=PAGE_USEFLAG;
		MyPrintf("update flag:%08X\r\n",UpdatePara.UpdateFlag);
	}
	else if(WriteFlag==2)
	{
		 for(snIndex=0;snIndex<sizeof(UpdatePara.SN);snIndex++)
		 {
				UpdatePara.SN[snIndex]=	snBuf[snIndex];		
		 }
	}
	cpuflash_erase(UPDATE_PARA_ADDRESS,UPDATE_PARA_ADDRESS+2*1024);	
	if(cpuflash_write(UPDATE_PARA_ADDRESS, (uint8_t *)&UpdatePara, sizeof(_UpdatePara))!=0)
	{
		return 1;
	}
	
	
	
	UpdateparaWPoint=(_UpdatePara *)UPDATE_PARA_ADDRESS;
	 MyPrintf("WriteFlag:%08X\r\n",UpdateparaWPoint->UpdateFlag);
	//Read_Update_Para();
	return 0;
}



uint8_t Read_Update_Para()
{
    
	   _UpdatePara *UpdateparaPoint=NULL;
	   uint16_t snIndex=0;
	    	//cpuflash_erase(UPDATE_PARA_ADDRESS,UPDATE_PARA_ADDRESS+2*1024);	
	   UpdateparaPoint=(_UpdatePara *)UPDATE_PARA_ADDRESS;
	   memset((uint8_t *)&UpdatePara,0x00,sizeof(_UpdatePara));
	   MyPrintf("UpdateFlag:%08X\r\n",UpdateparaPoint->UpdateFlag);
	   if(UpdateparaPoint->UpdateFlag==PAGE_USEFLAG)
		 {
				
				UpdatePara.UpdateFlag=0x00;
			  for(snIndex=0;snIndex<sizeof(UpdatePara.SN);snIndex++)
			  {
					UpdatePara.SN[snIndex]=	UpdateparaPoint->SN[snIndex];		
				}
				cpuflash_erase(UPDATE_PARA_ADDRESS,UPDATE_PARA_ADDRESS+2*1024);	
				if(cpuflash_write(UPDATE_PARA_ADDRESS, (uint8_t *)&UpdatePara, sizeof(_UpdatePara))!=0)
				{
					return 1;
				}
		 
		 }
		 else
		 {
				UpdatePara.UpdateFlag=0x00;
			  for(snIndex=0;snIndex<sizeof(UpdatePara.SN);snIndex++)
			  {
					UpdatePara.SN[snIndex]=	UpdateparaPoint->SN[snIndex];		
				}
		 
		 }
	   return 0;
}



uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }
  
	
	if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  return bank;
}


uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  return page;
}
