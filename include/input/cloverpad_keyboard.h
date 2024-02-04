#pragma once

#include <Keyboard.h>

/// @brief Modified version of the RP2040 `Keyboard_` class, which sends combined keyboard reports.
class CloverpadKeyboard_ : public Keyboard_
{
protected:
    // Override sendReport to no-op, so we only allow sending combined reports
    virtual void sendReport(KeyReport *) override {};

public:
    CloverpadKeyboard_(void){};

    /// @brief Sends the combined keyboard report.
    void sendReport()
    {
        Keyboard_::sendReport(&this->_keyReport);
    }
};

/// @brief The current keyboard configuration to use.
extern CloverpadKeyboard_ CloverpadKeyboard;
