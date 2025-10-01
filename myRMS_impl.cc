/* -*- c++ -*- */
/*
 * Copyright 2025 DelHatch.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "myRMS_impl.h"
#include <gnuradio/io_signature.h>
#include "math.h"

#define maxGain 50.0
//#define zeroDbVoltage 5.0

namespace gr {
namespace customModule {

using input_type = float;
using output_type = float;

myRMS::sptr myRMS::make(float Rbase, float Cbase, float samp_rate, float Idrain, float Cdyn, float k, float zeroDbV)
{
    return gnuradio::make_block_sptr<myRMS_impl>(Rbase, Cbase, samp_rate, Idrain, Cdyn, k, zeroDbV);
}

/* The private constructor */
myRMS_impl::myRMS_impl(float Rbase, float Cbase, float samp_rate, float Idrain, float Cdyn, float k, float zeroDbV)
    : gr::sync_block("myRMS",
                     gr::io_signature::make(
                         2 /* min inputs */, 2 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
{
  _Rbase = Rbase;  // Rbase provided in ohms
  _Cbase = Cbase * 0.000001;  // Cbase provided in uF
  _samp_rate = samp_rate;    // Sample rate in samples per second
  _Idrain = Idrain * 0.000001;  // I drain in uA
  _Cdyn = Cdyn * 0.000001;    // Cdyn in uF
  _k = k;
  _oldRMS = 0.0;
  _zeroDbV = zeroDbV;
}

/* Our virtual destructor. */
myRMS_impl::~myRMS_impl() {}

int myRMS_impl::work(int noutput_items,
                   gr_vector_const_void_star& input_items,
                   gr_vector_void_star& output_items)
{
    auto in0 = static_cast<const input_type*>(input_items[0]);
    auto in1 = static_cast<const input_type*>(input_items[1]);
    auto out0 = static_cast<output_type*>(output_items[0]);

    float RMSin = 0.0;
    float RMSin1 = 0.0;
    float deltaV = 0.0;
    float deltaRMS = 0.0;

    for( int n = 0; n < noutput_items; n++ ) {

      // First, calculate the instantaneous RMS value of the avg of the 2 inputs
      RMSin = abs( in0[n] );
      RMSin1 = abs( in1[n] );
      RMSin = (RMSin + RMSin1) / 2.0;
      // How many dB below 5 volts is this input? Use 5 Vp-p as 0 dB. (50mV = -100 dB)
      RMSin = 20.0 * log10( RMSin / _zeroDbV );
      if( RMSin < -maxGain ) RMSin = -maxGain;  // Cap low signals to -50 dBFS. Do nothing if that low.
      // Now map output from 0 volts RMS (for -50dB in) to 0.6 volt (for 0db in).
      //    This will be the voltage on the capacitor.
      // At the end of all this, we want the output to be between -0.3 and +0.3. This will
      //    drive the VCA to swing between cut 50dB to increase by 50 dB.
      //    Of course, this will be modified by the user's selection of the threshold and
      //       the amount of dynamic range expansion to apply.
      RMSin = ( (0.6/maxGain) * RMSin ) + 0.6;

      deltaRMS = RMSin - _oldRMS;
      if( deltaRMS >= 0 ) { // If true, use calcs for a Rising Edge.
          // Calculate the current the input resistor, which is I=deltaRMS/Rbase
          // The calculate the voltage change on the Cbase cap =(I*(1/samp_rate))/Cbase
          deltaV = deltaRMS / ( _Rbase * _Cbase * _samp_rate );
          // Now calculate the new voltage on the capacitor
          _oldRMS = deltaV + _oldRMS;
      }
      else {  // Falling edge. Simple = just discharge the capacitor with Idrain.
          deltaV = _Idrain / ( _Cbase * _samp_rate );
          _oldRMS = _oldRMS - deltaV;
      }
      out0[n] = _oldRMS - 0.3;  // All calc's previously mapped from 0 to +0.6. Now -0.3 to +0.3.
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace customModule */
} /* namespace gr */
