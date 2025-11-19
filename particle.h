#ifndef PARTICLE_H
#define PARTICLE_H

#define SCENE_WIDTH 800
#define SCENE_HEIGHT 600

// Particle structure
typedef struct Particle {
    int x, y;       // Position
    int dx, dy;     // Velocity
    int lifetime;   // Remaining life
} Particle;

// Linked List Node
typedef struct ParticleCell {
    Particle data;
    struct ParticleCell *prev, *next;
} ParticleCell;

// Linked List
typedef struct ParticleList {
    ParticleCell *head, *tail;
    int size;
} ParticleList;

// Core functions
void initParticleList(ParticleList *list);
void addParticle(ParticleList *list, Particle p);
void removeParticle(ParticleList *list, ParticleCell *cell);
void updateParticles(ParticleList *list);
void cleanupParticles(ParticleList *list);

// New assignment functions
void applyGlobalForce(ParticleList *list, float forceX, float forceY);
void clearAllParticles(ParticleList *list);

// BST rendering
void renderParticlesUsingBST(ParticleList *list);

#endif
