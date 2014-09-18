
#include "main.h"

//
// see http://http.developer.nvidia.com/GPUGems/gpugems_ch38.html
//

void Fluid2D::init()
{
    data.allocate(gridSize, gridSize);
    for (long y = 0; y < gridSize; y++)
    {
        for (long x = 0; x < gridSize; x++)
        {
            Cell &c = data(y, x);
            //c.color = vec3f(1.0f, 0.0f, 0.0f);
            //if ( (x / 16 + y / 16) % 2 == 0)
            //    c.color = vec3f(0.0f, 0.0f, 1.0f);
            c.level = (gridSize * 0.5f - y) * 2.0f / gridSize;
        }
    }

    newData = data;

    dt = 0.2f;
    gridScale = 1.0f;
    gridScaleInv = 1.0f / gridScale;
}

void Fluid2D::step()
{
    updateBoundaries();
    advectAll();
    diffuseVelocities();
    applyForces();

    updateBoundaries();
    computeDivergence();

    computePressure();

    updateBoundaries();
    subtractPressureGradient();
}

void Fluid2D::advectAll()
{
    // TODO: implement Keenan's advection

    for (long y = 1; y < gridSize - 1; y++)
    {
        for (long x = 1; x < gridSize - 1; x++)
        {
            const Cell &c = data(y, x);
            vec2f pos = vec2f((float)x, (float)y) - dt * gridScaleInv * c.velocity;
            newData(y, x) = sampleCell(pos.x, pos.y);
        }
    }

    std::swap(data, newData);
}

void Fluid2D::diffuseVelocities()
{
    // TODO: handle non-zero viscocity
}

void Fluid2D::applyForces()
{
    // TODO: mouse-controlled forces

    //data(8, 10).velocity += 0.1f * dt * vec2f(0.8f, 0.2f);

    for (long y = 1; y < gridSize - 1; y++)
    {
        for (long x = 1; x < gridSize - 1; x++)
        {
            Cell &c = data(y, x);
            c.velocity.y -= dt * 0.0001f;
        }
    }
}

void Fluid2D::updateBoundaries()
{
    auto fixBoundary = [](Cell &boundary, const Cell &interior)
    {
        boundary.velocity = -interior.velocity;
        boundary.pressure = interior.pressure;
    };

    for (int x = 0; x < gridSize; x++)
    {
        fixBoundary(data(0, x), data(1, x));
        fixBoundary(data(gridSize - 1, x), data(gridSize - 2, x));
    }

    for (int y = 0; y < gridSize; y++)
    {
        fixBoundary(data(y, 0), data(y, 0));
        fixBoundary(data(y, gridSize - 1), data(y, gridSize - 2));
    }
}

void Fluid2D::computeDivergence()
{
    for (long y = 1; y < gridSize - 1; y++)
    {
        for (long x = 1; x < gridSize - 1; x++)
        {
            Cell &c = data(y, x);

            Cell &l = data(y, x - 1);
            Cell &r = data(y, x + 1);
            Cell &t = data(y - 1, x);
            Cell &b = data(y + 1, x);

            c.divergence = 0.5f * gridScaleInv * ((r.velocity.x - l.velocity.x) + (t.velocity.y - b.velocity.y));
        }
    }
}

void Fluid2D::computePressure()
{
    const UINT jacobiIterationCount = 80;

    //
    // initial pressure guess is zero
    //
    for (auto &c : data)
    {
        c.pressure = 0.0f;
        c.newPressure = 0.0f;
    }

    for (UINT jacobiIteration = 0; jacobiIteration < jacobiIterationCount; jacobiIteration++)
    {
        pressureJacobiIteration();
    }
}

void Fluid2D::pressureJacobiIteration()
{
    // b = divergence
    // x = pressure
    float alpha = -gridScale * gridScale;
    float rBeta = 0.25f;

    for (long y = 1; y < gridSize - 1; y++)
    {
        for (long x = 1; x < gridSize - 1; x++)
        {
            Cell &c = data(y, x);

            Cell &l = data(y, x - 1);
            Cell &r = data(y, x + 1);
            Cell &t = data(y - 1, x);
            Cell &b = data(y + 1, x);

            c.newPressure = (l.pressure + r.pressure + b.pressure + t.pressure + alpha * c.divergence) * rBeta;

            if (c.level < 0.0f)
            {
                c.newPressure = 0.0f;
            }
        }
    }

    //vector<Cell> dataVecB(data.begin(), data.end());
    //int a = 5;

    for (auto &c : data)
        c.pressure = c.newPressure;
}

void Fluid2D::subtractPressureGradient()
{
    for (long y = 1; y < gridSize - 1; y++)
    {
        for (long x = 1; x < gridSize - 1; x++)
        {
            Cell &c = data(y, x);

            Cell &l = data(y, x - 1);
            Cell &r = data(y, x + 1);
            Cell &t = data(y - 1, x);
            Cell &b = data(y + 1, x);

            c.newVelocity = c.velocity - 0.5f * gridScaleInv * vec2f(r.pressure - l.pressure, t.pressure - b.pressure);
        }
    }

    for (auto &c : data)
        c.velocity = c.newVelocity;
}
