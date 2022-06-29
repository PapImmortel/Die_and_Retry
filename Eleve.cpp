#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "glut.h" 

using namespace std;

#define ECRAN_ACCUEIL 0
#define ECRAN_OPTIONS 1
#define INIT_PARTIE 2
#define ECRAN_JEU 3
#define ECRAN_GAME_OVER 4
#define ECRAN_WIN 5

#define SCORE_MOMIE 200
#define SCORE_DIAMOND 150

#define COMPTEUR_TRAP 50
#define MIN_TRAP 70

struct _NiveauDonjon {
    string Map;
    string Map1 =
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M           MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MMM MMM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M       M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "MMM M M MMM M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M M     M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M MMM MMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M  M      MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M  M M MM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M  M M M  MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MM M MMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M    M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        ;
    string Map2 =
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M           MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MMM MMM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M       M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "MMM M M MMM M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M M     M MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M MMM MMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M   M  M      MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M  M M MM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M  M M M  MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MM M MMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M    M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M M MMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M M      M    MMMMMMMMMMMMMMMMMMMMMMMMMM";

    int niveau;
    void setNiveau(int _niveau) { niveau = _niveau; }
    int getNiveau() { return niveau; }
    void setTexture() {
        if (niveau == 1)
        {
            Map = Map1;
        }
        else if (niveau == 2)
        {
            Map = Map2;
        }
    }


};

struct Rectangle {
    int xMin, xMax, yMin, yMax;
    Rectangle(int _xMin, int _yMin, int _xMax, int _yMax) {
        xMin = _xMin, xMax = _xMax, yMin = _yMin, yMax = _yMax;
    }

    V2 getCoordonneeMin() { return V2(xMin, yMin); }
    V2 getCoordonneeMax() { return V2(xMax, yMax); }
};

struct _TexturePack {
    string textureMur = "[SGGWSSWS]"
        "[GSSSGGSW]"
        "[GSGGGGSS]"
        "[WSSGGWGG]"
        "[GSGWGGGG]"
        "[SGGSWSGS]"
        "[GGWSGGGS]"
        "[GGGGSSGG]"
        ;
    string textureSol = "[LLFJLLFJ]"
        "[LLFJLFFJ]"
        "[LFFJLFFF]"
        "[LFJJLFFJ]"
        "[FFJJLFJJ]"
        "[LFJJLFJJ]"
        "[LFFJLFFJ]"
        "[LLFJLFFJ]"
        ;

    V2 Size;
    int IdTexMur;
    int IdTexSol;
};
bool InterRectRect(Rectangle R1, Rectangle R2) {
    if (R1.yMax < R2.yMin) {
        return false;
    }
    if (R1.yMin > R2.yMax) {
        return false;
    }
    if (R1.xMin > R2.xMax) {
        return false;
    }
    if (R1.xMax < R2.xMin) {
        return false;
    }
    return true;
};


struct _Heros {
    int xMin, xMax, yMin, yMax, width;
    string textureAccrocheDroit = 
        "[   KKK ]"
        "[  K  CK]"
        "[   KKK ]"
        "[    K K]"
        "[    KK ]"
        "[    K  ]"
        "[    KKK]"
        "[    K  ]"
        "[     K ]"
        ;
    string textureAccrocheGauche =
        "[ KKK   ]"
        "[KC  K  ]"
        "[ KKK   ]"
        "[K K    ]"
        "[ KK    ]"
        "[  K    ]"
        "[KKK    ]"
        "[  K    ]"
        "[ K     ]"
        ;
    string textureSaut = "[  KKK  ]"
        "[ KC CK ]"
        "[  KKK  ]"
        "[K  K  K]"
        "[ KKKKK ]"
        "[   K   ]"
        "[   K   ]"
        "[  K K  ]"
        "[ K   K ]";

    V2 Size;
    int IdTex;
    int textureActuelle;
    int sautRestant = 2;
    
    V2 Pos = V2(45, 45);
    Rectangle getRect() {
        return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    void setSautRestant(int _SautRestant) {
        sautRestant = _SautRestant;
    }
    int getSautRestant() {
        return sautRestant;
    }
    void setTexture(int Texture) {
        
        if (Texture==0) {

            textureActuelle = 0;
            IdTex = G2D::InitTextureFromString(Size, textureSaut);
            Size = Size * 2; // on peut zoomer la taille du sprite

        }
        else if (Texture == 1) {
            textureActuelle = 1;
            IdTex = G2D::InitTextureFromString(Size, textureAccrocheDroit);
            Size = Size * 2; // on peut zoomer la taille du sprite
        }
        else if (Texture == 2) {
            textureActuelle = 2;
            IdTex = G2D::InitTextureFromString(Size, textureAccrocheGauche);
            Size = Size * 2; // on peut zoomer la taille du sprite
        }
    }
};





struct GameData {
    
    _NiveauDonjon NiveauDonjon;

    bool Mur(int x, int y) { return NiveauDonjon.Map[(20 - y - 1) * 40 + x] == 'M'; }
    int Lpix = 40;
    int ecran = 0;
    _Heros Heros;
    _TexturePack TexturePack;
    GameData() {}
};

GameData G;


void affichage_ecran_accueil() {
    G2D::DrawStringFontMono(V2(50, 400), "Bienvenue dans le jeu du labyrinthe !",
        20, 4, Color::White);
    G2D::DrawStringFontMono(V2(80, 300), "Appuyez sur ENTER pour continuer.", 20,
        3, Color::Green);
}

void affichage_ecran_options() {
    G2D::DrawStringFontMono(V2(100, 500), "Choisissez votre difficulte !", 23, 3,
        Color::White);
    G2D::DrawStringFontMono(V2(50, 300),
        "Appuyez sur A pour lancer le mode FACILE", 16, 3,
        Color::Green);
    G2D::DrawStringFontMono(V2(50, 250),
        "Appuyez sur B pour lancer le mode MOYEN", 16, 3,
        Color::Yellow);
    G2D::DrawStringFontMono(V2(50, 200),
        "Appuyez sur C pour lancer le mode DIFFICILE", 16, 3,
        Color::Red);

    G2D::DrawStringFontMono(
        V2(10, 100), "Pour tirer avec le pistolet, appuyer sur la touche b", 16,
        3, Color::Cyan);
}

void affichage_init_partie() {
    G2D::DrawStringFontMono(V2(220, 100), "Appuyez sur ENTER", 16, 3, Color::Cyan);
    G2D::DrawCircle(V2(150, 250), 50, Color::Green);
    G2D::DrawCircle(V2(450, 450), 30, Color::Cyan);
    G2D::DrawCircle(V2(250, 550), 60, Color::Blue);
    G2D::DrawCircle(V2(500, 50), 30, Color::Magenta);
}

void affichage_ecran_jeu() {
    for (int x = 0; x < 40; x++)
        for (int y = 0; y < 20; y++) {
            int xx = x * G.Lpix - 6;
            int yy = y * G.Lpix;
            if (G.Mur(x, y))
                G2D::DrawRectWithTexture(G.TexturePack.IdTexMur, V2(xx, yy),
                    G.TexturePack.Size);
            else {
                G2D::DrawRectWithTexture(G.TexturePack.IdTexSol, V2(xx, yy),
                    G.TexturePack.Size);
            }
        }

    
    G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);

    
}

void affichage_ecran_game_over() {
    G2D::DrawStringFontMono(V2(70, 500), "Game over", 80, 10, Color::Red);

    G2D::DrawStringFontMono(V2(50, 200),
        "Appuyez sur ENTER pour faire une autre partie.", 16,
        3, Color::Green);
}
void affichage_ecran_win() {
    G2D::DrawStringFontMono(V2(70, 500), "You WIN !!!!", 80, 10, Color::Green);
    G2D::DrawStringFontMono(V2(50, 200),
        "Appuyez sur ENTER pour faire une autre partie.", 16,
        3, Color::White);
}
void render() {
    G2D::ClearScreen(Color::Black);
    if (G.ecran == ECRAN_ACCUEIL) {
        affichage_ecran_accueil();
    }
    if (G.ecran == ECRAN_OPTIONS) {
        affichage_ecran_options();
    }
    if (G.ecran == INIT_PARTIE) {
        affichage_init_partie();
    }
    if (G.ecran == ECRAN_JEU) {
        affichage_ecran_jeu();
    }
    if (G.ecran == ECRAN_GAME_OVER) {
        affichage_ecran_game_over();
    }
    if (G.ecran == ECRAN_WIN) {
        affichage_ecran_win();
    }
    G2D::Show();
}


int gestion_ecran_accueil() {
    if (G2D::IsKeyPressed(Key::ENTER)) {
        return 1;
    }
    return 0;
}
int gestion_ecran_options() {
    // * facile
    if (G2D::IsKeyPressed(Key::A)) {
        return 2;
    }
    return 1;
}

int InitPartie() {
    
    if (G2D::IsKeyPressed(Key::ENTER)) {
        
        return 3;
    }
    return 2;
}
int gestion_ecran_jeu() {
    if (G2D::IsKeyPressed(Key::Q)) {
        G.Heros.Pos.x-=4;
    }

    if (G2D::IsKeyPressed(Key::D)) {
        G.Heros.Pos.x+=4;
    }
    if (G2D::IsKeyPressed(Key::SPACE)) {
        G.Heros.Pos.y += 4;
    }


    return 3;
}
int gestion_ecran_game_over() {
    if (G2D::IsKeyPressed(Key::ENTER)) {
        return 1;
    }
    return 4;
}
int gestion_ecran_win() {
    if (G2D::IsKeyPressed(Key::ENTER)) {
        return 1;
    }
    return 5;
}
void Logic() {
    if (G.ecran == ECRAN_ACCUEIL) {
        G.ecran = gestion_ecran_accueil();
    }

    if (G.ecran == ECRAN_OPTIONS) {
        G.ecran = gestion_ecran_options();
    }
    if (G.ecran == INIT_PARTIE) {
        G.ecran = InitPartie();
    }
    if (G.ecran == ECRAN_JEU) {
        G.ecran = gestion_ecran_jeu();
    }

    if (G.ecran == ECRAN_GAME_OVER) {
        G.ecran = gestion_ecran_game_over();
    }
    if (G.ecran == ECRAN_WIN) {
        G.ecran = gestion_ecran_win();
    }
}

void AssetsInit() {
    G.NiveauDonjon.setNiveau(1);
    G.NiveauDonjon.setTexture();

    G.TexturePack.IdTexMur =
        G2D::InitTextureFromString(G.TexturePack.Size, G.TexturePack.textureMur);
    G.TexturePack.IdTexSol =
        G2D::InitTextureFromString(G.TexturePack.Size, G.TexturePack.textureSol);
    G.TexturePack.Size =
        G.TexturePack.Size * 5; // on peut zoomer la taille du sprite
    G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.textureAccrocheDroit);
    G.Heros.Size = G.Heros.Size * 5; // on peut zoomer la taille du sprite
}
int main(int argc, char* argv[]) {
    G2D::InitWindow(argc, argv, V2(G.Lpix * 40, G.Lpix * 20), V2(100, 100),
        string("Labyrinthe"));

    AssetsInit();

    G2D::Run(Logic, render);
}