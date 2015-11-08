#################Cuckoo###################

Timers:

We used the three times available in this 
project. The first one is responsible to 
sample the audio, every 1/8000 s a 
interruption will play any sound that was 
selected. The second does the same function 
but every second. The third timer /*still 
have to understand*/.

Interrupts:

The first interrupt plays the byte of sound
selected before after 1/8000s. The second 
interrupt selects the according sound to be 
played.

Control of timer:

The time can be set using the serial port. 
So using a computer a initial time can be
determined. 