#include <stdio.h>
#include <stdlib.h>
#include "particle.h"

void promptAddParticle(ParticleList *list) {
    Particle p;
    printf("Enter x y dx dy lifetime:\n");
    if (scanf("%d%d%d%d%d", &p.x, &p.y, &p.dx, &p.dy, &p.lifetime) != 5) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    addParticle(list, p);
    printf("Particle added.\n");
}

void promptDeleteParticle(ParticleList *list) {
    int index, i = 0;
    printf("Enter particle index to delete (starting 0):\n");
    if (scanf("%d", &index) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    ParticleCell *curr = list->head;
    while (curr && i < index) {
        curr = curr->next;
        i++;
    }
    if (curr) {
        removeParticle(list, curr);
        printf("Particle deleted.\n");
    } else {
        printf("Invalid index.\n");
    }
}

void promptApplyGlobalForce(ParticleList *list) {
    float fx, fy;
    printf("Enter global forceX and forceY:\n");
    if (scanf("%f %f", &fx, &fy) != 2) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    applyGlobalForce(list, fx, fy);
    printf("Global force applied.\n");
}

int main() {
    ParticleSystem system;
    initParticleSystem(&system);

    int choice;
    int running = 1;

    while (running) {
        printf("\nMenu:\n");
        printf("1. Add Particle\n");
        printf("2. Delete Particle\n");
        printf("3. Update Particles\n");
        printf("4. Render Animation\n");
        printf("5. Render Using BST\n");
        printf("6. Apply Global Force\n");   // ⭐ NEW OPTION
        printf("7. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                promptAddParticle(&system.list);
                break;
            case 2:
                promptDeleteParticle(&system.list);
                break;
            case 3:
                updateParticles(&system);
                printf("Particles updated.\n");
                break;
            case 4:
                renderAnimation(&system);
                break;
            case 5:
                renderParticlesUsingBST(&system.list);
                break;
            case 6:
                promptApplyGlobalForce(&system.list);   // CALLS NEW FUNCTION.
                break;
            case 7:
                running = 0;
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    cleanupParticles(&system.list);
    return 0;
}


