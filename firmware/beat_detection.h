#ifndef __BEAT_DETECTION_
#define __BEAT_DETECTION_

struct ICUDriver;

void do_beatdetection();
void init_beatdetection();
void pulse_width_callback(ICUDriver *icu_driver);
void periode_width_callback(ICUDriver *driver);
void timer_overflow_callback(ICUDriver *driver);

#endif
