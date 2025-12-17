#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700
#define MAX_INPUT 500

// Sample_texts
char *codeTexts[] = {
    "int main() { return 0; }",
    "for(int i=0; i<10; i++) { }",
    "char* str = \"Hello\";",
    "if(x > 0) { sum = x + y; }",
    "#include <stdio.h>"};

char *standardTexts[] = {
    "The quick brown fox jumps over the lazy dog.",
    "Practice makes perfect in everything you do.",
    "Time flies when you are having fun.",
    "Knowledge is power and typing is efficiency.",
    "Every journey begins with a single step."};

char *sprintTexts[] = {
    "the and for are you can have",
    "one two three four five six",
    "cat dog run jump fly sit",
    "yes now try get see use",
    "all new big old red hot"};

// Program state
int currentState = 0; // 0=MODE_SELECT, 1=TESTING, 2=RESULTS
int selectedMode = 0; // 1=Code, 2=Standard, 3=Sprint
char *currentText = NULL;
char userInput[MAX_INPUT] = "";
int inputlen = 0;
time_t startTime = 0;
int testActive = 0;
int wpm = 0;
float accuracy = 0.0f;
int totalWords = 0;
int timeTaken = 0;

// Functions
int countWords(char *text)
{
    int count = 0, inWord = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            inWord = 0;
        }
        else if (inWord == 0)
        {
            inWord = 1;
            count++;
        }
    }
    return count;
}

float calculateAccuracy(char *original, char *typed)
{
    int correct = 0;
    int total = strlen(original);
    int minLen = (strlen(typed) < total) ? strlen(typed) : total;

    for (int i = 0; i < minLen; i++)
    {
        if (original[i] == typed[i])
            correct++;
    }
    return ((float)correct / total) * 100.0f;
}

void Text()
{
    int index = rand() % 5;
    if (selectedMode == 1)
    {
        currentText = codeTexts[index];
    }
    else if (selectedMode == 2)
    {
        currentText = standardTexts[index];
    }
    else
    {
        currentText = sprintTexts[index];
    }
}

void Pre_Test()
{
    testActive = 1;
    startTime = time(NULL);
    inputlen = 0;
    userInput[0] = '\0';
    currentState = 1;
}

void Post_Test()
{
    testActive = 0;
    time_t endTime = time(NULL);
    timeTaken = (int)difftime(endTime, startTime);

    totalWords = countWords(userInput);
    wpm = (timeTaken > 0) ? (totalWords * 60) / timeTaken : 0;
    accuracy = calculateAccuracy(currentText, userInput);

    currentState = 2;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Typing Speed Test\nby Fireteam Forerunner");
    SetTargetFPS(60);

    srand(time(NULL));

    // Colors (plain Windows style)
    Color bgColor = (Color){240, 240, 240, 255};     // Light gry background
    Color windowColor = (Color){255, 255, 255, 255}; // White window
    Color borderColor = (Color){180, 180, 180, 255}; // Gray border
    Color textColor = (Color){0, 0, 0, 255};         // Black text
    Color buttonColor = (Color){240, 240, 240, 255}; // Gray button
    Color buttonHover = (Color){225, 225, 225, 255}; // Lighter gray hover

    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();

        // Input handling for typing
        if (currentState == 1 && testActive)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if (key >= 32 && key <= 125 && inputlen < MAX_INPUT - 1)
                {
                    userInput[inputlen] = (char)key;
                    inputlen++;
                    userInput[inputlen] = '\0';

                    if (inputlen >= strlen(currentText))
                    {
                        finishTest();
                    }
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && inputlen > 0)
            {
                inputlen--;
                userInput[inputlen] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                finishTest();
            }
        }

        BeginDrawing();
        ClearBackground(bgColor);

        // Draw main window
        DrawRectangle(50, 50, 800, 600, windowColor);
        DrawRectangleLines(50, 50, 800, 600, borderColor);

        if (currentState == 0)
        {
            // MODE SELECTION
            DrawText("TYPING SPEED TEST", 280, 85, 28, textColor);
            DrawText("By Fireteam Forerunner", 298, 122, 21, textColor);
            DrawText("Select Test Mode:", 340, 165, 20, textColor);

            // Code Mode button
            Rectangle codeBtn = {100, 220, 200, 100};
            int codeHover = CheckCollisionPointRec(mousePos, codeBtn);
            DrawRectangleRec(codeBtn, codeHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(codeBtn, 1, borderColor);
            DrawText("Code Mode", 135, 250, 20, textColor);
            DrawText("Programming", 137, 280, 16, textColor);

            if (codeHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedMode = 1;
                loadRandomText();
                startTest();
            }

            // Standard Mode button
            Rectangle stdBtn = {350, 220, 200, 100};
            int stdHover = CheckCollisionPointRec(mousePos, stdBtn);
            DrawRectangleRec(stdBtn, stdHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(stdBtn, 1, borderColor);
            DrawText("Standard Mode", 365, 250, 20, textColor);
            DrawText("General Typing", 370, 280, 16, textColor);

            if (stdHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedMode = 2;
                loadRandomText();
                startTest();
            }

            // Sprint Mode button
            Rectangle sprBtn = {600, 220, 200, 100};
            int sprHover = CheckCollisionPointRec(mousePos, sprBtn);
            DrawRectangleRec(sprBtn, sprHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(sprBtn, 1, borderColor);
            DrawText("Sprint Mode", 630, 250, 20, textColor);
            DrawText("Max Speed", 635, 280, 16, textColor);

            if (sprHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selectedMode = 3;
                loadRandomText();
                startTest();
            }
        }
        else if (currentState == 1)
        {
            // TESTING
            int elapsed = (int)difftime(time(NULL), startTime);
            DrawText(TextFormat("Time: %02d:%02d", elapsed / 60, elapsed % 60), 380, 80, 24, textColor);

            // Sample text
            DrawText("Sample Text:", 70, 140, 18, textColor);
            DrawRectangle(70, 170, 760, 100, windowColor);
            DrawRectangleLines(70, 170, 760, 100, borderColor);
            DrawText(currentText, 80, 200, 20, textColor);

            // Input
            DrawText("Your Input:", 70, 300, 18, textColor);
            DrawRectangle(70, 330, 760, 100, windowColor);
            DrawRectangleLines(70, 330, 760, 100, borderColor);
            DrawText(userInput, 80, 360, 20, textColor);

            // Cursor
            if ((elapsed % 2) == 0)
            {
                int cursorX = 80 + MeasureText(userInput, 20);
                DrawRectangle(cursorX, 360, 2, 20, textColor);
            }

            DrawText("Type the text above. Press Enter when done.", 220, 460, 18, textColor);

            // Reset button
            Rectangle resetBtn = {380, 510, 140, 40};
            int resetHover = CheckCollisionPointRec(mousePos, resetBtn);
            DrawRectangleRec(resetBtn, resetHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(resetBtn, 1, borderColor);
            DrawText("Reset", 425, 520, 20, textColor);

            if (resetHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                testActive = 0;
                inputlen = 0;
                userInput[0] = '\0';
                loadRandomText();
                startTest();
            }
        }
        else if (currentState == 2)
        {
            // RESULTS
            DrawText("TEST RESULTS", 340, 100, 28, textColor);

            // Result boxes
            DrawRectangle(100, 180, 150, 120, windowColor);
            DrawRectangleLines(100, 180, 150, 120, borderColor);
            DrawText(TextFormat("%d", wpm), 150, 210, 36, textColor);
            DrawText("WPM", 155, 260, 18, textColor);

            DrawRectangle(280, 180, 150, 120, windowColor);
            DrawRectangleLines(280, 180, 150, 120, borderColor);
            DrawText(TextFormat("%.0f%%", accuracy), 315, 210, 36, textColor);
            DrawText("Accuracy", 305, 260, 18, textColor);

            DrawRectangle(460, 180, 150, 120, windowColor);
            DrawRectangleLines(460, 180, 150, 120, borderColor);
            DrawText(TextFormat("%d", totalWords), 515, 210, 36, textColor);
            DrawText("Words", 505, 260, 18, textColor);

            DrawRectangle(640, 180, 150, 120, windowColor);
            DrawRectangleLines(640, 180, 150, 120, borderColor);
            DrawText(TextFormat("%ds", timeTaken), 685, 210, 36, textColor);
            DrawText("Time", 685, 260, 18, textColor);

            // Feedback
            char *feedback = "Good job! Keep practicing!";
            if (selectedMode == 1 && accuracy >= 95)
                feedback = "Excellent accuracy with code!";
            else if (selectedMode == 3 && wpm >= 60)
                feedback = "Lightning fast speed!";
            else if (selectedMode == 2 && wpm >= 50 && accuracy >= 90)
                feedback = "Excellent performance!";

            DrawRectangle(150, 350, 600, 80, windowColor);
            DrawRectangleLines(150, 350, 600, 80, borderColor);
            DrawText(feedback, 200, 380, 20, textColor);

            // Buttons
            Rectangle tryBtn = {250, 470, 140, 40};
            int tryHover = CheckCollisionPointRec(mousePos, tryBtn);
            DrawRectangleRec(tryBtn, tryHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(tryBtn, 1, borderColor);
            DrawText("Try Again", 270, 480, 18, textColor);

            if (tryHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                loadRandomText();
                startTest();
            }

            Rectangle backBtn = {410, 470, 140, 40};
            int backHover = CheckCollisionPointRec(mousePos, backBtn);
            DrawRectangleRec(backBtn, backHover ? buttonHover : buttonColor);
            DrawRectangleLinesEx(backBtn, 1, borderColor);
            DrawText("Back", 460, 480, 18, textColor);

            if (backHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentState = 0;
                selectedMode = 0;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
