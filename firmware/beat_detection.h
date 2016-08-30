#ifndef __BEAT_DETECTION_
#define __BEAT_DETECTION_

struct ICUDriver;
struct ICUConfig;

void init_beatdetection();
void pulse_width_callback(ICUDriver *icu_driver);
void periode_width_callback(ICUDriver *driver);
void timer_overflow_callback(ICUDriver *driver);

struct ICUConfig *beatdetection_config();
struct ICUDriver *beatdetection_driver();


#endif
