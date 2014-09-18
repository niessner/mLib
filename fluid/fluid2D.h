
//
// each Cell represents the values centered exactly at (x, y)
//
struct Cell
{
    Cell()
    {
        pressure = 0.0f;
        divergence = 0.0f;
    }
    vec2f velocity;
    float pressure;
    float divergence;
    vec3f color;

    //
    // temporaries
    //
    float newPressure;
    vec2f newVelocity;
};

inline Cell operator + (const Cell &a, const Cell &b)
{
    Cell result;
    result.velocity = a.velocity + b.velocity;
    result.color = a.color + b.color;
    return result;
}

inline Cell operator - (const Cell &a, const Cell &b)
{
    Cell result;
    result.velocity = a.velocity - b.velocity;
    result.color = a.color - b.color;
    return result;
}

inline Cell operator * (const Cell &a, float b)
{
    Cell result;
    result.velocity = a.velocity * b;
    result.color = a.color * b;
    return result;
}

inline Cell operator * (float b, const Cell &a)
{
    Cell result;
    result.velocity = a.velocity * b;
    result.color = a.color * b;
    return result;
}

class Fluid2D
{
public:
    void init();
    void step();

    vec2f sampleVelocity(float x, float y)
    {
        int x0 = math::clamp(math::floor(x), 0L, gridSize - 2);
        int y0 = math::clamp(math::floor(y), 0L, gridSize - 2);
        
        const Cell &c00 = data(y0 + 0, x0 + 0);
        const Cell &c10 = data(y0 + 0, x0 + 1);
        const Cell &c01 = data(y0 + 1, x0 + 0);
        const Cell &c11 = data(y0 + 1, x0 + 1);
        
        float xs = x - x0;
        float ys = y - y0;

        vec2f lerpX0 = math::lerp(c00.velocity, c10.velocity, xs);
        vec2f lerpX1 = math::lerp(c01.velocity, c11.velocity, xs);
        
        return math::lerp(lerpX0, lerpX1, ys);
    }

    Cell sampleCell(float x, float y)
    {
        int x0 = math::clamp(math::floor(x), 0L, gridSize - 2);
        int y0 = math::clamp(math::floor(y), 0L, gridSize - 2);

        const Cell &c00 = data(y0 + 0, x0 + 0);
        const Cell &c10 = data(y0 + 0, x0 + 1);
        const Cell &c01 = data(y0 + 1, x0 + 0);
        const Cell &c11 = data(y0 + 1, x0 + 1);

        float xs = x - x0;
        float ys = y - y0;

        Cell lerpX0 = math::lerp(c00, c10, xs);
        Cell lerpX1 = math::lerp(c01, c11, xs);

        return math::lerp(lerpX0, lerpX1, ys);
    }

    void advectAll();
    void diffuseVelocities();
    void applyForces();
    void updateBoundaries();
    void computeDivergence();
    void computePressure();
    void pressureJacobiIteration();
    void subtractPressureGradient();

    const long gridSize = 32;
    float dt;
    float gridScale;
    float gridScaleInv;

    Grid2D<Cell> data;
    Grid2D<Cell> newData;
};