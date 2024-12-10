#include "comms/DInputBackend.hpp"

#include "core/CommunicationBackend.hpp"
#include "core/state.hpp"

#include <TUGamepad.hpp>

DInputBackend::DInputBackend(InputSource **input_sources, size_t input_source_count)
    : CommunicationBackend(input_sources, input_source_count) {
    _gamepad = new TUGamepad();
    _gamepad->begin();

    while (!USBDevice.mounted()) {
        delay(1);
    }
}

DInputBackend::~DInputBackend() {
    _gamepad->resetInputs();
    delete _gamepad;
}

void DInputBackend::SendReport() {
    ScanInputs(InputScanSpeed::SLOW);
    ScanInputs(InputScanSpeed::MEDIUM);

    while (!_gamepad->ready()) {
        tight_loop_contents();
    }

    ScanInputs(InputScanSpeed::FAST);

    UpdateOutputs();

    // Digital outputs
    _gamepad->setButton(0, _outputs.a); // B
    _gamepad->setButton(1, _outputs.b); // A
    _gamepad->setButton(2, _outputs.x); // Y
    _gamepad->setButton(3, _outputs.y); // X
    _gamepad->setButton(4, _outputs.buttonL); // ButtonR / Z
    _gamepad->setButton(5, _outputs.triggerRDigital); // TriggerRDigital / R
    _gamepad->setButton(6, _outputs.buttonR); // ButtonL / LB
    _gamepad->setButton(7, _outputs.triggerLDigital && _outputs.triggerRDigital); // TriggerLDigital
    // _gamepad->setButton(8, _outputs.select);
    _gamepad->setButton(9, _outputs.rightStickClick); // Start
    _gamepad->setButton(10, _outputs.start); // RightStickClick
    // _gamepad->setButton(11, _outputs.leftStickClick);
    // _gamepad->setButton(12, _outputs.home);

    // Analog outputs
    _gamepad->leftXAxis(_outputs.leftStickX);
    _gamepad->leftYAxis(255 - _outputs.leftStickY);
    _gamepad->rightXAxis(_outputs.rightStickX);
    _gamepad->rightYAxis(255 - _outputs.rightStickY);
    // _gamepad->triggerLAnalog(_outputs.triggerLAnalog + 1);
    // _gamepad->triggerRAnalog(_outputs.triggerRAnalog + 1);

    // D-pad Hat Switch
    _gamepad->hatSwitch(_outputs.dpadLeft, _outputs.dpadRight, _outputs.dpadDown, _outputs.dpadUp);

    _gamepad->sendState();
}
