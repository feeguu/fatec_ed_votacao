#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define maxQtd 50
#define maxNome 60
#define maxSigla 10

typedef struct Pessoa Pessoa;
struct Pessoa
{
    char nome[maxNome]; // nome da pessoa
    int idade;          // idade da pessoa
};

typedef struct Professor Professor;
struct Professor
{
    Pessoa pes;           // assim professor tem nome e idade
    int codigo;           // código do professor
    char depto[maxSigla]; // departamento onde trabalha
};

typedef struct Aluno Aluno;
struct Aluno
{
    Pessoa pes;           // assim aluno tem nome e idade
    int matricula;        // número de matrícula do aluno
    int ano;              // ano no curso.
    char depto[maxSigla]; // departamento onde estuda
};

typedef struct TG TG;
struct TG
{
    int codigo;
    int autor;            // equivalente à matrícula do aluno autor
    int orientador;       // equivalente ao código do professor
    char titulo[maxNome]; // título do TG
    int qtdeVotos;        // somatório dos votos recebidos
};

typedef struct Eleitor Eleitor;
struct Eleitor
{
    char cpf[15];
    bool votou; // true se já votou; false se não votou
    int codigoTG;
};

Professor docentes[maxQtd]; // vetor de professores
int qtdeDocentes;           // qtde de professores no vetor
Aluno formandos[maxQtd];    // vetor de alunos
int qtdeFormandos;          // qtde de alunos no vetor
TG listaPIs[maxQtd];        // vetor de TGs
int qtdeTGs;                // qtde de TGs no vetor
Eleitor comissao[maxQtd];   // vetor de eleitores
int qtdeEleitores;          // qtde de eleitores no veto

int validaCPF(const char *cpf)
{
    int c = 0;
    int digitsCount = 0;

    int digits[11];

    while (cpf[c] != '\0')
    {
        c++;
    }

    // Possui 14 caracteres
    if (c != 14)
    {
        return 0;
    }

    // Respeita xxx.xxx.xxx-xx
    for (int i = 0; i < 14; i++)
    {
        if (i == 3 || i == 7)
        {
            if (cpf[i] != '.')
            {
                return 0;
            }
        }
        else if (i == 11)
        {
            if (cpf[i] != '-')
            {
                return 0;
            }
        }
        else
        {
            if (cpf[i] < '0' || cpf[i] > '9')
            {
                return 0;
            }
            else
            {
                digits[digitsCount++] = cpf[i] - '0';
            }
        }
    }

    int v1 = 0;

    for (int i = 0; i < 9; i++)
    {
        v1 += digits[i] * (10 - i);
    }

    if ((v1 % 11) < 2)
    {
        if (digits[9] != 0)
        {
            return 0;
        }
    }
    else
    {
        if (digits[9] != 11 - (v1 % 11))
        {
            return 0;
        }
    }

    int v2 = 0;

    for (int i = 0; i < 10; i++)
    {
        v2 += digits[i] * (11 - i);
    }

    if ((v2 % 11) < 2)
    {
        if (digits[10] != 0)
        {
            return 0;
        }
    }
    else
    {
        if (digits[10] != 11 - (v2 % 11))
        {
            return 0;
        }
    }

    int d = digits[0];
    int same = 1;

    for (int i = 0; i < 11; i++)
    {
        same &= (d == digits[i]);
    }

    if (same)
    {
        return 0;
    }

    return 1;
}

int loadParcial()
{
    FILE *arq = fopen("parcial.txt", "r");
    if (arq == NULL)
    {
        printf("Arquivo faltando parcial.txt\n");
        return 0;
    }
    int qtdeVotos;

    fscanf(arq, "%d", &qtdeVotos);

    for (int i = 0; i < qtdeVotos; i++)
    {
        char cpf[15];
        int codigoTG;
        fscanf(arq, "%s %d", cpf, &codigoTG);

        for (int j = 0; j < qtdeEleitores; j++)
        {
            if (strcmp(cpf, comissao[j].cpf) == 0)
            {
                comissao[j].votou = 1;
                comissao[j].codigoTG = codigoTG;
            }
        }

        for (int j = 0; j < qtdeTGs; j++)
        {
            if (listaPIs[j].codigo == codigoTG)
            {
                listaPIs[j].qtdeVotos++;
            }
        }
    }

    return 1;
}

int saveParcial()
{
    FILE *arq = fopen("parcial.txt", "w");
    if (arq == NULL)
    {
        printf("Erro ao salvar parcial.txt\n");
        return 0;
    }

    fprintf(arq, "%d\n", qtdeEleitores);

    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (comissao[i].votou)
        {
            fprintf(arq, "%s %d\n", comissao[i].cpf, comissao[i].codigoTG);
        }
    }

    return 1;
}

int saveResultado()
{
    FILE *arq = fopen("resultado.txt", "w");
    if (arq == NULL)
    {
        printf("Erro ao salvar resultado.txt\n");
        return 0;
    }
    TG *vencedores = (TG *)malloc(qtdeTGs * sizeof(TG));
    if (vencedores == NULL)
    {
        printf("Erro ao alocar memoria para vencedores\n");
        return 0;
    }

    int maxVotos = 0;

    for (int i = 0; i < qtdeTGs; i++)
    {
        if (listaPIs[i].qtdeVotos > maxVotos)
        {
            maxVotos = listaPIs[i].qtdeVotos;
        }
    }

    int qtdeVencedores = 0;

    for (int i = 0; i < qtdeTGs; i++)
    {
        if (listaPIs[i].qtdeVotos == maxVotos)
        {
            vencedores[qtdeVencedores++] = listaPIs[i];
        }
    }

    fprintf(arq, "PI Vencedor\n");
    for (int i = 0; i < qtdeVencedores; i++)
    {
        TG *vencedor = &vencedores[i];
        fprintf(arq, "Codigo: %d\n", vencedor->codigo);
        fprintf(arq, "Titulo: %s\n", vencedor->titulo);

        for (int j = 0; j < qtdeFormandos; j++)
        {
            if (formandos[j].matricula == vencedor->autor)
            {
                fprintf(arq, "Aluno: %s\n", formandos[j].pes.nome);
                fprintf(arq, "Curso aluno: %s\n", formandos[j].depto);
                break;
            }
        }

        for (int j = 0; j < qtdeDocentes; j++)
        {
            if (docentes[j].codigo == vencedor->orientador)
            {
                fprintf(arq, "Orientador: %s\n", docentes[j].pes.nome);
                fprintf(arq, "Deptos orientador: %s\n", docentes[j].depto);
                break;
            }
        }
        fprintf(arq, "\n");
    }

    fprintf(arq, "Eleitores presentes\n");

    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (comissao[i].votou)
        {
            fprintf(arq, "%s\n", comissao[i].cpf);
        }
    }

    fprintf(arq, "\n");

    fprintf(arq, "Eleitores ausentes\n");

    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (!comissao[i].votou)
        {
            fprintf(arq, "%s\n", comissao[i].cpf);
        }
    }
    free(vencedores);
    return 1;
}

int loadDocentes()
{
    FILE *arq = fopen("professor.txt", "r");
    if (arq == NULL)
    {
        printf("Arquivo faltando professor.txt\n");
        return 0;
    }

    fscanf(arq, "%d\n", &qtdeDocentes);

    for (int i = 0; i < qtdeDocentes; i++)
    {
        // %[^\n] para ler até o final da linha, com espaços
        fscanf(arq, " %d %s %d %[^\n]", &docentes[i].codigo, docentes[i].depto, &docentes[i].pes.idade, docentes[i].pes.nome);
    }

    return 1;
}

int validateAuthor(int matricula, char *dpto)
{
    for (int i = 0; i < qtdeFormandos; i++)
    {
        if (formandos[i].matricula == matricula && strcmp(formandos[i].depto, dpto) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int orientadorExists(int codigo)
{
    for (int i = 0; i < qtdeDocentes; i++)
    {
        if (docentes[i].codigo == codigo)
        {
            return 1;
        }
    }
    return 0;
}

int loadTG(char *dpto)
{
    // PI_XXX.txt
    char filename[11];
    sprintf(filename, "PI_%s.txt", dpto);
    FILE *arq = fopen(filename, "r");

    if (arq == NULL)
    {
        printf("Arquivo faltando %s\n", filename);
        return 0;
    }
    int qtdeTGsDpto;
    fscanf(arq, "%d", &qtdeTGsDpto);

    for (int i = 0; i < qtdeTGsDpto; i++)
    {
        int codigo, autor, orientador;

        char titulo[maxNome];

        fscanf(arq, "%d %d %d %[^\n]", &codigo, &autor, &orientador, titulo);

        int valid = 1;
        valid &= validateAuthor(autor, dpto);
        valid &= orientadorExists(orientador);

        if (!valid)
        {
            printf("Erro no arquivo %s\n", filename);
            return 0;
        }

        listaPIs[qtdeTGs + i].codigo = codigo;
        listaPIs[qtdeTGs + i].autor = autor;
        listaPIs[qtdeTGs + i].orientador = orientador;
        strcpy(listaPIs[qtdeTGs + i].titulo, titulo);
    }

    qtdeTGs += qtdeTGsDpto;

    return 1;
}

int loadTGs()
{
    // Fazendo isso para tentar ler todos, invés de abortar execução no primeiro erro, assim mostrando todos os erros
    int valid = 1;
    valid &= loadTG("AMS");
    valid &= loadTG("ADS");
    valid &= loadTG("DSM");
    valid &= loadTG("CEX");
    valid &= loadTG("EMP");
    valid &= loadTG("LOG");
    valid &= loadTG("POL");

    return valid;
}

int loadAlunos()
{
    FILE *arq = fopen("aluno.txt", "r");
    if (arq == NULL)
    {
        printf("Arquivo faltando aluno.txt\n");
        return 0;
    }

    fscanf(arq, "%d", &qtdeFormandos);

    for (int i = 0; i < qtdeFormandos; i++)
    {
        fscanf(arq, "%d %*d %s %d %[^\n]", &formandos[i].matricula, formandos[i].depto, &formandos[i].pes.idade, formandos[i].pes.nome);
    }

    return 1;
}

int loadComissao()
{
    FILE *arq = fopen("comissao.txt", "r");
    if (arq == NULL)
    {
        printf("Arquivo faltando comissao.txt\n");
        return 0;
    }

    fscanf(arq, "%d", &qtdeEleitores);

    for (int i = 0; i < qtdeEleitores; i++)
    {
        char cpf[15];
        fscanf(arq, "%s", cpf);

        if (!validaCPF(cpf))
        {
            printf("CPF %s invalido\n", cpf);
            exit(1);
        }

        strcpy(comissao[i].cpf, cpf);
    }

    return 1;
}

int cpfExists(const char *cpf)
{
    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (strcmp(comissao[i].cpf, cpf) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int alreadyVoted(const char *cpf)
{
    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (strcmp(comissao[i].cpf, cpf) == 0)
        {
            return !comissao[i].votou;
        }
    }
    return 0;
}

int tgExists(int codigo)
{
    for (int i = 0; i < qtdeTGs; i++)
    {
        if (listaPIs[i].codigo == codigo)
        {
            return 1;
        }
    }
    return 0;
}

int enterVoto()
{
    char cpf[15];
    while (1)
    {
        printf("Digite o CPF do eleitor: ");
        scanf("%s", cpf);

        if (!validaCPF(cpf))
        {
            printf("CPF invalido\n");
        }
        else if (!cpfExists(cpf))
        {
            printf("CPF nao pertence a comissao\n");
        }
        else if (!alreadyVoted(cpf))
        {
            printf("CPF ja votou\n");
        }
        else
        {
            break;
        }
    }
    int codigoTG;
    while (1)
    {
        printf("Digite o codigo do PI: ");
        scanf("%d", &codigoTG);
        if (!tgExists(codigoTG))
        {
            printf("Codigo do PI invalido\n");
            printf("Digite novamente\n");
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < qtdeEleitores; i++)
    {
        if (strcmp(cpf, comissao[i].cpf) == 0)
        {
            comissao[i].votou = 1;
            comissao[i].codigoTG = codigoTG;
        }
    }

    for (int i = 0; i < qtdeTGs; i++)
    {
        if (listaPIs[i].codigo == codigoTG)
        {
            listaPIs[i].qtdeVotos++;
        }
    }

    return 1;
}
/*
a) Entrar com voto
b) Suspender votação
c) Encerrar votação
*/
void menu2()
{
    while (1)
    {
        printf("a) Entrar com voto\n");
        printf("b) Suspender votacao\n");
        printf("c) Concluir votacao\n");

        char option;
        printf("Digite a opcao desejada: ");
        scanf(" %c", &option);
        if (option == 'a' || option == 'A')
        {
            enterVoto();
        }
        else if (option == 'b' || option == 'B')
        {
            saveParcial();
            break;
        }
        else if (option == 'c' || option == 'C')
        {
            saveResultado();
            break;
        }
        else
        {
            printf("Opcao invalida\n");
            printf("Digite novamente\n");
        }
    }

    exit(0);
}

void menu1()
{
    while (1)
    {
        printf("a) Iniciar uma nova votacao\n");
        printf("b) Continuar com votacao gravada\n");
        char option;
        printf("Digite a opcao desejada: ");
        scanf("%c", &option);

        if (option == 'a' || option == 'A')
        {
            menu2();
            break;
        }
        else if (option == 'b' || option == 'B')
        {
            if (loadParcial())
            {
                menu2();
                break;
            }
        }

        else
        {
            printf("Opcao invalida\n");
            printf("Digite novamente\n");
        }
    }
}

int main(void)
{
    // Fazendo isso para mostrar a mensagem de erros de ambos, caso ambos não existam antes de sair
    /*
    fazer isso:
    if(!loadDocentes() || !loadAlunos() || !loadTGs()) {
        exit(1);
    } não funciona porque se o primeiro for falso, o segundo nem é executado, short-circuit
    */

    int valid = 1;

    valid &= loadDocentes();
    valid &= loadAlunos();
    valid &= loadComissao();
    valid &= loadTGs();

    if (!valid)
    {
        exit(1);
    }

    menu1();

    return 0;
}