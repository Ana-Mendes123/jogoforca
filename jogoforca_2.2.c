#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>  //para caracteres especiais -> usar wchar_t em vez de char e wcslen em vez de strlen
#include <locale.h> //para dizer o país que tem caracteres especiais
/*
 * -Já dá com acentos
 * -Já dá com espaços
 * -Já dá para peguntar qual a categoria (mas são precisas mais categorias)
 * -Já dá o boneco
 */

void ler_fich(FILE *f, int n, wchar_t *m, wchar_t *masked)
{
    wchar_t ch;
    int linhas = 0;

    for (ch = fgetwc(f); ch != WEOF; ch = fgetwc(f))
    {
        if (ch == '\n')
            linhas++;
        if (linhas == n - 1)
        {
            m = fgetws(m, 20, f);
            for (int i = 0; i < wcslen(m) - 1; i++)
            {
                if (i == 1)
                {
                    masked[i] = tolower(m[i]);
                }
                masked[i] = m[i];
            }
            break;
        }
    }
    fclose(f);
}

char sem_acentos(wchar_t c)
{
    char b;
    switch (c)
    {
    case L'á':
    case L'à':
    case L'â':
    case L'ã':
        b = 'a';
        return b;

    case L'é':
    case L'ê':
    case L'ë': // para citroen
        b = 'e';
        return b;

    case L'í':
        b = 'i';
        return b;

    case L'ó':
    case L'ô':
    case L'õ':
        b = 'o';
        return b;

    case L'ú':
        b = 'u';
        return b;

    case L'ç':
        b = 'c';
        return b;

    case L'š': // para skoda
        b = 's';
        return b;

    default:
        b = (char)c;
        return b;
    }
}

void boneco(int tentativas)
{
    printf("  ______             \n");
    printf(" |      |            \n");
    printf(" |      %c           \n", (tentativas <= 5) ? 79 : ' ');
    printf(" |     %c%c%c        \n", (tentativas <= 3) ? 92 : ' ', (tentativas <= 4) ? 124 : ' ', (tentativas <= 2) ? 47 : ' ');
    printf(" |      %c           \n", (tentativas <= 4) ? 124 : ' ');
    printf(" |     %c %c         \n", (tentativas <= 1) ? 47 : ' ', (tentativas == 0) ? 92 : ' ');
    printf("_|__                 \n");
    printf("\n");
}

int main()
{
    srand(time(0));
    setlocale(LC_ALL, "pt_PT.UTF-8");
    FILE *f;
    char escolha;
    wchar_t *m, *revealed, *masked;
    char escolhidas[26] = {[0 ... 25] = 1}; // de 0 ate 25 é tudo 1
    int flag;

    m = (wchar_t *)malloc(21 * sizeof(wchar_t)); // usando pointers para guardar characteres
    revealed = (wchar_t *)malloc(47 * sizeof(wchar_t));
    masked = (wchar_t *)malloc(47 * sizeof(wchar_t));

    printf("Escreve o número da categoria que queres:\n");
    printf("[1] Animais\n");
    printf("[2] Países\n");
    printf("[3] Marcas de carros\n");
    scanf("%c", &escolha);

    switch (escolha)
    {
    // Animais
    case '1':
    {
        int n = rand() % (42 - 1) + 1; // existem 42 animais

        f = fopen("Animais.txt", "r");
        if (f == NULL)
        {
            fprintf(stderr, "no such file\n");
            exit(1);
        }
        ler_fich(f, n, m, masked);
        break;
    }
    // Paises
    case '2':
    {
        int n = rand() % (31 - 1) + 1; // existem 31 países

        f = fopen("Paises.txt", "r");
        if (f == NULL)
        {
            fprintf(stderr, "no such file\n");
            exit(1);
        }
        ler_fich(f, n, m, masked);
        break;
    }
    // Marcas de carros
    case '3':
    {
        int n = rand() % (76 - 1) + 1; // existem 76 marcas

        f = fopen("Marcas_carros.txt", "r");
        if (f == NULL)
        {
            fprintf(stderr, "no such file\n");
            exit(1);
        }
        ler_fich(f, n, m, masked);
        break;
    }
    default:
        printf("Excuse me what?");
        return -1;
    }

    int tentativas = 6;
    printf("Tens 6 tentativas\n");

    for (int i = 0; i < wcslen(masked); i++)
    {
        revealed[i] = '_';
        if (masked[i] == ' ')
        {
            revealed[i] = ' ';
        }
        printf("%lc", revealed[i]);
    }
    printf("\n");

    while (1)
    {
        wchar_t ch;
        flag = 0;
        printf("Escolhe uma letra: ");
        scanf(" %lc", &ch);
        char letra = sem_acentos(ch);
        int pos = letra - 'a';

        // ver letras na lista e comparar
        if (escolhidas[pos] == 0)
        {
            printf("\n");
            printf("Já escolheste essa letra\n");
            printf("Ainda tens %d tentativas\n\n", tentativas);
            for (int i = 0; i < wcslen(revealed); i++)
            {
                printf("%lc", revealed[i]);
            }
            printf("\n");
            continue;
        }

        escolhidas[pos] = 0; // letra ja esta escolhida

        // ver se a letra corresponde
        for (int i = 0; i < wcslen(masked); i++)
        {
            if (sem_acentos(masked[i]) == ch || ch == tolower(masked[i]))
            { // acentos e maiusculas
                revealed[i] = masked[i];
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("Tens %d tentativas\n", --tentativas);
            boneco(tentativas);
            if (tentativas == 0)
            {
                printf("Oof perdeste :v\n");
                printf("A palavra era %ls\n", masked);
                return 0;
            }
        }
        else
        { // if (flag == 1)
            if (wcscmp(revealed, masked) == 0)
            {
                for (int j = 0; j < wcslen(revealed); j++)
                {
                    printf("%lc", revealed[j]);
                }
                printf("\n");
                printf("Viva ganhaste! :D\n");
                return 0;
            }
        }
        for (int j = 0; j < wcslen(revealed); j++)
        {
            printf("%lc", revealed[j]);
        }
        printf("\n");
    }
}
