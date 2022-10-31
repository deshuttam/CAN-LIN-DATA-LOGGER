#include <stm32f407xx.h> 
#include <stm32f4xx.h> 
#include "LIN.h"
extern df data_frame[10]; 
int time_cnt = 0,k=0; 
char protocol;
/**************************Controller Area Network configuration functions ************************/
/****************************************************************************************** 
Function Name:- can_initialize
Input Parameters:- *can_init_handle: pointer to the structure 'CAN_Init' 
Description:- It configures the Baudrate and Sampling point of CAN1 peripheral.
It configures the CAN1 Filter registers with the expected Identifiers. 
It enables and prioritizes CAN1 receive interrupt.
******************************************************************************************/
void can_initialize(CAN_Init *can_init_handle)
{ RCC->AHB1ENR |= 0X00000002; 
RCC->APB1ENR |= 1<<25; 
GPIOB->MODER |= 0X0A<<16; 
GPIOB->OSPEEDR |=0x0A<<16; 
GPIOB->AFR[1] |= 0X99<<0;
CAN_BASE->CAN_MCR.INRQ = INIT_MODE_EN; 
CAN_BASE->CAN_MCR.SLEEP=!(SLEEP_MODE_EN);
while((CAN_BASE->CAN_MSR.INAK!=1) && (CAN_BASE->CAN_MSR.SLAK!=0)); 
if((CAN_BASE->CAN_MSR.INAK==1) && (CAN_BASE->CAN_MSR.SLAK==0))
{ CAN_BASE->CAN_BTR.BRP = can_init_handle->BaudratePrescaler; 
CAN_BASE->CAN_BTR.SJW = 1;
CAN_BASE->CAN_BTR.TS1 = can_init_handle->Timesegment1; 
CAN_BASE->CAN_BTR.TS2 = can_init_handle->Timesegment2; 
CAN_BASE->CAN_MCR.DBF = 0;
CAN_BASE->CAN_MCR.NART = 1;
CAN_BASE->CAN_MCR.TTCM = 1;
}
CAN_BASE->CAN_MCR.INRQ = !(INIT_MODE_EN); 
while(CAN_BASE->CAN_MSR.INAK!=0);
FILTER_BASE->CAN_FMR.FINIT = 1;
FILTER_BASE->CAN_FMR.CAN2SB = 14; 
FILTER_BASE->CAN_FS1R.FSC = 0x000000FF; 
FILTER_BANK->CAN_F0R1.FB = 0x10<<21; 
FILTER_BANK->CAN_F0R2.FB = 0x20<<21; 
FILTER_BANK->CAN_F1R1.FB = 0x30<<21; 
FILTER_BANK->CAN_F1R2.FB = 0x40<<21; 
FILTER_BANK->CAN_F2R1.FB = 0x50<<21; 
FILTER_BANK->CAN_F2R2.FB = 0x0A<<21; 
FILTER_BANK->CAN_F3R1.FB = 0x0B<<21; 
FILTER_BANK->CAN_F3R2.FB = 0x0C<<21; 
FILTER_BANK->CAN_F4R1.FB = 0x0D<<21; 
FILTER_BANK->CAN_F4R2.FB = 0x0E<<21; 
FILTER_BANK->CAN_F5R1.FB = 0x0F<<21; 
FILTER_BANK->CAN_F5R2.FB = 0x11<<21; 
FILTER_BANK->CAN_F6R1.FB = 0x12<<21; 
FILTER_BANK->CAN_F6R2.FB = 0x13<<21; 
FILTER_BANK->CAN_F7R1.FB = 0x14<<21; 
FILTER_BASE->CAN_FM1R.FBM = 0x000000FF; 
FILTER_BASE->CAN_FFA1R.FFA = 0x00000000; 
FILTER_BASE->CAN_FA1R.FACT = 0x000000FF; 
FILTER_BASE->CAN_FMR.FINIT = 0;
CAN_BASE->CAN_IER.FMPIE0 =RX_IRQ_EN; 
NVIC_EnableIRQ(CAN1_RX0_IRQn); 
NVIC_SetPriority(CAN1_RX0_IRQn,1);
}


/****************************************************************************************** 
Function Name:- can_data_config
Input Parameters:- *can_data_handle: pointer to the structure 'CAN_Message' 
Description:- It checks for empty Transmit mailbox.
It configures the Identifier, Data length and Data in the empty CAN1 Transmit mailbox. 
It starts the transmission of data frame on the CAN bus.
It checks if transmission is successful and calls ‘UART_tx_data_from_CAN()' function to display transmitted 
message on GUI.
It updates the 'STATUS' field in Transmit window of GUI to indicate successful transmission.
******************************************************************************************/
void can_data_config(CAN_Message *can_data_handle)
{ int status,I;
if((CAN_BASE->CAN_MSR.INAK==0) && (CAN_BASE->CAN_MSR.SLAK==0))
{ can_mailbox_status(&status); 
if(status==0)
{ TRANSMIT_BASE->CAN_TI0R.STID = can_data_handle->Identifier; 
TRANSMIT_BASE->CAN_TI0R.EXID = 0;
TRANSMIT_BASE->CAN_TI0R.IDE = 0; 
TRANSMIT_BASE->CAN_TI0R.RTR = DATA_FRAME; 
TRANSMIT_BASE->CAN_TDT0R.TGT = 0;
TRANSMIT_BASE->CAN_TDT0R.DLC = can_data_handle->DataLength; 
for(i=0;i<TRANSMIT_BASE->CAN_TDT0R.DLC;i++)
{ switch(i)
{
case 0: TRANSMIT_BASE->CAN_TDL0R.DATA0 =can_data_handle->Data[i];break; 
case 1: TRANSMIT_BASE->CAN_TDL0R.DATA1 = can_data_handle->Data[i];break; 
case 2: TRANSMIT_BASE->CAN_TDL0R.DATA2 = can_data_handle->Data[i];break; 
case 3: TRANSMIT_BASE->CAN_TDL0R.DATA3 = can_data_handle->Data[i];break; 
case 4: TRANSMIT_BASE->CAN_TDH0R.DATA4 = can_data_handle->Data[i];break; 
case 5: TRANSMIT_BASE->CAN_TDH0R.DATA5 = can_data_handle->Data[i];break; 
case 6: TRANSMIT_BASE->CAN_TDH0R.DATA6 = can_data_handle->Data[i];break; 
case 7: TRANSMIT_BASE->CAN_TDH0R.DATA7 = can_data_handle->Data[i];break;
}
}
TRANSMIT_BASE->CAN_TI0R.TXRQ = TRANSMISSION_EN;
if((CAN_BASE->CAN_ESR.TEC == 0x00) && (CAN_BASE->CAN_ESR.REC == 0x00))
UART_tx_data_from_CAN(time_cnt,can_data_handle->Identifier, 
can_data_handle->DataLength,can_data_handle->Data);
for(int m=0;m<1000000;m++);
}
if(status==1)
{ TRANSMIT_BASE->CAN_TI1R.STID = can_data_handle->Identifier; 
TRANSMIT_BASE->CAN_TI1R.EXID = 0;
TRANSMIT_BASE->CAN_TI1R.IDE = 0; 
TRANSMIT_BASE->CAN_TI1R.RTR = DATA_FRAME; 
TRANSMIT_BASE->CAN_TDT1R.TGT = 0;
TRANSMIT_BASE->CAN_TDT1R.DLC = can_data_handle->DataLength; 
for(i=0;i<TRANSMIT_BASE->CAN_TDT1R.DLC;i++)
{ switch(i)
{
case 0: TRANSMIT_BASE->CAN_TDL1R.DATA0 = can_data_handle->Data[i];break; 
case 1: TRANSMIT_BASE->CAN_TDL1R.DATA1 = can_data_handle->Data[i];break; 
case 2: TRANSMIT_BASE->CAN_TDL1R.DATA2 = can_data_handle->Data[i];break; 
case 3: TRANSMIT_BASE->CAN_TDL1R.DATA3 = can_data_handle->Data[i];break; 
case 4: TRANSMIT_BASE->CAN_TDH1R.DATA4 = can_data_handle->Data[i];break; 
case 5: TRANSMIT_BASE->CAN_TDH1R.DATA5 = can_data_handle->Data[i];break; 
case 6: TRANSMIT_BASE->CAN_TDH1R.DATA6 = can_data_handle->Data[i];break; 
case 7: TRANSMIT_BASE->CAN_TDH1R.DATA7 = can_data_handle->Data[i];break;
}
}
TRANSMIT_BASE->CAN_TI1R.TXRQ = TRANSMISSION_EN;
if((CAN_BASE->CAN_ESR.TEC == 0x00) && (CAN_BASE->CAN_ESR.REC == 0x00))
UART_tx_data_from_CAN(time_cnt,can_data_handle->Identifier, 
can_data_handle->DataLength,can_data_handle->Data);
for(int m=0;m<1000000;m++);
}
else if(status==2)
{ TRANSMIT_BASE->CAN_TI2R.STID =can_data_handle->Identifier; 
TRANSMIT_BASE->CAN_TI2R.EXID = 0;
TRANSMIT_BASE->CAN_TI2R.IDE = 0; 
TRANSMIT_BASE->CAN_TI2R.RTR = DATA_FRAME; 
TRANSMIT_BASE->CAN_TDT2R.TGT = 0;
TRANSMIT_BASE->CAN_TDT2R.DLC = can_data_handle->DataLength; 
for(i=0;i<TRANSMIT_BASE->CAN_TDT2R.DLC;i++)
{ switch(i)
{
case 0: TRANSMIT_BASE->CAN_TDL2R.DATA0 = can_data_handle->Data[i];break; 
case 1: TRANSMIT_BASE->CAN_TDL2R.DATA1 = can_data_handle->Data[i];break; 
case 2: TRANSMIT_BASE->CAN_TDL2R.DATA2 = can_data_handle->Data[i];break;
case 3: TRANSMIT_BASE->CAN_TDL2R.DATA3 = can_data_handle->Data[i];break; 
case 4: TRANSMIT_BASE->CAN_TDH2R.DATA4 = can_data_handle->Data[i];break; 
case 5: TRANSMIT_BASE->CAN_TDH2R.DATA5 = can_data_handle->Data[i];break; 
case 6: TRANSMIT_BASE->CAN_TDH2R.DATA6 = can_data_handle->Data[i];break; 
case 7: TRANSMIT_BASE->CAN_TDH2R.DATA7 = can_data_handle->Data[i];break;
}
}
TRANSMIT_BASE->CAN_TI2R.TXRQ = TRANSMISSION_EN;
if((CAN_BASE->CAN_ESR.TEC == 0x00) && (CAN_BASE->CAN_ESR.REC == 0x00))
UART_tx_data_from_CAN(time_cnt,can_data_handle->Identifier, 
can_data_handle->DataLength,can_data_handle->Data);
for(int m=0;m<1000000;m++);
}
}
}


/****************************************************************************************** 
Function Name:- can_remote_config
Input Parameters:- *can_remote_handle: pointer to the structure 'CAN_Message' 
Description:- It checks for empty Transmit mailbox.
It configures the Identifier and Data length to be requested, in the empty CAN1 Transmit mailbox. 
It sends the remote frame request on the CAN bus.
******************************************************************************************/
void can_remote_config(CAN_Message *can_remote_handle)
{ int status;
if((CAN_BASE->CAN_MSR.INAK==0) && (CAN_BASE->CAN_MSR.SLAK==0))
{ can_mailbox_status(&status); 
if(status==0)
{ TRANSMIT_BASE->CAN_TI0R.STID = can_remote_handle->Identifier; 
TRANSMIT_BASE->CAN_TI0R.EXID = 0;
TRANSMIT_BASE->CAN_TI0R.IDE = 0; 
TRANSMIT_BASE->CAN_TI0R.RTR = REMOTE_FRAME; 
TRANSMIT_BASE->CAN_TDT0R.TGT = 0;
TRANSMIT_BASE->CAN_TDT0R.DLC = can_remote_handle->DataLength; 
TRANSMIT_BASE->CAN_TI0R.TXRQ = TRANSMISSION_EN;
}
else if(status==1)
{ TRANSMIT_BASE->CAN_TI1R.STID = can_remote_handle->Identifier; 
TRANSMIT_BASE->CAN_TI1R.EXID = 0;
TRANSMIT_BASE->CAN_TI1R.IDE = 0; 
TRANSMIT_BASE->CAN_TI1R.RTR = REMOTE_FRAME; 
TRANSMIT_BASE->CAN_TDT1R.TGT = 0;
TRANSMIT_BASE->CAN_TDT1R.DLC = can_remote_handle->DataLength; 
TRANSMIT_BASE->CAN_TI1R.TXRQ = TRANSMISSION_EN;
}
else if(status==2)
{ TRANSMIT_BASE->CAN_TI2R.STID=can_remote_handle->Identifier; 
TRANSMIT_BASE->CAN_TI2R.EXID = 0;
TRANSMIT_BASE->CAN_TI2R.IDE = 0; 
TRANSMIT_BASE->CAN_TI2R.RTR = REMOTE_FRAME; 
TRANSMIT_BASE->CAN_TDT2R.TGT = 0;
TRANSMIT_BASE->CAN_TDT2R.DLC=can_remote_handle->DataLength; 
TRANSMIT_BASE->CAN_TI2R.TXRQ = TRANSMISSION_EN;
}
}
}


/****************************************************************************************** 
Function Name:- can_mailbox_status
Input Parameters:- *pstatus: pointer to the variable 'status' in can_data_config() and can_remote_config() 
functions
Description:- It checks which of the three CAN1 Transmit mailbox is empty.
******************************************************************************************/ 
void can_mailbox_status(int *pstatus)
{ if((CAN_BASE->CAN_TSR.TME0)==1)
*pstatus=0;
else if((CAN_BASE->CAN_TSR.TME1)==1)
*pstatus=1;
else if((CAN_BASE->CAN_TSR.TME2)==1)
*pstatus=2;
}


/****************************************************************************************** 
Function Name:- CAN1_RX0_IRQHandler
Description:- It is the Interrupt Sub-routine called when a valid CAN message is received.
It reads the Identifier, Data length and Data of the received valid message from CAN1 Rx FIFO 
It calls 'UART_tx_data_from_CAN()' function to display received message on GUI.
******************************************************************************************/
void CAN1_RX0_IRQHandler()
{ uint16_t Identifier_r; 
uint8_t Datalength_r; 
char Data_r[9];
int TimeStamp_r;
GPIOD->ODR ^= (0xFUL << 12); 
if(CAN_BASE->CAN_RF0R.FMP >0)
{ Identifier_r = TRANSMIT_BASE->CAN_RI0R.STID; 
Datalength_r = TRANSMIT_BASE->CAN_RDT0R.DLC; 
TimeStamp_r = time_cnt;
for(int i=0;i<TRANSMIT_BASE->CAN_RDT0R.DLC;i++)
{ switch(i)
{
case 0: Data_r[0] = TRANSMIT_BASE->CAN_RDL0R.DATA0;break; 
case 1: Data_r[1] = TRANSMIT_BASE->CAN_RDL0R.DATA1;break; 
case 2: Data_r[2] = TRANSMIT_BASE->CAN_RDL0R.DATA2;break; 
case 3: Data_r[3] = TRANSMIT_BASE->CAN_RDL0R.DATA3;break; 
case 4: Data_r[4] = TRANSMIT_BASE->CAN_RDH0R.DATA4;break; 
case 5: Data_r[5] = TRANSMIT_BASE->CAN_RDH0R.DATA5;break; 
case 6: Data_r[6] = TRANSMIT_BASE->CAN_RDH0R.DATA6;break; 
case 7: Data_r[7] = TRANSMIT_BASE->CAN_RDH0R.DATA7;break;
}
}
CAN_BASE->CAN_RF0R.RFOM = RX_FIFO_CLR;
UART_tx_data_from_CAN(TimeStamp_r,Identifier_r,Datalength_r,Data_r);
}
/******************************TIMER configuration functions **********************************/
/****************************************************************************************** 
Function Name:- timer_initialize
Input Parameters:- *timer_init_handle
Description:- It configures the Clock frequency, Counter direction, Prescaler and Period of TIMER2 peripheral.
It enables and prioritizes TIMER2 interrupt.
******************************************************************************************/
void timer_initialize(TIM_Init *timer_init_handle)
{ RCC->APB1ENR |= 0x00000001; 
RCC->AHB1ENR |= 0x00000008; 
GPIOD->MODER |=(0x55<<24);
TIM_BASE->TIM_CR1.CKD = CLK_DIV1; 
TIM_BASE->TIM_CR1.DIR = CNT_UP; 
TIM_BASE->TIM_SMCR.SMS = CLK_INT;
TIM_BASE->TIM_PSC.PSC = timer_init_handle->Prescaler; 
TIM_BASE->TIM_ARR.ARR = timer_init_handle->Period; 
TIM_BASE->TIM_DIER.UIE = TIM2_IRQ_EN;
NVIC_EnableIRQ(TIM2_IRQn); 
NVIC_SetPriority(TIM2_IRQn, 0);
}
/**************************************************************** 
Function Name:- timer_enable
Input Parameters:- prr: indicates the protocol being used. 
Description:- It enables the TIMER2 counter.
****************************************************************/
void timer_enable(char prr)
{ protocol = prr;
TIM_BASE->TIM_CR1.CEN = CNT_EN;
}


/*************************************************************************** 
Function Name:- TIM2_IRQHandler
Description:- It is the Interrupt Sub-routine called when TIMER2-UIF bit is set.
It clears the TIMER2 interrupt request.
It increments a variable 'time_cnt' which keeps track of time elapsed.
It implements a algorithm to transmit the CAn messages periodically as per the time specified 
in GUI.
***************************************************************************/
void TIM2_IRQHandler()
{ int i,j=0; 
time_cnt++;
NVIC_ClearPendingIRQ(TIM2_IRQn); 
if (TIM_BASE->TIM_SR.UIF == 1)
{ //GPIOD->ODR ^= (0xFUL << 12);
if(protocol == 'C')
{for(i=0;i<max_msg;i++)
{if(time_cnt % data_frame[i].d_time==0)
{ k++;
can_transmit_data(data_frame[i].d_id,data_frame[i].d_dlc,data_frame[i].d_data);
if(time_cnt == 0 || k>=4)
for(j=0;j<1000000;j++);
}
}
for(i=0;i<r_max_msg;i++)
{if(time_cnt % remote_frame[i].r_time==0)
{ k++;
can_transmit_remote(remote_frame[i].r_id,remote_frame[i].r_len); 
if(time_cnt == 0 || k>=4)
for(j=0;j<1000000;j++);
}
}
}
else if(protocol == 'L')
{for(i=0;i<max_msg;i++)
{if(time_cnt % data_frame[i].d_time==0)
{ k++;
lin_master_transmit_header(data_frame[i].d_id,data_frame[i].d_data);
}
}
for(i=0;i<r_max_msg;i++)
{if(time_cnt % remote_frame[i].r_time==0)
{ k++;
lin_master_request_data(remote_frame[i].r_id);
}
}
}
TIM_BASE->TIM_SR.UIF = 0;
}
}


/***************** Universal Asynchronous Receiver Transmitter configuration functions****************/
/****************************************************************************************** 
Function Name:- USART_initialize
Input Parameters:- *USART_init_: pointer to the structure 'USART_initial_config'. 
Description:- It enables the USART2 peripheral.
It configures the Baudrate of USART2 peripheral. 
It enables and prioritizes USART2 interrupt.
******************************************************************************************/
void USART_initialize(USART_initial_config *USART_init_)
{ RCC->APB1ENR|=1<<17; 
RCC->AHB1ENR|=1<<0;
GPIOA->MODER |= 0X000000A0; 
GPIOA->AFR[0] |= 0x77<<8;
USART_BASE->USART_CR1.UE= USART_init_->ue;
USART_BASE->USART_BRR.DIV_FRACTION= USART_init_->div_fraction; 
USART_BASE->USART_BRR.DIV_MANTISSA= USART_init_->div_mantissa; 
USART_BASE->USART_CR1.RXNEIE = USART_init_->rxneie; 
NVIC_EnableIRQ(USART2_IRQn);
NVIC_SetPriority(USART2_IRQn,2);
}


/****************************************************************************************** 
Function Name:- USART_transmit
Input Parameters:- *USART_transceiver_: pointer to the structure 'USART_transceiver_config'. 
Description:- It checks if Transmit data register is empty.
It configures the data to be transmitted in USART2 Data register. 
It checks if transmission is completed.
******************************************************************************************/
void USART_transmit(USART_transciever_config *USART_transciever_)
{ while(USART_BASE->USART_SR.TXE!=1);
USART_BASE->USART_DR.DR = USART_transciever_->dr; 
while(USART_BASE ->USART_SR.TC!=1);
}


/****************************************************************************************** 
Function Name:- USART_receive
Input Parameters:- *USART_transceiver_: pointer to the structure 'USART_transceiver_config'. 
Output Parameters:- d: character read from USART_DR register.
Description:- It checks if Read data register is not empty.
It reads USART_DR register and return received data to 'receive_from_GUI()' function.
******************************************************************************************/
char USART_recieve(USART_transciever_config *USART_transciever_)
{ static char d;
while(USART_BASE->USART_SR.RXNE !=1);
USART_transciever_->dr= USART_BASE->USART_DR.DR; 
d =USART_transciever_->dr;
return d;
}
/****************************************************************************************** 
Function Name:- USART_receive_enable
Input Parameters:- *USART_transceiver_: pointer to the structure 'USART_transceiver_config'. 
Description:- It enables the USART2 receiver.
******************************************************************************************/
void USART_recieve_enable(USART_transciever_config *USART_transciever_)
{ USART_BASE->USART_CR1.RE = USART_transciever_->re;
}


/****************************************************************************************** 
Function Name:- USART_transmit_enable
Input Parameters:- *USART_transceiver_: pointer to the structure 'USART_transceiver_config'. 
Description:- It enables the USART2 transmitter.
******************************************************************************************/
void USART_transmit_enable(USART_transciever_config *USART_transciever_)
{ USART_BASE->USART_CR1.TE = USART_transciever_->te;
}


/****************************************************************************************** 
Function Name:- USART2_IRQHandler
Description:- It is the Interrupt Sub-routine called when data is received.
It clears the USART2 interrupt.
It calls the 'UART_rx_data_to_CAN()' function to sends the data to CAN1
******************************************************************************************/
void USART2_IRQHandler()
{ NVIC_ClearPendingIRQ(USART2_IRQn);
if(USART_BASE->USART_SR.RXNE !=0)
UART_rx_data_to_CAN();
}
/********************* Local Interconnect Network configuration functions **************************/
/****************************************************************************************** 
Function Name:- lin_init_
Input Parameters:- *lin_initialize: pointer to structure 'lin_initial_config'.
Description:- It enables the LIN on USART3 peripheral.
It configures the Baudrate of LIN on USART3 peripheral. 
It enables USART3 transmitter and receiver.
It enables and prioritizes USART3 interrupt.
******************************************************************************************/
void lin_init_(lin_initial_config *lin_initialize)
{ RCC->APB1ENR |=1<<18; 
RCC->AHB1ENR |=1<<1;
GPIOB->MODER |=0x00A00000; 
GPIOB->AFR[1] |= 0x77<<8; 
GPIOD->MODER |= (0x55<<24);
LIN_BASE->USART_CR1.UE = lin_initialize->ue;
LIN_BASE->USART_BRR.DIV_FRACTION = lin_initialize->div_fraction; 
LIN_BASE->USART_BRR.DIV_MANTISSA = lin_initialize->div_mantissa; 
LIN_BASE->USART_CR1.TE = lin_initialize->te;
LIN_BASE->USART_CR1.RE = lin_initialize->re; 
LIN_BASE->USART_CR2.LINEN =lin_initialize->linen; 
LIN_BASE->USART_CR1.RXNEIE = 1; 
NVIC_EnableIRQ(USART3_IRQn);
NVIC_SetPriority(USART3_IRQn,0);
}


/****************************************************************************************** 
Function Name:- transmit_break
Input Parameters:- *lin_transceiver: pointer to the structure 'lin_transceiver_config'. 
Description:- It enables the SBK bit to send break character.
******************************************************************************************/
void transmit_break(lin_transciever_config *lin_transceiver)
{ LIN_BASE->USART_CR1.SBK = lin_transceiver->sbk;
}
/****************************************************************************************** 
Function Name:- transmit_sync
Input Parameters:- *lin_transceiver: pointer to the structure 'lin_transceiver_config'. 
Description:- It writes the sync field(0x55) in USART3 Data register.
******************************************************************************************/
void transmit_sync(lin_transciever_config *lin_transceiver)
{ LIN_BASE->USART_DR.DR = lin_transceiver->dr;
}
/****************************************************************************************** 
Function Name:- transmit_id
Input Parameters:- *lin_transceiver: pointer to the structure 'lin_transceiver_config'. 
Description:- It writes id in USART3 Data register.
******************************************************************************************/
void transmit_id(lin_transciever_config *lin_transceiver)
{ LIN_BASE->USART_DR.DR = lin_transceiver->dr;
}
/****************************************************************************************** 
Function Name:- transmit_data
Input Parameters:- *lin_transceiver: pointer to the structure 'lin_transceiver_config'. 
Description:- It writes data in USART3 Data register.
******************************************************************************************/
void transmit_data(lin_transciever_config *lin_transceiver)
{ LIN_BASE->USART_DR.DR = lin_transceiver->dr;
}

/****************************************************************************************** 
Function Name:- receive_response_
Input Parameters:- *lin_transceiver: pointer to the structure 'lin_transceiver_config'. 
Output Parameters:- data: data byte read from USART3 Data register.
Description:- It reads the data byte from USART3 Data register.
******************************************************************************************/
char receive_response_(lin_transciever_config *lin_transceiver)
{ char data;
lin_transceiver->dr = LIN_BASE->USART_DR.DR; 
data = lin_transceiver->dr;
return data;
}

/****************************************************************************************** 
Function Name:- USART3_IRQHandler
Description:- It is the Interrupt Sub-routine called when data is received.
It clears the USART3 interrupt.
It calls the 'lin_receive_response()' function to read the received data.
******************************************************************************************/

void USART3_IRQHandler()
{ NVIC_ClearPendingIRQ(USART3_IRQn); 
GPIOD->ODR^=(0xFUL<<12); 
if(LIN_BASE->USART_SR.RXNE !=0)
{ lin_receive_response();
}
}
