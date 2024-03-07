# Byte

Byte is a bytebeat synthesizer similar to ALGODRONE or ByteBeat-Xfade. You can provide 
up to 16 bytebeat expressions (defined as BYTEBEAT\_1 through BYTEBEAT\_16 in the code), and 
then select and play them.  Feel free to change them!  But they must be valid C/C++ 
expressions or they won't compile. You can change the pitch and tuning, as well as the 
volume.  Additionally you can reset the bytebeat algorithm at any time by sending a
trigger or gate to DIGITAL OUT.  

## Building ByteBeats

A bytebeat is any C expression using the 32-bit variable 't'.  It's called 16384 times a second
and the result, mod 255, is output as an audio sample.

Byte has a special feature: a special variable called x which ranges from 0 to 255.  It's
set by CV coming in to IN 3, and is by default 0.  You could stick x in your expressions
to control them in real-time via CV.

Byte is fragile: if in your expression you modify Byte's internal variables (like triggered or pitchCV
or even t!) you can seriously break Byte.  So don't do that.

There are many bytebeat expressions available online.  For a small collection, see
[Viznut's archive](http://viznut.fi/demos/unix/bytebeat_formulas.txt).  Note that some of these
are designed for JavaScript or have Sine or Pow in them: they are unlikely to work.

You can test bytebeats [on the following webpage](https://greggman.com/downloads/examples/html5bytebeat/html5bytebeat.html)
before adding them to GRAINS.  You can change the Hz values on that page as well.  GRAINS's
default HZ is 16384, though you can turn it down to half of that or less.

## Tuning

You can't really tune BYTE: to do so would require changing the sampling rate, and Mozzi doesn't
make it easy to do that without modifying Mozzi's config in its library.  Maybe later.

On Pot 1 I have set things up so you can change the pitch to some degree.  There are 16
settings.  Setting 8 (typically a bit to the left of due center) is the default setting.  
Since Mozzi's sampling rate is 16384 Hz, this is about one octave above and twice as fast
as the traditional pitch, which normally corresponds to 8000 Hz.  If you turn the knob 
to the LEFT of this, Byte will get slower and lower in pitch. It does this by just playing 
fewer times when Mozzi asks it to. It's pretty crude but it works.  If you turn the knob to 
the RIGHT, Byte will get higher in pitch but also change how it plays.  This is because it 
is incrementing the "t" variable by more and more (by default it's incremented by 1).



## Configuration

#### IN 1
Pitch CV
#### IN 2
Bytebeat CV
#### IN 3
Auxilliary Variable 1 CV  (x)
#### AUDIO IN (A)
[UNUSED]
#### AUDIO OUT
Out
#### DIGITAL OUT (D) 
Reset
#### POT 1
Pitch Scaling       

[Set the switch to MAN probably]
#### POT 2
Bytebeat

[Set the switch to MAN probably]
#### POT 3
Volume

