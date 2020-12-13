#include <c64/input.h>

#include <string.h>

#include <puzl.h>
#include <c64/c64.h>

byte KeyCodeStates[NUMBER_OF_KEY_CODES];

void InitializeInput(void)
{
	memset(KeyCodeStates, 0, NUMBER_OF_KEY_CODES * sizeof(byte));

	ControllerAxisXState = 0;
	ControllerAxisYState = 0;
	ControllerButtonState = 0;
}

void UpdateKeyCodeStates(void)
{
	byte rowSelection, columnSelection;
	byte columnInfo;
	byte* keyCodeStates = &KeyCodeStates[0];

	// CIA#1 Port set to output.
	SET_MEMORY_BYTE(CIDDRA, 0xff);

	// CIA#1 Port B set to input.
	SET_MEMORY_BYTE(CIDDRB, 0x00);

	// Start row selection with 11111110 (will be inverted when fed into CIAPRA).
	rowSelection = 0x01;
	do
	{
		// Set row.
		SET_MEMORY_BYTE(CIAPRA, ~rowSelection);

		// Start column selection with 00000001.
		columnSelection = 0x01;
		do
		{
			// Read column.
			columnInfo = GET_MEMORY_BYTE(CIAPRB) & columnSelection;
			*keyCodeStates = (columnInfo == 0) ? 1 : 0;

			++keyCodeStates;

			columnSelection <<= 1;
		}
		while (columnSelection != 0);

		rowSelection <<= 1;
	}
	while (rowSelection != 0x00);
}
