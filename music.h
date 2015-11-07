#ifndef MUSIC_H
#define MUSIC_H

#include <math.h>

#define Cb  -1
#define C   0
#define C#  1
#define Cx  2
#define Db  1
#define D   2
#define D#  3
#define Ebb 2
#define Eb  3
#define E   4
#define Fb  4
#define F   5
#define F#  6
#define Fx  7
#define Gb  6
#define G   7
#define G#  8
#define Ab  8
#define A   9
#define A#  10
#define Bbb 9
#define Bb  10
#define B   11
#define B#  12

#define NOTE(midi) (midi % 12)
#define OCTAVE(midi) ((int)midi/12 - 1)
#define MIDI(note, octave) ((octave+1)*12+note)

#define KEY_Bbb -9
#define KEY_Fb  -8
#define KEY_Cb  -7
#define KEY_Gb  -6
#define KEY_Db  -5
#define KEY_Ab  -4
#define KEY_Eb  -3
#define KEY_Bb  -2
#define KEY_F   -1
#define KEY_C    0
#define KEY_G    1
#define KEY_D    2
#define KEY_A    3
#define KEY_E    4
#define KEY_B    5
#define KEY_F#   6
#define KEY_C#   7
#define KEY_G#   8
#define KEY_D#   9
#define KEY_A#  10
#define KEY_E#  11
#define KEY_B#  12

#define MODE_MAJOR       0
#define MODE_DORIAN     -2
#define MODE_PHRYGIAN   -4
#define MODE_LYDIAN      1
#define MODE_MIXOLYDIAN -1
#define MODE_MINOR      -3
#define MODE_LOCRIAN    -5

#define KEYSIG(tonic, mode) (tonic+mode)
#define CF_SEMITONES(cf) (7*cf % 12)

#define CF_D1 -7
#define CF_P1  0
#define CF_A1  7
#define CF_m2 -5
#define CF_M2  2
#define CF_m3 -3
#define CF_M3  4
#define CF_D4 -8
#define CF_P4 -1
#define CF_A4  6
#define CF_D5 -6
#define CF_P5  1
#define CF_m6 -4
#define CF_M6  3
#define CF_m7 -2
#define CF_M7  5

float pitch (int m, float bend = 0.0, float tuning = 440.0)
{
  return tuning*pow(2.0, (m + bend/100.0 - MIDI(A, 4))/12.0);
}

#endif // MUSIC_H
