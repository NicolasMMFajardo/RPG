#include <iostream>
#include <time.h>

using namespace std;

//structs;
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
    int Vida;
    int PosX;
    int PosY;
    Arma arma;
};
struct Bloco {
    bool bloqueado;
    bool inimigo;
    Inimigo* ponteiroInimigo;
};
struct Mapa {
    int Altura;
    int Largura;
    Bloco** Matriz;
};
struct Fase {
    string Nome;
    Mapa mapa;
    int nInimigos;
    Inimigo* Inimigos;
};

//templates;
template <typename SerVivo>
bool morreu(SerVivo servivo);

template <typename Atacante, typename Atacado>
Atacado ataque(Atacante atacante, Atacado atacado);

//funções para o funcionamento do código;
int menu();
void sobre();
Jogador batalha(Jogador Player, Inimigo& inimigo);
void Movimentar(Jogador Player, Fase fase);
void imprimirMatriz(char** matriz, Fase fase);
char** CriarMatriz(char** matriz, Fase fase);
void ganhou();
bool busca(char** matriztela, int i, int j, int er, int ec);

//Funções para criação;
Mapa CriarMapa(int Altura, int Largura);
Fase CriarFase(string nome, int numInimigos, Inimigo* inimigos, int alturaMapa, int larguraMapa);

int main() {
    Arma Graveto = { 1, 3 };
    Arma EspadaDeMadeira = { 2, 6 };
    Arma EspadaSimples = { 4, 12 };
    Arma MachadoSimples = { 6, 18 };
    Arma EspadaLonga = { 8, 24 };
    Arma MachadoDuplo = { 10, 30 };

    Inimigo Goblin = { "Goblin", 5, Graveto };
    Inimigo LiderGoblin = { "Lider Goblin", 15, EspadaDeMadeira };
    Inimigo Troll = { "Troll", 10, EspadaSimples };
    Inimigo LiderTroll = { "Lider Troll", 25, MachadoSimples };
    Inimigo Orcc = { "Orcc", 60, EspadaLonga };
    Inimigo VetInimigos[5] = { Goblin, LiderGoblin, Troll, LiderTroll, Orcc };

    Jogador Player = { 100, 0, 0, MachadoDuplo };

    bool fim = false;
    srand(time(NULL));

    while (fim == false) {
        int acao = menu();
        Fase CampoDasFeras = CriarFase("Campo das Feras", 5, VetInimigos, 10, 10);

        switch (acao) {
        case 1:

            Movimentar(Player, CampoDasFeras);
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

int menu() {
    system("cls");
    int acao;
    cout << "--- MENU ---\n1. Iniciar fase 'Campo das Feras';\n2. Sobre;\n3. Finalizar programa;\nNumero da acao desejada: ";
    cin >> acao;
    return acao;
}

void sobre() {
    system("cls");
    cout << "--- SOBRE ---\n\nTrabalho de ponteiros, recursividade e templates onde deveriamos desenvolver um  RPG com uma pequena gameplay.\nDesenvolvedor: Nicolas M. M. Fajardo\n31/05/2022 - Itajai, SC.\n\n";
}

Jogador batalha(Jogador Player, Inimigo& inimigo) {
    system("cls");

    bool morteinimigo = false;
    while (morteinimigo == false) {
        Player = ataque <Inimigo, Jogador>(inimigo, Player);
        cout << inimigo.Nome << " atacou o jogador que ficou com " << Player.Vida << " de vida.\n";
        inimigo = ataque <Jogador, Inimigo>(Player, inimigo);
        cout << "O jogador atacou " << inimigo.Nome << " ao mesmo tempo, que ficou com " << inimigo.Vida << " de vida.\n";

        if (morreu<Jogador>(Player) == true) {
            cout << "O jogador morreu!\n";
            system("pause");
            return Player;
        }
        if (morreu<Inimigo>(inimigo) == true) {
            cout << inimigo.Nome << " foi morto!\n\n";
            morteinimigo = true;
        }
    }
    return Player;
}

void Movimentar(Jogador Player, Fase fase) {
    char** matriztela = new char* [fase.mapa.Altura];
    for (int i = 0; i < fase.mapa.Altura; i++) {
        matriztela[i] = new char[fase.mapa.Largura];
    }
    matriztela = CriarMatriz(matriztela, fase);

    bool check = false;
    matriztela[Player.PosX][Player.PosY] = '@';

    int i = 0;
    while (true) {
        system("cls");

        if (fase.mapa.Matriz[Player.PosX][Player.PosY].inimigo == true) {
            Player = batalha(Player, *fase.mapa.Matriz[Player.PosX][Player.PosY].ponteiroInimigo);
            if (morreu<Inimigo>(*fase.mapa.Matriz[Player.PosX][Player.PosY].ponteiroInimigo) == true) {
                fase.mapa.Matriz[Player.PosX][Player.PosY].inimigo = false;
                i++;
            }
            if (morreu<Jogador>(Player) == true) {
                system("pause");
                return;
            }
        }

        if (i == fase.nInimigos && (Player.PosX == fase.mapa.Altura - 1) && (Player.PosY == fase.mapa.Largura - 1)) {
            ganhou();
            return;
        }
        imprimirMatriz(matriztela, fase);

        cout << "\nEntre com a movimentacao desejada\nCima - w\nBaixo - s\nEsquerda - a\nDireita - d\n\nAcao: ";
        char mov;
        cin >> mov;
        switch (mov) {
        case 'w':
            if (Player.PosX > 0 && fase.mapa.Matriz[Player.PosX - 1][Player.PosY].bloqueado == false) {
                Player.PosX--;
                matriztela[Player.PosX][Player.PosY] = '@';
                matriztela[Player.PosX + 1][Player.PosY] = '-';
            }
            break;
        case 's':
            if (Player.PosX < fase.mapa.Altura - 1 && fase.mapa.Matriz[Player.PosX + 1][Player.PosY].bloqueado == false) {
                Player.PosX++;
                matriztela[Player.PosX][Player.PosY] = '@';
                matriztela[Player.PosX - 1][Player.PosY] = '-';
            }
            break;
        case 'a':
            if (Player.PosY > 0 && fase.mapa.Matriz[Player.PosX][Player.PosY - 1].bloqueado == false) {
                Player.PosY--;
                matriztela[Player.PosX][Player.PosY] = '@';
                matriztela[Player.PosX][Player.PosY + 1] = '-';
            }
            break;
        case 'd':
            if (Player.PosY < fase.mapa.Largura - 1 && fase.mapa.Matriz[Player.PosX][Player.PosY + 1].bloqueado == false) {
                Player.PosY++;
                matriztela[Player.PosX][Player.PosY] = '@';
                matriztela[Player.PosX][Player.PosY - 1] = '-';
            }
            break;
        }
    }
}

char** CriarMatriz(char** matriz, Fase fase) {
    for (int i = 0; i < fase.mapa.Altura; i++) {
        for (int j = 0; j < fase.mapa.Largura; j++) {
            if (fase.mapa.Matriz[i][j].bloqueado == true) {
                matriz[i][j] = 'o';
            }
            else if (fase.mapa.Matriz[i][j].inimigo == true) {
                matriz[i][j] = 'x';
            }
            else {
                matriz[i][j] = '-';
            }
        }
    }
    return matriz;
}

void imprimirMatriz(char** matriz, Fase fase) {
    cout << "--- Campo das Feras ---\n\n - : Caminho\n x : Inimigo\n o : Obstaculo\n @ : Jogador\n\n";
    for (int i = 0; i < fase.mapa.Altura; i++) {
        for (int j = 0; j < fase.mapa.Largura; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

void ganhou() {
    cout << "            Você Ganhou!\n\n";
    cout << "             ___________" << endl;
    cout << "            '._==_==_=_.'" << endl;
    cout << "            .-\\:       /-." << endl;
    cout << "           | (|:.     |) |" << endl;
    cout << "            '-|:. 1   |-'" << endl;
    cout << "              \\::.    /" << endl;
    cout << "               '::. .'" << endl;
    cout << "                 ) (" << endl;
    cout << "               _.' '._" << endl;
    cout << "            `'''''''''''`" << endl;
    system("pause");
}

Mapa CriarMapa(int Altura, int Largura) {
    Mapa novomapa = { Altura, Largura };
    novomapa.Matriz = new Bloco * [novomapa.Altura];
    for (int i = 0; i < novomapa.Altura; i++) {
        novomapa.Matriz[i] = new Bloco[novomapa.Largura];
    }
    for (int i = 0; i < novomapa.Altura; i++) {
        for (int j = 0; j < novomapa.Largura; j++) {
            int r = rand() % 2;
            if (r == 0) {
                novomapa.Matriz[i][j].bloqueado = true;
            }
            else {
                novomapa.Matriz[i][j].bloqueado = false;
            }
        }
    }
    novomapa.Matriz[0][0].bloqueado = false;
    novomapa.Matriz[novomapa.Altura - 1][novomapa.Largura - 1].bloqueado = false;
    return novomapa;
}

Fase CriarFase(string nome, int numInimigos, Inimigo* inimigos, int alturaMapa, int larguraMapa) {
    Fase fase;
    fase.Nome = nome;
    fase.nInimigos = numInimigos;
    fase.Inimigos = new Inimigo[fase.nInimigos];
    fase.Inimigos = inimigos;

    char** matrizChar = new char* [larguraMapa];
    for (int i = 0; i < larguraMapa; i++) {
        matrizChar[i] = new char[alturaMapa];
    }

    int rand_altura;
    int rand_largura;

    bool mapavalido = false;
    while (!mapavalido) {
        fase.mapa = CriarMapa(alturaMapa, larguraMapa);
        matrizChar = CriarMatriz(matrizChar, fase);
        mapavalido = busca(matrizChar, 0, 0, 9, 9);
    }

    for (int i = 0; i < 5; i++) {
        do {
            for (int l = 0; l < larguraMapa; l++) {
                for (int a = 0; a < alturaMapa; a++) {
                    if (fase.mapa.Matriz[l][a].bloqueado) {
                        matrizChar[l][a] = 'o';
                    }
                    else {
                        matrizChar[l][a] = '-';
                    }
                }
            }
            rand_altura = rand() % alturaMapa;
            rand_largura = rand() % larguraMapa;
        } while (((rand_largura == 0 && rand_altura == 0) || (rand_largura == 9 && rand_altura == 9)) ||
            (fase.mapa.Matriz[rand_largura][rand_altura].inimigo ||
                fase.mapa.Matriz[rand_largura][rand_altura].bloqueado) ||
            !busca(matrizChar, 0, 0, rand_largura, rand_altura));

        fase.mapa.Matriz[rand_largura][rand_altura].inimigo = true;
        fase.mapa.Matriz[rand_largura][rand_altura].ponteiroInimigo = &fase.Inimigos[i];
    }
    return fase;
}

bool busca(char** matriztela, int inicioI, int inicioJ, int destinoI, int destinoJ) {
    if (matriztela[inicioI][inicioJ] != '-')
        return false;

    if (inicioI == destinoI && inicioJ == destinoJ)
        return true;

    matriztela[inicioI][inicioJ] = '+';

    if (inicioI != 0 && busca(matriztela, inicioI - 1, inicioJ, destinoI, destinoJ))
        return true;
    if (inicioI != 9 && busca(matriztela, inicioI + 1, inicioJ, destinoI, destinoJ))
        return true;
    if (inicioJ != 0 && busca(matriztela, inicioI, inicioJ - 1, destinoI, destinoJ))
        return true;
    if (inicioJ != 9 && busca(matriztela, inicioI, inicioJ + 1, destinoI, destinoJ))
        return true;

    return false;
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
