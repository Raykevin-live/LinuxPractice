#pragma once

class NoCopy
{
public:
    NoCopy(){};
    NoCopy(const NoCopy& ) = delete;
    const NoCopy& operator=(const NoCopy&) = delete;

    ~NoCopy()
    {}
};