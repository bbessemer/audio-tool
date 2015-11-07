#ifndef MUSIC_H
#define MUSIC_H

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

float pitch (int m, float bend = 0.0, float tuning = 440.0)
{
  return tuning*pow(2.0, (m + bend/100 - MIDI(A, 4))/12.0);
}

#endif // MUSIC_H
