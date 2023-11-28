#pragma once

// 2d vector of floats
class TVec2f {
    public:
        // constructors
        // default
        TVec2f();
        // parameterized
        TVec2f(float u_, float v_);
    
        // vars
        float u, v;
};