#pragma once

#include <Keyboard.h>

/// @brief Modified version of the RP2040 `Keyboard_` class, which sends combined keyboard reports.
class CloverpadKeyboard_ : public Keyboard_
{
private:
    /// @brief Whether the keyboard report can currently be sent.
    bool can_send_report = false;

protected:
    virtual void sendReport(KeyReport *keys) override
    {
        if (this->can_send_report)
        {
            Keyboard_::sendReport(keys);
        }
    }

public:
    CloverpadKeyboard_(void){};

    /// @brief Sends the combined keyboard report.
    void sendReport()
    {
        this->can_send_report = true;
        sendReport(&this->_keyReport);
        this->can_send_report = false;
    }
};

/// @brief The current keyboard configuration to use.
CloverpadKeyboard_ CloverpadKeyboard;
