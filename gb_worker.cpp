#include "gb_worker.h"
#include <QDir>
#include <iostream>


gb_worker::gb_worker()
{
   gb_cpu = new gb_cpu_state();

   gb_gpu = new gb_gpu_state();
   gb_jpd = new gb_joypad_state();
   gb_timer = new gb_timer_state();

   gb_mem = new gb_mem_state();
   gb_int = new gb_interrupt_state();
   gb_lcd = new gb_lcd_state();

   romFileName = QString("/Users/gillesenglebert/Documents/Programming/Emulatoren/gbemulator/qtgb/roms/red.gb");
   biosFileName = QString("/Users/gillesenglebert/Documents/Programming/Emulatoren/gbemulator/qtgb/bios.gb");

   gb_reset();

}

gb_worker::~gb_worker()
{
    delete gb_cpu;
    delete gb_gpu;
    delete gb_jpd;
    delete gb_timer;
    delete gb_mem;
    delete gb_int;
    delete gb_lcd;
}

void gb_worker::run() {
    int t;

    forever {
        t = gb_cpu->execute();
            gb_timer->execute(t);
            gb_int->execute();
            gb_gpu->execute(t);
            gb_jpd->execute();
        //check for vsync
        if (gb_gpu->shouldDisplay)
            emit frameCalculated(&gb_lcd->pxMap.m_vertices);
    }
    emit finished();
}

void gb_worker::keyDown(char key) {
    gb_jpd->keyDown(key);
}

void gb_worker::keyUp(char key) {
   gb_jpd->keyUp(key);
}

void gb_worker::gb_reset() {
    gb_cpu->reset();
    gb_gpu->reset();
    gb_mem->reset();
    gb_int->reset();
    gb_jpd->reset();
    gb_timer->reset();

    gb_int->loadMEM(gb_mem);


    gb_cpu->loadMEM(gb_mem);
    if (gb_cpu->loadBIOS(biosFileName.toStdString()) == 0)
       cout << "Couldn't load BIOS" << endl;
    if (gb_cpu->loadROM(romFileName.toStdString()) == 0)
        cout << "Couldn't load ROM" << endl;

    gb_cpu->loadINT(gb_int);


    gb_gpu->loadLCD(gb_lcd);
    gb_gpu->loadMEM(gb_mem);
    gb_gpu->loadINT(gb_int);

    gb_jpd->loadMEM(gb_mem);
    gb_jpd->loadINT(gb_int);

    gb_timer->loadMEM(gb_mem);
    gb_timer->loadINT(gb_int);
}
