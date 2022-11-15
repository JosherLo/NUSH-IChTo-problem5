# NUSH-IChTo-problem5

Computational simulation of my solution to problem 5 of NUSH IChTo selections, "Willy Wonka's New Candy".

## Overview

There are 5 tastes, each stored in a microcapsule. There are 4 different microcapsule, each with the same internal radius, but different wall thickness.

For the first 4 tastes, there are an additional 4 more compounds ("removers"), as described in my presentation. Removers remove the flavour compound from the solution.

## How does it work?

The program simulates the breakdown of the microcapsule starch wall by enzyme amylase and records the volume of substance in the microcapsule that has been released at each time step.
                   
### Notes

- **Pseudo-zero order** reaction is assumed.

- The kinetic constant of amylase does not matter, as only one reaction is taking place. 

- Each **time step is relative**. Radius and thickness are measured in micrometers, however this is not necessary, as volume can also be relative.

- The radius is defined as the distance from the center of the microcapsule to the edge of the shell, and not the center of the shell (in other words, the radius includes the wall thickness).

- The reactions to remove all the flavours are instantaneous and goes to completion.

### 1) Normal Distribution

First, the program generates a list of numbers representing the radius and thickness of each microcapsule. 

This is done using a normal distribution. The values of the mean and standard deviation were obtained from literature, and interpolated for values that did not exist.

### 2) Enzyme activity

At each timestep, the volume of the microcapsule shell is calculated and a certain volume of the shell is removed. Since the reaction is pseudo-zero order, the same amount of volume is removed from the shells at each time step.

Next, the resulting radius and thickness is calculated.

### 3) Output

When the volume of the shell is calculated to be below 0, the microcapsuel is assumed to have burst, and the volume inside the microcapsule is added to a list of the total volume released at each step. 

The volume of the remover is subtracted from the total volume released at each step.

At the end of the simulation, the program writes all this data to a file.

## Visualisation

Graphs obtained were visualised using mathematica.