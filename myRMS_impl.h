/* -*- c++ -*- */
/*
 * Copyright 2025 DelHatch.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_CUSTOMMODULE_MYRMS_IMPL_H
#define INCLUDED_CUSTOMMODULE_MYRMS_IMPL_H

#include <gnuradio/customModule/myRMS.h>

namespace gr {
namespace customModule {

class myRMS_impl : public myRMS
{
private:
    float _Rbase;
    float _Cbase;
    float _samp_rate;
    float _Idrain;
    float _Cdyn;
    float _k;
    float _oldRMS;
    float _zeroDbV;

public:
    myRMS_impl(float Rbase, float Cbase, float samp_rate, float Idrain, float Cdyn, float k, float zeroDbV);
    ~myRMS_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace customModule
} // namespace gr

#endif /* INCLUDED_CUSTOMMODULE_MYRMS_IMPL_H */
