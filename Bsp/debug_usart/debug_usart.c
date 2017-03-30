
  
#include "debug_usart.h"


static uint8_t cmd_uart_rx_buffer[CMD_UART_RX_BUFFER_SIZE];
static const uint32_t DATA_COUNTER_MAX = CMD_UART_RX_BUFFER_SIZE - 1;
static uint32_t buffer_counter = 0;
static uint32_t cmd_byte_counter = 0;





///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		HAL_UART_Transmit(&DEBUG_USART, (uint8_t *) &ch, 1, 0xFFFF);
		return (ch);
}


void UARTInterruptHandler( USART_TypeDef *USARTx )
{
  if(LL_USART_IsActiveFlag_RXNE(USARTx) && LL_USART_IsEnabledIT_RXNE(USARTx))
  {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
    uint8_t data = LL_USART_ReceiveData8(USARTx);	
    if(TERMINATOR == data)
    {
        //insert
        cmd_data_t cmd_data;
        cmd_data.data = &cmd_uart_rx_buffer[buffer_counter - cmd_byte_counter];
        cmd_data.len = cmd_byte_counter;
        cmd_data.isProcessed = false;
        insert(cmd_data);
    }
    else
    {
        cmd_byte_counter++;
        
        if(buffer_counter ==  DATA_COUNTER_MAX)
        {
            buffer_counter = 0; 
            //how to check if the data of cmd_uart_rx_buffer[0] has been processed, NOT another circlar array queue, need a special indicator
            
        }
        else
        {
            cmd_uart_rx_buffer[buffer_counter++] = data;
        }
    }
  } 
}



/*********************************************END OF FILE**********************/
