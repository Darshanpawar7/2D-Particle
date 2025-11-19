#include <stdio.h>
#include <stdlib.h>
#include "particle.h"

// Input helper: Add particle
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

// Input helper: Delete particle by index
void promptDeleteParticle(ParticleList *list) {
    int index, i = 0;
    printf("Enter particle index to delete:\n");

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

int main() {
    ParticleList system;
    initParticleList(&system);

    int choice;
    int running = 1;

    while (running) {
        printf("\n==== MENU ====\n");
        printf("1. Add Particle\n");
        printf("2. Delete Particle\n");
        printf("3. Update Particles\n");
        printf("4. Render Using BST\n");
        printf("5. Apply Global Force\n");
        printf("6. Clear All Particles\n");
        printf("7. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                promptAddParticle(&system);
                break;

            case 2:
                promptDeleteParticle(&system);
                break;

            case 3:
                updateParticles(&system);
                break;

            case 4:
                renderParticlesUsingBST(&system);
                break;

            case 5: {
                float fx, fy;
                printf("Enter global forceX forceY: ");
                scanf("%f %f", &fx, &fy);
                applyGlobalForce(&system, fx, fy);
                break;
            }

            case 6:
                clearAllParticles(&system);
                break;

            case 7:
                running = 0;
                break;

            default:
                printf("Invalid choice\n");
        }
    }

    cleanupParticles(&system);
    return 0;
}
