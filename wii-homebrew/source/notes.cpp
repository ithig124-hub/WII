#include "notes.h"
#include "graphics.h"
#include "input.h"
#include <stdio.h>

#define MAX_NOTES 10
#define MAX_NOTE_LENGTH 256

typedef struct {
    char title[64];
    char content[MAX_NOTE_LENGTH];
    bool active;
} Note;

static Note notes[MAX_NOTES];
static int selectedNote = 0;
static int scrollOffset = 0;
static bool editMode = false;

void initNotes() {
    // Initialize with some sample notes
    notes[0].active = true;
    strcpy(notes[0].title, "Welcome");
    strcpy(notes[0].content, "Welcome to Wii Dashboard Notes! Press A to add new notes.");
    
    notes[1].active = true;
    strcpy(notes[1].title, "Shopping List");
    strcpy(notes[1].content, "Milk, Eggs, Bread, Cheese");
    
    notes[2].active = true;
    strcpy(notes[2].title, "TODO");
    strcpy(notes[2].content, "1. Finish project\n2. Call mom\n3. Exercise");
    
    for (int i = 3; i < MAX_NOTES; i++) {
        notes[i].active = false;
    }
    
    // Try to load notes from SD card
    FILE* file = fopen("sd:/apps/wii-dashboard/notes.dat", "rb");
    if (file) {
        fread(notes, sizeof(Note), MAX_NOTES, file);
        fclose(file);
    }
}

void cleanupNotes() {
    // Save notes to SD card
    FILE* file = fopen("sd:/apps/wii-dashboard/notes.dat", "wb");
    if (file) {
        fwrite(notes, sizeof(Note), MAX_NOTES, file);
        fclose(file);
    }
}

void updateNotes() {
    InputState* input = getInput();
    
    // B button to go back
    if (input->bButton && !editMode) {
        changeScene(SCENE_DASHBOARD);
        return;
    }
    
    if (!editMode) {
        // Navigation mode
        if (input->dpadY < 0) {
            selectedNote = (selectedNote - 1 + MAX_NOTES) % MAX_NOTES;
            if (selectedNote < scrollOffset) {
                scrollOffset = selectedNote;
            }
        } else if (input->dpadY > 0) {
            selectedNote = (selectedNote + 1) % MAX_NOTES;
            if (selectedNote >= scrollOffset + 5) {
                scrollOffset = selectedNote - 4;
            }
        }
        
        // A button to add new note or edit existing
        if (input->pressed) {
            if (input->plusButton) {
                // Add new note
                for (int i = 0; i < MAX_NOTES; i++) {
                    if (!notes[i].active) {
                        notes[i].active = true;
                        strcpy(notes[i].title, "New Note");
                        strcpy(notes[i].content, "Edit this note...");
                        selectedNote = i;
                        break;
                    }
                }
            }
        }
        
        // Minus button to delete note
        if (input->minusButton) {
            if (notes[selectedNote].active) {
                notes[selectedNote].active = false;
                notes[selectedNote].title[0] = '\0';
                notes[selectedNote].content[0] = '\0';
            }
        }
    }
}

void renderNotes() {
    // Draw title
    drawText(260, 30, "Notes", COLOR_WHITE, 2.0f);
    
    // Draw notes list
    int displayCount = 0;
    for (int i = scrollOffset; i < MAX_NOTES && displayCount < 5; i++) {
        if (notes[i].active) {
            float y = 80 + (displayCount * 70);
            
            // Glass container
            u32 bgColor = (i == selectedNote) ? COLOR_GLASS_MEDIUM : COLOR_GLASS_LIGHT;
            drawGlassRectangle(50, y, 540, 60, bgColor);
            
            // Note title
            drawText(60, y + 10, notes[i].title, COLOR_CYAN, 1.2f);
            
            // Note content preview (first 50 chars)
            char preview[52];
            strncpy(preview, notes[i].content, 50);
            preview[50] = '\0';
            if (strlen(notes[i].content) > 50) {
                strcat(preview, "...");
            }
            drawText(60, y + 35, preview, COLOR_WHITE, 0.8f);
            
            displayCount++;
        }
    }
    
    // Instructions
    drawText(80, 420, "D-Pad: Navigate | +: Add Note | -: Delete | B: Back", COLOR_WHITE, 0.9f);
    
    // Show scroll indicator if needed
    if (scrollOffset > 0) {
        drawText(600, 100, "^", COLOR_WHITE, 2.0f);
    }
    int activeCount = 0;
    for (int i = 0; i < MAX_NOTES; i++) {
        if (notes[i].active) activeCount++;
    }
    if (scrollOffset + 5 < activeCount) {
        drawText(600, 380, "v", COLOR_WHITE, 2.0f);
    }
}
