
// This is a guard condition so that contents of this file are not included
// more than once.

#ifndef SETUP_H
#define	SETUP_H

extern void setup(void);
extern void setuptmr0(void);
extern void resettmr0(void);
extern void configint(void);

//I2C functions
extern void setupI2C(void);
extern void wait_I2C(void);
extern void start_I2C(void);
extern void restart_I2C(void);
extern void stop_I2C(void);
extern void send_ACK(void);
extern void send_NACK(void);
extern __bit write_I2C(uint8_t);
extern uint8_t read_I2C(void);


#endif