#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2025 DelHatch.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#


import numpy as np
from gnuradio import gr

class PyVCA(gr.sync_block):
    """
    docstring for block PyVCA
    """
    def __init__(self, expansion=1.0):
        gr.sync_block.__init__(self,
            name="PyVCA",
            in_sig=[ np.float32, np.float32 ],
            out_sig=[ np.float32 ])
        self.expansion = expansion

    def set_expansion(self, expansion):
        self.expansion = expansion

    def work(self, input_items, output_items):
        in0 = input_items[0]
        gain = input_items[1]
#        print(type(gain))
#        out0 = output_items[0]
        gain = np.clip(gain, -0.3, 0.3)
        db = ( (self.expansion / 0.3) * gain) / 20.0
        output_items[0][:] = in0 * (10 ** db)
        return len(output_items[0])
