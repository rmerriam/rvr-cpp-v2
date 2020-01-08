/**
 * Copyright 2019 Bradley J. Snyder <snyder.bradleyj@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <iostream>
#include <cmath>
#include "PID.h"

using namespace std;

PID::PID(double dt, double max_out, double min_out, double Kp, double Kd, double Ki) :
    mDt(dt), mMaxOut(max_out), mMinOut(min_out), mKp(Kp), mKd(Kd), mKi(Ki), mPreviousErr(0), mIntegral(0) {
}

double PID::operator()(double setpoint, double pv) {

    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double proportional = mKp * error;

    // Integral term
    mIntegral += error * mDt;
    double integral = mKi * mIntegral;

    // Derivative term
    double d_error = (error - mPreviousErr) / mDt;
    double derivative = mKd * d_error;

    // Calculate total output
    double output = proportional + integral + derivative;

    // Restrict to max/min
    if (output > mMaxOut) {
        output = mMaxOut;
    }
    else if (output < mMinOut) {
        output = mMinOut;
    }

    // Save error to previous error
    mPreviousErr = error;

    return output;
}

