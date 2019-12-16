#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

int i=0, j=0, q=0, n, dado, *cadeia; //variaveis contadoras
char alfabeto[80] = "{abc}defghijklmnopqrstuvwyxzABCDEFGHIJKLMNOPQRSTUVWYXZ0123456789=<>()+-%,;[]/:"; //string com o alfabeto da linguagem
char token[70]; //string para armazenamento temporário dos tokens (reseta a cada linha)
int operAspas = 0; //variável para identificar erros no fechamento de aspas
int simbolosErr = 0; // variável para identificar erros de simbolos que não pertencem a linguagem
int alfaErr = 0; // variável para identificar erros no alfabeto

//struct da lista de simbolos
typedef struct{
    char simbolo[100];
        //link próximo item da lista
    struct ListaSimbolos *link;
}ListaSimbolos;

//struct para pilha
struct no{
	int inf;
	struct no* prox;
};

typedef struct no No;

//struct para pilha
struct pilha{
	No* topo;
};

typedef struct pilha Pilha;

Pilha *pil;

Pilha* criaPilha(); //função para criar Pilha
int pilhaVazia(Pilha* p); //função para identificar se pilha vazia
No* inserirIni(No* a, int v); //função para inserir no inicio da lista utilizada pela pilha
No* removerIni(No* a);//função para remover no inicio da lista utilizada pela pilha
void push(Pilha* p, int v); //função para inserir na pilha
int pop(Pilha* p);//função para remover da pilha
void liberaPilha(Pilha* p);
void imprimirPilha(Pilha* p);
void reconhecerLinha(char linha[100], ListaSimbolos *lista); //função que realiza a análise das linhas lidas pelo arquivo
void criaTabela(ListaSimbolos *lista); //função para criar a tabela de simbolos
void insereTabela(ListaSimbolos *lista, char *novoSimbolo); //função para inserir no inicio da tabela de simbolos (utilizada para as funções num e string)

void main(){
    setlocale(LC_ALL, "");
	int ret=0;
    char arquivo[261];
    FILE *arq;
    char linha[100];
    char *result;
    int numeroLinha; //conta o número da linha atual na análise
    strcpy(token, "\0");

	//cria lista simplesmente encadeada para tabela de simbolos
    ListaSimbolos *lista;
    lista = (ListaSimbolos *) malloc(sizeof(ListaSimbolos)); //Aloca nó Head da lista
    //Checa se possui memória disponível para alocar o nó head
    if(!lista){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }
    lista->link = NULL;
    criaTabela(lista);
    
    //cria pilha
    pil = criaPilha();

	setbuf(stdin,NULL);
	printf("Informe o nome do arquivo em txt a ser analisado.\nExemplo.: ArquivoFonte1.txt\n");
    fgets(arquivo, 261, stdin);
    arquivo[strcspn(arquivo, "\n")] = 0;

    arq = fopen(arquivo, "rt");

    if (arq == NULL){
        printf("Problemas na abertura do arquivo\n");
        return;
    } // Verifica se houve erro na abertura do arquivo
    numeroLinha = 1;

    // Lê o arquivo
    printf("\nAnálise em andamento...\n");
    while (!feof(arq)) {
        result = fgets(linha, 100, arq);
        reconhecerLinha(linha, lista); //chama função para reconhecer a linha lida do arquivo
        
        printf("\nLinha %d lida\n", numeroLinha);
        numeroLinha++;
    }
    fclose(arq);

	//valida os operadores de fechamento {}()
    while(pil->topo != NULL){
			if( (pil->topo->inf == 0) && (q == 4) ){
				dado = pop(pil);
                q=5;
			}else{
				if( (pil->topo->inf == 0) && (q == 2) ){
					dado = pop(pil);
                    q=5;
				}else{
					q=-1;
					dado = pop(pil);
                }
			}

    }
    printf("\n=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
    printf("\nStatus da compilação:\n");
    if( (q==5) && (pil->topo == NULL) ){
        printf("\nOperadores de fechamento: Ok\n");
    }else{
        printf("\nOperadores de fechamento: Erro\n");
    }
    if(operAspas > 0){
    	printf("\nOperador Aspas: Erro\n");
	}else{
		printf("\nOperador Aspas: Ok\n");
	}
    
    printf("\nTabela de Símbolos: %d erro(s)\n", simbolosErr);
    printf("\nAlfabeto: %d erro(s)\n", alfaErr);

}

Pilha* criaPilha(){
	Pilha* p = (Pilha*) malloc(sizeof(Pilha));
	p->topo = NULL;
	return p;
}

int pilhaVazia(Pilha* p){
	return (p->topo==NULL);
}

No* inserirIni(No* a, int v){
	No* p = (No*) malloc(sizeof(No));
	p->inf = v;
	p->prox = a;
	return p;
}

No* removerIni(No* a){
	No* p = a->prox;
	free(a);
	return p;
}

//Insercao da pilha
void push(Pilha* p, int v){
	p->topo = inserirIni(p->topo, v);
}

// Remocao da pilha
int pop(Pilha* p){
	int v;
	if(pilhaVazia(p)){
		printf("Pilha Vazia");
		exit(1);
	}
	v = p->topo->inf;
	p->topo = removerIni(p->topo);
	return v;
}

void liberaPilha(Pilha* p){
	No* r = p->topo;
	while( r != NULL){
		No* ro = r->prox;
		free(r);
		r = ro;
	}
	free(p);
}

void imprimirPilha(Pilha* p){
	No* r = p->topo;
	printf("\nNossa Pilha\n");
	if( r == NULL){
		printf("\nPilha vazia\n");
	}
	while(r != NULL){
		printf("|%d|\n", r->inf);
		r = r->prox;
	}
}

void reconhecerLinha(char linha[100], ListaSimbolos *lista){
	int ret=0, c=0;
	char *pch;

	//começa a ler a linha
	for(i=0; i<=100; i++){
		if(linha[i]=='\n'){
            break;
        }
        //trata o operador de fechamento Aspas
        if(linha[i] == '"'){
            i++;
            while(linha[i] != '"'){
                i++;
                if(linha[i] == '\n'){
                    operAspas++;
                    break;
                }
            }
            i++;

        }


        if(i <= 100){
            
            //trata o alfabeto
            while ((alfabeto[j] != linha[i]) || (j<=80)){
                j++;
            }
            
            if(alfabeto[j] == linha[i]) {

                //trata os operadores de fechamento {}()
                if( (linha[i]=='(') && (pil->topo == NULL) && (q==0)){
                    push(pil, 0);
                    push(pil, 1);
                    q=1;
                }else{
                    if((linha[i]=='{') && (pil->topo == NULL) && (q==0)) {
                        push(pil, 0);
                        push(pil, 2);
                        q=3;
                    }else{
                        if( (linha[i]=='(') && (pil->topo->inf == 1) && (q==1)){
                            push(pil, 1);
                        }else{
                            if( (linha[i]==')') && (pil->topo->inf == 1) && (q==1)){
                                dado = pop(pil);
                                q = 2;
                            }else{
                                if((linha[i]=='(') && (pil->topo->inf == 2) && (q==3)){
                                    push(pil, 1);
                                    q = 1;
                                }else{
                                    if((linha[i]=='(') && (q==2)){
                                        push(pil, 1);
                                        q = 1;
                                   }else{
                                        if( (linha[i]=='{') && (pil->topo->inf == 2) && (q==3)) {
                                            push(pil, 2);
                                        }else{
                                            if( (linha[i]=='{') && (q==4)){
                                                push(pil, 2);
                                                q = 3;
                                            }else{
                                                if( (linha[i]=='{') && (q==2)){
                                                    push(pil, 2);
                                                    q = 3;
                                                }else{
                                                    if((linha[i]=='}') && (pil->topo->inf == 2) && (q==3)){
                                                        dado = pop(pil);
                                                        q = 4;
                                                    }else{
                                                        if((linha[i]=='}') && (pil->topo->inf == 2) && (q==4)){
                                                            dado = pop(pil);
                                                        }else{
                                                            if((linha[i]=='}') && (pil->topo->inf == 2) && (q==2)){
                                                                dado = pop(pil);
                                                                q = 4;
                                                            }else{
                                                                if((linha[i]==')') && (pil->topo->inf == 1) && (q==2)){
                                                                    dado = pop(pil);
                                                                }else{
                                                                    if((linha[i]=='(') && (pil->topo->inf == 0) && (q==4)){
                                                                        push(pil, 1);
                                                                        q = 1;
                                                                    }else{
                                                                    	if((linha[i]=='(') || (linha[i]==')') || (linha[i]=='{') || (linha[i]=='}')){
                                                                        q = -1;
                                                                    	}
																	}
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                //trata os símbolos
                if((linha[i] != ' ') && (linha[i] != ',') && (linha[i] != '(') && (linha[i] != ')') && (linha[i] != '=') && (linha[i] != '<') && (linha[i] != '>') && (linha[i] != '+') && (linha[i] != '-')&& (linha[i] != '%') && (linha[i] != ';') && (linha[i] != '[') && (linha[i] != ']') && (linha[i] != '{') && (linha[i] != '}') && (linha[i] != '/') && (linha[i] != '\n') && (linha[i] != '\t')){
                    token[c] = linha[i];

                    
                    c++;

                }else{
                    
                    ListaSimbolos *svlink;
                    svlink = lista->link;

                    if ((strcmp(token,"num") == 0) || (strcmp(token,"string") == 0)){
                    	
                        pch = strtok (linha," ,\n");

                        while (pch != NULL){
                            insereTabela(lista, pch);
                            pch = strtok (NULL, " ,\n");
                        }
                    }else{
                        if ((token[0] != '\0') && (token[0] != ' ') && (token[0] != '\\') && (token[0] != '0') && (token[0] != '1') && (token[0] != '2') && (token[0] != '3') && (token[0] != '4') && (token[0] != '5') && (token[0] != '6') && (token[0] != '7') && (token[0] != '8') && (token[0] != '9')){
							while ((svlink != NULL) && (strcmp(svlink->simbolo,token) != 0)){
	                            svlink = svlink->link;
	                        }
	
	                        if (svlink == NULL){
	                            simbolosErr++;
								
	                        }
                    	}
                    }
                    memset(token, 0, sizeof(token));
					
                    c = 0;
                }
            }else{
                alfaErr++;

            }
        }
        j = 0;
        
    }

}

void criaTabela(ListaSimbolos *lista){
    char *result;
    FILE *arq;
    arq = fopen("ListaSimbolos.txt", "rt");
    if (arq == NULL){
        printf("\nArquivo da tabela de simbolos nulo.\n");
        return;
    }

    ListaSimbolos *p;                                //Declara novo nó na lista

    ListaSimbolos *svlink;     //Cria um Salva link para armazenar o link de Head

    //Checa se possui memória disponível para alocar o novo nó
    if(!p){
        printf("\nSem memoria disponivel!\n");
        exit(1);
    }

    while (!feof(arq)){
    p = (ListaSimbolos *) malloc (sizeof(ListaSimbolos));
    svlink = lista->link;
    result = fscanf(arq, "%s ", &p->simbolo);
    

    lista->link = p;                //Armazena p no link de Head
    p->link = svlink;               //Armazena o anterior link de head em p
    }

    fclose(arq);
}

void insereTabela(ListaSimbolos *lista, char *novoSimbolo){
    ListaSimbolos *p;                                //Declara novo nó na lista
    p = (ListaSimbolos *) malloc (sizeof(ListaSimbolos)); //Aloca novo nó na memória

    //Checa se possui memória disponível para alocar o novo nó
    if(!p){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }

    strcpy(p->simbolo, novoSimbolo);

    ListaSimbolos *svlink;
    svlink = lista->link; //Cria um Salva link para armazenar o link de Head
    lista->link = p;                //Armazena p no link de Head
    p->link = svlink;

}
