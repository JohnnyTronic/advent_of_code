#pragma once

enum Terrain {
    PATH,
    FOREST,
    NORTHSLOPE,
    EASTSLOPE,
    SOUTHSLOPE,
    WESTSLOPE
};

char TerrainToChar(Terrain terrain) {
    switch (terrain) {
    case PATH: return '.';
    case FOREST: return '#';
    case NORTHSLOPE: return '^';
    case EASTSLOPE: return '>';
    case SOUTHSLOPE: return 'v';
    case WESTSLOPE: return '<';
    default: throw "Unrecognized terrain.";
    }
}