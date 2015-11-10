#################Cuckoo###################
Gustavo Marques Netto
Lucas Salla Pagnan
##########################################

Sounds:

The sounds were written in the program memory
given the size needed. They were converted to
8khz@8-bits.

Timers:

We used the three timers available in the UNO
board. The first one is responsible to "play"
the audio, every 1/8000s the interrupt routine 
will be called and change the duty-cycle of 
the PWM based on the current sample.

The second timer calls its interrupt routine
each second, effectively being the "clock".

The third timer sets the PWM output of the
speaker pin, this is what makes the sounds work.
By changing its duty-cycle, the average value of
the output changes, if the PWM is fast enough it is
able to produce the sound. This is an 8-bit timer,
the samples are also 8 bit, so no conversion is
needed at the program. The hardware compares the
value in the OCR2A register, if the counter is
lower than that, the output will be high, if not,
the output will be low.


Interrupts:

The first interrupt plays the byte of sound
selected before after 1/8000s. The second 
interrupt selects the according sound to be 
played.


Setting up the timer:

When the device is turned on it waits for the user
to input the initial time via the Serial port. After
this, all timers and interrupts are configured.


Tests:

The timer was tested and it worked as intended, there is
just one small inherent error, that arises from the clock
source. In our tests, in 20 hours, it had deviated 15 seconds.