
/********************************************************************************
* Include Headers
********************************************************************************/

#include "usb_data.h"

#include "usbd_cdc_if.h"
#include "driver_ucan.h"
#include "tool.h"
#include "inter_flash.h"
#include "usb_device.h"
#include "stm32g4xx_hal_fdcan.h"



//#include "main.h"



_UsbData UsbData;
_ProDeal ProDeal;


uint8_t TTDataBuf[512];

//unsigned int TotalULen=0;




void Usb_Data_Init()
{
	UsbData.head=0;
	UsbData.tail=0;
	memset(UsbData.buf,0x000,sizeof(UsbData.buf));
	ProDeal.cmd=0;
	ProDeal.index=0;
	ProDeal.len=0;
  memset(ProDeal.buf,0x000,sizeof(ProDeal.buf));
	

}

void Usb_Buf_In(uint8_t * buf,uint32_t length)
{
	uint32_t i=0;
	//TotalULen+=length;
	for(i=0;i<length;i++)
	{
		UsbData.buf[UsbData.head++]=buf[i];
		if(UsbData.head>=USB_BUF_SIZE)
		{
				UsbData.head=0;
		}
	}
}

void USB_Data_Out()
{
	
	uint8_t TTData=0;
	static uint8_t ProStep=0;
	static uint8_t CheckSum=0;
	uint16_t getIndex=0;
   
	
	/*while(1)
	{
		if(UsbData.head==UsbData.tail)
		{
			//getIndex=0;
			 break;
		}
	
		TTData = UsbData.buf[UsbData.tail++];
	  if(UsbData.tail >= USB_BUF_SIZE)
	  {
			 UsbData.tail = 0;
	  }
	 
	  TTDataBuf[getIndex++]=TTData;
	}
	if(getIndex>0)
	{
		CDC_Transmit_FS(TTDataBuf,getIndex);
	}
	return;*/
	
	while(1)
	{
	if(UsbData.head==UsbData.tail)
	{
		return;
	}
	
	 TTData = UsbData.buf[UsbData.tail++];

	 if(UsbData.tail >= USB_BUF_SIZE)
	 {
			UsbData.tail = 0;
	 }
	 //CDC_Transmit_FS((uint8_t *)&TTData,1);
	  //MyPrintf("%02X",TTData);
	 //return;
	 switch(ProStep)
	 {
		 case 0:
					if(TTData == 0x23)
					{
						ProStep++;
					}
					break;
		 	case 1:
					ProDeal.cmd=TTData;
			    ProStep++;
					break;
			case 2:
				  ProStep++;
				  ProDeal.cmd=(ProDeal.cmd*0x100+TTData);
					break;
			case 3:
				  ProDeal.len=TTData;
			    ProStep++;
					break;
			case 4:
					ProDeal.len=(ProDeal.len*0x100+TTData);
			    ProDeal.index=0;
					ProStep++;
					CheckSum=0;
					break;
			case 5:
				  CheckSum+=TTData;
				  ProDeal.buf[ProDeal.index++]=TTData;
			    if(ProDeal.index>=ProDeal.len)
					{
							ProStep++;
					}
				  break;
			case 6:
				   if(TTData==CheckSum)
					 {
							ProStep++;
					 }
					 else
					 {
							MyPrintf("check sum error\r\n");
						 	ProStep=0;
					 }
					 break;
			case 7:
				  if(TTData == 0x24)
					{
						//MyPrintf("usb Total len:%d\r\n",TotalULen);
						Pro_Parse(ProDeal);
					}
					ProStep=0;
				  break;
			default:
				  ProStep=0;
				  break;		
	 
	 }
 }

}




void Pro_Parse(_ProDeal TProData)
{
	
	  uint16_t index=0;
	  uint16_t TLength=0;
	  uint8_t TTbuf[256];
	  uint16_t TIndex=0;
	  uint16_t ResponeId=0;
	  uint8_t idCnt = 0;
	  uint8_t idData = 0;
	  _CANSetPara  canTempSetPara;
	
	
		switch(TProData.cmd)
		{
			
			
			MyPrintf("recive cmd:%04X\r\n",TProData.cmd);
				case 0x0100:
					   MyPrintf("0100 cmd deal\r\n");
					   ResponeId=TProData.buf[index++]*0x100;
						 ResponeId+=TProData.buf[index++];
				     switch(ResponeId)
						 {
							 case 0x0800:
										break;
							 case 0x0801:
								    if(TProData.buf[index]==0)
										{
										}
										else
										{
										}
										break;
							 case 0x0802:
								    if(TProData.buf[index]==0)
										{
										}
										else
										{
										}										
										break;
							 default:
										break;								 
						 }
					   MyPrintf("Recive Answer\r\n");
						 break;
		    case 0x0101:
					  MyPrintf("0101 cmd deal\r\n");
  					Can2Para.BandRate=TProData.buf[index++]*0x100;
						 Can2Para.BandRate+=TProData.buf[index++];
						 MyPrintf("CanPara.BandRate:%04X\r\n",Can2Para.BandRate);
						 Can2Para.CANMode=TProData.buf[index++];
						 //MyPrintf("Can1Para.CANMode:%d\r\n",Can1Para.CANMode);
						 Can2Para.MaskCode=TProData.buf[index++]*0x1000000;
						 Can2Para.MaskCode+=TProData.buf[index++]*0x10000;
						 Can2Para.MaskCode+=TProData.buf[index++]*0x100;
						 Can2Para.MaskCode+=TProData.buf[index++];
				      MyPrintf("CanPara.MaskCode:%08X\r\n",Can2Para.MaskCode);
				     Can2Para.prescaler=TProData.buf[index++]*0x100;
						 Can2Para.prescaler+=TProData.buf[index++];
				     Can2Para.SJW=TProData.buf[index++];
				     Can2Para.bs1=TProData.buf[index++];
				     Can2Para.bs2=TProData.buf[index++];
				     MyPrintf("Can1Para.prescaler:%d,SJW:%d,bs1:%d,bs2:%d\r\n",Can2Para.prescaler,Can2Para.SJW,Can2Para.bs1,Can2Para.bs2);
						 if(Can2Para.CANFirstFlag!=0)
						 {
							  CAN2_POWER_OFF;
								HAL_FDCAN_DeInit(&hfdcan2);
						 }
						 HAL_Delay(10);
						 Can2Para.CANFirstFlag=1;
						// MyPrintf("here-1\r\n");
						 TTbuf[0]=MX_CAN2_Init(Can2Para);
						 // MyPrintf("here-2\r\n");
						 //if()
						 //MyPrintf("can1 init result:%02X\r\n",TTbuf[0]);
						 Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
						 CAN2_POWER_ON;
						 
						 MyPrintf("VER:%s,SN:%s\r\n",VER,UpdatePara.SN);
						 //Usb_Pro_Pack(0x0801,TTbuf,1);
					   break;
				 case 0x0102: 
					   MyPrintf("0102 cmd deal\r\n");
						 Can3Para.BandRate=TProData.buf[index++]*0x100;
						 Can3Para.BandRate+=TProData.buf[index++];
						 //MyPrintf("Can2Para.BandRate:%d\r\n",Can2Para.BandRate);
						 Can3Para.CANMode=TProData.buf[index++];
						 //MyPrintf("Can2Para.CANMode:%d\r\n",Can2Para.CANMode);
						 Can3Para.MaskCode=TProData.buf[index++]*0x1000000;
						 Can3Para.MaskCode+=TProData.buf[index++]*0x10000;
						 Can3Para.MaskCode+=TProData.buf[index++]*0x100;
						 Can3Para.MaskCode+=TProData.buf[index++];
				 
				 
				     Can3Para.prescaler=TProData.buf[index++]*0x100;
						 Can3Para.prescaler+=TProData.buf[index++];
				     Can3Para.SJW=TProData.buf[index++];
				     Can3Para.bs1=TProData.buf[index++];
				     Can3Para.bs2=TProData.buf[index++];
				     MyPrintf("Can2Para.prescaler:%d,SJW:%d,bs1:%d,bs2:%d\r\n",Can2Para.prescaler,Can2Para.SJW,Can2Para.bs1,Can2Para.bs2);
						 //MyPrintf("Can2Para.MaskCode:%08X\r\n",Can2Para.MaskCode);
						 if(Can3Para.CANFirstFlag!=0)
						 {
							  CAN3_POWER_OFF;
								HAL_FDCAN_DeInit(&hfdcan3);
						 }
						 HAL_Delay(10);
						 Can3Para.CANFirstFlag=1;
						 TTbuf[0]=MX_CAN3_Init(Can3Para);
						 CAN3_POWER_ON;
						 Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
						 //Usb_Pro_Pack(0x0802,TTbuf,1);
					   break;
				 case 0x0103:
				 	
				 	idCnt = TProData.buf[index++];
					if(idCnt > 0)
					{
						for(TIndex=0;TIndex<idCnt;TIndex++)
						{
							idData = TProData.buf[index++];
							switch(idData)
							{
								case 0:
									canTempSetPara.CANChanel = TProData.buf[index++];
									break;
								case 1:
									canTempSetPara.CANFunMode = TProData.buf[index++];
									break;
								case 2:
									canTempSetPara.bandRate = TProData.buf[index] + TProData.buf[index+1]*0x100 + TProData.buf[index+2]*0x10000 + TProData.buf[index+3]*0x1000000;
									index += 4;
									break;
								case 3:
									canTempSetPara.procotolType = TProData.buf[index++];
									break
								case 4:
									canTempSetPara.PhyInterface = TProData.buf[index++];
									break;
								default:
									break;
							}
						}
						if(canTempSetPara.CANChanel == 0)
						{
							Can2SetPataInfo = canTempSetPara;
							//if(Can3Para.CANFirstFlag!=0)
						 	//{
							// 	CAN3_POWER_OFF;
							//	HAL_FDCAN_DeInit(&hfdcan3);
						 	//}
						 	HAL_Delay(10);
						 	//Can3Para.CANFirstFlag=1;
						 	TTbuf[0]=MX_CAN2_Init(Can2SetPataInfo);
						 	CAN2_POWER_ON;
						 	Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
						}
						else if(canTempSetPara.CANChanel == 1)
						{
							Can3SetPataInfo = canTempSetPara;
							//if(Can3Para.CANFirstFlag!=0)
						 	//{
							// 	CAN3_POWER_OFF;
							//	HAL_FDCAN_DeInit(&hfdcan3);
						 	//}
						 	HAL_Delay(10);
						 	//Can3Para.CANFirstFlag=1;
						 	TTbuf[0]=MX_CAN3_Init(Can3SetPataInfo);
						 	CAN3_POWER_ON;
						 	Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
						}
					}
				 	break;
				 case 0x0105:
					      MyPrintf("0105 cmd deal\r\n");
					   
							if(TProData.buf[index]==0)
							{
									/*TxHeader2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
									TxHeader2.BitRateSwitch = FDCAN_BRS_OFF;
									TxHeader2.FDFormat = FDCAN_CLASSIC_CAN;
									TxHeader2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
									TxHeader2.MessageMarker = 0;
									TxHeader2.DataLength=FDCAN_DLC_BYTES_8;
								
									index++;
								  if(TProData.buf[index]==0)
									{
										 TxHeader2.IdType=FDCAN_STANDARD_ID;
									}
									else
									{
											TxHeader2.IdType=FDCAN_EXTENDED_ID;
									}
									index++;
									 if(TProData.buf[index]==0)
									{
                      TxHeader2.TxFrameType=FDCAN_DATA_FRAME;
									}
									else
									{
											TxHeader2.TxFrameType=FDCAN_REMOTE_FRAME;
									}
									index++;
									TxHeader2.Identifier=TProData.buf[index++]*0x1000000;
									TxHeader2.Identifier+=TProData.buf[index++]*0x10000;
									TxHeader2.Identifier+=TProData.buf[index++]*0x100;
									TxHeader2.Identifier+=TProData.buf[index++];
									
									TLength=TProData.buf[index++]*0X100;
									TLength+=TProData.buf[index++];
								  for(TIndex=0;TIndex<TLength;TIndex++)
									{
											TTbuf[TIndex]=TProData.buf[index++];
									}
								
									if(USER_CAN2_Send(TxHeader2,TTbuf)==0)
									{
											//MyPrintf("Recive can data and send data success\r\n");
									}
									else
									{
											MyPrintf("Can1 data send fail\r\n");
									}
									
									break;
									*/
	/***								
    TxHeader2.Identifier = 0x666;
	 TxHeader2.IdType = FDCAN_STANDARD_ID;
  TxHeader2.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader2.DataLength = FDCAN_DLC_BYTES_16;
  TxHeader2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader2.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader2.FDFormat = FDCAN_FD_CAN;
  TxHeader2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader2.MessageMarker = 0;
   ***/
   TxHeader2.Identifier = 0x321;
  TxHeader2.IdType = FDCAN_STANDARD_ID;
  TxHeader2.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader2.DataLength = FDCAN_DLC_BYTES_16;
  TxHeader2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader2.BitRateSwitch = FDCAN_BRS_ON;
  TxHeader2.FDFormat = FDCAN_FD_CAN;
  TxHeader2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader2.MessageMarker = 0;
	  MyPrintf("FDCAN send BRS_ON\r\n");
									//TxHeader2.ErrorStateIndicator=
								
									//index++;
								 /* if(TProData.buf[index]==0)
									{
										 TxHeader2.IdType=FDCAN_STANDARD_ID;
										MyPrintf("FDCAN_STANDARD_ID\r\n");
									}
									else
									{
											TxHeader2.IdType=FDCAN_EXTENDED_ID;
											MyPrintf("FDCAN_EXTENDED_ID\r\n");
									}*/
									//index++;
									/*
									 if(TProData.buf[index]==0)
									{
                      TxHeader2.TxFrameType=FDCAN_DATA_FRAME;
										MyPrintf("FDCAN_DATA_FRAME\r\n");
									}
									else
									{
											TxHeader2.TxFrameType=FDCAN_REMOTE_FRAME;
										MyPrintf("FDCAN_REMOTE_FRAME\r\n");
									}*/
									//index++;
									/*TxHeader2.Identifier=TProData.buf[index++]*0x1000000;
									TxHeader2.Identifier+=TProData.buf[index++]*0x10000;
									TxHeader2.Identifier+=TProData.buf[index++]*0x100;
									TxHeader2.Identifier+=TProData.buf[index++];*/
									MyPrintf("FDCAN_ID:%08X\r\n",TxHeader2.Identifier);
									
									TLength=TProData.buf[index++]*0X100;
									TLength+=TProData.buf[index++];
								  for(TIndex=0;TIndex<16;TIndex++)
									{
											TTbuf[TIndex]=TIndex;
									}
								   /*TxHeader2.Identifier = 0x111;
										TxHeader2.IdType = FDCAN_STANDARD_ID;
										TxHeader2.TxFrameType = FDCAN_DATA_FRAME;
										TxHeader2.DataLength = FDCAN_DLC_BYTES_8;
											TxHeader2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
												TxHeader2.BitRateSwitch = FDCAN_BRS_ON;
													TxHeader2.FDFormat = FDCAN_FD_CAN;
														TxHeader2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
											TxHeader2.MessageMarker = 0;*/
									MyPrintf("TTBuf[0]:%02X,Tbuf2:%02X\r\n",TTbuf[0],TTbuf[1]);
									if(USER_CAN2_Send(TxHeader2,TTbuf)==0)
									{
											//MyPrintf("Recive can data and send data success\r\n");
									}
									else
									{
											MyPrintf("Can1 data send fail\r\n");
									}
							}
							else
							{
								  TxHeader3.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
									TxHeader3.BitRateSwitch = FDCAN_BRS_OFF;
									TxHeader3.FDFormat = FDCAN_CLASSIC_CAN;
									TxHeader3.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
									TxHeader3.MessageMarker = 0;
									TxHeader3.DataLength=FDCAN_DLC_BYTES_8;
								  	  MyPrintf("CLASSICCAN send BRS_off\r\n");
								
									index++;
								  if(TProData.buf[index]==0)
									{
										 TxHeader3.IdType=FDCAN_STANDARD_ID;
									}
									else
									{
											TxHeader3.IdType=FDCAN_EXTENDED_ID;
									}
									index++;
									 if(TProData.buf[index]==0)
									{
                      TxHeader3.TxFrameType=FDCAN_DATA_FRAME;
									}
									else
									{
											TxHeader3.TxFrameType=FDCAN_REMOTE_FRAME;
									}
									index++;
									TxHeader3.Identifier=TProData.buf[index++]*0x1000000;
									TxHeader3.Identifier+=TProData.buf[index++]*0x10000;
									TxHeader3.Identifier+=TProData.buf[index++]*0x100;
									TxHeader3.Identifier+=TProData.buf[index++];
									
									TLength=TProData.buf[index++]*0X100;
									TLength+=TProData.buf[index++];
								  for(TIndex=0;TIndex<TLength;TIndex++)
									{
											TTbuf[TIndex]=TProData.buf[index++];
									}
									//TxHeader3.DataLength=FDCAN_DLC_BYTES_8;
									if(USER_CAN3_Send(TxHeader3,TTbuf)==0)
									{
											//MyPrintf("Recive can dataTxHeader3 and send data success\r\n");
									}
									else
									{
											MyPrintf("Can2 data send fail\r\n");
									}
							};
							break;
				 case 0x0108:
					 MyPrintf("0108 cmd deal\r\n");
							if(!cpuflash_WriteData(1,(uint8_t *)&(TProData.buf[index])))
							{
								  TTbuf[0]=0;
									Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
								  MyPrintf("device delay 60s and reset\r\n");
								  HAL_Delay(60);
								  NVIC_SystemReset();
								
							}
							else
							{
								 TTbuf[0]=1;
								 Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
							}
							break;
				 case 0x0109:
					 MyPrintf("0109 cmd deal\r\n");
					   if(Can2Para.CANFirstFlag!=0)
						 {
							  CAN2_POWER_OFF;
							  Can2Para.CANFirstFlag=0;
								HAL_FDCAN_DeInit(&hfdcan2);
						 }
						 if(Can3Para.CANFirstFlag!=0)
						 {
								CAN3_POWER_OFF;
							  Can3Para.CANFirstFlag=0;
								HAL_FDCAN_DeInit(&hfdcan3);
						 }
						 Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
						 break;
				 case 0x010a:
					 MyPrintf("010a cmd deal\r\n");
					    if(!cpuflash_WriteData(2,(uint8_t *)&(TProData.buf[index])))
							{
								  TTbuf[0]=0;
									Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
							}
							else
							{
								 TTbuf[0]=1;
								 Usb_Pro_PackT(0x0800 ,TProData.cmd,TTbuf,1);
							}
							break;
				default:
						 break;
		}

}

void Usb_Pro_Pack(uint16_t cmd ,uint8_t *buf,uint16_t length)
{

    //Usb_Send_Data(buf,length);
    //return;	
		uint16_t ProIndex=0;
    uint16_t PackIndex=0;
	  uint8_t CheckSum=0;
	
	  TTDataBuf[ProIndex++]=0x23;
	  TTDataBuf[ProIndex++]=(uint8_t)((cmd>>8)&0xff);
	  TTDataBuf[ProIndex++]=(uint8_t)(cmd&0xff);
		TTDataBuf[ProIndex++]=(uint8_t)((length>>8)&0xff);
		TTDataBuf[ProIndex++]=(uint8_t)(length&0xff);
	  
	  for(PackIndex=0;PackIndex<length;PackIndex++)
	  {
			TTDataBuf[ProIndex]=buf[PackIndex];
			CheckSum+=TTDataBuf[ProIndex];
			ProIndex++;
		}
		TTDataBuf[ProIndex++]=CheckSum;
	  TTDataBuf[ProIndex++]=0x24;
		Usb_Send_Data(TTDataBuf,ProIndex);
	 
}

uint8_t Usb_Send_Data(uint8_t *buf,uint16_t length)
{
	uint16_t SendIndex=0;
	uint16_t LessData;
	uint32_t GetDealyTIme=0;
	
	uint8_t SendResult=0;
	
	
	  GetSysTick((uint32_t *)&GetDealyTIme);
		if(length>USB_SEND_SIZE)
		{
				for(SendIndex=0;SendIndex<length;SendIndex+=USB_SEND_SIZE)
			  {
				
						 /*USBD_LL_Transmit(&hUsbDeviceFS,
                             CDC_IN_EP,
                             (uint8_t *)&buf[SendIndex],
                             64);*/
					while(CDC_Transmit_FS(buf,USB_SEND_SIZE))
					{
						 if(CheckSysTick((uint32_t *)&GetDealyTIme,3))
						 {
							 	//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15, GPIO_PIN_SET);
							 USB_POWER_OFF;
							  SendResult=1;
								break;
						 }
					}
				}
				if(length%USB_SEND_SIZE!=0)
				{
					LessData=length%USB_SEND_SIZE;
					while(CDC_Transmit_FS((uint8_t *)&buf[length-LessData],LessData))
					{
						 if(CheckSysTick((uint32_t *)&GetDealyTIme,3))
						 {
							  USB_POWER_OFF;
							  //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15, GPIO_PIN_SET);
							  SendResult=1;
								break;
						 }
					}
				}
		}
		else
		{
				while(CDC_Transmit_FS(buf,length))
				{
					 if(CheckSysTick((uint32_t *)&GetDealyTIme,3))
					 {
						  USB_POWER_OFF;
						  //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15, GPIO_PIN_SET);
						  SendResult=1;
							break;
					 }
				}
		}
		if(SendResult==0)
		{
			USB_POWER_ON;
			//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15, GPIO_PIN_RESET);
		}
    return SendResult;
}



void Usb_Pro_PackT(uint16_t cmd ,uint16_t ResponeCmd,uint8_t *buf,uint16_t length)
{

    //Usb_Send_Data(buf,length);
    //return;	
		uint16_t ProIndex=0;
	  uint16_t TPackLength=0;
		uint8_t CheckSum=0;
	  uint16_t PackIndex=0;
	
	
	
	  TPackLength=length;
	  TPackLength+=2;
  
	  TTDataBuf[ProIndex++]=0x23;
	  TTDataBuf[ProIndex++]=(uint8_t)((cmd>>8)&0xff);
	  TTDataBuf[ProIndex++]=(uint8_t)(cmd&0xff);
		TTDataBuf[ProIndex++]=(uint8_t)((TPackLength>>8)&0xff);
		TTDataBuf[ProIndex++]=(uint8_t)(TPackLength&0xff);
	
	
	
	
	  TTDataBuf[ProIndex]=(uint8_t)((ResponeCmd>>8)&0xff);
		CheckSum+= TTDataBuf[ProIndex];
		ProIndex++;
	  TTDataBuf[ProIndex]=(uint8_t)(ResponeCmd&0xff);
	  CheckSum+= TTDataBuf[ProIndex];
		ProIndex++;
		
	  for(PackIndex=0;PackIndex<length;PackIndex++)
	  {
			TTDataBuf[ProIndex]=buf[PackIndex];
			CheckSum+=	TTDataBuf[ProIndex];
			ProIndex++;
		
		}
		TTDataBuf[ProIndex++]=CheckSum;
	  TTDataBuf[ProIndex++]=0x24;
		Usb_Send_Data(TTDataBuf,ProIndex);
	 
}