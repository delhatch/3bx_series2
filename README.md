<h1>Description</h1>
<p>This project creates a 3-band audio expander, implemented in GNU Radio Companion.</p>
<p>I run it on a Raspberry Pi 5, with a HiFiBerry DAC+ ADC for audio input and output, but should run on nearly every platform through the gnuradio-companion audio input and output module.</p>
<h2>Features</h2>
<ul>
  <li>Provides dynamic range expansion. This helps restore the musical impact that is reduced by the use of compression, which is used by many FM radio stations. It also adds some "life" to audio sources such a vinyl records, tapes, and even AAD CDs.</li>
  <li>Operates on three audio bands.
  <li>Adjustable "Expansion" level. Use a little expansion for CDs, or a lot for FM radio.</li>
  <li>Adjustable "Threshold" level. This compensates for audio at different levels.</li>
  <li>Meters to show how much expansion is being applied to each of the three audio bands.</li>
</ul>
<h2>Processing</h2>
<p>First the incoming audio is split into 3 audio bands, low- mid- and high-frequency bands.</p>
<p>The audio goes thourgh a VCA, and then is summed to create the audio output.</p>
<p>The VCA is controlled by the output of an RMS detector.</p>
