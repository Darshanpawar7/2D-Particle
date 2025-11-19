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

int main() {
    ParticleSystem system;
    initParticleSystem(&system);

    int choice;
    int running = 1;

    while (running) {
        printf("\nMenu:\n1. Add Particle\n2. Delete Particle\n3. Update Particles\n4. Render Animation\n5. Render Using BST\n6. Exit\nEnter choice: ");
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
                running = 0;
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    cleanupParticles(&system.list);
    return 0;
}

