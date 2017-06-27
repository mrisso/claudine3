#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/time.h>

#define MAX            16384
#define NUM_MEDIA      3
#define NUM_PESQUISAS  1024

typedef long TipoChave;

typedef struct TipoRegistro {
	TipoChave Chave;
	/* outros componentes */
} TipoRegistro;

typedef struct TipoNo * TipoApontador;

typedef struct TipoNo {
	TipoRegistro Reg;
	TipoApontador Esq, Dir;
} TipoNo;

typedef TipoApontador TipoDicionario;

int Altura(TipoApontador p)
{
	int e, d;

	if(p == NULL)
		return -1;
	 
	e = Altura(p->Esq);
	d = Altura(p->Dir);

	if(e > d)
		return e + 1;

	return d + 1;
}

void Pesquisa(TipoRegistro *x, TipoApontador *p)
{
	if (*p == NULL) 
	{
		printf("Erro: Registro nao esta presente na arvore\n");
		return;
	}

	if (x->Chave < (*p)->Reg.Chave) 
	{
		Pesquisa(x, &(*p)->Esq);
		return;
	}

	if (x->Chave > (*p)->Reg.Chave)
		Pesquisa(x, &(*p)->Dir);

	else *x = (*p)->Reg;
} 

void Insere(TipoRegistro x, TipoApontador *p)
{
	if (*p == NULL) 
	{
		*p = (TipoApontador)malloc(sizeof(TipoNo));
		(*p)->Reg = x; 
		(*p)->Esq = NULL; 
		(*p)->Dir = NULL;
		return;
	}

	if (x.Chave < (*p)->Reg.Chave) 
	{
		Insere(x, &(*p)->Esq); 
		return; 
	}

	if (x.Chave > (*p)->Reg.Chave)
		Insere(x, &(*p)->Dir);

	else
		printf("Erro : Registro ja existe na arvore\n");
} 

void Inicializa(TipoApontador *Dicionario)
{
	*Dicionario = NULL;
}

void Antecessor(TipoApontador q, TipoApontador *r)
{
	if ((*r)->Dir != NULL) 
	{
		Antecessor(q, &(*r)->Dir);
		return;
	}
	q->Reg = (*r)->Reg;
	q = *r; 
	*r = (*r)->Esq;
	free(q);
} 

void Retira(TipoRegistro x, TipoApontador *p)
{
	TipoApontador Aux;
	if (*p == NULL) 
	{
		printf("Erro : Registro nao esta na arvore\n");
		return;
	}

	if (x.Chave < (*p)->Reg.Chave) { Retira(x, &(*p)->Esq); return; }

	if (x.Chave > (*p)->Reg.Chave) { Retira(x, &(*p)->Dir); return; }

	if ((*p)->Dir == NULL) 
	{
		Aux = *p;  *p = (*p)->Esq;
		free(Aux);
		return;
	}

	if ((*p)->Esq != NULL) 
	{
		Antecessor(*p, &(*p)->Esq);
		return;
	}

	Aux = *p;  *p = (*p)->Dir;
	free(Aux);
}  

void Central(TipoApontador p)
{
	if (p == NULL) return;

	Central(p->Esq);
	printf("%ld\n", p->Reg.Chave);
	Central(p->Dir);
} 

void TestaI(TipoNo *p, int pai)
{
	if (p == NULL) return;

	if (p->Esq != NULL) 
	{
		if (p->Reg.Chave < p->Esq->Reg.Chave) 
		{
			printf("Erro: Pai %ld menor que filho a esquerda %ld\n", p->Reg.Chave, 
				 p->Esq->Reg.Chave);

			exit(1);
		}
	}
	if (p->Dir != NULL) 
	{
		if (p->Reg.Chave > p->Dir->Reg.Chave) 
		{
			printf("Erro: Pai %ld maior que filho a direita %ld\n",  p->Reg.Chave, 
				 p->Dir->Reg.Chave);

			exit(1);
		}
	}
	TestaI(p->Esq, p->Reg.Chave);
	TestaI(p->Dir, p->Reg.Chave);
}


void Testa(TipoNo *p)
{
	if (p != NULL)
		TestaI(p, p->Reg.Chave);
}

double rand0a1() {
	double resultado=  (double) rand()/ RAND_MAX; /* Dividir pelo maior inteiro */
	if(resultado>1.0) resultado = 1.0;
	return resultado;
}

void Permut( TipoChave A[], int n) {
	int i,j; TipoChave b;
	for(i = n; i>0; i --) 
    {
		j = (i * rand0a1());
		b = A[i];
		A[i] = A[j];
		A[j] = b;
    }
}

int main(int argc, char *argv[])
{
	//Checar se o tamanho foi passado
	if(argc != 2)
	{
		printf("Uso: %s <tamanho-da-árvore>\n", argv[0]);
		return 1;
	}

	//Pegando tamanho fornecido pelo usuário
	int tamanho = atoi(argv[1]);

	if(tamanho > MAX)
	{
		printf("Tamanho é maior que o máximo permitido.\n");
		return 2;
	}

	printf("Tamanho: %d\n", tamanho);

	struct timeval inicio, fim;
	int tmicro = 0;
	int altura = 0;

	TipoNo *Dicionario;
	TipoRegistro x; TipoChave vetor[MAX];
	int i, j;

	Inicializa(&Dicionario);
	/* Gera uma permutação aleatoria de chaves entre 1 e MAX */
	for (i = 0; i < tamanho; i++) vetor[i] = i;

	srand(time(NULL));

	//Calcula a média das alturas das árvores
	for(i = 0; i < NUM_MEDIA; i++)
	{
		Permut(vetor,tamanho-1);
		/* Insere cada chave na arvore e testa sua integridade apos cada insercao */
		for (j = 0; j < tamanho; j++) 
		{
			x.Chave = vetor[j];
			Insere(x, &Dicionario);
			Testa(Dicionario);
		}

		altura += Altura(Dicionario);

		for(j = 0; j < NUM_PESQUISAS; j++)
		{
			x.Chave = rand() % tamanho;
			gettimeofday(&inicio, NULL);
			Pesquisa(&x, &Dicionario);
			gettimeofday(&fim, NULL);
			tmicro += (int) (1000000 * (fim.tv_sec - inicio.tv_sec) + (fim.tv_usec - inicio.tv_usec));
		}

		/* Retira a raiz da arvore ate que ela fique vazia */
		for (j = 0; j < tamanho; j++) 
		{
			x.Chave = Dicionario->Reg.Chave;
			Retira(x, &Dicionario);
			Testa(Dicionario);
		}
	}

	printf("Altura média das árvores: %d\n", altura/NUM_MEDIA);

	printf("Tempo para pesquisar %d registros: %dus\n", NUM_PESQUISAS, tmicro/NUM_MEDIA);

	return 0;
} 
