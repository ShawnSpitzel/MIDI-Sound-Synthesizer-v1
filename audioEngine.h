#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include "noiseMaker.h"
#include <unordered_map>
NoiseMaker<int>* sound = nullptr;
std::unordered_map<WPARAM, double> activeNotes = {};
double SineSoundMaker(double freq, double dTime) {
    return sin(freq * 2.0 * 3.14159 * dTime);
}
double SquareSoundMaker(double freq, double dTime) {
    double amplitude = 1.0;
    double squareness = 3.0;
    return ((amplitude) / (pow(abs(squareness), (sin((squareness * freq * 3.14159 * dTime) - 0.0)))) + 1.0) - (amplitude / 2.0);
}
double MakeSineNoise(double dTime)
{
    double dOutput = 0.0;
    for (const auto& key : activeNotes)
    {
        double freq = key.second;
        dOutput += SineSoundMaker(freq, dTime);
    }
    return dOutput * 0.5; // Master Volume  
}
double MakeSquareNoise(double dTime)
{

    double dOutput = 0.0;
    for (const auto& key : activeNotes)
    {
        double freq = key.second;
        dOutput += SquareSoundMaker(freq, dTime);
    }
    return dOutput * 0.5; // Master Volume
}
void handleNote(WPARAM wParam) {
    double noteFreq = 0.0;
    switch (wParam)
    {
    case 0x51: // C5
        noteFreq = 523.25;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x57: // D5
        noteFreq = 587.33;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x45: // E5
        noteFreq = 659.25;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x52: // F5
        noteFreq = 698.46;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x54: // G5
        noteFreq = 783.99;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x59: // A5
        noteFreq = 880.00;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x55: // B5
        noteFreq = 987.77;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x49: // C6
        noteFreq = 1046.50;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x4F: // D6
        noteFreq = 1174.66;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x50: // E6
        noteFreq = 1318.51;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x5A: // C4
        noteFreq = 261.626;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x58: // D4
        noteFreq = 293.665;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x43: // E4
        noteFreq = 329.628;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x56: // F4
        noteFreq = 349.228;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x42: // G4
        noteFreq = 392.000;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x4E: // A4
        noteFreq = 440.000;
        activeNotes[wParam] = noteFreq;
        break;
    case 0x4D: // B4
        noteFreq = 493.883;
        activeNotes[wParam] = noteFreq;
        break;
    }

}
std::vector<std::wstring> devices = NoiseMaker<int>::GetDevices();
NoiseMaker<int> sound(devices[0]);
sound.SetUserFunction(MakeSineNoise);