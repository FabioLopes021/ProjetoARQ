#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "historico.h"
#include "grafo.h"

void inserirHistorico(Historico** inicio, int idc, int idm, int ide, double custof, float custo, char localinicial[], char localfinal[], struct tm start, struct tm end){
    Historico *new;

    new = (Historico *) malloc(sizeof(Historico));

    new->idCliente = idc;
    new->idMeio = idm;
    new->id = ide;
    new->custoFinal = custof;
    new->Custo = custo;
    new->inicio = start;
    new->fim = end;
    strcpy(new->localinicial, localinicial);
    strcpy(new->localfinal, localfinal);

    new->next = (*inicio);
    (*inicio) = new;
}

void inserirHistoricoInicio(Historico** inicio, int idc, int idm, float custo, char localinicial[]){
    Historico *new;
    time_t t = time(NULL);
    
    new = (Historico *) malloc(sizeof(Historico));

    new->idCliente = idc;
    new->idMeio = idm;
    new->id = generateidHistorico(*inicio);
    new->Custo = custo;
    new->custoFinal = 0;
    new->inicio = *localtime(&t);
    strcpy(new->localinicial, localinicial);

    new->next = (*inicio);
    (*inicio) = new;    
}


int inserirHistoricoFinal(Historico* inicio, int ide, char localfinal[]){
    time_t t = time(NULL);
    double custof;    

    if(!inicio)
        return 0;

    while(inicio != NULL && inicio->id != ide)
        inicio = inicio->next;

    inicio->fim = *localtime(&t);
    inicio->custoFinal = calculoCustoTotal(inicio, ide);
    strcpy(inicio->localfinal, localfinal);

    return 1;     
}

Historico *removerHistorico(Historico **inicio, int id, int i){
    Historico *aux;

    if (!*inicio){
        printf("A base de dados esta vazia\n");
        return NULL;
    }

    if(!(existeHistorico((*inicio), id))){
        printf("Nao existe nenhum cliente resgistado com o NIF indicado...\n");
        return NULL;
    }
    
    if (((*inicio)->id == id) && i == 1){
        aux = (*inicio)->next;
        free((*inicio));
        (*inicio) = aux;
        return aux;
    }else if (((*inicio)->id == id)){
        aux = (*inicio)->next;
        free((*inicio));
        return aux;
    }else{
        (*inicio)->next = removerHistorico(&(*inicio)->next, id, ++i);
        return (*inicio);
    }
}

int existeHistorico(Historico* inicio, int id){
    if (!inicio)
        return 0;
    
    if (inicio->id == id)   
        return 1;
    return existeHistorico(inicio->next, id);
}

double calculoCustoTotal(Historico *entrada, int ide){
    double seg, min;
    time_t startTime, endTime;

    while(entrada != NULL && entrada->id != ide)
        entrada = entrada->next;


    startTime = mktime(&(entrada->inicio));
    endTime = mktime(&(entrada->fim));
    
    seg = difftime(endTime, startTime);
    min = seg/60;

    return min * entrada->Custo;
}


float calculoDist(Historico *entrada, VerticeList *v, int ide){
    int inicial, final;

    if (entrada == NULL)
        return -1;

    while(entrada != NULL && entrada->id != ide)
        entrada = entrada->next;

    verticePorGeocode(v, &inicial, entrada->localinicial);
    verticePorGeocode(v, &final, entrada->localfinal);

    return menorCaminho(v, inicial, final);
}

int idEntrada(Historico *entrada, int idm){
    if(!entrada)
        return 0;
        
    while(entrada != NULL){
        if (entrada->idMeio == idm && entrada->custoFinal == 0)
            return entrada->id;
        entrada = entrada->next;
    }

    return 0;
}

int generateidHistorico(Historico *inicio){
    int max = 1;

    if (!inicio)
        return max;

    while(inicio != NULL){
        if (max < inicio->id)
            max = inicio->id;
        inicio = inicio->next;
    }

    return ++max;
}

int imprimirHistorico(Historico *inicio,VerticeList *v){

    if(!inicio)
        return 0;

    printf("\n--------------HISTORICO------------------\n");

    while(inicio != NULL){
        printf("ID Historico: %d\n", inicio->id);
        printf("ID Cliente: %d\n", inicio->idCliente);
        printf("ID Meio: %d\n", inicio->idMeio);
        printf("Local inicio: %s\n", NOME_PONTOS[auxprinthistorico(v, inicio->localinicial)]);
        printf("Local final: %s\n", NOME_PONTOS[auxprinthistorico(v, inicio->localfinal)]);
        printf("Inicio do aluguer: %02d/%02d/%04d %02d:%02d:%02d\n", 
            inicio->inicio.tm_mday, inicio->inicio.tm_mon + 1, inicio->inicio.tm_year + 1900,
            inicio->inicio.tm_hour, inicio->inicio.tm_min, inicio->inicio.tm_sec);
        printf("Fim do aluguer: %02d/%02d/%04d %02d:%02d:%02d\n", 
            inicio->fim.tm_mday, inicio->fim.tm_mon + 1, inicio->fim.tm_year + 1900,
            inicio->fim.tm_hour, inicio->fim.tm_min, inicio->fim.tm_sec);
        printf("Custo Meio (minuto): %.2f\n", inicio->Custo);
        printf("Custo Total: %.2f\n", inicio->custoFinal);

        if(inicio->next != NULL)
            printf("-----------------------------------------\n");
        inicio = inicio->next;
    }

    return 1;
}

int imprimirHistoricoCliente(Historico *inicio,VerticeList *v, int id){
    if(!inicio)
        return 0;

    printf("\n----------HISTORICO CLIENTE--------------\n");

    while(inicio != NULL){
        if (inicio->idCliente == id){
            printf("ID Historico: %d\n", inicio->id);
            printf("ID Cliente: %d\n", inicio->idCliente);
            printf("ID Meio: %d\n", inicio->idMeio);
            printf("Local inicio: %s\n", NOME_PONTOS[auxprinthistorico(v, inicio->localinicial)]);
            printf("Local final: %s\n", NOME_PONTOS[auxprinthistorico(v, inicio->localfinal)]);
            printf("Inicio do aluguer: %02d/%02d/%04d %02d:%02d:%02d\n", 
                inicio->inicio.tm_mday, inicio->inicio.tm_mon + 1, inicio->inicio.tm_year + 1900,
                inicio->inicio.tm_hour, inicio->inicio.tm_min, inicio->inicio.tm_sec);
            printf("Fim do aluguer: %02d/%02d/%04d %02d:%02d:%02d\n", 
                inicio->fim.tm_mday, inicio->fim.tm_mon + 1, inicio->fim.tm_year + 1900,
                inicio->fim.tm_hour, inicio->fim.tm_min, inicio->fim.tm_sec);
            printf("Custo Meio (minuto): %.2f\n", inicio->Custo);
            printf("Custo Total: %.2f\n", inicio->custoFinal);

        if(inicio->next != NULL)
            printf("-----------------------------------------\n");
        }
        
        inicio = inicio->next;
    }

    return 1;
}

int numEntradasCliente(Historico *inicio, int id){
    int i = 0;

    if (!inicio)
        return 0;

    while (inicio != NULL){
        if (inicio->idCliente == id){
            i++;
        }
        inicio = inicio->next;
    }

    return i;
}

void readHistorico(Historico **inicio){
    FILE* fp;
    int id, idc, idm;
    float custo;
    double custofinal;
    struct tm start;
    struct tm end;
    char line[1024], localinicial[MAX_GEOCODE], localfinal[MAX_GEOCODE];
	char* campo1, * campo2, * campo3, * campo4, * campo5, * campo6, * campo7, *campo8, *campo9, *aux = NULL;
    time_t tempoinic, tempofinal;
    
    fp = fopen("hist.txt","r");


    if (fp != NULL) {
		while (fgets(line, sizeof(line), fp)) {

            campo1 = strtok(line, ";");
			campo2 = strtok(NULL, ";");
			campo3 = strtok(NULL, ";");
			campo4 = strtok(NULL, ";");
			campo5 = strtok(NULL, ";");
			campo6 = strtok(NULL, ";");
			campo7 = strtok(NULL, ";");
			campo8 = strtok(NULL, ";");
			campo9 = strtok(NULL, ";");

            id = atoi(campo1);
            idc = atoi(campo2);
            idm = atoi(campo3);
            custo = atof(campo4);
            custofinal = atof(campo5);
            strcpy(localinicial, campo6);
            strcpy(localfinal, campo7);
            tempoinic = strtol(campo8,&aux,10);
            aux = NULL;
            tempofinal = strtol(campo9,&aux,10);

            start = *localtime(&tempoinic);
            end = *localtime(&tempofinal);

			
            inserirHistorico(&(*inicio), idc, idm, id, custofinal, custo, localinicial, localfinal, start, end);
		}
		fclose(fp);
	}
	else {
		printf("Erro ao abrir o ficheiro");
	} 

}



void guardarHistorico(Historico* inicio){
    FILE* fp;
    time_t timeinit, fim;

    fp = fopen("hist.txt","w");

    if (fp!=NULL){
        
        while (inicio != NULL){
        timeinit = mktime(&(inicio->inicio));
        fim = mktime(&(inicio->fim));
        fprintf(fp,"%d;%d;%d;%.2f;%.2f;%s;%s;", inicio->id, inicio->idCliente, inicio->idMeio, inicio->Custo, inicio->custoFinal, inicio->localinicial, inicio->localfinal); 
        fprintf(fp,"%ld;%ld\n", timeinit, fim);
        inicio = inicio->next;
        }
        
        fclose(fp);

        printf("Historico guardado com sucesso\n");
    }else
        printf("Erro ao abrir ficheiro do Historico\n");
}



void lerHistoricoBin(Historico **inicio){
    FILE *fp;
    Historico *new;

    new = (Historico *) malloc(sizeof(Historico));

    fp = fopen("histt.bin", "rb");

    if(fp != NULL){

       while (fread(new, sizeof(Historico), 1, fp) == 1) {
            if(new != NULL){
                inserirHistorico(&(*inicio), new->idCliente, new->idMeio, new->id, new->custoFinal, new->Custo, new->localinicial, new->localfinal, new->inicio, new->fim);
            }
             
        }
        free(new);
        fclose(fp);

        printf("Meios binarios lidos com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario\n");
    }
}



void guardarHistoricoBin(Historico *inicio){
    FILE *fp;

    fp = fopen("histt.bin", "wb");

    if(fp != NULL){

        while (inicio != NULL) {
            fwrite(inicio, sizeof(Historico), 1, fp);
            inicio = inicio->next;
        }

        fclose(fp);

        printf("Meios guardados em binario com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario\n");
    }
}