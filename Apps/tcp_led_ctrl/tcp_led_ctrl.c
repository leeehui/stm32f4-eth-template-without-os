

#include "tcp_led_ctrl.h"


uint8_t led_buffer[CH_NUM][CH_LED_NUM * 3];
static uint16_t led_num_to_send = 0;


static void fill_led_buffer(uint8_t *data_start);
static void trigger_led();
static bool config_ip(uint8_t *data_start);



int32_t process_one_frame(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es, uint8_t *frame_data, uint16_t frame_len)
{
    RECV_FRAME_PTR_t frame_ptr = (RECV_FRAME_PTR_t)frame_data;
    uint8_t *data = (uint8_t *)(&(frame_ptr->data_len));
    uint8_t xor_value = *data++;//first XOR
    uint32_t xor_counter = frame_ptr->data_len;//xor = XOR(len(2) + RGB data), frame_ptr->data_len = (RGB data + xorValue(1))
    if(frame_ptr->start != FRAME_START)
    {
        debug(info, "frame_ptr->start wrong!");
        return 1;
    }
    
    //frame_len = frame_ptr->len + start(1) + end(1) + datalen(2)
    if(frame_ptr->data_len != frame_len - 4) 
    {
        debug(info, "frame_ptr->data_len wrong!");
        return 2;
    }
    
    //ignore Xor check
    for(int i = 0; i < xor_counter; i++)
    {
        xor_value ^= *data++;
    }
    
    if(xor_value != *data)
    {
        debug(info, "xor error");
    }
    
    switch(frame_ptr->cmd)
    {
        case RGB_DATA:
        {
            fill_led_buffer(&(frame_ptr->data));
            send_ack(tpcb, es, RGB_DATA_ACK,STATUS_OK);
            break;
        }
        case RGB_TRIGGER:
        {
            trigger_led();
            send_ack(tpcb, es, RGB_TRIGGER_ACK,STATUS_OK);
            break;
        }
        case HEART_BEAT:
        {
            break;
        }
        case CONFIG_IP:
        {                       
            if(config_ip(&(frame_ptr->data)))
            {
                send_ack(tpcb, es, CONFIG_IP_ACK,STATUS_OK);
            }
            else
            {
                send_ack(tpcb, es, CONFIG_IP_ACK,STATUS_ERR);
            }
            break;
        }
        default:break;
    }
    return 0;
}

static void fill_led_buffer(uint8_t *data_start)
{
    RGB_DATA_PTR_t rgb_data_ptr = (RGB_DATA_PTR_t)data_start; 
    uint16_t led_start = rgb_data_ptr->led_start;
    uint16_t led_end = rgb_data_ptr->led_end;
    uint8_t channel = rgb_data_ptr->channel;
    uint16_t buffer_addr_start = led_start * 3;
    uint16_t len = (led_end - led_start + 1) * 3;

    if(channel > CH_NUM)
    {
        //error channel num
        debug(info, "channel wrong!");
        return;
    }
    if(led_end > CH_LED_NUM - 1)
    {
        debug(info, "led_end too large!");
        return;
    }
    if(led_end < led_start)
    {
        //error channel num
        debug(info, "led_end is less than led_start!");
        return;
    }    
    if(led_num_to_send < led_end)
    {
        led_num_to_send = led_end;
    }
    
    memcpy(&led_buffer[channel][buffer_addr_start], &rgb_data_ptr->rgb_data, len);	
}

static bool config_ip(uint8_t *data_start)
{
    IP_DATA_PTR_t ip_data_ptr = (IP_DATA_PTR_t)data_start;
    ip4_addr_t ipaddr;
    IP4_ADDR(&ipaddr, ip_data_ptr->ip_addr[0], ip_data_ptr->ip_addr[1], ip_data_ptr->ip_addr[2], ip_data_ptr->ip_addr[3]);
    return set_ip(ipaddr, ip_data_ptr->ip_port);
}

//just for function test
static void update_channel_data(uint8_t channel, uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t size = CH_LED_NUM * 3;
    for(int i = 0; i < size; i+=3)
    {
        led_buffer[channel][i] = green;
        led_buffer[channel][i+1] = red;
        led_buffer[channel][i+2] = blue;
    }
}

//just for function test
static void update_all_data(uint8_t red, uint8_t green, uint8_t blue)
{
    for(int i = 0; i < CH_NUM; i++)
    {
        update_channel_data(i, red,green,blue);
    }
}
void delay_ms(uint32_t delay)
{
    for(int i=0; i<delay; i++)
    {
        for(int j=0; j<168067; j++)
        {}
    }
}

//just for function test
void test()
{
    for(int i = 0; i < 255; i++)
    {
        update_all_data(0,i,0);
        ws2812_send(&led_buffer[0][0], 
                   &led_buffer[1][0],
                   &led_buffer[2][0],
                   &led_buffer[3][0],
                   &led_buffer[4][0],
                   &led_buffer[5][0],
                   CH_LED_NUM*3
                        );
        delay_ms(1);
    }
}

void test_rect()
{
    for(int i = 0; i < 255; i++)
    {
        //update_all_data(0,i,0);

        led_buffer[0][0] = 0;
        led_buffer[0][1] = 255;
        led_buffer[0][2] = 255;
        led_buffer[0][3] = 0;
        led_buffer[0][4] = 0;
        led_buffer[0][5] = 255;
        ws2812_send(&led_buffer[0][0], 
                   &led_buffer[1][0],
                   &led_buffer[2][0],
                   &led_buffer[3][0],
                   &led_buffer[4][0],
                   &led_buffer[5][0],
                   CH_LED_NUM*3
                        );
        delay_ms(1000);
        led_buffer[0][0] = 255;
        led_buffer[0][1] = 0;
        led_buffer[0][2] = 0;
        led_buffer[0][3] = 255;
        led_buffer[0][4] = 255;
        led_buffer[0][5] = 0;
        ws2812_send(&led_buffer[0][0], 
                   &led_buffer[1][0],
                   &led_buffer[2][0],
                   &led_buffer[3][0],
                   &led_buffer[4][0],
                   &led_buffer[5][0],
                   CH_LED_NUM*3
                        );
        delay_ms(1000);
    }
}
	
extern  __IO uint32_t uwTick;
static void trigger_led()
{
    __set_PRIMASK(1);
    ws2812_send(&led_buffer[0][0], 
               &led_buffer[1][0],
               &led_buffer[2][0],
               &led_buffer[3][0],
               &led_buffer[4][0],
               &led_buffer[5][0],
               (led_num_to_send+1) * 3
                    );
    __set_PRIMASK(0);
    
   uwTick += 28;

}

void send_ack(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es, uint8_t cmd, uint8_t status)
{

    uint8_t data[7] = {FRAME_START,3,0,0,0,0,FRAME_END}; 
    data[3] = cmd;
    data[4] = status;
    data[5] = data[1] ^ data[2] ^ data[3] ^ data[4];
    //es->state = ES_RECEIVED;
//	es->pcb = tpcb;
    es->p = pbuf_alloc(PBUF_TRANSPORT, 7 , PBUF_POOL);
    if(NULL != es->p)
    {
        pbuf_take(es->p, data, 7);
        tcp_echoserver_send(tpcb, es);
    }
    else
    {
        printf("pbuf_alloc return null\n");
    }

}





