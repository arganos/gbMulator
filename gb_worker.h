#ifndef GB_WORKER_H
#define GB_WORKER_H

#include "gb_core/gb_cpu.h"
#include "gb_core/gb_gpu.h"
#include "gb_core/gb_mem.h"
#include "gb_core/gb_cartridge.h"
#include "gb_core/gb_lcd.h"
#include "gb_core/gb_joypad.h"
#include "gb_core/gb_interrupt.h"
#include "gb_core/gb_timer.h"

#include <QObject>

class gb_worker : public QObject
{
    Q_OBJECT
public:
    gb_worker();
     ~gb_worker();

public slots:
    void run();

signals:
    void frameCalculated(sf::VertexArray* vert);
    void finished();
    void error(QString err);

private:

    //Modules of the Gameboy
    gb_cpu_state* gb_cpu;
    gb_gpu_state* gb_gpu;

    gb_joypad_state* gb_jpd;
    gb_timer_state* gb_timer;

    gb_mem_state* gb_mem;
    gb_interrupt_state* gb_int;
    gb_lcd_state* gb_lcd;
};

#endif // GB_WORKER_H
