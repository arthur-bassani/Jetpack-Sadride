#include "raylib.h"
#include "sadride.h"

typedef struct{ //posicoes do mouse para detectar botoes
    int x; 
    int y;
}mouse_t;

int main(){ //funcao main para testar menu.c | apagar depois!
    tela_inicial();
    return 0;
}

int tela_inicial(){
    InitWindow(JANELA_X, JANELA_Y, "Jetpack Sadride");
    mouse_t mouse;

    EnableCursor(); 
    ShowCursor();
    
    char titulo[]= "Jetpack Sadride Demo";

    while (!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(10,10);
        DrawRectangleLines(10, 10, JANELA_X-20, JANELA_Y-20, WHITE);

        DrawText(titulo, JANELA_X / 2 - (MeasureText(titulo, 26)/2), JANELA_Y/2 - 60, 26, WHITE);
        //formula para centralizar texto: (metade da tela / 2) - (tamanho texto / 2)
        
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            mouse.x = GetMouseX();                                 
            mouse.y = GetMouseY();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
