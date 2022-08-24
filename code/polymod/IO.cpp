#include "IO.h"

IO::IO() {
  
}

float IO::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
        case MIDI_PITCH:
        returnVal = tempFreqs[polyChannel];
        break;

        case MIDI_GATE:
        returnVal = tempGates[polyChannel];
        break;

        case MAIN_INPUT_OUT:

        break;

    }
    return returnVal;
}

void IO::handleMidiMessage(MidiEvent m)
{
    switch (m.type)
    {
        case NoteOn:
        {
            if (m.data[1] > 0)
            {
                NoteOnEvent p = m.AsNoteOn();
                handleNoteOn(p.note);
            } else {
                NoteOffEvent p = m.AsNoteOff();
                handleNoteOff(p.note);
            }
        }
        break;
        case NoteOff:
        {
            NoteOffEvent p = m.AsNoteOff();
            handleNoteOff(p.note);
        }
        break;
        default:
        break;
    }
}

void IO::handleNoteOn(int noteNum) {
    // find oldest note slot
    int oldestNoteSlot = 0;
    int oldestNoteOffTime = midiNotes[0][NOTE_OFF_TIME];
    for(int i=0; i<MAX_POLYPHONY; i++) {
        if(midiNotes[i][NOTE_OFF_TIME] < oldestNoteOffTime) {
            oldestNoteOffTime = midiNotes[i][NOTE_OFF_TIME];
            oldestNoteSlot = i;
        }
    }
    midiNotes[oldestNoteSlot][NOTE_NUM] = noteNum;
    midiNotes[oldestNoteSlot][NOTE_ON_TIME] = System::GetNow();
    midiNotes[oldestNoteSlot][NOTE_OFF_TIME] = INT32_MAX;
    tempFreqs[oldestNoteSlot] = (noteNum - 49.0f) / 120.0f;
    tempGates[oldestNoteSlot] = 0.5;
}

void IO::handleNoteOff(int noteNum)
{
    // find matching notes
    for (int i = 0; i < MAX_POLYPHONY; i++)
    {
        if (midiNotes[i][NOTE_NUM] == noteNum)
        {
            midiNotes[i][NOTE_NUM] = noteNum;
            midiNotes[i][NOTE_OFF_TIME] = System::GetNow();
            tempGates[i] = 0.0;
        }
    }
}