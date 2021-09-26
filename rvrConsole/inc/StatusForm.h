/*
 * GeneralStatusForm.h
 *
 *  Created on: Apr 2, 2017
 *      Author: rmerriam
 */

#ifndef GeneralStatusForm_H_
#define GeneralStatusForm_H_

#include <iostream>
using namespace std;

#include "NField.h"
#include "NForm.h"
using namespace scr;

#include "FormBase.h"
//--------------------------------------------------------------------------------------------------------------------------
class StatusForm : public FormBase {
public:
    StatusForm(int const y, int const x);

    virtual void updateScreen() override;

private:
    void wall_time();

    DataFieldPtr mWallTime;
    DataFieldPtr mSerialName;
    DataFieldPtr mVersion;

    DataFieldPtr mMouse;

    LabelFieldPtr mKeys;

};
#endif /* GeneralStatusForm_H_ */
