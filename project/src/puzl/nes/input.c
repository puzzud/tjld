#include <nes/input.h>

#include <string.h>

#include <puzl.h>
#include <nes/nes.h>

void InitializeInput(void)
{
	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0;
}
