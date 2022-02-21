#include <kernel.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "include/pad.hpp"

static char padBuf[256] __attribute__((aligned(64)));

static char actAlign[6];
static int actuators;

int port, slot;

void pad::pad_init()
{
    int ret;

    padInit(0);

    port = 0; // 0 -> Connector 1, 1 -> Connector 2
    slot = 0; // Always zero if not using multitap

    std::cout << "PortMax: " << padGetPortMax() << "\n" <<  std::endl;
    std::cout << "SlotMax: " << padGetSlotMax(port) << "\n" << std::endl;
    if((ret = padPortOpen(port, slot, padBuf)) == 0) 
    {
        std::cout << "padOpenPort failed: " << ret << "\n";
        SleepThread();
    }

    if(!initializePad(port, slot)) 
    {
        std::cout << "pad initalization failed\n";
        SleepThread();
    }
}


int pad::waitPadReady(int port, int slot)
{
    int state;
    int lastState;
    char stateString[16];

    state = padGetState(port, slot);
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) 
    {
        if (state != lastState) 
        {
            padStateInt2String(state, stateString);
            printf("Please wait, pad(%d,%d) is in state %s\n", port, slot, stateString);
        }
        lastState = state;
        state=padGetState(port, slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1) 
    {
        std::cout << "Pad OK!\n";
    }
    return 0;
}

/*
 * pad::initializePad()
 */
int pad::initializePad(int port, int slot)
{

    int ret;
    int modes;
    int i;

    pad::waitPadReady(port, slot);

    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    printf("The device has %d modes\n", modes);

    if (modes > 0) {
        printf("( ");
        for (i = 0; i < modes; i++) {
            printf("%d ", padInfoMode(port, slot, PAD_MODETABLE, i));
        }
        printf(")");
    }

    printf("It is currently using mode %d\n",
               padInfoMode(port, slot, PAD_MODECURID, 0));

    // If modes == 0, this is not a Dual shock controller
    // (it has no actuator engines)
    if (modes == 0) 
    {
        std::cout << "This is a digital controller?\n";
        return 1;
    }

    // Verify that the controller has a DUAL SHOCK mode
    i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) 
    {
        std::cout << "This is no Dual Shock controller\n";
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
    if (ret == 0) 
    {
        std::cout << "This is no Dual Shock controller??\n";
        return 1;
    }

    std::cout << "Enabling dual shock functions\n";

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    waitPadReady(port, slot);
    printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

    waitPadReady(port, slot);
    printf("enterPressMode: %d\n", padEnterPressMode(port, slot));

    waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    std::cout << "# of actuators: " << actuators << "\n";

    if (actuators != 0) {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;

        waitPadReady(port, slot);
        printf("padSetActAlign: %d\n",
                   padSetActAlign(port, slot, actAlign));
    }
    else
    {
        std::cout << "Did not find any actuators.\n";
    }

    pad::waitPadReady(port, slot);

    return 1;
}

struct padButtonStatus pad::readPad(int port, int slot)
{
    struct padButtonStatus buttons;
    int ret;    

    do {
    	ret = padGetState(port, slot);
    } while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1));  

    ret = padRead(port, slot, &buttons);      

    return buttons;

}

int pad::isButtonPressed(u32 button)
{
   int ret;
   u32 paddata;
   
   struct padButtonStatus padbuttons;
   
   while (((ret=padGetState(0, 0)) != PAD_STATE_STABLE)&&(ret!=PAD_STATE_FINDCTP1)&&(ret != PAD_STATE_DISCONN)); // more error check ?
   if (padRead(0, 0, &padbuttons) != 0)
   {
    	paddata = 0xffff ^ padbuttons.btns;
     	if(paddata & button)
            return 1;
   }
   return 0;

}
