#include <QApplication>
#include "mycanvas.h"
#include "mainwindow.h"
#include <QFrame>
#include <iostream>
#include "gb_cpu.h"
#include "gb_gpu.h"
#include "gb_mem.h"
#include "gb_cartridge.h"
#include "gb_lcd.h"
#include "gb_joypad.h"
#include "gb_interrupt.h"
#include "gb_timer.h"


/*
 Blarggs tests

 CPU_INSTRS
 nope -> infinite loop

 BUT

 1: yup
 2: 05
 3: yup
 4: yup
 5: yup
 6: yup
 7: yup
 8: yup
 9: yup
 10: yup
 11: yup

 INSTR_TIMING

 yup

 MEM_TIMING

 for later

 */

#include <SFML/Graphics.hpp>
#include <ctime>

using namespace std;


/*static gb_cpu_state gb_cpu;
static gb_gpu_state gb_gpu;

static gb_joypad_state gb_jpd;
static gb_timer_state gb_timer;

static gb_mem_state gb_mem;
static gb_interrupt_state gb_int;*/


int main(int argc, char *argv[])
{
    QApplication App(argc, argv);

    // Create the main frame
    MainWindow* wnd = new MainWindow();

    wnd->setWindowTitle("gbmulator");
    wnd->resize(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE);
    wnd->show();

    // Create a SFML view inside the main frame
    MyCanvas* SFMLView = new MyCanvas(wnd, QPoint(0, 0), QSize(SCREEN_WIDTH*PIXEL_SIZE, SCREEN_HEIGHT*PIXEL_SIZE));
    SFMLView->show();
    return App.exec();



    /*gb_int.loadMEM(gb_mem);

        gb_cpu.loadMEM(gb_mem);
        gb_cpu.loadBIOS("bios.gb");
        gb_cpu.loadROM("roms/red.gb");
      //  gb_cpu.loadROM("roms/mem_timing/individual/01-read_timing.gb");

       // gb_cpu.loadROM("roms/instr_timing/source/custumtest.gb");
        gb_cpu.loadINT(gb_int);


        //gb_gpu.loadWINDOW(window);
        gb_gpu.loadMEM(gb_mem);
        gb_gpu.loadINT(gb_int);

        gb_jpd.loadMEM(gb_mem);
        gb_jpd.loadINT(gb_int);

        gb_timer.loadMEM(gb_mem);
        gb_timer.loadINT(gb_int);


        int  t;


        for (;;) {
            t = gb_cpu.execute();
            gb_timer.execute(t);
            gb_int.execute();
            gb_gpu.execute(t);
            gb_jpd.execute();

        }

        gb_cpu.core_dump();

        return 0;*/

}
