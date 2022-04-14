#include <iostream>
#include <time.h>

using namespace std;

//structs;
struct Arma;
struct Inimigo;
struct Jogador;
struct Fase;

//templates;
template <typename SerVivo>
bool morreu(SerVivo servivo);

template <typename Atacante, typename Atacado>
Atacado ataque(Atacante atacante, Atacado atacado);

//funções para o funcionamento do código;
int menu();
void sobre();
void JogarFase(Jogador Player, Fase CampoDasFeras);

//funções para declarar armas, inimigos, jogador e fase;
Arma DeclararArma(int danomin, int danomax);
Inimigo DeclararInimigo(int vida, Arma arma, string nome);
Jogador DeclararJogador(int vidabase, Arma arma, int nivel);
Fase DeclararFase(string nome, Inimigo inimigo1, Inimigo inimigo2, Inimigo inimigo3, Inimigo inimigo4, Inimigo inimigo5);

int main() {
    Arma Graveto = DeclararArma(1, 3);
    Arma EspadaDeMadeira = DeclararArma(2, 6);
    Arma EspadaSimples = DeclararArma(4, 12);
    Arma MachadoSimples = DeclararArma(6, 18);
    Arma EspadaLonga = DeclararArma(8, 24);
    Arma MachadoDuplo = DeclararArma(10, 30);

    Inimigo Goblin = DeclararInimigo(5, Graveto, "Goblin");
    Inimigo LiderGoblin = DeclararInimigo(15, EspadaDeMadeira, "Lider Goblin");
    Inimigo Troll = DeclararInimigo(10, EspadaSimples, "Troll");
    Inimigo LiderTroll = DeclararInimigo(25, MachadoSimples, "Lider Troll");
    Inimigo Orcc = DeclararInimigo(60, EspadaLonga, "Orcc");

    Jogador Player = DeclararJogador(15, MachadoDuplo, 2);

    Fase CampoDasFeras = DeclararFase("Campo das Feras", Goblin, LiderGoblin, Troll, LiderTroll, Orcc);

    bool fim = false;
    srand(time(NULL));

    while (fim == false) {
        int acao = menu();
        switch (acao) {
        case 1:
            JogarFase(Player, CampoDasFeras);
            break;
        case 2:
            sobre();
            system("pause");
            break;
        case 3: 
            system("cls");
            fim = true;
            break;
        default:
            system("cls");
            cout << "Acao invalida.\n";
            system("pause");
            break;
        }
    }
    return 0;
}

struct Arma {
    int DanoMin;
    int DanoMax;
};
struct Inimigo {
    string Nome;
    int Vida;
    Arma arma;
};
struct Jogador {
    int Nivel;
    int Vida;
    Arma arma;
};
struct Fase {
    string Nome;
    Inimigo Inimigo[5];
};

int menu() {
    system("cls");
    int acao;
    cout << "--- MENU ---\n1. Iniciar fase 'Campo das Feras';\n2. Sobre;\n3. Finalizar programa;\nNumero da acao desejada: ";
    cin >> acao;
    return acao;
}
void sobre() {
    system("cls");
    cout << "--- SOBRE ---\n\nTrabalho de structs e templates onde deveriamos desenvolver uma simulacao de batalha de um RPG.\nDesenvolvedor: Nicolas M. M. Fajardo\n05/04/2022 - Itajai, SC.\n\n";
}
void JogarFase(Jogador Player, Fase fase) {
    system("cls");
    cout << "Entrando em " << fase.Nome << endl;
    
    for (int i = 0; i < 5; i++) {
        bool morteinimigo = false;
        while (morteinimigo == false) {
            Player = ataque <Inimigo, Jogador>(fase.Inimigo[i], Player);
            cout << fase.Inimigo[i].Nome << " atacou o jogador que ficou com " << Player.Vida << " de vida.\n";
            fase.Inimigo[i] = ataque <Jogador, Inimigo>(Player, fase.Inimigo[i]);
            cout << "O jogador atacou " << fase.Inimigo[i].Nome << " ao mesmo tempo, que ficou com " << fase.Inimigo[i].Vida << " de vida.\n";

            if (morreu(Player) == true) {
                cout << "O jogador morreu!\n";
                system("pause");
                return;
            }
            if (morreu(fase.Inimigo[i]) == true) {
                cout << fase.Inimigo[i].Nome << " foi morto!\n\n";
                morteinimigo = true;
            }
        }
    }
    system("pause");
}

Arma DeclararArma(int danomin, int danomax) {
    Arma arma;
    arma.DanoMax = danomax;
    arma.DanoMin = danomin;
    return arma;
}
Inimigo DeclararInimigo(int vida, Arma arma, string nome) {
    Inimigo inimigo;
    inimigo.Nome = nome;
    inimigo.Vida = vida;
    inimigo.arma = arma;
    return inimigo;
}
Jogador DeclararJogador(int vidabase, Arma arma, int nivel) {
    Jogador jogador;
    jogador.arma = arma;
    jogador.Vida = vidabase * nivel;
    jogador.Nivel = nivel;
    return jogador;
}
Fase DeclararFase(string nome, Inimigo inimigo1, Inimigo inimigo2, Inimigo inimigo3, Inimigo inimigo4, Inimigo inimigo5) {
    Fase fase;
    fase.Nome = nome;
    fase.Inimigo[0] = inimigo1;
    fase.Inimigo[1] = inimigo2;
    fase.Inimigo[2] = inimigo3;
    fase.Inimigo[3] = inimigo4;
    fase.Inimigo[4] = inimigo5;
    return fase;
}

template <typename SerVivo>
bool morreu(SerVivo servivo) {
    if (servivo.Vida <= 0)
        return true;
    return false;
}
template <typename Atacante, typename Atacado>
Atacado ataque(Atacante atacante, Atacado atacado) {
    atacado.Vida -= rand() % (atacante.arma.DanoMax - atacante.arma.DanoMin + 1) + atacante.arma.DanoMin;
    return atacado;
}

void bbsort(int *aw, int N) {
    int i, j, aux;
    for (i = N - 1; i >= 1; i--) {
        for (j = 0; j < i; j++) {
            if (aw[i] < aw[j]) {
                aux = aw[i];
                aw[i] = aw[j];
                aw[j] = aux;
            }
        }
    }
}