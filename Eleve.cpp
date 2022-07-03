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


struct _NiveauDonjon {
    string Map;
    string Map1 =
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M       MMM                            M"
        "M       MMMM                           M"
        "M                                      M"
        "M        M       MMMM                  M"
        "M       MM       MMMM                  M"
        "M      MMMM                            M"
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";
    string Map2 =
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M                                      M"
        "M       MMM                            M"
        "M                                      M"
        "M                                      M"
        "M        M       MMMM                  M"
        "M       MMM                            M"
        "M       MMM                            M"
        "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";

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
        "[ KKK   ]"
        "[K  CK  ]"
        "[ KKK   ]"
        "[  K   K]"
        "[  KKKK ]"
        "[  K    ]"
        "[  KKKKK]"
        "[    KK ]"
        "[      K]"
        ;
    string textureAccrocheGauche =
        "[   KKK ]"
        "[  KC  K]"
        "[   KKK ]"
        "[K   K  ]"
        "[ KKKK  ]"
        "[    K  ]"
        "[KKKKK  ]"
        "[ KK    ]"
        "[K      ]"
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
    string textureMarche = "[  KKK  ]"
        "[ KC CK ]"
        "[  KKK  ]"
        "[   K   ]"
        "[  KKK  ]"
        "[ K K K ]"
        "[K  K  K]"
        "[  K K  ]"
        "[ K   K ]";

    V2 Size;
    int IdTex;
    int textureActuelle;
    int sautRestant = 2;
    int hauteurMax = 0;
    bool tombe = false;
    void setTombe(bool _tombe) {
        tombe = _tombe;
    }
    V2 Pos = V2(120, 60);
    Rectangle getRect() {
        return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    void setHauteurMax(int _hauteurMax){
        hauteurMax = _hauteurMax;
    }
    int getHauteurMax() {
        return hauteurMax;
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
            IdTex = G2D::InitTextureFromString(Size, textureMarche);
            Size = Size * 3; // on peut zoomer la taille du sprite

        }
        else if (Texture == 1) {
            textureActuelle = 1;
            IdTex = G2D::InitTextureFromString(Size, textureAccrocheDroit);
            Size = Size * 3; // on peut zoomer la taille du sprite
        }
        else if (Texture == 2) {
            textureActuelle = 2;
            IdTex = G2D::InitTextureFromString(Size, textureAccrocheGauche);
            Size = Size * 3; // on peut zoomer la taille du sprite
        }
        else if (Texture == 3) {
            textureActuelle = 3;
            IdTex = G2D::InitTextureFromString(Size, textureSaut);
            Size = Size * 3; // on peut zoomer la taille du sprite
        }
    }
};

struct _Mur
{
    string texture =
        "[SGGWSSWS]"
        "[GSSSGGSW]"
        "[GSGGGGSS]"
        "[WSSGGWGG]"
        "[GSGWGGGG]"
        "[SGGSWSGS]"
        "[GGWSGGGS]"
        "[GGGGSSGG]"
        ;
    V2 Size;
    int IdTex;
    V2 Pos;


    _Mur(V2 _Pos) { Pos = _Pos; }
    Rectangle getRect() {
        return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
};




struct GameData {
    
    _NiveauDonjon NiveauDonjon;

    bool Mur(int x, int y) { return NiveauDonjon.Map[(20 - y - 1) * 40 + x] == 'M'; }
    int Lpix = 40;
    int ecran = 0;
    bool appuieSpaceBar = false;
    _Heros Heros;
    _TexturePack TexturePack;
    vector<_Mur> murs = {};
    void setMurs() {
        murs.clear();
        murs.push_back(_Mur(V2(530, 367)));
        murs.push_back(_Mur(V2(48, 535)));
        murs.push_back(_Mur(V2(253, 290)));

        for (_Mur& mur : murs) {
            mur.IdTex = G2D::InitTextureFromString(mur.Size, mur.texture);
            mur.Size = mur.Size * 3; // on peut zoomer la taille du sprite
        }
    }
    GameData() {}
};

GameData G;
bool getTapeUnMur(V2 newPos, V2 Size) {
    for (_Mur& mur : G.murs) {
        if (InterRectRect(Rectangle(newPos.x, newPos.y, newPos.x + Size.x, newPos.y + Size.y), mur.getRect()))
        {
            return true;
        }
    }

    return (G.Mur(newPos.x / 40, newPos.y / 40)) ||
        (G.Mur((newPos.x + Size.x) / 40, (newPos.y + Size.y) / 40)) ||
        (G.Mur((newPos.x) / 40, (newPos.y + Size.y) / 40)) ||
        (G.Mur((newPos.x + Size.x) / 40, (newPos.y) / 40));
}
void gestionSaut() {
    if (G.Heros.getHauteurMax() > 0)
    {
        for (int i = 0; i < 2; i++)
        {
            if (getTapeUnMur(G.Heros.Pos + V2(0,1), G.Heros.Size))
            {
                G.Heros.setHauteurMax(0);
                G.Heros.setTombe(true);
                return;
            }
            
            else {
                G.Heros.Pos.y += 1;
                G.Heros.setHauteurMax(G.Heros.getHauteurMax() - 2);
                if (G.Heros.getHauteurMax() <= 0)
                {
                    G.Heros.setHauteurMax(0);
                    G.Heros.setTombe(true);
                }
            }
        }
    }
    else if (G.Heros.tombe)
    {
        int nbMoove;
        if (G.Heros.textureActuelle == 1 || G.Heros.textureActuelle == 2)
        {
            if (getTapeUnMur(G.Heros.Pos + V2(1, 0), G.Heros.Size) || getTapeUnMur(G.Heros.Pos + V2(-1, 0), G.Heros.Size)) {
                nbMoove = 1;
            }
            else {
                nbMoove = 3;
                G.Heros.setTexture(3);
            }
        }
        else
        {
            nbMoove = 3;
        }
        for (int i = 0; i < nbMoove; i++)
        {
            if (getTapeUnMur(G.Heros.Pos + V2(0,-1), G.Heros.Size))
            {
                G.Heros.setTombe(false);
                G.Heros.setSautRestant(2);
                G.Heros.setTexture(0);
                return;
            }
            else
            {
                G.Heros.Pos.y -=1;
            }
        }
        
    }
    else if (!getTapeUnMur(G.Heros.Pos + V2(0, -1), G.Heros.Size)) {
        G.Heros.setHauteurMax(0);
        G.Heros.setTombe(true);
    }
}
void switchBarreEspace() {

    if (G2D::IsKeyPressed(Key::SPACE) && G.Heros.getSautRestant() > 0 && !G.appuieSpaceBar) {
        G.Heros.setHauteurMax(G.Heros.getHauteurMax() + 100);
        G.Heros.setSautRestant(G.Heros.getSautRestant() - 1);
        G.appuieSpaceBar = !G.appuieSpaceBar;
        G.Heros.setTombe(false);
        if (G.Heros.textureActuelle!=3)
        {
            G.Heros.setTexture(3);
        }

    }
    else if (!G2D::IsKeyPressed(Key::SPACE) && G.appuieSpaceBar) {
        G.appuieSpaceBar = !G.appuieSpaceBar;
    }

}
bool collision(_Heros& heros) {
    if (getTapeUnMur(heros.Pos, heros.Size)) {

        if (G2D::IsKeyPressed(Key::Q))
        {
            heros.Pos.x += 1;
            if (heros.textureActuelle != 2)
            {
                heros.setTexture(2);
            }
        }
        if (G2D::IsKeyPressed(Key::D)) {

        
            heros.Pos.x-=1;
            if (heros.textureActuelle != 1)
            {
                heros.setTexture(1);
            }
        }
        return true;
    }

    return false;
    
}

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
    for (_Mur& mur : G.murs) {
        G2D::DrawRectWithTexture(mur.IdTex, mur.Pos, mur.Size);   
    }

    if (G.Heros.textureActuelle!=0) {
        if (getTapeUnMur(G.Heros.Pos + V2(0, -2), G.Heros.Size))
        {
            G.Heros.setTexture(0);
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
        G.setMurs();
        return 3;
    }
    return 2;
}
int gestion_ecran_jeu() {
    if (G2D::IsKeyPressed(Key::Q)) {
        for (int i = 0; i < 4; i++)
        {
            G.Heros.Pos.x -= 1;
            if (collision(G.Heros)) 
            {
                break;
            }
        }
    }

    if (G2D::IsKeyPressed(Key::D)) {
        for (int i = 0; i < 4; i++)
        {
            G.Heros.Pos.x += 1;
            if (collision(G.Heros))
            {
                break;
            }
        }
    }
    switchBarreEspace();
    gestionSaut();



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
    G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.textureMarche);
    G.Heros.Size = G.Heros.Size * 3; // on peut zoomer la taille du sprite
}
int main(int argc, char* argv[]) {
    G2D::InitWindow(argc, argv, V2(G.Lpix * 40, G.Lpix * 20), V2(100, 100),
        string("Labyrinthe"));

    AssetsInit();

    G2D::Run(Logic, render);
}