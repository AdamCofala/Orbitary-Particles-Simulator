# Orbital Gravity Simulation

## Description

Orbital Gravity Simulation is a real-time particle physics simulation using SFML. The program allows users to create gravity sources and particles, which interact with each other based on gravitational forces. It provides an interactive environment where users can observe orbital motion, adjust simulation parameters, and create custom scenarios.


![s](https://github.com/user-attachments/assets/4c0cff75-650f-4f33-be26-b3259e143e3f)





## Features

- Add particles with right-click
- Add gravity sources with left-click
- Adjust simulation parameters using keyboard shortcuts
- Real-time particle physics and color updates
- FPS counter and simulation statistics
- Collision toggle and frame rate capping

## Controls

- **Right Click** - Spawn particles at the mouse position
- **Left Click** - Create a gravity source at the mouse position
- **Left Arrow** - Clear all particles
- **Right Arrow** - Decrease collision setting
- **C** - Clear all gravity sources
- **D** - Increase particle count by 100
- **A** - Decrease particle count by 100
- **E** - Increase gravity strength by 1000
- **Q** - Decrease gravity strength by 1000
- **S** - Create a predefined simulation setup

## Dependencies

- SFML (Simple and Fast Multimedia Library)
- C++17 or newer

## Math Behind Orbital Gravity Simulation

#### 1. **Gravitational Force Calculation**
The gravitational force is simplified using the inverse square law, where the acceleration is proportional to the inverse square of the distance between a particle and a gravity source. The force is calculated as:

$$
a_x = \frac{G \cdot \text{strength} \cdot \text{distance}_x}{r^2} \times \Delta t
$$
$$
a_y = \frac{G \cdot \text{strength} \cdot \text{distance}_y}{r^2} \times \Delta t
$$

#### 2. **Distance Calculation**
The distance between a particle and a gravity source is calculated using:

$$
r = \sqrt{\text{distance}_x^2 + \text{distance}_y^2}
$$

#### 3. **Acceleration Update**
The particle's velocity is updated with the calculated acceleration:

$$
v_x = v_x + a_x
$$

$$
v_y = v_y + a_y
$$

#### 4. **Speed Limit**
To prevent unrealistic speeds, the velocity is scaled down if it exceeds the maximum allowed:

$$
v_x = \left( \frac{v_x}{\text{speed}} \right) \times \text{max speed}
$$

$$
v_y = \left( \frac{v_y}{\text{speed}} \right) \times \text{max speed}
$$

#### 5. **Collision Handling**
When a particle hits the window boundaries, its velocity is inverted:

$$
\text{if } x < 0 \text{ or } x > \text{window width} v_x = -v_x
$$

$$
\text{if } y < 0 \text{ or } y > \text{window height} v_y = -v_y
$$

#### 6. **Time Step Update**
The position of the particle is updated each frame using:

$$
x_{\text{particle}} = x_{\text{particle}} + v_x
$$

$$
y_{\text{particle}} = y_{\text{particle}} + v_y
$$

This approach creates realistic orbital motion with interactive gravity simulations.

