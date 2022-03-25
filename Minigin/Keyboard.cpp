#include "MiniginPCH.h"
#include "Keyboard.h"

void dae::Keyboard::ProcessInput()
{
}

bool dae::Keyboard::IsDownThisFrame(int key) const
{
    return GetKeyState(key) & 0x80;
}
