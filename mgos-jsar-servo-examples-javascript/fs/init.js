load('api_config.js');
load('api_gpio.js');
load('api_timer.js');
load('api_sys.js');
load('api_jsar.js');
load('api_jsar_servo.js');

let led_state = false;

let servo1 = Servo.create(); 
let servo2 = Servo.create(); 

servo1.attach(18);
servo2.attach_full(19, 1000, 2000);

Timer.set(2000, Timer.REPEAT, function() {
  if (led_state === false) {
    led_state = true;
    JsAr.expanderWriteLed(1);
    servo1.write(45);
    servo2.write(45);
  } else {
    led_state = false;
    JsAr.expanderWriteLed(0);
    servo1.write(135);
    servo2.write(135);
  }

  print(servo1.read());
  print(servo2.read_us());

}, null);