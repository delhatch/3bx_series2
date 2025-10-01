#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2025 DelHatch.
#
import numpy as np
from gnuradio import gr

MAXGAIN = 50.0

class PyRMS(gr.sync_block):
    """
    docstring for block PyRMS
    """
    def __init__(self, Rbase=50.0,Cbase=3.6,samp_rate=44100.0,Idrain=7.0,Cdyn=1.0,k=1.0,zeroDbV=1.0):
        gr.sync_block.__init__(self,
            name="PyRMS",
            in_sig=[np.float32, np.float32],
            out_sig=[np.float32])
        self.Rbase = Rbase
        self.Cbase = Cbase * 0.000001
        self.samp_rate = samp_rate
        self.Idrain = Idrain * 0.000001
        self.Cdyn = Cdyn * 0.000001
        self.k = k;
        self.zeroDbV = zeroDbV
        self.oldRMS = 0.0
#        print("I")
#        print(self.zeroDbV)

    def set_zeroDbV( self, zeroDbV ):
        self.zeroDbV = zeroDbV
#        print("C")
#        print(self.zeroDbV)

    def work(self, input_items, output_items):
        in0 = input_items[0]
        in1 = input_items[1]
        out = output_items[0]

        deltaV = 0.0
        deltaRMS = 0.0
        denom2 = self.Cbase * self.samp_rate
        denom1 = denom2 * self.Rbase
#        print("W")
#        print(self.zeroDbV)
#        print(in0[0])
        # Full-wave rectify, then average each audio signal
        in0 = ( np.abs(in0) + np.abs(in1) ) / 2.0
#        print(in0[0])
        # Calc the RMS power, given that 0 dB = the level of zeroDbV
        in0 = 20.0 * np.log10( in0 / self.zeroDbV )
        # If signal too large, clip to 0.0. If too small, set to -50 dB
        in0 = np.clip(in0, -MAXGAIN, 0.0)
        # Convert to a voltage between 0.0 and 0.6
        in0 = ( (0.6/MAXGAIN) * in0 ) + 0.6
        #print(in0)

        outArray = np.empty_like(in0)
        for i, x in enumerate(in0):
            deltaRMS = x - self.oldRMS
            if( deltaRMS >= 0.0 ):
                deltaV = deltaRMS / denom1
                self.oldRMS = deltaV + self.oldRMS
            else:
                deltaV = self.Idrain / denom2
                self.oldRMS = self.oldRMS - deltaV
            outArray[i] = self.oldRMS - 0.3

        output_items[0][:] = outArray
        return len(output_items[0])
