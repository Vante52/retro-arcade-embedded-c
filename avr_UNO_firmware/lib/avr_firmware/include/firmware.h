#ifndef FIRMWARE_H
#define FIRMWARE_H

#ifdef __cplusplus
extern "C" {
#endif

void firmware_init(void);
void firmware_run_once(void);

#ifdef __cplusplus
}
#endif

#endif