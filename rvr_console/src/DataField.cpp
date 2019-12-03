#include "DataField.h"
namespace scr {

    DataField::DataField(const int height, const int width, const int row, const int col, const int nmem, const int nbuf) :
            NField { height, width, row, col, nmem, nbuf } {
        optionsOff(O_ACTIVE | O_EDIT);
        justify(JUSTIFY_RIGHT);
    }

}
