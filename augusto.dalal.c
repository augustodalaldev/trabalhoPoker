/**************************************************
*
* Augusto Cesar Dalal Miranda
* Trabalho 1
* Professor(a): Diego Padilha Rubert
*
*/

#include <stdio.h>

#define MAX_C 5

//enum e structs
typedef enum {
    COPAS = 'C',
    ESPADAS = 'E',
    OUROS = 'O',
    PAUS = 'P'
} t_naipe;

typedef struct {
    short valor;
    t_naipe naipe;
} t_carta;

typedef enum {
    PAR = 15,
    DOISPARES,
    TRINCA,
    SEQUENCIA,
    FLUSH,
    FULL,
    QUADRA,
    SFLUSH,
    RFLUSH
} t_valor_m;

typedef struct {
    t_carta cartas[MAX_C];
    t_valor_m valor;
} t_mao;

//ordenação cartas
t_mao insertionSortCards(t_mao mao);

//computar ganhador
int ganhador(t_valor_m vMao1, t_valor_m vMao2);
int countCartas(t_mao mao, int cartaVal);

int rFlush(t_mao mao);
int sFlush(t_mao mao);
int quadra(t_mao mao);
int fHouse(t_mao mao);
int flush(t_mao mao);
int sequencia(t_mao mao);
int trinca(t_mao mao);
int doisPares(t_mao mao);
int par(t_mao mao);

t_valor_m countPontos(t_mao mao);

//definições e conversões
void setNaipe(char n, t_naipe *naipe);
void setVal(char v[3], short *valor);
char convertNaipeChar(t_naipe naipe);
char convertValChar(short val);

//impressão
void imprime(int ganhador, t_mao mao);

int main(){
    t_mao mao1;
    t_mao mao2;

    char v[3];
    char naipe;

    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++){
        //input mao1
        for(int j = 0; j < MAX_C; j++){
            scanf("%s %c", v, &naipe);
            setVal(v, &mao1.cartas[j].valor);
            setNaipe(naipe, &mao1.cartas[j].naipe);
        }

        //input mao2
        for(int j = 0; j < MAX_C; j++){
            scanf("%s %c", v, &naipe);
            setVal(v, &mao2.cartas[j].valor);
            setNaipe(naipe, &mao2.cartas[j].naipe);
        }

        mao1 = insertionSortCards(mao1);
        mao2 = insertionSortCards(mao2);

        mao1.valor = countPontos(mao1);
        mao2.valor = countPontos(mao2);

        int g = ganhador(mao1.valor, mao2.valor);

        if(g == 1) imprime(g, mao1);
        else if(g == 2) imprime(g, mao2);
        else printf("E\n");
    }
    return 0;
}

//ordenação cartas
//OBS: utilizei o método insertion sort visto em sala, com alterações para atender aos critérios do trabalho

t_mao insertionSortCards(t_mao mao){
    int i, j, naipeEMaior;
    t_carta c;
    for (i = 1; i < MAX_C; i++) {
        c = mao.cartas[i];

        naipeEMaior = 1;
        for (j = i - 1; j >= 0 && mao.cartas[j].valor >= c.valor && naipeEMaior == 1; j--){
            //ordenação pelo valor
            if(mao.cartas[j].valor > c.valor) mao.cartas[j+1] = mao.cartas[j];

            //ordenação pelo naipe
            else if(mao.cartas[j].valor == c.valor){
                t_naipe naipe = mao.cartas[j].naipe;
                if(naipe == ESPADAS && c.naipe != ESPADAS){
                    mao.cartas[j+1] = mao.cartas[j];
                }
                else if(naipe == COPAS && c.naipe != ESPADAS && c.naipe != COPAS){
                    mao.cartas[j+1] = mao.cartas[j];
                }
                else if(naipe == OUROS && c.naipe != ESPADAS && c.naipe != COPAS && c.naipe != OUROS){
                    mao.cartas[j+1] = mao.cartas[j];
                }else{
                    //caso em que o naipe em c é menor do que em mao.cartas[j].naipe
                    //e j não pode continuar diminuindo
                    naipeEMaior = 0;
                    //como o j diminuirá logo em seguida, e só depois o programa vai ler naipeEMenor 
                    //eu incremento j para que a indexação fique correta
                    j++;
                }
            }
        }
        mao.cartas[j+1] = c;
    }

    return mao;
}

//computar ganhador
//OBS: todas as verificações são realizadas considerando uma mão ordenada

int ganhador(t_valor_m vMao1, t_valor_m vMao2){
    if(vMao1 > vMao2) return 1;
    if(vMao1 < vMao2) return 2;
    return 0;
}

int countCartas(t_mao mao, int v){
    int k = 0;
    for(int i = 0; i < MAX_C; i++) if(mao.cartas[i].valor == v) k++;
    return k;
}

int rFlush(t_mao mao){
    int rFlush[MAX_C] = {10, 11, 12, 13, 14}, i;
    t_naipe naipe = mao.cartas[0].naipe;

    for(i = 0; i < MAX_C && mao.cartas[i].valor == rFlush[i] && mao.cartas[i].naipe == naipe; i++);
    if(i == MAX_C) return 1;
    return 0;
}

int sFlush(t_mao mao){
    int i, val = mao.cartas[0].valor;
    t_naipe naipe = mao.cartas[0].naipe;

    for(i = 1; i < MAX_C && mao.cartas[i].valor == val+1 && mao.cartas[i].naipe == naipe; i++, val++);
    if(i == MAX_C) return 1;
    
    return 0;
}

int quadra(t_mao mao){
    if(countCartas(mao, mao.cartas[0].valor) == 4) return 1;
    if(countCartas(mao, mao.cartas[1].valor) == 4) return 1;
    return 0;
}

int fHouse(t_mao mao){
    int v = mao.cartas[2].valor;

    if(mao.cartas[1].valor == v || mao.cartas[3].valor == v)
        if(mao.cartas[0].valor == mao.cartas[1].valor)
            if(mao.cartas[4].valor == mao.cartas[3].valor)
                return 1;

    return 0;

}

int flush(t_mao mao){
    int i;
    t_naipe naipe = mao.cartas[0].naipe;

    for(i = 1; i < MAX_C && mao.cartas[i].naipe == naipe; i++);
    if(i == MAX_C) return 1;

    return 0;
}

int sequencia(t_mao mao){
    int i, val = mao.cartas[0].valor;
    for(i = 1; i < MAX_C && mao.cartas[i].valor == val+1; i++, val++);
    if(i == MAX_C) return 1;

    return 0;
}

int trinca(t_mao mao){
    if(countCartas(mao, mao.cartas[2].valor) == 3) return 1;

    return 0;
}

int doisPares(t_mao mao){
    if((mao.cartas[0].valor == mao.cartas[1].valor) && (mao.cartas[2].valor == mao.cartas[3].valor)) return 1;
    
    if((mao.cartas[1].valor == mao.cartas[2].valor) && (mao.cartas[3].valor == mao.cartas[4].valor)) return 1;

    if((mao.cartas[0].valor == mao.cartas[1].valor) && (mao.cartas[3].valor == mao.cartas[4].valor)) return 1;

    return 0;
}

int par(t_mao mao){
    for(int i = 1; i < 4; i++) if(countCartas(mao, mao.cartas[i].valor) == 2) return 1;
    
    return 0;
}

//OBS: preferi dividir a lógica de cada mão em funções individuais prezando pela organização e legibililidade do código.
t_valor_m countPontos(t_mao mao){

    if(rFlush(mao)) return RFLUSH;

    if(sFlush(mao)) return SFLUSH;

    if(quadra(mao)) return QUADRA;

    if(fHouse(mao)) return FULL;

    if(flush(mao)) return FLUSH;

    if(sequencia(mao)) return SEQUENCIA;

    if(trinca(mao)) return TRINCA;

    if(doisPares(mao)) return DOISPARES;

    if(par(mao)) return PAR;

    return mao.cartas[MAX_C-1].valor;
}

//definições e conversões

void setNaipe(char n, t_naipe *naipe){
    if(n == 'C') *naipe = COPAS;
    if(n == 'E') *naipe = ESPADAS;
    if(n == 'O') *naipe = OUROS;
    if(n == 'P') *naipe = PAUS;
}

void setVal(char v[3], short *valor){
    //toda essa lógica devido ao 10 que pode ser colocado como input
    char c = v[0];
    if(c == '1') *valor = 10;
    else if(c == 'J') *valor = 11;
    else if(c == 'Q') *valor = 12;
    else if(c == 'K') *valor = 13;
    else if(c == 'A') *valor = 14;
    // '2' até '9', tabela ascii
    else  *valor = ((int) c) - 48; 
}

char convertNaipeChar(t_naipe naipe){
    if(naipe == COPAS) return 'C';
    if(naipe == ESPADAS) return 'E';
    if(naipe == OUROS) return 'O';
    else return 'P';
}

char convertValChar(short val){
    if(val == 10) return '1';
    if(val == 11) return 'J';
    if(val == 12) return 'Q';
    if(val == 13) return 'K';
    if(val == 14) return 'A';
    else return (char) (val + 48);
}

//impressão

void imprime(int ganhador, t_mao mao){
    printf("%d ", ganhador);
    for(int i = 0; i < MAX_C; i++){
        char val = convertValChar(mao.cartas[i].valor);
        char naipe = convertNaipeChar(mao.cartas[i].naipe); 

        printf("%c", val);
        //verificando se val == 1, para printar 10 na tela
        if(val == '1') printf("0");
        printf(" %c", naipe);
        
        if(i != 4) printf(" ");
    }
    printf("\n");
}
