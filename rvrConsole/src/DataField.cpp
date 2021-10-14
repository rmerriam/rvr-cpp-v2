#include "DataField.h"
namespace scr {

    DataField::DataField(uint8_t const height, uint8_t const width, uint8_t const row, uint8_t const col, uint8_t const nmem, uint8_t const nbuf) :
        NField { height, width, row, col, nmem, nbuf } {
        optionsOff(O_ACTIVE | O_EDIT);
        justify(JUSTIFY_RIGHT);
    }

    //--------------------------------------------------------------------------------------------------------------------------
    void DataField::setData(bool const& data) {
        string txt { data ? "true" : "false" };
        write(txt);
    }

    //--------------------------------------------------------------------------------------------------------------------------
    void DataField::setData(RvrMsgView const& data) {
        string s { data.begin(), data.end() };
        write(s);
    }

    //--------------------------------------------------------------------------------------------------------------------------
    void DataField::setData(string const& data) {
        write(data);
    }
}
