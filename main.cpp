#include <iostream>
#include <Windows.h>
#include <Xinput.h>

// Link with Xinput library
#pragma comment(lib, "Xinput.lib")

// Function to move the mouse
void MoveMouse(int dx, int dy) {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.mouseData = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.time = 0;
    SendInput(1, &input, sizeof(INPUT));
}

// Function to perform left mouse click
void LeftMouseClick() {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT)); // Clear the input structure
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void RightMouseClick() {
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN; // Press right mouse button
    SendInput(1, &input, sizeof(INPUT));
    ZeroMemory(&input, sizeof(INPUT)); // Clear the input structure
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP; // Release right mouse button
    SendInput(1, &input, sizeof(INPUT));
}


// Main function
int main() {
    // Initialize Xinput
    XINPUT_STATE state;
    DWORD result;
    bool leftClickDown = false;

    std::cout << "Controller Mouse Control - Press Ctrl+C to exit\n";

    // Main loop
    while (true) {
        // Get the state of the controller
        result = XInputGetState(0, &state);
        if (result == ERROR_SUCCESS) {
            // Extract normalized thumbstick positions
            SHORT thumbX = state.Gamepad.sThumbLX;
            SHORT thumbY = state.Gamepad.sThumbLY;
            SHORT rightThumbX = state.Gamepad.sThumbRX;
            SHORT rightThumbY = state.Gamepad.sThumbRY;

            // Map thumbstick values to mouse movement
            double dx, dy;
            if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                // Increase mouse speed if left trigger is pressed
                dx = static_cast<double>(thumbX) * .0005; // Scale for faster mouse movement
                dy = static_cast<double>(thumbY) * .0005; // Scale for faster mouse movement
            }
            else {
                dx = static_cast<double>(thumbX) * .0003; // Scale for mouse movement
                dy = static_cast<double>(thumbY) * .0003; // Scale for mouse movement
            }

            if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                // Increase mouse speed if left trigger is pressed
                if (leftClickDown == false) {
                    leftClickDown = true;
                    INPUT input;
                    input.type = INPUT_MOUSE;
                    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                    SendInput(1, &input, sizeof(INPUT));
                }
            }
            else {
                if (leftClickDown == true) {
                    leftClickDown = false;
                    INPUT input;
                    input.type = INPUT_MOUSE;
                    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                    SendInput(1, &input, sizeof(INPUT));
                }
            }

            // Check if L1 button is pressed
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
                // Press Ctrl key
                keybd_event(VK_CONTROL, 0, 0, 0);

                // Press Minus key
                keybd_event(VK_SUBTRACT, 0, 0, 0);

                // Add a small delay
                Sleep(50);

                // Release Minus key
                keybd_event(VK_SUBTRACT, 0, KEYEVENTF_KEYUP, 0);

                // Release Ctrl key
                keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
            }

            // Check if R1 button is pressed
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
                // Press Ctrl key
                keybd_event(VK_CONTROL, 0, 0, 0);

                // Press Plus key
                keybd_event(VK_ADD, 0, 0, 0);

                // Add a small delay
                Sleep(50);

                // Release Plus key
                keybd_event(VK_ADD, 0, KEYEVENTF_KEYUP, 0);

                // Release Ctrl key
                keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
            }

            // Invert dy to match screen coordinates
            dy = -dy;

            // Move the mouse
            MoveMouse(dx, dy);

            // Check if A button is pressed
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
                LeftMouseClick(); // Perform left mouse button down event
                Sleep(200); // Add a delay to avoid multiple clicks
            }

            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
                RightMouseClick(); // Perform right mouse click
                Sleep(200); // Add a delay to avoid multiple clicks
            }

            // Check if X button is pressed
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
                keybd_event(VK_BACK, 0, 0, 0); // Perform 
                Sleep(200); // Add a delay to avoid multiple clicks
                keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0); // Release Backspace key
            }

            // Check if B button is pressed
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
                keybd_event(VK_RETURN, 0, 0, 0); // Press Enter key
                Sleep(200); // Add a delay to avoid multiple clicks
                keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0); // Release Enter key
            }



            // Simulate mouse scroll with right joystick
            int scroll_amount = static_cast<int>(rightThumbY) / 500; // Scale for scroll amount
            if (scroll_amount != 0) {
                INPUT input;
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                input.mi.mouseData = scroll_amount;
                input.mi.dwExtraInfo = 0;
                input.mi.time = 0;
                SendInput(1, &input, sizeof(INPUT));
            }

            // Check D-pad directions
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
                keybd_event(VK_UP, 0, 0, 0); // Press arrow up key
                Sleep(50); // Add a small delay
                keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0); // Release arrow up key
            }
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
                keybd_event(VK_DOWN, 0, 0, 0); // Press arrow down key
                Sleep(50); // Add a small delay
                keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0); // Release arrow down key
            }
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
                keybd_event(VK_LEFT, 0, 0, 0); // Press arrow left key
                Sleep(50); // Add a small delay
                keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0); // Release arrow left key
            }
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
                keybd_event(VK_RIGHT, 0, 0, 0); // Press arrow right key
                Sleep(50); // Add a small delay
                keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0); // Release arrow right key
            }

            // Delay to avoid flooding
            Sleep(10);
        }
    }

    return 0;
}