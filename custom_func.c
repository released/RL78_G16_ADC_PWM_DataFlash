/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "inc_main.h"

#include "misc_config.h"
#include "custom_func.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_TRIG_BTN1                       	    (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_TRIG_BTN2                 	        (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_TIMER_PERIOD_SPECIFIC                 (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_TRIG_ADC_CH                           (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_TRIG_ADC_LOG                          (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_REVERSE7                              (flag_PROJ_CTL.bit7)

#define FLAG_PROJ_TRIG_1                                (flag_PROJ_CTL.bit8)
#define FLAG_PROJ_TRIG_2                                (flag_PROJ_CTL.bit9)
#define FLAG_PROJ_TRIG_3                                (flag_PROJ_CTL.bit10)
#define FLAG_PROJ_TRIG_4                                (flag_PROJ_CTL.bit11)
#define FLAG_PROJ_PWM0_2_DUTY_INC                       (flag_PROJ_CTL.bit12)
#define FLAG_PROJ_PWM0_2_DUTY_DEC                       (flag_PROJ_CTL.bit13)
#define FLAG_PROJ_PWM0_4_DUTY_INC                       (flag_PROJ_CTL.bit14)
#define FLAG_PROJ_PWM0_4_DUTY_DEC                       (flag_PROJ_CTL.bit15)

#define FLAG_PROJ_TRIG_INT1                             (flag_PROJ_CTL.bit16)
#define FLAG_PROJ_TRIG_INT2                             (flag_PROJ_CTL.bit17)
#define FLAG_PROJ_TRIG_INT3                             (flag_PROJ_CTL.bit18)
#define FLAG_PROJ_REVERSE19                             (flag_PROJ_CTL.bit19)
#define FLAG_PROJ_TRIG_GPIO1_OUT                        (flag_PROJ_CTL.bit20)
#define FLAG_PROJ_TRIG_GPIO2_OUT                        (flag_PROJ_CTL.bit21)
#define FLAG_PROJ_TRIG_GPIO3_OUT                        (flag_PROJ_CTL.bit22)
#define FLAG_PROJ_TRIG_GPIO4_OUT                        (flag_PROJ_CTL.bit23)

#define FLAG_PROJ_PRESSED_INT1                          (flag_PROJ_CTL.bit24)
#define FLAG_PROJ_PRESSED_INT2                          (flag_PROJ_CTL.bit25)
#define FLAG_PROJ_PRESSED_INT3                          (flag_PROJ_CTL.bit26)
#define FLAG_PROJ_REVERSE27                             (flag_PROJ_CTL.bit27)
#define FLAG_PROJ_REVERSE28                             (flag_PROJ_CTL.bit28)
#define FLAG_PROJ_REVERSE29                             (flag_PROJ_CTL.bit29)
#define FLAG_PROJ_REVERSE30                             (flag_PROJ_CTL.bit30)
#define FLAG_PROJ_REVERSE31                             (flag_PROJ_CTL.bit31)


/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned int counter_tick = 0;
volatile unsigned int btn_counter_tick = 0;

#define BTN_PRESSED_LONG                                (2500)

#define ENABLE_BUTTON
#define ENABLE_LOG_PWM
#define ENABLE_LOG_ADC

/*
    DATA FLASH
*/

unsigned char data_flash_data_read[DF_WRITE_DATA_LENGTH] = {0};
unsigned char data_flash_data_write[DF_WRITE_DATA_LENGTH] = {0};

/*
    ADC
*/
// #define VBG_VOLTAGE                                     (0.815)  // v
#define VBG_VOLTAGE                                     (815)  // mv

/*G15 V_BGR : 0.815 V*/
unsigned long vdd_Vbgr = 0;
unsigned short adc_buffer[11] = {0};

/*
    PWM
*/
#define DUTY_RESOLUTION                                 (100)
volatile unsigned int pwm0_2_duty = 0;
volatile unsigned int pwm0_4_duty = 0;

/*
    GPIO OUT
*/
#define GPIO1_OUT                                       (P12_bit.no1)
#define GPIO2_OUT                                       (P12_bit.no2)
#define GPIO3_OUT                                       (P2_bit.no1)
#define GPIO4_OUT                                       (P2_bit.no2)

/*
    GPIO INPUT
*/
#define GPIO5_INPUT                                     (P1_bit.no6)
#define GPIO6_INPUT                                     (P1_bit.no7)
#define GPIO7_INPUT                                     (P0_bit.no6)
#define GPIO8_INPUT                                     (P0_bit.no7)

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void set_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 1;
}

void reset_TIMER_PERIOD_1000MS(void)
{
    FLAG_PROJ_TIMER_PERIOD_1000MS = 0;
}

bool Is_TIMER_PERIOD_1000MS_Trig(void)
{
    return FLAG_PROJ_TIMER_PERIOD_1000MS;
}

unsigned int btn_get_tick(void)
{
	return (btn_counter_tick);
}

void btn_set_tick(unsigned int t)
{
	btn_counter_tick = t;
}

void btn_tick_counter(void)
{
	btn_counter_tick++;
    if (btn_get_tick() >= 60000)
    {
        btn_set_tick(0);
    }
}

unsigned int get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned int t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000)
    {
        set_tick(0);
    }
}

void delay_ms(unsigned int ms)
{
	#if 1
    unsigned int tickstart = get_tick();
    unsigned int wait = ms;
	unsigned int tmp = 0;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000 -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
	
	#else
	TIMER_Delay(TIMER0, 1000*ms);
	#endif
}

/*
    OUTPUT: set with output 1
    P121
    P122
    P21
    P22
*/
void GPIO_OUTPUT_Process(void)
{
    if (FLAG_PROJ_TRIG_GPIO1_OUT)
    {
        FLAG_PROJ_TRIG_GPIO1_OUT = 0;
        GPIO1_OUT = ~GPIO1_OUT;
        printf_tiny("GPIO1 output toggle\r\n");
    }

    if (FLAG_PROJ_TRIG_GPIO2_OUT)
    {
        FLAG_PROJ_TRIG_GPIO2_OUT = 0;
        GPIO2_OUT = ~GPIO2_OUT;
        printf_tiny("GPIO2 output toggle\r\n");
    }

    if (FLAG_PROJ_TRIG_GPIO3_OUT)
    {
        FLAG_PROJ_TRIG_GPIO3_OUT = 0;
        GPIO3_OUT = ~GPIO3_OUT;
        printf_tiny("GPIO3 output toggle\r\n");
    }

    if (FLAG_PROJ_TRIG_GPIO4_OUT)
    {
        FLAG_PROJ_TRIG_GPIO4_OUT = 0;
        GPIO4_OUT = ~GPIO4_OUT;
        printf_tiny("GPIO4 output toggle\r\n");
    }

}

/*
    INPUT: set with pull up
    P16
    P17
    P06
    P07

*/
void GPIO_INPUT_Process(void)
{

    if (GPIO5_INPUT == 0)   // detect low
    {
        // printf_tiny("GPIO5 detect low\r\n");
    }
    GPIO1_OUT = (GPIO5_INPUT == 0) ? (0) : (1);

    if (GPIO6_INPUT == 0)   // detect low
    {
        // printf_tiny("GPIO6 detect low\r\n");
    }
    GPIO2_OUT = (GPIO6_INPUT == 0) ? (0) : (1);

    if (GPIO7_INPUT == 0)   // detect low
    {
        // printf_tiny("GPIO7 detect low\r\n");
    }
    GPIO3_OUT = (GPIO7_INPUT == 0) ? (0) : (1);

    if (GPIO8_INPUT == 0)   // detect low
    {
        // printf_tiny("GPIO8 detect low\r\n");
    }
    GPIO4_OUT = (GPIO8_INPUT == 0) ? (0) : (1);

}

void EXT_INT3_Process_in_IRQ(void)    
{
    static uint8_t cnt_int3 = 0;
    FLAG_PROJ_TRIG_INT3 = 1;

    if (cnt_int3 == 0)
    {
        FLAG_PROJ_PRESSED_INT3 = 1;
    }
    else if (cnt_int3 == 1)
    {
        FLAG_PROJ_PRESSED_INT3 = 0;
    }

    cnt_int3 = (cnt_int3 >= 1) ? (0) : (cnt_int3+1);
}
void EXT_INT2_Process_in_IRQ(void)    
{
    static uint8_t cnt_int2 = 0;
    FLAG_PROJ_TRIG_INT2 = 1;

    if (cnt_int2 == 0)
    {
        FLAG_PROJ_PRESSED_INT2 = 1;
    }
    else if (cnt_int2 == 1)
    {
        FLAG_PROJ_PRESSED_INT2 = 0;
    }

    cnt_int2 = (cnt_int2 >= 1) ? (0) : (cnt_int2+1);
}
void EXT_INT1_Process_in_IRQ(void)    
{
    static uint8_t cnt_int1 = 0;
    FLAG_PROJ_TRIG_INT1 = 1;

    if (cnt_int1 == 0)
    {
        FLAG_PROJ_PRESSED_INT1 = 1;
    }
    else if (cnt_int1 == 1)
    {
        FLAG_PROJ_PRESSED_INT1 = 0;
    }

    cnt_int1 = (cnt_int1 >= 1) ? (0) : (cnt_int1+1);
}

void EXT_INT_Process(void)  //set both edge
{     
    if (FLAG_PROJ_TRIG_INT1)
    {
        FLAG_PROJ_TRIG_INT1 = 0;
        if (FLAG_PROJ_PRESSED_INT1)
        {            
            printf_tiny("INT1 pressed\r\n");
        }
        else
        {
            printf_tiny("INT1 released\r\n");
        }
    }

      
    if (FLAG_PROJ_TRIG_INT2)
    {
        FLAG_PROJ_TRIG_INT2 = 0;
        if (FLAG_PROJ_PRESSED_INT2)
        {            
            printf_tiny("INT2 pressed\r\n");
        }
        else
        {
            printf_tiny("INT2 released\r\n");
        }
    }     


    if (FLAG_PROJ_TRIG_INT3)
    {
        FLAG_PROJ_TRIG_INT3 = 0;
        if (FLAG_PROJ_PRESSED_INT3)
        {            
            printf_tiny("INT3 pressed\r\n");
        }
        else
        {
            printf_tiny("INT3 released\r\n");
        }
    }
}


unsigned int get_TAU0_4_pwm_ch_duty(void)
{
	return (pwm0_4_duty);
}

void set_TAU0_4_pwm_ch_duty(unsigned int duty)
{
    pwm0_4_duty = duty;
}

/*copy from R_Config_TAU0_4_Create*/
void generate_TAU0_4_pwm_ch(void)
{
    TOM0 |= _0080_TAU_CH7_SLAVE_OUTPUT;
    TOL0 &= (uint16_t)~_0080_TAU_CH7_OUTPUT_LEVEL_L;
    TO0 &= (uint16_t)~_0080_TAU_CH7_OUTPUT_VALUE_1;
    TOE0 |= _0080_TAU_CH7_OUTPUT_ENABLE;
}

void PWM0_4_Process_Adjust(void)
{
    unsigned int temp_duty = 0;
    unsigned int duty_hex = 0;
    unsigned short data_reg_default = get_TAU0_4_pwm_ch_data_reg_default();
    unsigned short tmp = 0;
    float tmp_f = 0.0f;

    if (FLAG_PROJ_PWM0_4_DUTY_INC)
    {
        FLAG_PROJ_PWM0_4_DUTY_INC = 0;

        temp_duty = get_TAU0_4_pwm_ch_duty();
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("+duty1(0_4):0x%02X,0x%02X(%4d)\r\n",temp_duty ,data_reg_default,data_reg_default);
        }
        #endif

        duty_hex = (data_reg_default / DUTY_RESOLUTION) * 1;
        temp_duty = (temp_duty >= data_reg_default) ? (data_reg_default) : (temp_duty + duty_hex ) ;  
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("+duty2(0_4):0x%02X,0x%02X\r\n",temp_duty ,duty_hex);
            printf_tiny("+duty2(0_4):0x%02X(%4d)/0x%02X(%4d)\r\n",temp_duty ,temp_duty,data_reg_default,data_reg_default);                 
        }
        #endif

        set_TAU0_4_pwm_ch_duty(temp_duty);
        #if defined (ENABLE_LOG_PWM)
        {
            tmp_f = (float) 100*(temp_duty/data_reg_default);
            // printf_tiny("+duty(0_4):0x%02X(%4d),%2.4f\r\n",temp_duty, temp_duty ,tmp_f);
        }
        #endif

        tmp = get_TAU0_4_pwm_ch_duty();
        set_TAU0_4_pwm_ch_data_reg(tmp);
        generate_TAU0_4_pwm_ch();
    }
    if (FLAG_PROJ_PWM0_4_DUTY_DEC)
    {
        FLAG_PROJ_PWM0_4_DUTY_DEC = 0;

        temp_duty = get_TAU0_4_pwm_ch_duty();
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("-duty1(0_4):0x%02X,0x%02X(%4d)\r\n",temp_duty ,data_reg_default,data_reg_default);
        }
        #endif

        duty_hex = (data_reg_default / DUTY_RESOLUTION) * 1;
        temp_duty = (temp_duty <= 0) ? (0) : (temp_duty - duty_hex ) ;   
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("-duty2(0_4):0x%02X,0x%02X\r\n",temp_duty ,duty_hex); 
            printf_tiny("-duty2(0_4):0x%02X(%4d)/0x%02X(%4d)\r\n",temp_duty ,temp_duty,data_reg_default,data_reg_default);                         
        }
        #endif

        set_TAU0_4_pwm_ch_duty(temp_duty);
        #if defined (ENABLE_LOG_PWM)
        {
            tmp_f = (float) 100*(temp_duty/data_reg_default);
            // printf_tiny("-duty(0_4):0x%02X(%4d),%2.4f\r\n",temp_duty, temp_duty ,tmp_f );
        }
        #endif

        tmp = get_TAU0_4_pwm_ch_duty();
        set_TAU0_4_pwm_ch_data_reg(tmp);
        generate_TAU0_4_pwm_ch();
    }
}

/*
    TAU0_2 PWM : 100K
*/

unsigned int get_TAU0_2_pwm_ch_duty(void)
{
	return (pwm0_2_duty);
}

void set_TAU0_2_pwm_ch_duty(unsigned int duty)
{
    pwm0_2_duty = duty;
}

/*copy from R_Config_TAU0_2_Create*/
void generate_TAU0_2_pwm_ch(void)
{
    TOM0 |= _0008_TAU_CH3_SLAVE_OUTPUT;
    TOL0 &= (uint16_t)~_0008_TAU_CH3_OUTPUT_LEVEL_L;
    TO0 &= (uint16_t)~_0008_TAU_CH3_OUTPUT_VALUE_1;
    TOE0 |= _0008_TAU_CH3_OUTPUT_ENABLE;
}

void PWM0_2_Process_Adjust(void)
{
    unsigned int temp_duty = 0;
    unsigned int duty_hex = 0;
    unsigned short data_reg_default = get_TAU0_2_pwm_ch_data_reg_default();
    unsigned short tmp = 0;
    float tmp_f = 0.0f;

    if (FLAG_PROJ_PWM0_2_DUTY_INC)
    {
        FLAG_PROJ_PWM0_2_DUTY_INC = 0;

        temp_duty = get_TAU0_2_pwm_ch_duty();
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("+duty1(0_2):0x%02X,0x%02X(%4d)\r\n",temp_duty ,data_reg_default,data_reg_default);
        }
        #endif

        duty_hex = (data_reg_default / DUTY_RESOLUTION) * 1;
        temp_duty = (temp_duty >= data_reg_default) ? (data_reg_default) : (temp_duty + duty_hex ) ;  
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("+duty2(0_2):0x%02X,0x%02X\r\n",temp_duty ,duty_hex);
            printf_tiny("+duty2(0_2):0x%02X(%4d)/0x%02X(%4d)\r\n",temp_duty ,temp_duty,data_reg_default,data_reg_default);                                 
        }
        #endif

        set_TAU0_2_pwm_ch_duty(temp_duty);
        #if defined (ENABLE_LOG_PWM)
        {
            tmp_f = (float) 100*(temp_duty/data_reg_default);
            // printf_tiny("+duty(0_2):0x%02X(%4d),%2.4f\r\n",temp_duty, temp_duty ,tmp_f);
        
        }
        #endif

        tmp = get_TAU0_2_pwm_ch_duty();
        set_TAU0_2_pwm_ch_data_reg(tmp);
        generate_TAU0_2_pwm_ch();
    }
    if (FLAG_PROJ_PWM0_2_DUTY_DEC)
    {
        FLAG_PROJ_PWM0_2_DUTY_DEC = 0;

        temp_duty = get_TAU0_2_pwm_ch_duty();
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("-duty1(0_2):0x%02X,0x%02X(%4d)\r\n",temp_duty ,data_reg_default,data_reg_default);        
        }
        #endif

        duty_hex = (data_reg_default / DUTY_RESOLUTION) * 1;
        temp_duty = (temp_duty <= 0) ? (0) : (temp_duty - duty_hex ) ;   
        #if defined (ENABLE_LOG_PWM)
        {
            // printf_tiny("-duty2(0_2):0x%02X,0x%02X\r\n",temp_duty ,duty_hex);     
            printf_tiny("-duty2(0_2):0x%02X(%4d)/0x%02X(%4d)\r\n",temp_duty ,temp_duty,data_reg_default,data_reg_default);         
        }
        #endif

        set_TAU0_2_pwm_ch_duty(temp_duty);
        #if defined (ENABLE_LOG_PWM)
        {
            tmp_f = (float) 100*(temp_duty/data_reg_default);
            // printf_tiny("-duty(0_2):0x%02X(%4d),%2.4f\r\n",temp_duty, temp_duty ,tmp_f );        
        }
        #endif

        tmp = get_TAU0_2_pwm_ch_duty();
        set_TAU0_2_pwm_ch_data_reg(tmp);
        generate_TAU0_2_pwm_ch();
    }
}




void GetADC(e_ad_channel_t ch)
{
    unsigned short tmp_buffer = 0;

    // R_Config_ADC_Set_ADChannel(ch);
    FLAG_PROJ_TRIG_ADC_CH = 0;
    R_Config_ADC_Start();
    while(!FLAG_PROJ_TRIG_ADC_CH);
    R_Config_ADC_Get_Result_10bit(&tmp_buffer);
    R_Config_ADC_Stop();
    FLAG_PROJ_TRIG_ADC_CH = 0;

    adc_buffer[ch] = tmp_buffer;    

    // printf_tiny("ch[%d]:0x%04X\r\n",ch,adc_buffer[ch]);

}

/*
    COPY FROM R_Config_ADC_Create
    channel : 10
    P20
*/
void ADC_Channel_config_Init(void)
{
    volatile uint16_t w_count;

    ADCEN = 1U;    /* supply AD clock */
    ADMK = 1U;    /* disable INTAD interrupt */
    ADIF = 0U;    /* clear INTAD interrupt flag */
    /* Set INTAD priority */
    ADPR1 = 1U;
    ADPR0 = 1U;
    /* Set ANI10 pin */
    PMC2 |= 0x01U;
    PM2 |= 0x01U;
    TSSEL1 &= 0xF7U;
    ADM0 = _00_AD_CONVERSION_CLOCK_8 | _00_AD_TIME_MODE_NORMAL_1;
    ADM2 = _00_AD_RESOLUTION_10BIT;
    ADS = _0A_AD_INPUT_CHANNEL_10;

    ADCE = 1U;

    /* Reference voltage stability wait time, 0.1us */
    for (w_count = 0U; w_count < AD_WAITTIME_B; w_count++ )
    {
        NOP();
    }
    
    // R_Config_ADC_Start();
}

void GetVREF(void)
{
    unsigned short tmp_buffer = 0;
    unsigned short adc_resolution = 1024;

    /*
        VDD = VBG * adc_resolution / ConversionResult
        VDD/VBG = adc_resolution / ConversionResult
    
    */
    // R_Config_ADC_Set_ADChannel(ADINTERREFVOLT);
    FLAG_PROJ_TRIG_ADC_CH = 0;
    R_Config_ADC_Start();    // to get ADC internal vref channel
    while(!FLAG_PROJ_TRIG_ADC_CH);
    R_Config_ADC_Get_Result_10bit(&tmp_buffer);
    R_Config_ADC_Stop();
    FLAG_PROJ_TRIG_ADC_CH = 0;

    vdd_Vbgr = (unsigned long) VBG_VOLTAGE*adc_resolution/tmp_buffer;

    // printf_tiny("Read VREF:%d(0x%04X),",tmp_buffer,tmp_buffer);
    // printf_tiny("VBGR:%4dmv\r\n",vdd_Vbgr);
}

/*
    COPY FROM R_Config_ADC_Create
    channel : internal reference voltage
*/
void ADC_VREF_config_Init(void)
{    
    volatile uint16_t w_count;

    ADCEN = 1U;    /* supply AD clock */
    ADMK = 1U;    /* disable INTAD interrupt */
    ADIF = 0U;    /* clear INTAD interrupt flag */
    /* Set INTAD priority */
    ADPR1 = 1U;
    ADPR0 = 1U;
    ADM0 = _00_AD_CONVERSION_CLOCK_8 | _00_AD_TIME_MODE_NORMAL_1;
    ADM2 = _00_AD_RESOLUTION_10BIT;
    ADS = _0D_AD_INPUT_INTERREFVOLT;

    ADCE = 1U;

    /* Reference voltage stability wait time, 0.1us */
    for (w_count = 0U; w_count < AD_WAITTIME_B; w_count++ )
    {
        NOP();
    }

    // R_Config_ADC_Start();  
}

void ADC_Process(void)
{    
    unsigned int mv = 0;

    // get ADC channel
    GetADC(ADCHANNEL10);
    mv = (vdd_Vbgr*adc_buffer[ADCHANNEL10]) >> 10;
    	
    #if defined (ENABLE_LOG_ADC)   //ADC debug log
    {
        if (FLAG_PROJ_TRIG_ADC_LOG)
        {
            printf_tiny("VREF:%d,",vdd_Vbgr);
            printf_tiny("ch[%d]:0x%04X",ADCHANNEL10,adc_buffer[ADCHANNEL10]);
            printf_tiny("(%d mv)", mv );
            printf_tiny("\r\n");
        }
    }
    #endif
    
}

void ADC_init(void)
{    
    // init internal vref channel
    ADC_VREF_config_Init();
    // get VREF
    GetVREF();

    // init normal adc channel
    ADC_Channel_config_Init();
}

void ADC_Process_in_IRQ(void)
{
    FLAG_PROJ_TRIG_ADC_CH = 1;
}


void Data_Flash_write_test(void)
{
    unsigned int i = 0;     
    e_sample_ret_t l_e_rfsp_status_flag;

    for ( i = 0 ; i < DF_WRITE_DATA_LENGTH; i++)
    {
        data_flash_data_write[i] = 0x00;
    }
    data_flash_data_write[0] = 0x5A;
    data_flash_data_write[1] = 0x5A;

    data_flash_data_write[2] = (*(uint8_t __far *)((uint32_t)DF_WRITE_ADDRESS + 2)); // read index 2 for next data increase
    data_flash_data_write[2] += 1;
    for ( i = 2 ; i < (DF_WRITE_DATA_LENGTH -2) ; i++)
    {
        data_flash_data_write[i+1] = data_flash_data_write[i] + 1;
    }

    data_flash_data_write[DF_WRITE_DATA_LENGTH-2] = 0xA5;
    data_flash_data_write[DF_WRITE_DATA_LENGTH-1] = 0xA5;

    l_e_rfsp_status_flag = Sample_DataFlashControl(DF_WRITE_ADDRESS, DF_WRITE_DATA_LENGTH, data_flash_data_write);
    printf_tiny("Sample_DataFlashControl finish(0x%02X)\r\n",l_e_rfsp_status_flag);

}

void Data_Flash_read(void)
{
    unsigned int cnt = 0;
    unsigned int check_write_data_addr = DF_WRITE_ADDRESS;
    
    for (cnt = 0 ; cnt < DF_WRITE_DATA_LENGTH; cnt++)
    {
        data_flash_data_read[cnt] = (*(uint8_t __far *)((uint32_t)check_write_data_addr));        
        check_write_data_addr++;
    }

    dump_buffer_hex(data_flash_data_read ,DF_WRITE_DATA_LENGTH);
}

int Data_Flash_init(void)
{
    /* Local variable definition */
    e_rfsp_ret_t   l_e_rfsp_status_flag;
    e_sample_ret_t l_e_sam_ret_value;
    // uint16_t       l_u16_count;
    uint16_t       wait_count;
    uint32_t       l_mclk_freq;

    l_mclk_freq = R_BSP_GetFclkFreqHz();
    l_mclk_freq = (l_mclk_freq + L_MCLK_ROUNDUP_VALUE)/L_MCLK_FREQ_1MHz;

    /* Oscillation stable time for HOCO. */
    for (wait_count = ((30 * (uint16_t)l_mclk_freq) / 4); wait_count > 0; wait_count--)
    {
         BSP_NOP();
    }
    
    /* Check whether HOCO is already started */
    if (SAMPLE_VALUE_U01_MASK0_1BIT == HIOSTOP)
    {
        /* Initialize RFSP */
        l_e_rfsp_status_flag = R_RFSP_Init((uint8_t)l_mclk_freq);
        printf_tiny("R_RFSP_Init finish(0x%02X)\r\n",l_e_rfsp_status_flag);
        
        // if (R_RFSP_ENUM_RET_STS_OK == l_e_rfsp_status_flag)
        // {
        //     /* Data flash blankcheck, erase, and write */
        //     l_e_sam_ret_value = Sample_DataFlashControl(DF_WRITE_ADDRESS, DF_WRITE_DATA_LENGTH, l_au08_write_data);
        // }
        // else
        // {
        //     /* Failed to initialize RFSP */
        //     l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_PARAMETER;
        // }
    }
    else
    {
        /* HOCO is not started */
        l_e_sam_ret_value = SAMPLE_ENUM_RET_ERR_CONFIGURATION;
    }
    
    return ((int)l_e_sam_ret_value);
    
}

void Timer_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000) == 0)
    {
        set_TIMER_PERIOD_1000MS();
    }

    if ((get_tick() % 100) == 0)
    {
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 1;
    }

    if ((get_tick() % 50) == 0)
    {

    }	

    #if defined (ENABLE_BUTTON)
    Button_Process_long_counter();
    #endif
}


/*
    G16 target board
    LED1 connected to P12, LED2 connected to P15
*/
void LED_Toggle(void)
{
    // PIN_WRITE(2,0) = ~PIN_READ(2,0);
    // PIN_WRITE(2,1) = ~PIN_READ(2,1);
    P1_bit.no2 = ~P1_bit.no2;
    P1_bit.no5 = ~P1_bit.no5;
}

void loop(void)
{
	// static unsigned int LOG1 = 0;

    if (Is_TIMER_PERIOD_1000MS_Trig())
    {
        reset_TIMER_PERIOD_1000MS();

        // printf_tiny("log(timer):%4d\r\n",LOG1++);
        LED_Toggle();             
    }

    if (FLAG_PROJ_TIMER_PERIOD_SPECIFIC)
    {
        FLAG_PROJ_TIMER_PERIOD_SPECIFIC = 0;
        ADC_Process();  // P20        
    }    

    if (FLAG_PROJ_TRIG_1)
    {
        FLAG_PROJ_TRIG_1 = 0;
        Data_Flash_write_test();
    }
    if (FLAG_PROJ_TRIG_2)
    {
        FLAG_PROJ_TRIG_2 = 0;
        Data_Flash_read();
    }

    EXT_INT_Process();
    GPIO_INPUT_Process();
    GPIO_OUTPUT_Process();

    #if defined (ENABLE_BUTTON)
    Button_Process_in_polling();
    #endif
    
    PWM0_2_Process_Adjust();    
    PWM0_4_Process_Adjust();

}

#if defined (ENABLE_BUTTON)
// G16 EVB , P137/INTP0 , set both edge 
void Button_Process_long_counter(void)
{
    if (FLAG_PROJ_TRIG_BTN2)
    {
        btn_tick_counter();
    }
    else
    {
        btn_set_tick(0);
    }
}

void Button_Process_in_polling(void)
{
    static unsigned char cnt = 0;

    if (FLAG_PROJ_TRIG_BTN1)
    {
        FLAG_PROJ_TRIG_BTN1 = 0;
        printf_tiny("BTN pressed(%d)\r\n",cnt);

        if (cnt == 0)   //set both edge  , BTN pressed
        {
            FLAG_PROJ_TRIG_BTN2 = 1;
        }
        else if (cnt == 1)  //set both edge  , BTN released
        {
            FLAG_PROJ_TRIG_BTN2 = 0;
        }

        cnt = (cnt >= 1) ? (0) : (cnt+1) ;
    }

    if ((FLAG_PROJ_TRIG_BTN2 == 1) && 
        (btn_get_tick() > BTN_PRESSED_LONG))
    {         
        printf_tiny("BTN pressed LONG\r\n");
        btn_set_tick(0);
        FLAG_PROJ_TRIG_BTN2 = 0;
    }
}

// G16 EVB , P137/INTP0
void Button_Process_in_IRQ(void)    
{
    FLAG_PROJ_TRIG_BTN1 = 1;
}
#endif

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf > 0x7F)
    {
        printf_tiny("invalid command\r\n");
    }
    else
    {
        printf_tiny("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '1':
                FLAG_PROJ_TRIG_1 = 1;
                break;
            case '2':
                FLAG_PROJ_TRIG_2 = 1;
                break;

            case '3':
                FLAG_PROJ_PWM0_2_DUTY_INC = 1;
                break;
            case '4':
                FLAG_PROJ_PWM0_2_DUTY_DEC = 1;
                break;

            case '5':
                FLAG_PROJ_PWM0_4_DUTY_INC = 1;
                break;
            case '6':
                FLAG_PROJ_PWM0_4_DUTY_DEC = 1;
                break;

                
            case '7':
                FLAG_PROJ_TRIG_GPIO1_OUT = 1;
                break;
            case '8':
                FLAG_PROJ_TRIG_GPIO2_OUT = 1;
                break;
            case '9':
                FLAG_PROJ_TRIG_GPIO3_OUT = 1;
                break;
            case '0':
                FLAG_PROJ_TRIG_GPIO4_OUT = 1;
                break;


            case 'A':
            case 'a':
                FLAG_PROJ_TRIG_ADC_LOG ^= 1;
                break;

            case 'X':
            case 'x':
                RL78_soft_reset(7);
                break;
            case 'Z':
            case 'z':
                RL78_soft_reset(1);
                break;
        }
    }
}

/*
    Reset Control Flag Register (RESF) 
    BIT7 : TRAP
    BIT6 : 0
    BIT5 : 0
    BIT4 : WDCLRF
    BIT3 : 0
    BIT2 : 0
    BIT1 : IAWRF
    BIT0 : LVIRF
*/
// void check_reset_source(void)
// {
//     /*
//         Internal reset request by execution of illegal instruction
//         0  Internal reset request is not generated, or the RESF register is cleared. 
//         1  Internal reset request is generated. 
//     */
//     uint8_t src = RESF;
//     printf_tiny("Reset Source <0x%08X>\r\n", src);

//     #if 1   //DEBUG , list reset source
//     if (src & BIT0)
//     {
//         printf_tiny("0)voltage detector (LVD)\r\n");       
//     }
//     if (src & BIT1)
//     {
//         printf_tiny("1)illegal-memory access\r\n");       
//     }
//     if (src & BIT2)
//     {
//         printf_tiny("2)EMPTY\r\n");       
//     }
//     if (src & BIT3)
//     {
//         printf_tiny("3)EMPTY\r\n");       
//     }
//     if (src & BIT4)
//     {
//         printf_tiny("4)watchdog timer (WDT) or clock monitor\r\n");       
//     }
//     if (src & BIT5)
//     {
//         printf_tiny("5)EMPTY\r\n");       
//     }
//     if (src & BIT6)
//     {
//         printf_tiny("6)EMPTY\r\n");       
//     }
//     if (src & BIT7)
//     {
//         printf_tiny("7)execution of illegal instruction\r\n");       
//     }
//     #endif

// }

/*
    7:Internal reset by execution of illegal instruction
    1:Internal reset by illegal-memory access
*/
//perform sofware reset
void _reset_by_illegal_instruction(void)
{
    static const unsigned char illegal_Instruction = 0xFF;
    void (*dummy) (void) = (void (*)(void))&illegal_Instruction;
    dummy();
}
void _reset_by_illegal_memory_access(void)
{
    // #if 1
    // const unsigned char ILLEGAL_ACCESS_ON = 0x80;
    // IAWCTL |= ILLEGAL_ACCESS_ON;            // switch IAWEN on (default off)
    // *(__far volatile char *)0x00000 = 0x00; //write illegal address 0x00000(RESET VECTOR)
    // #else
    // signed char __far* a;                   // Create a far-Pointer
    // IAWCTL |= _80_CGC_ILLEGAL_ACCESS_ON;    // switch IAWEN on (default off)
    // a = (signed char __far*) 0x0000;        // Point to 0x000000 (FLASH-ROM area)
    // *a = 0;
    // #endif
}

void RL78_soft_reset(unsigned char flag)
{
    switch(flag)
    {
        case 7: // do not use under debug mode
            _reset_by_illegal_instruction();        
            break;
        case 1:
            _reset_by_illegal_memory_access();
            break;
    }
}

// retarget printf
int __far putchar(int c)
{
    // G16 , UART0
    STMK0 = 1U;    /* disable INTST0 interrupt */
    TXD0 = (unsigned char)c;
    while(STIF0 == 0)
    {

    }
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    return c;
}

void hardware_init(void)
{
    // const unsigned char indicator[] = "hardware_init";
    BSP_EI();
    R_Config_UART0_Start();         // UART , P03 , P04
    R_Config_TAU0_1_Start();        // TIMER
    
    #if defined (ENABLE_BUTTON)
    R_Config_INTC_INTP0_Start();    // BUTTON , P137 
    #endif

    // ext. INT ,  set both edge
    /*
        INTP7 : P61
        INTP8 : P10
        INTP9 : P11
    */
    R_Config_INTC_INTP7_Start(); 
    R_Config_INTC_INTP8_Start(); 
    R_Config_INTC_INTP9_Start(); 

    // GPIO
    /*
        OUTPUT: set with output 1
        P121
        P122
        P21
        P22

        INPUT: set with pull up
        P16
        P17
        P06
        P07
    */

    Data_Flash_init();
    Data_Flash_read();

    // ADC : P20/AIN10
    ADC_init();                     

    // PWM output : P41/TO03 , 10us/100K
    R_Config_TAU0_2_Start();
    // PWM output : P05/TO07 , 100us/10K
    R_Config_TAU0_4_Start();

    // check_reset_source();
    // printf("%s finish\r\n\r\n",__func__);
    printf_tiny("%s finish\r\n\r\n",__func__);
}
