#include "mgos.h"
#include "mgos_pwm.h"
#include "mgos_jsar.h"
#include "mgos_jsar_servo.h"

#define LED_BUILDIN   (2)

Servo* myservo1;
Servo* myservo2;

uint16_t angle = 0;

/* Led task - green led on board */
static void led_task(void *arg)
{
    mgos_gpio_toggle(LED_BUILDIN);
    (void) arg;
}


/* Servo task1 */
static void servo_task1(void *arg)
{   

    mgos_servo_write(myservo1, 45);
    mgos_servo_write_us(myservo2, 2000);
    LOG(LL_INFO, ("S1 angle: %d", mgos_servo_read(myservo1)));
    LOG(LL_INFO, ("S1 us:    %d", mgos_servo_read_us(myservo1)));
    
    mgos_msleep(2000);

    mgos_servo_write_us(myservo1, 2000);
    mgos_servo_write(myservo2, 45);

    LOG(LL_INFO, ("S1 angle: %d", mgos_servo_read(myservo1)));
    LOG(LL_INFO, ("S1 us:    %d\n", mgos_servo_read_us(myservo1)));
    (void) arg;
}


enum mgos_app_init_result mgos_app_init(void) 
{
    myservo1 = mgos_servo_create();
    mgos_servo_attach(myservo1, 18);

    myservo2 = mgos_servo_create();
    mgos_servo_attach_full(myservo2, 19, 1000, 2000);

	/* Set tasks */
    mgos_set_timer(1000 , MGOS_TIMER_REPEAT, led_task, NULL);
  	mgos_set_timer(4000 , MGOS_TIMER_REPEAT, servo_task1, NULL);


	return MGOS_APP_INIT_SUCCESS;
}












