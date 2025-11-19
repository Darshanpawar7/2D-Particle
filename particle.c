#include <stdio.h>
#include <stdlib.h>
#include "particle.h"

// -----------------------------
// Initialize empty particle list
// -----------------------------
void initParticleList(ParticleList *list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

// -----------------------------
// Add particle to end of linked list
// -----------------------------
void addParticle(ParticleList *list, Particle p) {
    ParticleCell *cell = malloc(sizeof(ParticleCell));
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

// -----------------------------
// Remove a particle node
// -----------------------------
void removeParticle(ParticleList *list, ParticleCell *cell) {
    if (!cell) return;

    Particle *p = &cell->data;
    printf("Removing particle at Position:(%d,%d) Lifetime:%d\n",
            p->x, p->y, p->lifetime);

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

// -----------------------------
// Update all particles: move and reduce lifetime
// -----------------------------
void updateParticles(ParticleList *list) {
    ParticleCell *curr = list->head, *next;

    while (curr) {
        next = curr->next;
        Particle *p = &curr->data;

        printf("Updating Particle at (%d,%d) Velocity=(%d,%d) Life=%d\n",
               p->x, p->y, p->dx, p->dy, p->lifetime);

        // Apply velocity to position
        p->x += p->dx;
        p->y += p->dy;

        // Decrease lifetime
        p->lifetime--;

        // Remove if dead or out of bounds
        if (p->lifetime <= 0 || p->x < 0 || p->x >= SCENE_WIDTH ||
            p->y < 0 || p->y >= SCENE_HEIGHT) {

            removeParticle(list, curr);
        }

        curr = next;
    }
}

// -----------------------------
// Completely free all particles
// -----------------------------
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

// ===============================================================
// ASSIGNMENT FUNCTIONS
// ===============================================================

// -----------------------------
// 1. Apply global force (like wind)
// Adds forceX, forceY to every particle’s velocity
// -----------------------------
void applyGlobalForce(ParticleList *list, float forceX, float forceY) {
    ParticleCell *curr = list->head;

    while (curr) {
        curr->data.dx += forceX;
        curr->data.dy += forceY;

        printf("Applied Force: New Velocity=(%d,%d)\n",
               curr->data.dx, curr->data.dy);

        curr = curr->next;
    }
}

// -----------------------------
// 2. Clear all particles (reset system)
// -----------------------------
void clearAllParticles(ParticleList *list) {
    ParticleCell *curr = list->head;

    while (curr) {
        ParticleCell *next = curr->next;
        free(curr);
        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;

    printf("All particles cleared.\n");
}

// ===============================================================
// BST Rendering Code
// ===============================================================

typedef struct BSTNode {
    Particle data;
    struct BSTNode *left, *right;
} BSTNode;

BSTNode* insertIntoBST(BSTNode* root, Particle p) {
    if (root == NULL) {
        BSTNode* newNode = malloc(sizeof(BSTNode));
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

// Inorder - prints lifetime sorted
void inorderBSTRender(BSTNode* root) {
    if (!root) return;

    inorderBSTRender(root->left);

    Particle *p = &root->data;
    printf("Render: Pos(%d,%d) Vel(%d,%d) Life:%d\n",
           p->x, p->y, p->dx, p->dy, p->lifetime);

    inorderBSTRender(root->right);
}

void freeBST(BSTNode* root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

void renderParticlesUsingBST(ParticleList *list) {
    BSTNode* root = NULL;
    ParticleCell *curr = list->head;

    while (curr) {
        root = insertIntoBST(root, curr->data);
        curr = curr->next;
    }

    inorderBSTRender(root);
    freeBST(root);
}
