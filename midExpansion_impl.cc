/* -*- c++ -*- */
/*
 * Copyright 2025 DelHatch.
 *
 * This module runs a single-pole, low-pass filter with a tau of 1 second.
 * But it does this with some additional tricks:
 *    The output will track the input, if the input is bigger than the output.
 *        The output will fall with time constant tau, but rise with the signal.
 *    But it only rises to 75% of the input.
 * This module then outputs the *smaller* of the input signal, or the output of
 *    of the 1-second time-constant filter (implemented in this module).
 * The creates an RMS output (VCA control signal) that falls quickly with an initial
 *    time constant (straight slope) set by the capacitor & current drain in the
 *    RMS module, but then falls slowly based on the tau set by this filter.
 */

#include "midExpansion_impl.h"
#include <gnuradio/io_signature.h>

// How far does the RMS output drop quickly, versus using the 1 second lp filter?
#define dropAmount 0.8

namespace gr {
namespace customModule {

using input_type = float;
using output_type = float;

midExpansion::sptr midExpansion::make()
{
    return gnuradio::make_block_sptr<midExpansion_impl>();
}


/*
 * The private constructor
 */
midExpansion_impl::midExpansion_impl()
    : gr::sync_block("midExpansion",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
    _alpha = exp( -1.0/( 2100.0 * 1.0 ) ); // Fs = 1050. tau = 1 second.
    _oneMAlpha = 1 - _alpha;     // Pre-calculate 1 minus alpha.
    _oldOutput = 0;
}

/*
 * Our virtual destructor.
 */
midExpansion_impl::~midExpansion_impl() {}

int midExpansion_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    auto in0 = static_cast<const input_type*>(input_items[0]);
    auto out0 = static_cast<output_type*>(output_items[0]);

    float filterOutput = 0;
    float input0 = 0;

    for( int n=0; n<noutput_items; n++ ) {
        input0 = in0[n] + 0.3;
        if( (input0 * dropAmount) >= _oldOutput ) {
            // If the input * 0.75 is greater than the filter's output, set the
            //    filter output to equal that. Keep filter "peaked up" to 75%
            //    of the large signal input.
            _oldOutput = input0 * dropAmount;
            filterOutput = _oldOutput;
            //out0[n] = _oldOutput;
        }
        else {
            // Input signal is small, so run the filter
            //printf("F");
            filterOutput = (_alpha * _oldOutput) + (_oneMAlpha * (input0 * dropAmount));
            _oldOutput = filterOutput;
            //out0[n] = output;
        }
        // Now output the bigger of the two signals. This means the slow tau filter's
        //      output will used once the fast RC decay in the RMS module has had a
        //      bit of time to rapidly drop the VCA gain. Then a slow decay of the
        //      VCA gain.
        if( input0 > filterOutput ) {
            out0[n] = input0 - 0.3;
        }
        else {
            out0[n] = filterOutput - 0.3;
        }
    }
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace customModule */
} /* namespace gr */
