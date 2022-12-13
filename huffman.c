
/*--------- Brenno Muller Vitorino --------  */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>




#define TAM 256

/*--------- estruturas --------  */

typedef struct no{
	char dado;
	int freq;
	struct no *prox;
}No;


typedef struct tree{
	char dado;
	int freq;
	struct tree *dir, *esq;
}Tree;


// Função para Juntar as arvores, somando as frequencias e deixando os caracteres nos folhas

void juntar_arvore(No **pos1, No **pos2, Tree **arvore){
	
	Tree *no1 = (Tree *)malloc(sizeof(Tree));
	no1->freq = (*pos1)->freq;
	no1->dado = (*pos1)->dado;
	no1->esq = NULL;
	no1->dir = NULL;

	Tree *no2 = (Tree *)malloc(sizeof(Tree));

	if((*pos1)->prox != NULL){	

		no2->freq = (*pos2)->freq;
		no2->dado = (*pos2)->dado;
		no2->esq = NULL;
		no2->dir = NULL;
	}else{
		
		no2->freq = 0;
		no2->dado = '*';
		no2->esq = NULL;
		no2->dir = NULL;
	}


	
	Tree *novo = (Tree *)malloc(sizeof(Tree));
	novo->freq = no1->freq + no2->freq;
	novo->dado = '*';
	novo->esq = no1;
	novo->dir = no2;
	
	
	//printf("%i\n", novo->freq);
	
	if(*arvore == NULL){
	     *arvore = novo;

	}else{
	
			    
		Tree *novo1 = (Tree *)malloc(sizeof(Tree));
		novo1->freq = (*arvore)->freq + novo->freq;
		novo1->dado = '*';
		novo1->esq = *arvore;
		novo1->dir = novo;
		
		*arvore = novo1;
	}	

}


void pre_ordem(Tree **r){
	
	if (*r != NULL){
		printf("%c [Freq: %i] \n", (*r)->dado, (*r)->freq);	
		pre_ordem(&(*r)->esq);
		pre_ordem(&(*r)->dir);
		
	}
}

int altura_arvore(Tree **arvore){
    int esq, dir;

    if(*arvore == NULL)
        return 1;
    else{
        esq = altura_arvore(&(*arvore)->esq) + 1;
        dir = altura_arvore(&(*arvore)->dir) + 1;

        if(esq > dir){
            return esq;
        }
        else{
            return dir;
        }
    }
}


// Dicionario com os codigos dos caminhos dos caracteres 

char** aloca_dicionario(int colunas){
    char **dicionario;
    int i;

    dicionario = malloc(sizeof(char*) * TAM);

    for(i = 0; i < TAM; i++)
        dicionario[i] = calloc(colunas, sizeof(char));

    return dicionario;
}


// criando  o dicionario

void dicionario(char **dici, Tree **raiz, char *caminho, int colunas){
    char esquerda[colunas], direita[colunas];

    if((*raiz)->esq == NULL && (*raiz)->dir == NULL)
        strcpy(dici[(*raiz)->dado], caminho);
    else{
        strcpy(esquerda, caminho);
        strcpy(direita, caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");

        dicionario(dici, &(*raiz)->esq, esquerda, colunas);
        dicionario(dici, &(*raiz)->dir, direita, colunas);
    }
}




void mostrar_dicionario(char **dicionario){
    int i;
    for(i = 0; i < TAM; i++){
        if(strlen(dicionario[i]) > 0)
            printf("%c: %s\n", i, dicionario[i]);
    }
}


// montando a arvore de huffman, com os caracateres sendo folhas.
// a arvore é montada a partir da lista de frequencia ordenada
// juntando os NOs e criando um pai com a soma das frequencias

void montar_arvore(No **lista, Tree **arvore){
	char **dicionario;
	
	
	No *aux = *lista;
	
	while(aux != NULL){


		//printf("%i\n", aux->prox);

		juntar_arvore(&aux, &aux->prox, &(*arvore));

		
		if(aux->prox == 0){
			aux = 0;
		}else{
			aux = aux->prox->prox;
		}

		
		
		
	}
	
	//printf("\n %s \n", "arvore");
	//pre_ordem(&(*arvore));

	
}



// verificando a existencia do char na lista de frequencia, se existir atribuir +1 a frequencia de tal caracter

int verificar_existencia(char valor, No **lista){

	if(*lista == NULL){
		return 1;
	}

	No *aux = *lista;
	while(aux != NULL){
		if(aux->dado == valor){
				aux->freq++;
				return 1;
		} 
		aux = aux->prox;
	}

	return 0;
}



// inserir um novo no fim da lista

void inserir(char valor, No **lista){
		No *novo = (No *)malloc(sizeof(No));
		novo->dado = valor;
		novo->prox = NULL;
		novo->freq = 1;


		if(valor != EOF){
			if(*lista == NULL){
				*lista = novo;
			}else{

				No *aux = *lista;
				while(aux->prox){
					aux = aux->prox;
				}

				aux->prox = novo;	

				
			}
		
		}	
		
}


// inserindo ordenado para tabela ordenada
void inserir_ordenado(int valor, int freq, No **lista){


	No *novo = (No *)malloc(sizeof(No));
	novo->dado = valor;
	novo->freq = freq;
	novo->prox = NULL;

	if(*lista == NULL){
		*lista = novo;
	}else if((*lista)->freq >= novo->freq){
		novo->prox = *lista;
		*lista = novo;
	}else{
		No *aux = *lista;
		while(aux->prox != NULL){
			//printf("%c", 'b');
			
			if(novo->freq < aux->prox->freq){
				break;
			}
			
			
			if(novo->freq >= aux->prox->freq){
				aux = aux->prox;
			} 
		}

		novo->prox = aux->prox;
		aux->prox = novo;
	}


	
}


// criando uma nova lista ordenada para ordenar a tabela frequencia, para enfim montar a arvore de huffman
No * ordenar_lista(No **frequencia){
		
		No *lista = NULL;

		No *aux = *frequencia;
		while(aux != NULL){
			
			inserir_ordenado(aux->dado, aux->freq, &lista);
			aux = aux->prox;
		}
	
		return lista;
}



void mostrar_lista(No **frequencia){
	
	No *aux = *frequencia;
	while(aux != NULL){
	
			printf("%c freq:[%d]\n", aux->dado, aux->freq);
			
		
		aux = aux->prox;
	}
}



//mostrar conteudo do arquivo de texto

void mostrar_conteudo(No **texto){
	
	No *aux = *texto;
	while(aux != NULL){
		printf("%c", aux->dado);
		aux = aux->prox;
	}
}


// ler arquivo de texto e salvar em uma lista encadeiada

void ler_arq(FILE *arq, No **ler_texto){
	char c;
	while(c != EOF){
		c = fgetc(arq);
		inserir(c, &(*ler_texto));	
	}
}

// calculando tamanho da string para fazer a alocação na variavel char *codigo 
// soma o tamanho de cada codigo de todos os caracteres do texto

int calcula_tamanho_string(char **dicionoario, No **texto){
    int i = 0, tam = 0;
    No *aux = *texto;
    while(aux != NULL){
        tam = tam + strlen(dicionoario[aux->dado]);
        i++;
        aux = aux->prox;
    }
    return tam + 1;
}


char* codificando(char **dicionario, No **texto){
	
    int i = 0, tam = calcula_tamanho_string(dicionario, &(*texto));
    char *codigo = calloc(tam, sizeof(char));
    No *aux = *texto;

    while(aux != NULL){
        strcat(codigo, dicionario[aux->dado]);
        aux = aux->prox;
    }

    return codigo;
}


//compactando o arquivo, salvando os codigos dos caracteres em um arquivo binario
 	
void compactar(unsigned char str[]){
    FILE *arquivo = fopen("compactado.bren", "wb");
    int i = 0, j = 7;
    unsigned char mascara, byte = 0; // 00000000

    if(arquivo){
        while(str[i] != '\0'){
            mascara = 1;
            if(str[i] == '1'){
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if(j < 0){ // tem um byte formado
                fwrite(&byte, sizeof(unsigned char), 1, arquivo);
                byte = 0;
                j = 7;
            }

            i++;
        }
        if(j != 7) //11010000
            fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        fclose(arquivo);
    }
    else
        printf("\nErro ao abrir/criar arquivo em compactar\n");
}


//recebe um byte do arquivo binario compactado e retorna 1 se for 
unsigned int bit(unsigned char byte, int i){
    unsigned char mascara = (1 << i);
    return byte & mascara;
}



/* descompactando arquivo, Neste caso o arquivo é descompactado pela propria arvore do programa
   a arvore não está sendo carregada do arquivo compactado .bren */

void descompactar(Tree **raiz){
	
    FILE *arquivo = fopen("compactado.bren", "rb");
    FILE *descompactar = fopen("descompactado.txt", "a");
    Tree *aux = *raiz;
    unsigned char byte; // 10111001
    int i;

    if(arquivo){
    	
    	// caso bit retorne 1 sabemos se temos que ir para esquerda ou direita na arvore 

        while(fread(&byte, sizeof(unsigned char), 1, arquivo)){
        	
            for(i = 7; i >= 0; i--){
                if(bit(byte, i))
                    aux = aux->dir;
                else
                    aux = aux->esq;

                if(aux->esq == NULL && aux->dir == NULL){
		    		 fprintf(descompactar, "%c", aux->dado);
                     //printf("%c", aux->dado);
                     aux = *raiz;
                }
            }
        }
        fclose(arquivo);
        fclose(descompactar);
    }
    else
        printf("\nErro ao abrir arquivo em descompactar\n");
}


int menu_2(){
	int opc;
	system("cls || clear");
	printf("\n\n");
	
	printf("MENU DE DESCOMPACTACAO \n");
	
   	printf("%s \n", "NAO FECHE O PROGRAMA !!!");
	printf("%s \n", "press [1] para ver a arvore de Huffman");
	printf("%s \n", "press [2] para ver lista de frequencia dos caracteres");
   	printf("%s \n", "press [3] para ver o codigo gerado do texto");
   	printf("%s \n", "press [4] para ver o Dicionario");
	printf("%s \n", "press [5] para descompactar texto");
	printf("press [0] e em seguida ENTER para Sair\n");

	printf("escolha uma opção: ");
	scanf("%i", &opc);

	return opc;
}


int iniciar_desconpactacao(char *arq){
	

	char *url = arq;
	No *ler_texto = NULL;
	No *frequencia = NULL;
	No *tabela_ordenada = NULL;
	Tree *arvore = NULL;
	char **dic;
	char c;
	int esc;
	int verifica = 0;
	int colunas= 0;

	FILE *arquivo;

	arquivo = fopen(url, "rt");

	//printf("%s \n", "Texto do arquivo");
	//
	//mostrar_conteudo(&ler_texto);



	/*-- montando a tabela de frquencia (lista)--*/

	while(c != EOF){
		c = fgetc(arquivo);

		/*-- verifica se o char ja existe na lista --*/
		verifica = verificar_existencia(c, &frequencia);

		
		if(frequencia == NULL){
			inserir(c, &frequencia);
		}


		if(!verifica){
			inserir(c, &frequencia);
		}
		
	}

	// printf("%s \n", "tabela frequencia");
	//	mostrar_lista(&frequencia);


	/*-- montando a tabela de ordenada de frquencia (ordenando a lista de frequencia)--*/

	tabela_ordenada = ordenar_lista(&frequencia);

	// printf("%s \n", "Lista ordenado por frquencia");
	// mostrar_lista(&tabela_ordenada);


	/*-- montando a arvore de huffman--*/

	montar_arvore(&tabela_ordenada, &arvore);

	/*-- percorrendo a arvore e montando o dicionario --*/

	colunas = altura_arvore(&arvore) + 1;
    dic = aloca_dicionario(colunas);
    dicionario(dic, &arvore, "", colunas);
        //	mostrar_dicionario(dic);

   	fclose(arquivo);


    /*-- compactar --*/

   

   
    arquivo = fopen(url, "rt");
    ler_arq(arquivo, &ler_texto);
    printf("%s \n", "Iniciando_compactacao");
   	char *codificado = codificando(dic, &ler_texto);
   	compactar(codificado);

   	system("cls || clear");
   	printf("\n\n");
   	printf("%s \n", "compactado com sucesso");
   	printf("%s \n", "seu arquivo compactado está na mesma pasta em que voce executou o programa");
   	printf("%s \n", "nome do arquivo compactado: compactado.bren");


   	


   	while(esc = menu_2()){

		switch(esc){
			case 1:
				printf("\n\n");
				printf("Arvore de Huffman");			
				pre_ordem(&arvore);
				printf("\n");
				printf("Press enter para voltar para o menu de descompactacao");
				
			break;

			case 2:
				printf("\n\n");
				printf("Lista de frequencia de caracteres");
				mostrar_lista(&tabela_ordenada);
				printf("\n");
				printf("Press enter para voltar para o menu de descompactacao");
				
			break;

		    case 3:
				printf("\n\n");
				printf("Texto codificado \n");
				printf("%s", codificado);
				printf("\n");
				printf("Press enter para voltar para o menu de descompactacao");
				
				
			break;


		    case 4:
				printf("\n\n");
				printf("Dicionario");
				mostrar_dicionario(dic);
				printf("\n");
				printf("Press enter para voltar para o menu de descompactacao");
				
			break;

			case 5:
				printf("\n\n");
				printf("Iniciando_descompactacao");
				printf("\n");
				printf("Texto descompactado \n");
				descompactar(&arvore);
				printf("\n seu arquivo descompactado está na mesma pasta em que voce executou o programa");
   				printf("\n nome do arquivo descompactado: descompactado.txt");
   				printf("\n");
				printf("Press enter para voltar para o menu de descompactacao");
   				
			break;

		}
		getchar();
		getchar();
	}

}


int menu(){
	int opc;
	system("cls || clear");
	printf("-----------------------------------------------------------------------------------------\n");
	printf("+ ||     || ||	  || ||||||||| |||||||      ||       ||             ||      ||||     || +\n");
	printf("+ ||     || ||	  || ||        ||         || ||    || ||          || ||     || ||    || +\n");
	printf("+ ||     || ||	  || ||        ||        ||    || ||   ||        ||   ||    ||  ||   || +\n");
	printf("+ ||||||||| ||	  || ||||||    ||||||   ||      ||      ||      |||||||||   ||   ||  || +\n");
	printf("+ ||     || ||    || ||        ||      ||                ||    ||       ||  ||    || || +\n");
	printf("+ ||     || ||    || ||        ||     ||                  ||  ||         || ||     |||| +\n");
	printf("+ ||     || |||||||| ||        ||    ||                    || ||         || ||       || +\n");
	printf("+                                                                                       +\n");
	printf("+                                [2022-BrennoMullerVitorino]                            +\n"); 
	printf("-----------------------------------------------------------------------------------------\n");
	printf("\n\n");
	printf("[0] - Sair\n");
	printf("[1] - compactar\n");


	printf("escolha uma opção: ");
	scanf("%i", &opc);

	return opc;
}



int main(){	
	
	

    int continuar = 1;
    char arquivo[200];
    int opc, vlr;
	
    while(opc = menu()){

		switch(opc){
			case 1:
				printf("Digite o nome do arquivo txt que deseja compactar: ");
				
				scanf("%s", arquivo);

				iniciar_desconpactacao(arquivo);
			break;

		}
		getchar();
		getchar();
	}

}
	
