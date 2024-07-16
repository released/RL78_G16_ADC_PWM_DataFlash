# RL78_G16_ADC_PWM_DataFlash
 RL78_G16_ADC_PWM_DataFlash

udpate @ 2024/07/16

1. initial G16 EVB , to test below function 

- UART : P03 , P04

- BUTTON : P137 

- INTP7 : P61

- INTP8 : P10

- INTP9 : P11

- GPIO OUTPUT: set with output 1 : P121 , P122 , P21 , P22

- GPIO INPUT: set with pull up : P16 , P17 , P06 , P07

- ADC : P20/AIN10

- PWM output : P41/TO03 , 10us/100K

- PWM output : P05/TO07 , 100us/10K

2. use RL78 G16 EVB to test data flash , refer to sample code and application note as below 

https://www.renesas.com/us/en/document/scd/rl78g15-group-renesas-flash-sample-program-type01-sc-version-data-flash-sample-code?language=en
  
https://www.renesas.com/us/en/document/apn/rl78g15-group-renesas-flash-sample-program-type01-sc-version-data-flash-application-notes
 
 
3. add data flash by Smart Configurator , base on application note 

4. modify setting , base on application note (Link Option > Section)

if MCU is 8K flash

.text,.data,.sdata,.RLIB,.SLIB,.textf,.constf/000D8,.const/01D00,.dataR,.bss/FFB00,.sdataR,.sbss/FFE20

if MCU is 16K flash

.text,.data,.sdata,.RLIB,.SLIB,.textf,.constf/000D8,.const/03D00,.dataR,.bss/FFB00,.sdataR,.sbss/FFE20

if MCU is 32K flash

.text,.data,.sdata,.RLIB,.SLIB,.textf,.constf/000D8,.const/07D00,.dataR,.bss/FFB00,.sdataR,.sbss/FFE20


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/Link_Options_Section_1.jpg)


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/Link_Options_Section_2.jpg)


5. set the common file header and c code from sample code , 

![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/include_file.jpg)


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/Common_Options_Additional_include_paths.jpg)

 
6. use terminal digit 1 , to test data flash write at byte index 2

use terminal digit 2 , to read data flash with expect length

![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_dataflash.jpg)


7. use terminal digit 3 , 4 , to test PWM0_2 duty increase , decrease

use terminal digit 5 , 6 , to test PWM0_4 duty increase , decrease


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_0_2_duty.jpg)


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_pwm.jpg)


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/scope_pwm_duty.jpg)


8. use terminal digit a , A , to enable/disable ADC log 

![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_adc.jpg)


9. use terminal digit 7 , 8 , 9 , 10 , to test GPIO output

![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_GPIO_OUT.jpg)


10. if GPIO input , external input detect , will trigger log as below 


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_GPIO_INTPUT.jpg)


![image](https://github.com/released/RL78_G16_ADC_PWM_DataFlash/blob/main/log_EXT_INT.jpg)


