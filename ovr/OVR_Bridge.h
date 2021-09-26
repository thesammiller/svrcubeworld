

unsigned int Random;  
#include "VrApi_Helpers.h"


void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS);

float RandomFloat() {
    Random = 1664525L * Random + 1013904223L;
    unsigned int rf = 0x3F800000 | (Random & 0x007FFFFF);
    return (*(float*)&rf) - 1.0f;
}

void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS) {
    for (int i = 0; i < NUM_ROTATIONS; i++) {
            Rotations[i].x = RandomFloat();
            Rotations[i].y = RandomFloat();
            Rotations[i].z = RandomFloat();
        }
}

void generate_random_locations(ovrVector3f *CubePositions, int *CubeRotations, int NUM_INSTANCES, ovrVector3f *Rotations, int NUM_ROTATIONS) {

    for (int i = 0; i < NUM_INSTANCES; i++) {
        volatile float rx, ry, rz;
        for (;;) {
            rx = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            ry = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            rz = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));

            
            // If too close to 0,0,0
            if (fabsf(rx) < 4.0f && fabsf(ry) < 4.0f && fabsf(rz) < 4.0f) {
                continue;
            }

            // Test for overlap with any of the existing cubes.
            bool overlap = false;
            for (int j = 0; j < i; j++) {
                if (fabsf(rx - CubePositions[j].x) < 4.0f && //4
                    fabsf(ry - CubePositions[j].y) < 4.0f &&
                    fabsf(rz - CubePositions[j].z) < 4.0f) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                break;
            }
        }

        //rx *= 0.1f;
        //ry *= 0.1f;
        //rz *= 0.1f;

        // Insert into list sorted based on distance.
        int insert = 0;
        const float distSqr = rx * rx + ry * ry + rz * rz;
        for (int j = i; j > 0; j--) {
            ovrVector3f* otherPos = &CubePositions[j - 1];
            const float otherDistSqr =
                otherPos->x * otherPos->x + otherPos->y * otherPos->y + otherPos->z * otherPos->z;
            if (distSqr > otherDistSqr) {
                insert = j;
                break;
            }
            CubePositions[j] = CubePositions[j - 1];
            CubeRotations[j] = CubeRotations[j - 1];
        }

        CubePositions[insert].x = rx;
        CubePositions[insert].y = ry;
        CubePositions[insert].z = rz;

        CubeRotations[insert] = (int)(RandomFloat() * (NUM_ROTATIONS - 0.1f));
    }
}