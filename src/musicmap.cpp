/** -------- **/

#include "musicmap.h"
#include "chords.h"
#include "samples.h"

slScalar MajorScaleTuning[8] = {1., 9./8., 5./4., 4./3., 3./2., 5./3., 15./8.};
slScalar Aug = 25./24.;
slScalar Dim = 24./25.;
int KeyNote = 60;

int OCTAVES = 2;
int BEATS_PER_MEASURE = 8;
int BEATS_PER_MINIMEASURE = 2;
int BEAT_LENGTH = _QUARTER;
slScalar DEFAULT_NOTE_LENGTH = 2;
slScalar NoteLength = DEFAULT_NOTE_LENGTH;
slScalar CHANNEL_HEIGHT = ROLL_HEIGHT * (1. / CHANNELS);
slScalar BEAT_WIDTH = ROLL_WIDTH * (1. / BEATS_PER_MEASURE);
slScalar ROLL_TOP = ((1 - ROLL_HEIGHT) / 2.) - 0.15;
slScalar SongPosition = 0;
slBU MeasureCount = 0;
slBU MelodyInstrument = 0;
slScalar MasterVolume = 0.2;

char* GetNoteName (int midival, int accmode)
{
    midival = NOTE(midival);
    switch (midival)
    {
        case 0: return "C";
        case 2: return "D";
        case 4: return "E";
        case 5: return "F";
        case 7: return "G";
        case 9: return "A";
        case 11: return "B";
        case 1: return "C#";
        case 3: return "Eb";
        case 6: return "F#";
        case 8: return "Ab";
        case 10: return "Bb";
        default: return "";
    }
}

void SetMasterVolume (slScalar to)
{
    MasterVolume = to;
}

int GetKeyNote ()
{
    return KeyNote;
}

void SetKeyNote (int to)
{
    KeyNote = to;
}

int GetBeatsPerMeasure ()
{
    return BEATS_PER_MEASURE;
}

slScalar GetDefaultNoteLength ()
{
    return DEFAULT_NOTE_LENGTH;
}

slScalar GetNoteLength ()
{
    return NoteLength;
}

slScalar GetBeatWidth ()
{
    return BEAT_WIDTH;
}

slScalar GetRollOffset ()
{
    return SongPosition * BEAT_WIDTH;
}

slScalar GetRollLeft ()
{
    return ((1 - ROLL_WIDTH) / 2.) - GetRollOffset();
}

slScalar GetNoteLengthByKey (Uint32 key)
{
    switch (key)
    {
        // short notes
        case SDLK_f: return 1; // eighth
        case SDLK_g: return 0.5; // sixteenth
        case SDLK_h: return 0.25; // 32nd
                     // long notes
        case SDLK_d: return BEATS_PER_MINIMEASURE; // (dotted) quarter
        case SDLK_s: return 2*BEATS_PER_MINIMEASURE; // (dotted) half
        case SDLK_a: return 4*BEATS_PER_MINIMEASURE; // whole or 2 dotted halves
        default: return 1;
    }
}

void NoteLengthKeyBind (slKeyBind* kb)
{
    NoteLength = GetNoteLengthByKey(kb->key);
}
struct NoteLengthBind
{
    slKeyBind* keybind;
    slScalar length;
};
NoteLengthBind** NoteLengthBinds = NULL;
slBU NoteLengthBindCount = 0;
void OnNoteLengthBindPress (slKeyBind* keybind)
{
    for (slBU cur = 0; cur < NoteLengthBindCount; cur++)
    {
        NoteLengthBind* item = *(NoteLengthBinds + cur);
        if (item->keybind == keybind)
        {
            NoteLength = item->length;
            return;
        };
    };
};
void CreateNoteLengthBind (char* name, Uint32 dflt_key, slScalar length)
{
    NoteLengthBind* out = malloc(sizeof(NoteLengthBind));
    out->keybind = slGetKeyBind(name,dflt_key);
    out->keybind->onpress = OnNoteLengthBindPress;
    out->length = length;
    slAddItemToList(&NoteLengthBinds,&NoteLengthBindCount,out);
};

void DrawGrid (SDL_Window* window = NULL, SDL_Renderer* renderer = NULL)
{
    slScalar roll_left = GetRollLeft();
    slScalar roll_right = roll_left + (MeasureCount * BEATS_PER_MEASURE * BEAT_WIDTH);
    slScalar roll_bottom = ROLL_TOP + (CHANNELS * CHANNEL_HEIGHT);
    slBU beatcount = MeasureCount * BEATS_PER_MEASURE;
    for (slBU beat = 0; beat <= beatcount; beat++)
    {
        slScalar x = roll_left + (beat * BEAT_WIDTH);
        if (beat % BEATS_PER_MINIMEASURE) slSetDrawColor(MINIMEASURE_BAR_COLOR);
        else if (beat % BEATS_PER_MEASURE) slSetDrawColor(MEASURE_BAR_COLOR);
        else slSetDrawColor(GRID_COLOR);
        slDrawScreenLine(x,ROLL_TOP,x,roll_bottom);
    }
    slSetDrawColor(GRID_COLOR);
    for (slBU chan = 0; chan <= CHANNELS; chan++)
    {
        slScalar y = ROLL_TOP + (chan * CHANNEL_HEIGHT);
        slDrawScreenLine(roll_left,y,roll_right,y);
    }
    slSetDrawColor(MARKER_COLOR);
    slScalar marker_x = roll_left + SongPosition * BEAT_WIDTH;
    slDrawScreenLine(marker_x,ROLL_TOP,marker_x,roll_bottom);
};

void DrawBackground (SDL_Window* window, SDL_Renderer* renderer)
{
    slSetDrawColor(BACKGROUND_COLOR);
    SDL_RenderFillRect(renderer,NULL);
}

Note** Notes = NULL;
slBU NoteCount = 0;

SDL_Color GetNoteColor (int relative)
{
    switch (relative)
    {
        case 0: return RED;
        case 1: return ORANGE;
        case 2: return YELLOW;
        case 3: return GREEN;
        case 4: return BLUE;
        case 5: return PURPLE;
        case 6: return MAGENTA;
        default: return WHITE;
    }
}

Note* SpawnNote ()
{
    Note* out = (Note *) malloc(sizeof(Note));
    out->box = slCreateBox();
    out->box->h = CHANNEL_HEIGHT;
    out->box->bordercolor = BLACK;
    out->box->backcolor = WHITE;
    out->box->z = 240;
    out->instrument = MelodyInstrument;
    out->volume = 1;
    out->type = MELODY_NOTE;
    out->instrument_box = slCreateBox(GetInstrumentIcon(out->instrument));
    slSetBoxDims(out->instrument_box,0.7,0.1,0.28,0.8,239);
    slRelBoxDims(out->instrument_box,out->box);
    slAddItemToList((void ***)&Notes, (slBU *)&NoteCount, (void *)out);
    return out;
}

Note* SpawnHiddenNote ()
{
    Note* out = (Note *) malloc(sizeof(Note));
    out->box = NULL;
    out->volume = 1;
    slAddItemToList((void ***)&Notes, (slBU *)&NoteCount, (void *)out);
    return out;
}

void RepositionNotes ()
{
    slScalar roll_left = GetRollLeft();
    for (slBU cur = 0; cur < NoteCount; cur++)
    {
        Note* note = *(Notes + cur);
        if (note->box)
        {
            note->box->w = BEAT_WIDTH * note->duration;
            note->box->x = roll_left + (note->start * BEAT_WIDTH);
            note->box->y = ROLL_TOP + (ROLL_HEIGHT - ((note->channel + 1) * CHANNEL_HEIGHT));
        }
    }
}
void RecalculateNotePitch (Note* note, slBS scale = MAJOR)
{
    if (note->box)
        note->box->backcolor = GetNoteColor((note->channel) % 7);
    note->pitch = GetPitch(KeyNote)*MajorScaleTuning[(note->channel + scale) % 7]*pow(2., ((int)note->channel/7));
    note->pitch *= pow(Aug, note->accidental);
}
void NewNoteAtClickPoint ()
{
    slScalar mousex, mousey;
    slGetMouse(&mousex, &mousey);
    // If it's out of bounds, don't even bother.
    slScalar roll_left = GetRollLeft();
    if (mousex < roll_left || mousex > roll_left + (((MeasureCount * BEATS_PER_MEASURE) - (DEFAULT_NOTE_LENGTH - 1)) * BEAT_WIDTH)) return;
    else if (mousey < ROLL_TOP || mousey > ROLL_TOP + (CHANNELS * CHANNEL_HEIGHT)) NewChordAtClickPoint();
    // It's not out of bounds, so figure out where it goes.
    else
    {
        slBU channel = (ROLL_HEIGHT - (mousey - ROLL_TOP)) / CHANNEL_HEIGHT;
        slBU start = (mousex - roll_left) / BEAT_WIDTH;
        Note* note = SpawnNote();
        note->start = start;
        note->channel = channel;
        note->accidental = 0;
        note->duration = NoteLength;
        RecalculateNotePitch(note);
    }
}
void DespawnNote (Note* todespawn)
{
    slRemoveItemFromList((void ***)&Notes,&NoteCount,todespawn);
    slDestroyBox(todespawn->box);
    free(todespawn);
}
Note* GrabbedNote = NULL;
//slScalar DragOffset;
void GrabNote ()
{
    slScalar mousex,mousey;
    slGetMouse(&mousex,&mousey);
    for (slBU cur = 0; cur < NoteCount; cur++)
    {
        Note* note = *(Notes + cur);
        if (note->box)
        {
            if (slPointOnBox(note->box,mousex,mousey))
            {
                GrabbedNote = note;
                //DragOffset = mousex - note->box->x;
                return;
            }
        }
    }
    // Will only be reached if no note is there to be dragged.
    NewNoteAtClickPoint();
}
void ReleaseNote ()
{
    if (GrabbedNote)
    {
        slScalar mousex,mousey;
        slGetMouse(&mousex,&mousey);
        slScalar channel = (ROLL_HEIGHT - (mousey - ROLL_TOP)) / CHANNEL_HEIGHT;
        slScalar beat = (((mousex - GrabbedNote->box->w / 2) - GetRollLeft()) / ROLL_WIDTH) * BEATS_PER_MEASURE;
        beat = slRound(beat * 2) / 2.f;
        if (channel < 0 || beat < 0 || channel >= CHANNELS || beat >= BEATS_PER_MEASURE * MeasureCount)
        {
            // Remove the note. It's been dragged into the margins.
            DespawnNote(GrabbedNote);
        }
        else
        {
            // The note has been dragged into a valid grid space.
            GrabbedNote->channel = channel;
            GrabbedNote->start = beat;
            RecalculateNotePitch(GrabbedNote);
        }
        // Either remove the note or
        // set its start and channel.
        GrabbedNote = NULL;
    }
}
void UpdateGrabbedNote ()
{
    if (GrabbedNote)
    {
        slGetMouse(&(GrabbedNote->box->x),&(GrabbedNote->box->y));
        GrabbedNote->box->x -= GrabbedNote->box->w / 2;
        GrabbedNote->box->y -= GrabbedNote->box->h / 2;
    }
}
slBU GetMeasureCount ()
{
    return MeasureCount;
}
void InsertMeasure (slBU where)
{
    if (where < MeasureCount)
    {
        // We need to reposition the notes after the insertion point.
        for (slBU cur = 0; cur < NoteCount; cur++)
        {
            Note* item = *(Notes + cur);
            if (item->start > where * BEATS_PER_MEASURE) item->start += BEATS_PER_MEASURE;
        }
    }
    MeasureCount++;
}
void RemoveMeasure (slBU where)
{
    if (MeasureCount)
    {
        for (slBU cur = 0; cur < NoteCount; cur++)
        {
            Note* item = *(Notes + cur);
            if (item->start / BEATS_PER_MEASURE == where)
            {
                // Clear out the notes that were inside this measure.
                DespawnNote(item);
                cur--; // The list has shrunk due to despawning this item.
            }
            else if (item->start > where * BEATS_PER_MEASURE)\
            {
                // Reposition notes after this measure.
                item->start -= BEATS_PER_MEASURE;
            }
        }
        MeasureCount--;
    }
}
float BeatsPerMinute = 120;
slScalar GetSongPosition ()
{
    return SongPosition;
}
void SetSongPosition (slScalar to)
{
    SongPosition = to;
}
slScalar GetSongLength ()
{
    return MeasureCount * BEATS_PER_MEASURE;
}
slScalar GetPitch (int midi_value)
{
    return 440 * powf(2,(midi_value - 69.) / 12);
}
#define DEFAULT_NOTE_VOLUME 0.2
float GetMixerSample (slScalar persample)
{
    slScalar sample = 0;
    //sample += sin((SongPosition * (TEST_HERTZ / (BeatsPerMinute / 60))) * M_PI) * 0.5;
    for (slBU cur = 0; cur < NoteCount; cur++)
    {
        Note* note = *(Notes + cur);
        if (note->start <= SongPosition && note->start + note->duration >= SongPosition)
        {
            float pitch = note->pitch;
            sample += GetInstrumentSample(note->instrument, pitch,
                    SongPosition - note->start) * MasterVolume * note->volume;
        }
    }
    // Advance the song.
    SongPosition += persample * (BeatsPerMinute / 60) * (BEATS_PER_MINIMEASURE);
    return sample;
}

slScalar SongProgress ()
{
    return SongPosition / (MeasureCount * BEATS_PER_MEASURE);
};

void SetSongProgress (slScalar to)
{
    SongPosition = to * (MeasureCount * BEATS_PER_MEASURE);
};
