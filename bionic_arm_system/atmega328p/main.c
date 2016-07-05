#include <avr/io.h>
#include <util/delay.h>

// Port D
#define FINGER_1 (1 << 2)       //pin  2 (IN) 
#define FINGER_2 (1 << 7)       //pin  5 (IN) 
#define FINGER_3 (1 << 4)       //pin  4 (IN) 
#define FINGER_4 (1 << 5)       //pin  7 (IN) 
#define SERVO_FINGER_1 (1 << 3) //pin  3 (OUT) PWM: OC2B

// Port B
#define SERVO_FINGER_2 (1 << 1) //pin  9 (OUT) PWM: OC1A
#define SERVO_FINGER_3 (1 << 2) //pin 10 (OUT) PWM: OC1B
#define SERVO_FINGER_4 (1 << 3) //pin 11 (OUT) PWM: OC2A

#define STEP_VARIATION_WRIST 10
#define CLOSE_FINGER 600 // 180 degree
#define OPEN_FINGER 97   // 0 degree

typedef enum finger_id{
  finger_1, finger_2, finger_3, finger_4
}fid_t;

// Fingers     1  2  3  4
int angle[] = {0,0,0,0};

void changeServoDirection (fid_t finger_id, int direction);
int getDirection(int pin);
void delay_us(uint16_t count);

int main(void){
    DDRD &= ~(FINGER_1);
    DDRD &= ~(FINGER_2);
    DDRD &= ~(FINGER_3);
    DDRD &= ~(FINGER_4);
    DDRD |= (SERVO_FINGER_1);

    DDRB |= (SERVO_FINGER_2);
    DDRB |= (SERVO_FINGER_3);
    DDRB |= (SERVO_FINGER_4);

    //Configure TIMER1
    TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
    TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

    ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).


    while(1){
     // changeServoDirection (finger_1, getDirection(FINGER_1));
      changeServoDirection (finger_2, getDirection(FINGER_2));
      changeServoDirection (finger_3, getDirection(FINGER_3));
     // changeServoDirection (finger_4, getDirection(FINGER_4));
    }

    return 0;
}

void delay_us(uint16_t count) {
  while(count--) {
    _delay_us(100);
  }
}

void changeServoDirection (fid_t finger_id, int direction){
    if (direction){
        if (angle[(int) finger_id] < CLOSE_FINGER)
            angle[(int) finger_id] += STEP_VARIATION_WRIST;
    }else{
        if (angle[(int) finger_id] > OPEN_FINGER)
            angle[(int) finger_id] -= STEP_VARIATION_WRIST;
    }

    OCR1A = angle[finger_2];
    OCR1B = angle[finger_3];
}

int getDirection(int pin){
    return PIND & pin;
}
