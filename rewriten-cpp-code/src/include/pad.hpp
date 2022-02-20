#include <libpad.h>

class pad
{
  public:  
    int waitPadReady(int port, int slot);
    int isButtonPressed(u32 button);
    int initializePad(int port, int slot);
    void pad_init();
    struct padButtonStatus readPad(int port, int slot);
};
