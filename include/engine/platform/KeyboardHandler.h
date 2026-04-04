//
// Created by Jasper Soete on 4-4-2026.
//

#pragma once

class VoxelGame;

class KeyboardHandler
{
public:
    KeyboardHandler(VoxelGame* game);
    void setup();
    void process();
private:
    VoxelGame* game;
};
