#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <power_clocks_lib.h>
#include <sysclk.h>
#include <compiler.h>
#include <board.h>
#include <stdio_usb.h>


#define CONFIG_USART_IF (AVR32_USART2)

#include "FreeRTOS.h"
#include "task.h"

#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00


void busy_delay_ms(int delay){
    for(; delay != 0; delay--){
        for(int i = 0; i < 2108; i++){
            asm volatile ("" ::: "memory");
        }
    }
}

void busy_delay_short(void){
    for(int i = 0; i < 10; i++){
        asm volatile ("" ::: "memory");
    }
}

void init(){
	board_init();
	
    gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
    gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
	    setbuf(stdout, NULL);
	    setbuf(stdin,  NULL);
    #endif
}

static void taskFn(void* args){
	const portTickType delay = 1000 / portTICK_RATE_MS;
	
    int iter = 0;

	while(1){
		gpio_toggle_pin(LED0_GPIO);
		printf("tick %d\n", iter++);
		
		vTaskDelay(delay);
	}
}

void toggleLED0 ( void* pvParameters){
	configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
	for(;;){
		
		gpio_toggle_pin(LED0_GPIO);
		vTaskDelay(200);
		
	}
	vTaskDelete( NULL );
}

void toggleLED1 ( void* pvParameters){
	configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
	for(;;){
		
		gpio_toggle_pin(LED1_GPIO);
		//wait 500 ticks. tick frequency is 1000Hz, so time period between ticks is 1/1000 = 1ms. 
		vTaskDelay(500);
	}
	vTaskDelete( NULL );
}


void taskA(void){
	xTaskCreate(toggleLED0, "Toggle LED0", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(toggleLED1, "Toggle LED1", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
	
	vTaskStartScheduler();
}

void test1 ( void* pvParameters){
	configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
	for(;;){
		if(gpio_pin_is_low(TEST_A)){
			gpio_set_pin_low(RESPONSE_A);
			busy_delay_short();
			gpio_set_pin_high(RESPONSE_A);
		}
		else{
			vTaskDelay(1);
		}
	}
	vTaskDelete( NULL );
}
void test2 ( void* pvParameters){
	configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
	for(;;){
		
		if(gpio_pin_is_low(TEST_B)){
			gpio_set_pin_low(RESPONSE_B);
			busy_delay_short();
			gpio_set_pin_high(RESPONSE_B);
		}
		else{
			vTaskDelay(1);
		}
	}
	vTaskDelete( NULL );
}
void test3 ( void* pvParameters){
	configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
	for(;;){
		
		if(gpio_pin_is_low(TEST_C)){
			busy_delay_ms(3);
			gpio_set_pin_low(RESPONSE_C);
			busy_delay_short();
			gpio_set_pin_high(RESPONSE_C);
		}
		else{
			vTaskDelay(1);
		}
	}
	vTaskDelete( NULL );
}

void taskB(void){
	xTaskCreate(test1, "Test1", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(test2, "Test2", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(test3, "Test3", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);
	
	vTaskStartScheduler();
}

int main(){
	init();
    /*xTaskCreate(taskFn, "", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);*/
	// Start the scheduler, anything after this will not run.
	//vTaskStartScheduler();
	//taskA();
	taskB();

    return 0;
}

