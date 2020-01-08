#include "LabelField.h"
namespace scr {
    //--------------------------------------------------------------------------------------------------------------------------
    LabelField::LabelField(char const* text, uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col,
        uint8_t const nmem, uint8_t const nbuf) :
        NField { height, width, row, col, nmem, nbuf } {
        optionsOff(O_ACTIVE | O_EDIT);
        justify(JUSTIFY_RIGHT);
        write(text);
    }

}
