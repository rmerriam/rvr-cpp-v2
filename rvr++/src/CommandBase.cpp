#include "CommandBase.h"
namespace rvr {

    //----------------------------------------------------------------------------------------------------------------------
    CommandBase::CommandBase(Devices const device, Request& request, TargetPort const target) :
        mDevice { device }, mRequest { request }, mTarget { target }, mAltTarget { makeAltProc() } {
    }

}
