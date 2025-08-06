#include "Main.h"
//------------------------------------------------------------------------------------------------------------
int main(void)
{
    asCommander commander;

    if (!commander.init())
        return -1;

    commander.run();

    return 0;
}
