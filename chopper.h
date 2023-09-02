#pragma once
#ifndef CHOPPER_H
#define CHOPPER_H
#include <stdint.h>
#include "Utility/dsp.h"
#ifdef __cplusplus

namespace bytebeat
{

#define PATTERN_STEPS_MAX 4
#define PATTERNS_MAX 8

class Chopper
{
  private:
    static uint8_t Patterns[PATTERNS_MAX][PATTERN_STEPS_MAX];

    void IncPatternStep(void);

  public:
    enum Mode
    {
        Quarter,
        Eight,
        Sixteen
    };

    Chopper() {}
    ~Chopper() {}

    /** Initializes the Chopper

        \param sample_rate - sample rate of the audio engine being run, and the frequency that the Process function will be called.

        Defaults:
        - freq_ = 100 Hz
        - amp_ = 0.5
    */
    void Init(float sample_rate);

    /** Changes the frequency of the Chopper, and recalculates phase increment.
    */
    inline void SetFreq(const float f)
    {
        freq_      = f;
        phase_inc_ = CalcPhaseInc(f);
    }


    /** Sets the amplitude of the waveform.
    */
    inline void SetAmp(const float a) { amp_ = a; }

    /** Sets the pulse width for WAVE_SQUARE and WAVE_POLYBLEP_SQUARE (range 0 - 1)
     */
    inline void SetPw(const float pw)
    {
        pw_     = daisysp::fclamp(pw, 0.0f, 1.0f);
        pw_rad_ = pw_ * TWOPI_F;
    }

    /** Returns true if cycle is at end of rise. Set during call to Process.
    */
    inline bool IsEOR() { return eor_; }

    /** Returns true if cycle is at end of cycle. Set during call to Process.
    */
    inline bool IsEOC() { return eoc_; }

    /** Returns true if cycle rising.
    */
    inline bool IsRising() { return phase_ < PI_F; }

    /** Returns true if cycle falling.
    */
    inline bool IsFalling() { return phase_ >= PI_F; }

    /** Processes the waveform to be generated, returning one sample. This should be called once per sample period.
    */
    float Process();


    /** Adds a value 0.0-1.0 (mapped to 0.0-TWO_PI) to the current phase. Useful for PM and "FM" synthesis.
    */
    void PhaseAdd(float _phase) { phase_ += (_phase * TWOPI_F); }
    /** Resets the phase to the input argument. If no argumeNt is present, it will reset phase to 0.0;
    */
    void Reset(float _phase = 0.0f);

    void NextPattern()
    {
        current_pattern_++;
        if(current_pattern_ >= PATTERNS_MAX)
            current_pattern_ = 0;
        // Rest pattern step?
        //pattern_step_ = 0;
    }

    void PrevPattern()
    {
        current_pattern_--;
        if(current_pattern_ < 0)
            current_pattern_ = PATTERNS_MAX - 1;
        // Rest pattern step?
        //pattern_step_ = 0;
    }

    inline int16_t GetCurrentPattern() { return current_pattern_; }

  private:
    float          CalcPhaseInc(float f);
    float          amp_, freq_, pw_, pw_rad_;
    float          sr_, sr_recip_;
    volatile float phase_, phase_inc_;
    float          last_out_, last_freq_;
    bool           eor_, eoc_;
    int16_t        current_pattern_, pattern_step_;
    Mode           mode_;
    //size_t         tick_;
};

} // namespace bytebeat
#endif
#endif
