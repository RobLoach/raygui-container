/**
 * raygui-container
 * 
 * Dependencies:
 *   - raylib
 *   - raygui
 */

#ifndef RAYGUI_CONTAINER_H_
#define RAYGUI_CONTAINER_H_

// TODO: Switch to a better memory management system.
#define RAYGUI_CONTAINER_MAX_ELEMENTS 100

typedef struct GuiElement {
    GuiControl controlType;
    Rectangle bounds;
    const char *text;
} GuiElement;

typedef struct GuiContainer {
    GuiElement* elements;
    GuiElement* activeElement;
} GuiContainer;

GuiContainer InitGuiContainer();
GuiElement* AddGuiButton(GuiContainer container, Rectangle bounds, const char* text);
void UnloadGuiContainer(GuiContainer container);
void UpdateGuiContainer(GuiContainer* container);
bool IsGuiButtonPressed(GuiContainer container, GuiElement* element);

#endif  // RAYGUI_CONTAINER_H_
#ifdef RAYGUI_CONTAINER_IMPLEMENTATION
#ifndef RAYGUI_CONTAINER_IMPLEMENTATION_ONCE
#define RAYGUI_CONTAINER_IMPLEMENTATION_ONCE

GuiContainer InitGuiContainer() {
    GuiContainer container = (GuiContainer){};
    container.elements = (struct GuiElement*)MemAlloc(RAYGUI_CONTAINER_MAX_ELEMENTS * sizeof(struct GuiElement));
    container.activeElement = container.elements;
    return container;
}

void UpdateGuiContainerActiveUp(GuiContainer* container) {
    for (int i = 1; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
        if (container->elements + i == container->activeElement) {
            // TODO: Find the first selectable target that is above.
            GuiElement* target = container->activeElement - 1;
            if (target->controlType != DEFAULT) {
                container->activeElement = target;
            }
            break;
        }
    }
}

void UpdateGuiContainerActiveDown(GuiContainer* container) {
    for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS - 1; i++) {
        if (container->elements + i == container->activeElement) {
            // TODO: Find the first selectable target that is below.
            GuiElement* target = container->activeElement + 1;
            if (target->controlType != DEFAULT) {
                container->activeElement = target;
            }
            break;
        }
    }
}

void UpdateGuiContainer(GuiContainer* container) {
    if (IsKeyPressed(KEY_UP)) {
        UpdateGuiContainerActiveUp(container);
    }
    else if (IsKeyPressed(KEY_DOWN)) {
        UpdateGuiContainerActiveDown(container);
    }
}

void UnloadGuiContainer(GuiContainer container) {
    MemFree(container.elements);
}

GuiElement* AddGuiButton(GuiContainer container, Rectangle bounds, const char* text) {
    GuiElement* element = container.elements;
    while (element->controlType != DEFAULT) {
        element++;
    }

    element->bounds = bounds;
    element->controlType = BUTTON;
    element->text = text;

    return element;
}

void DrawGuiContainer(GuiContainer container) {
    for (int i = 0; i < RAYGUI_CONTAINER_MAX_ELEMENTS; i++) {
        GuiElement* element = container.elements + i;

        // Override the active GUI state.
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
        }
    }
}

bool IsGuiButtonPressed(GuiContainer container, GuiElement* element) {
    // If the button is not active, it's not being pushed.
    if (container.activeElement != element) {
        return false;
    }

    // Check Keyboard
    if (IsKeyPressed(KEY_ENTER)) {
        return true;
    }

    return false;
}

#endif // RAYGUI_CONTAINER_IMPLEMENTATION_ONCE
#endif // RAYGUI_CONTAINER_IMPLEMENTATION