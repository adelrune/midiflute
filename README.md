# Midiflute

The goal of this project was to make a midi flute-like instruments and to have the means to define
custom fingerings via easily readable fingering files.

This is the complete code needed to create a midi flute-like instrument.

Included features are:
- Midi note on and note off with ajustable (in the code) thresholds.
- Midi breath control signal.
- Fingerings that are editable with "simple" fingering files via a c code generator written in python.

Notably missing:

- Schematics and part list
- Any form of patch changing
- build script for the instrument

Here is a demo of the thing plugged into a waldorf blofeld with the breath control set to control filter cutoff : https://www.youtube.com/watch?v=eW2wzeu2eAs&feature=youtu.be
