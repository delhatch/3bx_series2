<h1>Description</h1>
<p>This project creates a 3-band audio expander, implemented in GNU Radio Companion.</p>
<p>The goal is to run it in a Raspberry Pi 5, with a HiFiBerry DAC+ ADC for audio input and output.</p>
<h2>Processing</h2>
<p>First the incoming audio is split into 3 audio bands, low- mid- and high-frequency bands.</p>
<p>The audio goes thourgh a VCA, and then is summed to create the audio output.</p>
<p>The VCA is controlled by the output of an RMS detector.</p>
