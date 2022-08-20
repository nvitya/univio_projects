/*
 *  file:     uio_device.h
 *  brief:    UNIVIO device final instance, project specific
 *  version:  1.00
 *  date:     2022-02-13
 *  authors:  nvitya
*/

#ifndef  _UIO_DEVICE_H_
#define  _UIO_DEVICE_H_

#include "uio_sensor.h"

class TUioDevice : public TUioSensor
{
public:
};

extern TUioDevice   g_uiodev;

#endif
