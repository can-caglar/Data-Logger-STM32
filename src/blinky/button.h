#ifndef BUTTON_H
#define BUTTON_H

// Button is on PA0

typedef void(*obs_func)(void);

void button_init(void);
void button_close(void);
int button_pressed(void);
void button_register_observer(obs_func func);

#endif
