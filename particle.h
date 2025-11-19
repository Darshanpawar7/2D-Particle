#ifndef PARTICLE_H
#define PARTICLE_H

#define SCENE_WIDTH 80
#define SCENE_HEIGHT 24
#define FRAME_DELAY 100000  // microseconds (0.1 seconds)

typedef struct Particle {
    int x, y;
    int dx, dy;
    int lifetime;
} Particle;

typedef struct ParticleCell {
    Particle data;
    struct ParticleCell *prev, *next;
} ParticleCell;

typedef struct ParticleList {
    ParticleCell *head, *tail;
    int size;
} ParticleList;

typedef struct ParticleSystem {
    ParticleList list;
    char frame[SCENE_HEIGHT][SCENE_WIDTH];
} ParticleSystem;

void initParticleSystem(ParticleSystem *system);
void initParticleList(ParticleList *list);
void addParticle(ParticleList *list, Particle p);
void removeParticle(ParticleList *list, ParticleCell *cell);
void updateParticles(ParticleSystem *system);
void cleanupParticles(ParticleList *list);

void renderAnimation(ParticleSystem *system);
void clearFrame(ParticleSystem *system);
void drawParticles(ParticleSystem *system);
void displayFrame(ParticleSystem *system);

void renderParticlesUsingBST(ParticleList *list);

#endif
