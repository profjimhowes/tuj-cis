Simulating mantle convection comes down to solving a system of conservation equations: mass, momentum, energy, and composition (or matter).

$$\textbf{Mass: }\nabla\cdot(\rho\vec{u})=-\frac{\partial\rho}{\partial t}$$
$$\textbf{Momentum: }\nabla\cdot(2\eta\dot{\varepsilon}_{\text{dev}})+\rho\vec{g}=\nabla p$$
$$\textbf{Energy: }\rho c_p(\frac{\partial T}{\partial t}+\vec{u}\cdot\nabla T)=\nabla\cdot(k\nabla T)+H$$
$$\textbf{Composition: }\frac{\partial c_i}{\partial t}+\vec{u}\cdot\nabla c_i=R_i$$

The outputs of the material model are the values of the coefficients (and partial derivatives) that make this system solvable. The inputs are the variables that appear inside the differential operators.

Composition is not an essential component per se, but is desirable when one wants to model advection and phase changes of heterogenous constitutive materials. The same advection-plus-reaction equation can also be used for internal state variables such as water content or grain size, as well as arbitrary scalar fields that do not affect the model output but may be used for analysis or visualization.

## Strain rate

Temperature, pressure, and composition unambiguously describe the thermodynamic state of a point in the mantle. However, strain rate is also an input to the model and represents something of a different nature.

The Stokes solver output is a velocity field, and strain rate is just the velocity gradient (a 3x3 tensor) field. The tensor can be decomposed uniquely into a symmetric component representing strain rate $\dot{\varepsilon}$ and an antisymmetric component representing vorticity $\Omega$.

$$\nabla\vec{u}=\underbrace{\tfrac{1}{2}(\nabla\vec{u}+\nabla\vec{u}^T)}_{\dot{\varepsilon}\text{ (strain rate)}}+\underbrace{\tfrac{1}{2}(\nabla\vec{u}-\nabla\vec{u}^T)}_{\Omega\text{ (vorticity)}}$$

Vorticity is just rigid rotation and doesn't deform the material, so only the strain rate term is used as input to the model. This is because the viscosity of non-Newtonian fluids (like mantle rock) is dependent upon the rate of deformation.

For mantle codes specifically, an isotropic model is assumed where the orientation of the material doesn't affect its response. This means that the orthogonal bases of the strain rate tensor can be ignored, and only the three invariants (components which are orientation-independent) affect the output.

$$I_1=\text{tr}(A), I_2=\tfrac{1}{2}\left[(\text{tr}A)^2-\text{tr}(A^2)\right], I_3=\text{det}(A)$$

The trace of the tensor (first invariant) represents the volumetric strain rate, which for incompressible fluids will always be $0$. The third is usually small with little dynamic variation. Thus, most mantle codes use only the second invariant, which (for a strain rate tensor) is simply its magnitude:

$$\dot{\varepsilon}_{II}=\sqrt{\tfrac{1}{2}\dot{\varepsilon}:\dot{\varepsilon}}$$

In the compressible case, the trace is not actually zero so the above approximation must be modified by subtracting out $\dot{\varepsilon}_{\text{vol}}=\tfrac{1}{3}(\nabla\cdot\vec{u})\mathbf{I}$ to yield the trace-free deviatoric component $\dot{\varepsilon}_{\text{dev}}$.

Thus, the viscosity of the material is a function of four inputs: temperature, pressure, composition, and the scalar $\dot{\varepsilon}_{II}$ rather than using the entire six-dimensional tensor. Genuinely anisotropic materials are typically modeled with more invariants formed jointly along the direction of anisotropy.

## Outputs

The model outputs
1. **Density** $\rho$. Mass per unit volume as a function of the rock composition along with temperature and pressure. Ranges from $3300$ to $5500\text{ kg}/\text{m}^3$ in the mantle.
2. **Specific heat capacity** $c_p$. Heat required to raise temperature by one degree at constant pressure (hence the p subscript). Roughly $1250\text{ J}/(\text{kg}\cdot\text{K})$ for mantle rock, with modest variation.
3. **Thermal expansivity** $\alpha$. Fractional change in volume per unit of temperature change at constant pressure. This is technically derived from the derivative of density over temperature, and is the root of convective forces. Mantle rock expands at about $3\times10^{-5}\text{ K}^{-1}$ at the surface, decreasing with depth.
4. **Compressibility** $\kappa_T$ (or $\beta$). Fractional change in volume per unit of pressure change at constant temperature. Similarly derived from density, and set to $0$ for incompressible fluid simulations.
5. **Thermal conductivity** $k$. Heat flux per unit of temperature gradient. Only weakly dependent on pressure and temperature, but may be strongly affected by composition. Possibly anisotropic (which would make it a tensor), but isotropic is the standard assumption. About $3$ to $5\text{ W}/(\text{m}\cdot\text{K})$ for mantle rock.
6. **Viscosity** $\eta$. The effective ratio between deviatoric stress and deviatoric strain rate. This is the most consequential component of the model, both in terms of predicted convection behavior and implementation challenges.

Mantle viscosity varies by eight orders of magnitude, from $10^{17}\text{ Pa}\cdot\text{s}$ for hot, wet asthenosphere up to $10^{25}\text{ Pa}\cdot\text{s}$ for cold lithosphere. Non-Newtonian viscosity is generally modeled as the harmonic mean of diffusion creep and dislocation creep, with a floor set at the yield envelope by Newtonian plastic flow.

$$\eta_\text{eff}=\inf\left[(\eta_\text{diff}^{-1}+\eta_\text{disl}^{-1})^{-1},\frac{\sigma_y}{2\dot{\varepsilon}_{II}}\right]$$

The yield envelope in turn is usually derived from the Drucker-Prager yield criterion.

## Source terms

These are components of the state equations that represent additive contributions or transformations rather than flux of conserved quantities. They are restricted to either heat or reaction rate terms.

Heat sources include
- Radiogenic heating as a function of volumetric composition
- Shear heating, from mechanical work performed by viscous deformation processes
- Adiabatic heating (usually cooling) of compressible fluids
- Latent heating from phase transitions (e.g. olivine to wadsleyite)

Although heat sources are inputs to the solver, they do not depend on the *constitutive* model and should be specified independently. Reaction rates, on the other hand, may depend on thermodynamic state and thus are outputs of the constitutive model.

## Newton's method and the Jacobian

The standard approach to solving a non-linear equation is Newton's method: linearize around a current guess, solve, update, and repeat until the error is sufficiently small. This technique requires the Jacobian, which is the matrix of partial derivatives. Dependencies between inputs and outputs are reflected in non-zero entries in this matrix.

One of the shortcomings of ASPECT is that it leaves it up to the user to provide a consistent and correct Jacobian for a given material model, which may be very tricky for complex rheologies. This is one place where our system can provide a much more robust material model interface, by using automatic differentiation to *derive* the correct Jacobians to "eliminate an entire class of bugs".