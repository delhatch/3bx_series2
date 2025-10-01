/* -*- c++ -*- */
/*
 * Copyright 2025 DelHatch.
 */

#ifndef INCLUDED_CUSTOMMODULE_MIDEXPANSION_IMPL_H
#define INCLUDED_CUSTOMMODULE_MIDEXPANSION_IMPL_H

#include <gnuradio/customModule/midExpansion.h>

namespace gr {
namespace customModule {

class midExpansion_impl : public midExpansion
{
private:
    float _oldOutput;
    float _alpha;
    float _oneMAlpha;

public:
    midExpansion_impl();
    ~midExpansion_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace customModule
} // namespace gr

#endif /* INCLUDED_CUSTOMMODULE_MIDEXPANSION_IMPL_H */
