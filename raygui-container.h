/**
 * raygui-container
 * 
 * Dependencies:
 *   - raylib
 *   - raygui
 */

#ifndef RAYGUI_CONTAINER_H_
#define RAYGUI_CONTAINER_H_

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// TODO: Switch to a better memory management system.
#define RAYGUI_CONTAINER_MAX_ELEMENTS 100

typedef struct GuiElement {
    GuiControl controlType;
    Rectangle bounds;
    const char *text;
    bool stateBool;
} GuiElement;

typedef struct GuiContainer {
    GuiElement* elements;
    GuiElement* activeElement;
} GuiContainer;

enum GuiDirection {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

GuiContainer InitGuiContainer();
void UnloadGuiContainer(GuiContainer container);
void UpdateGuiContainer(GuiContainer* container);
GuiElement* AddGuiButton(GuiContainer container, Rectangle bounds, const char* text);
GuiElement* AddCheckBox(GuiContainer container, Rectangle bounds, const char* text, bool checked);
bool IsGuiButtonPressed(GuiContainer container, GuiElement* element);
bool IsGuiCheckBoxChecked(GuiContainer container, GuiElement* element);

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif  // RAYGUI_CONTAINER_H_

#ifdef RAYGUI_CONTAINER_IMPLEMENTATION
#ifndef RAYGUI_CONTAINER_IMPLEMENTATION_ONCE
#define RAYGUI_CONTAINER_IMPLEMENTATION_ONCE

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

#include <math.h> // sqrtf

GuiContainer InitGuiContainer() {
    GuiContainer container = (GuiContainer){};
    container.elements = (struct GuiElement*)MemAlloc(RAYGUI_CONTAINER_MAX_ELEMENTS * sizeof(struct GuiElement));
    container.activeElement = container.elements;
    return container;
}

Vector2 GetGuiElementCenter(GuiElement* element) {
    Rectangle bounds = element->bounds;
    return (Vector2){bounds.x + bounds.width / 2, bounds.y + bounds.height / 2};
}

bool IsGuiElementSelectable(GuiElement* element) {
    switch (element->controlType) {
        case BUTTON:
        case CHECKBOX:
            return true;
    }
    return false;
}

bool IsGuiElementPressed(GuiContainer* container, GuiElement* element) {
    // If the button is not active, it's not being pushed.
    if (container->activeElement != element) {
        return false;
    }

    // Keyboard or Gamepad
    // TODO: Allow changing keyboard/gamepad controls?
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        return true;
    }

    // Mouse Button
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mousePosition, element->bounds)) {
            return true;
        }
    }
}

void SetNextActiveGuiElement(GuiContainer* container, int direction) {
    GuiElement* active = container->activeElement;

    Vector2 activeCenter = GetGuiElementCenter(active);
    float targetDistance = 99999999; // The furthest away.
    GuiElement* target = NULL;

    // Find the closest selectable element in the desired direction.
    for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
        GuiElement* current = container->elements + i;

        // The current element already has focus.
        if (current == container->activeElement) {
            continue;
        }

        // Only focus on selectable elements.
        if (!IsGuiElementSelectable(current)) {
            continue;
        }

        // Determine if the element is in the desired direction.
        bool checkIt = false;
        switch (direction) {
            case UP:
                checkIt = current->bounds.y < active->bounds.y;
                break;
            case DOWN:
                checkIt = current->bounds.y > active->bounds.y;
                break;
            case LEFT:
                checkIt = current->bounds.x < active->bounds.x;
                break;
            case RIGHT:
                checkIt = current->bounds.x > active->bounds.x;
                break;
        }
        if (!checkIt) {
            continue;
        }

        // Find the closest element, based on the magnitude of the center of the elements.
        Vector2 currentCenter = GetGuiElementCenter(current);
        Vector2 distance = (Vector2){activeCenter.x - currentCenter.x, activeCenter.y - currentCenter.y};
        float currentDistance = sqrtf(distance.x * distance.x + distance.y * distance.y);
        if (currentDistance < targetDistance) {
            target = current;
            targetDistance = currentDistance;
        }
    }

    if (target != NULL) {
        container->activeElement = target;
    }
}

void UpdateGuiContainer(GuiContainer* container) {
    if (IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
        SetNextActiveGuiElement(container, UP);
        return;
    }
    else if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
        SetNextActiveGuiElement(container, DOWN);
        return;
    }
    else if (IsKeyPressed(KEY_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
        SetNextActiveGuiElement(container, RIGHT);
        return;
    }
    else if (IsKeyPressed(KEY_LEFT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
        SetNextActiveGuiElement(container, LEFT);
        return;
    }

    // Switch the active element if the mouse moved.
    Vector2 mouseMoved = GetMouseDelta();
    if (mouseMoved.x != 0 && mouseMoved.y != 0) {
        Vector2 mousePosition = GetMousePosition();
        for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
            GuiElement* current = container->elements + i;
            if (IsGuiElementSelectable(current)) {
                if (CheckCollisionPointRec(mousePosition, current->bounds)) {
                    container->activeElement = current;
                    return;
                }
            }
        }
    }

    // Allow Tab Indexing
    if (IsKeyPressed(KEY_TAB)) {
        // Find the index of active element.
        int index = -1;
        for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
            if (container->activeElement == container->elements + i) {
                index = i;
                break;
            }
        }

        // Find the next selectable element after the current one.
        while (++index < RAYGUI_CONTAINER_MAX_ELEMENTS) {
            GuiElement* current = container->elements + index;
            if (IsGuiElementSelectable(current)) {
                container->activeElement = current;
                break;
            }
        }

        // If it reached the end, loop back to the beginning.
        if (index == RAYGUI_CONTAINER_MAX_ELEMENTS) {
            container->activeElement = container->elements;
        }
    }

    // Update the state of any elements.
    switch (container->activeElement->controlType) {
        case CHECKBOX:
            if (IsGuiElementPressed(container, container->activeElement)) {
                container->activeElement->stateBool = !container->activeElement->stateBool;
            }
            break;
        case BUTTON:
            // Nothing. The state is checked with IsGuiButtonPressed()
            break;
    }
}

void UnloadGuiContainer(GuiContainer container) {
    MemFree(container.elements);
}

GuiElement* AddElement(GuiContainer* container, GuiElement element) {
    for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
        GuiElement* current = container->elements + i;
        if (current->controlType == DEFAULT) {
            current->bounds = element.bounds;
            current->controlType = element.controlType;
            current->stateBool = element.stateBool;
            current->text = element.text;
            return current;
        }
    }

    return NULL;
}

GuiElement* AddGuiButton(GuiContainer container, Rectangle bounds, const char* text) {
    GuiElement element;
    element.bounds = bounds;
    element.text = text;
    element.controlType = BUTTON;
    return AddElement(&container, element);
}

GuiElement* AddCheckBox(GuiContainer container, Rectangle bounds, const char* text, bool checked) {
    GuiElement element;
    element.bounds = bounds;
    element.text = text;
    element.controlType = CHECKBOX;
    element.stateBool = checked;
    return AddElement(&container, element);
}

void DrawGuiContainer(GuiContainer container) {
    for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
        GuiElement* element = container.elements + i;

        // Flip raygui's GUI focus state based on the active element.
        if (container.activeElement == element) {
            guiState = STATE_FOCUSED;
        }
        else {
            guiState = STATE_NORMAL;
        }

        // Display the element.
        switch (element->controlType) {
            case BUTTON:
                GuiButton(element->bounds, element->text);
                break;
            case CHECKBOX:
                GuiCheckBox(element->bounds, element->text, element->stateBool);
                break;
        }
    }
}

bool IsGuiButtonPressed(GuiContainer container, GuiElement* element) {
    return IsGuiElementPressed(&container, element);
}

bool IsGuiCheckBoxChecked(GuiContainer container, GuiElement* element) {
    return element->stateBool;
}

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif // RAYGUI_CONTAINER_IMPLEMENTATION_ONCE
#endif // RAYGUI_CONTAINER_IMPLEMENTATION