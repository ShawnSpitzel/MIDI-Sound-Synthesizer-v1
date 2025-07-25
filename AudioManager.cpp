#include "AudioManager.h"
#include <cmath>

AudioManager* AudioManager::s_instance = nullptr;

AudioManager::AudioManager()
{    s_instance = this;
}

AudioManager::~AudioManager()
{
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

bool AudioManager::Initialize()
{
    std::vector<std::wstring> devices = NoiseMaker<int>::GetDevices();
    if (devices.empty())
    {
        return false;
    }

    m_sound = std::make_unique<NoiseMaker<int>>(devices[0]);
    m_sound->SetUserFunction(AudioManager::StaticNoiseCallback);
    return true;
}

void AudioManager::Shutdown()
{
    m_sound.reset(); 
}

void AudioManager::HandleKeyDown(WPARAM wParam)
{
    if (m_activeNotes.find(wParam) == m_activeNotes.end())
    {
        MapNoteFrequency(wParam);
    }
}

void AudioManager::HandleKeyUp(WPARAM wParam)
{
    m_activeNotes.erase(wParam);
}

void AudioManager::SetWaveType(WaveType type)
{
    m_currentWaveType = type;
}

double AudioManager::StaticNoiseCallback(double dTime)
{
    if (s_instance)
    {
        switch (s_instance->m_currentWaveType)
        {
        case WaveType::Sine:
            return s_instance->MakeSineNoise(dTime);
        case WaveType::Square:
            return s_instance->MakeSquareNoise(dTime);
        default:
            return 0.0;
        }
    }
    return 0.0;
}

double AudioManager::SineSoundMaker(double freq, double dTime) const
{
    return sin(freq * 2.0 * 3.14159 * dTime);
}

double AudioManager::SquareSoundMaker(double freq, double dTime) const
{
    double amplitude = 1.0;
    double squareness = 3.0;
    if (std::abs(squareness) < 0.001) squareness = 0.001;
    return ((amplitude) / (std::pow(std::abs(squareness), (std::sin((squareness * freq * 3.14159 * dTime) - 0.0)))) + 1.0) - (amplitude / 2.0);
}

double AudioManager::MakeSineNoise(double dTime)
{
    double dOutput = 0.0;
    for (const auto& key : m_activeNotes)
    {
        double freq = key.second;
        dOutput += SineSoundMaker(freq, dTime);
    }
    return dOutput * 0.5;
}

double AudioManager::MakeSquareNoise(double dTime)
{
    double dOutput = 0.0;
    for (const auto& key : m_activeNotes)
    {
        double freq = key.second;
        dOutput += SquareSoundMaker(freq, dTime);
    }
    return dOutput * 0.5;
}

void AudioManager::MapNoteFrequency(WPARAM wParam)
{
    double noteFreq = 0.0;
    switch (wParam)
    {
    case 0x51: noteFreq = 523.25; break; // C5 (Q)
    case 0x57: noteFreq = 587.33; break; // D5 (W)
    case 0x45: noteFreq = 659.25; break; // E5 (E)
    case 0x52: noteFreq = 698.46; break; // F5 (R)
    case 0x54: noteFreq = 783.99; break; // G5 (T)
    case 0x59: noteFreq = 880.00; break; // A5 (Y)
    case 0x55: noteFreq = 987.77; break; // B5 (U)
    case 0x49: noteFreq = 1046.50; break; // C6 (I)
    case 0x4F: noteFreq = 1174.66; break; // D6 (O)
    case 0x50: noteFreq = 1318.51; break; // E6 (P)
    case 0x5A: noteFreq = 261.626; break; // C4 (Z)
    case 0x58: noteFreq = 293.665; break; // D4 (X)
    case 0x43: noteFreq = 329.628; break; // E4 (C)
    case 0x56: noteFreq = 349.228; break; // F4 (V)
    case 0x42: noteFreq = 392.000; break; // G4 (B)
    case 0x4E: noteFreq = 440.000; break; // A4 (N)
    case 0x4D: noteFreq = 493.883; break; // B4 (M)
    default: return;
    }
    m_activeNotes[wParam] = noteFreq;
}