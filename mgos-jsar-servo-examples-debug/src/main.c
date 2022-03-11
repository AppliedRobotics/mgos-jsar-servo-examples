#include "mgos.h"
#include "mgos_pwm.h"
#include "mgos_jsar.h"
#include "mgos_jsar_servo.h"
//#include <stdlib. h>

#define UARTno (0)
#define LED_BUILDIN   (2)

Servo* myservo1;
Servo* myservo2;

char buf[256];
uint16_t g_len = 0;
uint8_t new_message = 0;

uint16_t xxx = 1500;
uint16_t yyy = 90; 

/* User UART callback */
void user_cb(uint16_t len, uint8_t *data, void *userdata)
{
    memcpy(buf + g_len, data, len);
    g_len += len;
    
    if (g_len > 1 && buf[g_len - 1] == '\n') {
        new_message = 1;
    }

	(void) userdata;
}

void uart_user(void *arg)
{
    if (new_message) {
        uint16_t value = atoi(buf);
        mgos_uart_printf(UARTno, "L:%d, I:%d\n", g_len, value);
        new_message = 0;
        g_len = 0;
        mgos_servo_write(myservo1, value);
        mgos_servo_write(myservo2, value);
        mgos_uart_printf(UARTno, " 1: %d", mgos_servo_read(myservo1));
        mgos_uart_printf(UARTno, " 2: %d\n", mgos_servo_read(myservo2));
    }


	(void) arg;
}

/* Led task - green led on board */
static void led_task(void *arg)
{
    mgos_gpio_toggle(LED_BUILDIN);
    (void) arg;
}

static void pwm1(void *arg)
{   
    mgos_pwm_set(23, 50, 0.075);
    (void) arg;
}

/* SERVO lib task */
static void pwm2(void *arg)
{   
    //mgos_uart_printf(UARTno, "L:%d, S:%s\n", g_len, buf);
    //mgos_uart_printf(UARTno, "X: %d", xxx);
    //mgos_uart_printf(UARTno, " 1: %d", mgos_servo_write_us(myservo1, xxx));
    //mgos_uart_printf(UARTno, " 2: %d\n", mgos_servo_write_us(myservo2, xxx));

    (void) arg;
}


enum mgos_app_init_result mgos_app_init(void) 
{
    myservo1 = mgos_servo_create();
    mgos_servo_attach(myservo1, 19);

    myservo2 = mgos_servo_create();
    mgos_servo_attach_full(myservo2, 18, 1000, 2000);

    mgos_pwm_set(23, 50, 0.075);

	/* Set callback for UART */
	mgos_dxl_setUserUartCb(user_cb, NULL);

	/* Set tasks */
    mgos_set_timer(500 , MGOS_TIMER_REPEAT, uart_user, NULL);
  	mgos_set_timer(500 , MGOS_TIMER_REPEAT, led_task, NULL);
    mgos_set_timer(1000 , MGOS_TIMER_REPEAT, pwm1, NULL);

	return MGOS_APP_INIT_SUCCESS;
}












