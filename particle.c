#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "particle.h"

void initParticleSystem(ParticleSystem *system) {
    initParticleList(&system->list);
    clearFrame(system);
}

void initParticleList(ParticleList *list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

void addParticle(ParticleList *list, Particle p) {
    ParticleCell *cell = (ParticleCell *)malloc(sizeof(ParticleCell));
    if (!cell) return;
    cell->data = p;
    cell->prev = list->tail;
    cell->next = NULL;

    if (list->tail)
        list->tail->next = cell;
    else
        list->head = cell;

    list->tail = cell;
    list->size++;
}

void removeParticle(ParticleList *list, ParticleCell *cell) {
    if (!cell) return;

    Particle *p = &cell->data;
    printf("Removing particle at Position:(%d,%d) Lifetime:%d\n", p->x, p->y, p->lifetime);

    if (cell->prev)
        cell->prev->next = cell->next;
    else
        list->head = cell->next;

    if (cell->next)
        cell->next->prev = cell->prev;
    else
        list->tail = cell->prev;

    free(cell);
    list->size--;
}

void updateParticles(ParticleSystem *system) {
    ParticleList *list = &system->list;
    ParticleCell *curr = list->head, *next;
    
    while (curr) {
        next = curr->next;
        Particle *p = &curr->data;

        // Update position
        p->x += p->dx;
        p->y += p->dy;

        // Elastic collision with boundaries
        if (p->x < 0) {
            p->x = 0;
            p->dx = -p->dx;  // Reverse horizontal direction
        } else if (p->x >= SCENE_WIDTH) {
            p->x = SCENE_WIDTH - 1;
            p->dx = -p->dx;
        }

        if (p->y < 0) {
            p->y = 0;
            p->dy = -p->dy;  // Reverse vertical direction
        } else if (p->y >= SCENE_HEIGHT) {
            p->y = SCENE_HEIGHT - 1;
            p->dy = -p->dy;
        }

        // Decrease lifetime
        p->lifetime--;

        // Remove particle if lifetime expired
        if (p->lifetime <= 0) {
            removeParticle(list, curr);
        }

        curr = next;
    }
}

void cleanupParticles(ParticleList *list) {
    ParticleCell *curr = list->head;
    while (curr) {
        ParticleCell *next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

void clearFrame(ParticleSystem *system) {
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++) {
            system->frame[i][j] = ' ';
        }
    }
}

void drawParticles(ParticleSystem *system) {
    ParticleCell *curr = system->list.head;
    while (curr) {
        Particle *p = &curr->data;
        if (p->x >= 0 && p->x < SCENE_WIDTH && p->y >= 0 && p->y < SCENE_HEIGHT) {
            system->frame[p->y][p->x] = '*';
        }
        curr = curr->next;
    }
}

void displayFrame(ParticleSystem *system) {
    // Clear screen (ANSI escape code)
    printf("\033[2J\033[H");
    
    // Draw top border
    printf("+");
    for (int i = 0; i < SCENE_WIDTH; i++) printf("-");
    printf("+\n");
    
    // Draw frame with particles
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < SCENE_WIDTH; j++) {
            printf("%c", system->frame[i][j]);
        }
        printf("|\n");
    }
    
    // Draw bottom border
    printf("+");
    for (int i = 0; i < SCENE_WIDTH; i++) printf("-");
    printf("+\n");
    
    // Display particle count and info
    printf("Active Particles: %d | Press Ctrl+C to stop animation\n", system->list.size);
}

void renderAnimation(ParticleSystem *system) {
    if (system->list.size == 0) {
        printf("No particles to animate!\n");
        return;
    }
    
    printf("Starting animation... Press Ctrl+C to stop\n");
    sleep(1);
    
    // Animate for a reasonable duration or until all particles expire
    int maxFrames = 200;
    int frameCount = 0;
    
    while (system->list.size > 0 && frameCount < maxFrames) {
        clearFrame(system);
        drawParticles(system);
        displayFrame(system);
        
        usleep(FRAME_DELAY);
        updateParticles(system);
        frameCount++;
    }
    
    printf("\nAnimation complete!\n");
}

// BST for rendering

typedef struct BSTNode {
    Particle data;
    struct BSTNode *left, *right;
} BSTNode;

BSTNode* insertIntoBST(BSTNode* root, Particle p) {
    if (!root) {
        BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
        newNode->data = p;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (p.lifetime < root->data.lifetime)
        root->left = insertIntoBST(root->left, p);
    else
        root->right = insertIntoBST(root->right, p);
    return root;
}

void inorderBSTRender(BSTNode* root) {
    if (!root) return;
    inorderBSTRender(root->left);
    Particle *p = &root->data;
    printf("Position:(%d,%d) Velocity:(%d,%d) Lifetime:%d\n", p->x, p->y, p->dx, p->dy, p->lifetime);
    inorderBSTRender(root->right);
}

void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

void renderParticlesUsingBST(ParticleList *list) {
    if (list->size == 0) {
        printf("No particles to render!\n");
        return;
    }
    
    BSTNode* root = NULL;
    ParticleCell *curr = list->head;
    while (curr) {
        root = insertIntoBST(root, curr->data);
        curr = curr->next;
    }
    printf("\n--- Particles Sorted by Lifetime (BST Inorder) ---\n");
    inorderBSTRender(root);
    freeBST(root);
}



// New Assignment Function #1
// Apply a global force (like wind/gravity) to every particle
void applyGlobalForce(ParticleList *list, float forceX, float forceY) {
    ParticleCell *curr = list->head;

    while (curr) {
        curr->data.dx += forceX;
        curr->data.dy += forceY;

        printf("Applied Force -> New Velocity: (%d, %d)\n",
               curr->data.dx, curr->data.dy);

        curr = curr->next;
    }
}


