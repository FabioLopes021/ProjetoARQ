#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"
#include "meio.h"

int adicionarVertice(VerticeList **v, int idvertice, char geocode[], char nome[])
{

    if (existeVertice(*v, idvertice) == 1)
        return 0;

    VerticeList *new = (VerticeList *)malloc(sizeof(VerticeList));
    VerticeList *aux = *v;

    new->vertice = idvertice;
    strcpy(new->geocode, geocode);
    strcpy(new->nome, nome);
    new->adj = NULL;
    new->next = NULL;

    if (*v == NULL)
    {
        *v = new;
    }
    else
    {
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = new;
    }

    return 1;
}

int adicionarAresta(VerticeList *v, int inicio, int fim, float peso)
{

    if (v == NULL)
        return 0;

    if ((existeVertice(v, inicio) != 1) || (existeVertice(v, fim) != 1))
        return 0;

    while (v->vertice != inicio)
    {
        v = v->next;
    }

    Adjacente *new = (Adjacente *)malloc(sizeof(Adjacente));
    new->peso = peso;
    new->origem = inicio;
    new->vertice = fim;
    new->next = NULL;

    if (v->adj != NULL)
    {
        Adjacente *aux = v->adj;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = new;
    }
    else
        v->adj = new;

    return 1;
}

int lerVertice(VerticeList *v, char *geocode)
{
    int i = 0, vertice;
    char aux[MAX_GEOCODE];

    if (v == NULL)
    {
        printf("De momento nao existe nenhuma localizaçao adicionada.\n");
        return 0;
    }

    printVertices(v);
    do
    {
        if (i == 0)
            printf("Indique a localizaçao do meio: ");
        else
            printf("Indique uma localizaçao valida: ");
        scanf("%d", &vertice);
        i++;
    } while (geocodePorVertice(v, vertice, aux) != 1);

    strcpy(geocode, aux);
    return 1;
}

int lerVerticePessoa(VerticeList *v)
{
    int i = 0, vertice;
    char aux[MAX_GEOCODE];

    if (v == NULL)
    {
        printf("De momento nao existe nenhuma localizaçao adicionada.\n");
        return -1;
    }

    printVertices(v);
    do
    {
        if (i == 0)
            printf("Indique a sua localizaçao: ");
        else
            printf("Indique uma localizaçao valida: ");
        scanf("%d", &vertice);
        i++;
    } while (geocodePorVertice(v, vertice, aux) != 1);

    return vertice;
}

void lerDadosVertice(VerticeList **v){
    char nome[MAX_NAME];
    char geocode[MAX_GEOCODE];
    int codigo;
    
    strcpy(geocode, "");
    strcpy(nome, "");
    clearbuffer();

    generico();
    printf("\n--------------- Adicionar Vertice ---------------\n");
    printf("Indique o nome do vertice: ");
    fgets(nome, MAX_NAME, stdin);
    nome[strlen(nome)-1] = '\0';

    printf("Indique o geocode do vertice (nome.nome.nome): ");
    fgets(geocode, MAX_GEOCODE, stdin);
    geocode[strlen(geocode)-1] = '\0';

    
    codigo = gerarIdVertice(*v);

    adicionarVertice(&(*v),codigo, geocode,nome);
}

void lerDadosAresta(VerticeList *v){
    int verticeo,verticed, codigo, i, dist;
    
    clearbuffer();

    generico();
    printVertices(v);
    i = 0;
    printf("\n--------------- Adicionar Aresta ---------------\n");
    do{
        if(i == 0)
            printf("Indique o vertice de origem: ");
        else
            printf("Indique um vertice valido: ");
        scanf("%d", &verticeo);
        i++;
    }while(existeVertice(v,verticeo) != 1);
    
    i = 0;

    do{
        if(i == 0)
            printf("Indique o vertice de destino: ");
        else
            printf("Indique um vertice valido: ");
        scanf("%d", &verticed);
        i++;
    }while(existeVertice(v,verticed) != 1);

    i = 0;

    do{
        if(i == 0)
            printf("Indique a distancia entre os dois vertices: ");
        else
            printf("Indique uma distancia valida: ");
        scanf("%d", &dist);
        i++;
    }while(dist <= 0);

    
    codigo = gerarIdVertice(v);

    if(existeAresta(v,verticeo,verticed) != 1 && (verticed != verticeo)){
        adicionarAresta(v,verticeo,verticed,dist);
    }else{
        printf("Nao é possivel adicionar duas vezes a mesma aresta.\n");
    }

}

int gerarIdVertice(VerticeList *inicio){
    int max = 0;

    if (!inicio)
        return max;

    while( inicio != NULL){
        if (max < inicio->vertice)
            max = inicio->vertice;

        inicio = inicio->next;
    }

    return ++max;
}

int existeVertice(VerticeList *v, int idvertice)
{
    if (v == NULL)
        return 2;

    while (v != NULL)
    {
        if (v->vertice == idvertice)
            return 1;
        v = v->next;
    }

    return 0;
}

int existeAresta(VerticeList *v, int verticeO, int verticeD){
    Adjacente *aux;

    if (v == NULL)
        return 2;

    while (v != NULL){
        if (v->vertice == verticeO){
            aux = v->adj;
            break;
        }
        v = v->next;
    }

    while(aux != NULL){
        if(aux->vertice == verticeD)
            return 1;
        aux = aux->next;
    }

    return 0;
}

int printGrafo(VerticeList *v)
{

    if (v == NULL)
        return 0;

    Adjacente *aux;

    printf("-----------Grafo-------------\n");
    while (v != NULL)
    {
        printf("Vertice: %d", v->vertice);
        aux = v->adj;
        while (aux != NULL)
        {
            printf("-> %d", aux->vertice);
            aux = aux->next;
        }
        printf(";\n");
        v = v->next;
    }

    return 1;
}

int printVertices(VerticeList *v)
{

    if (v == NULL)
        return 0;

    printf("-----------Grafo-------------\n");
    while (v != NULL)
    {
        printf("Vertice %d: %s\n", v->vertice, v->nome);
        v = v->next;
    }
    printf("-----------------------------\n");

    return 1;
}

int printGrafoNomes(VerticeList *v)
{

    if (v == NULL)
        return 0;

    Adjacente *aux;

    printf("-----------Grafo-------------\n");
    while (v != NULL)
    {
        printf("Vertice: %s", NOME_PONTOS[v->vertice]);
        aux = v->adj;
        while (aux != NULL)
        {
            printf("-> %s", NOME_PONTOS[aux->vertice]);
            aux = aux->next;
        }
        printf(";\n");
        v = v->next;
    }

    return 1;
}

int printGeocodeVertice(VerticeList *v)
{
    if (v == NULL)
        return 0;

    printf("Localizaçoes\n");
    while (v != NULL)
    {
        printf("vertice: %d -> %s\n", v->vertice, NOME_PONTOS[v->vertice]);
        v = v->next;
    }

    return 1;
}

int numVertices(VerticeList *v)
{
    int i = 0;

    if (v == NULL)
        return i;

    while (v != NULL)
    {
        i++;
        v = v->next;
    }

    return i;
}

// Retorna o geocode do vertice ao qual corresponde o id
int geocodePorVertice(VerticeList *v, int vertice, char *geocode)
{
    if (v == NULL)
        return 0;

    while (v != NULL)
    {
        if (v->vertice == vertice)
        {
            strcpy(geocode, v->geocode);
            return 1;
        }
        v = v->next;
    }

    return 0;
}

// Retorna o id do vertice ao qual corresponde o geocode
int verticePorGeocode(VerticeList *v, int *vertice, char geocode[])
{
    if (v == NULL)
        return 0;

    while (v != NULL)
    {
        if (strcmp(geocode, v->geocode) == 0)
        {
            *vertice = v->vertice;
            return 1;
        }
        v = v->next;
    }

    return 0;
}

int auxprinthistorico(VerticeList *v, char geocode[])
{
    if (v == NULL)
        return -1;

    while (v != NULL)
    {
        if (strcmp(geocode, v->geocode) == 0)
        {
            return v->vertice;
        }
        v = v->next;
    }

    return -1;
}

// Determinar se vértice 'id' já foi visitado
int visitado(int sequencia[], int pos, int id)
{
    int i;
    for (i = 0; i < pos; i++)
        if (sequencia[i] == id)
            return (1);
    return (0);
}

// Listar os caminhos existentes entre dois vértices passados
// por parâmetro
void listarCaminhosAux(VerticeList *v, int origem, int destino, int sequencia[], int posicao, int pesoTotal)
{
    int i;
    VerticeList *head = v;
    Adjacente *aux;
    sequencia[posicao] = origem;
    if (origem == destino)
    {
        for (i = 0; i < posicao; i++)
            printf("%d->", sequencia[i]); // escrita de um caminho
        printf("%d (%d)\n", destino, pesoTotal);
    }
    else
    {
        while (v->vertice != origem && v != NULL)
            v = v->next;
        aux = v->adj;
        while (aux != NULL)
        {
            if (!visitado(sequencia, posicao, aux->vertice))
                listarCaminhosAux(head, aux->vertice, destino, sequencia, posicao + 1,
                pesoTotal + aux->peso);
            aux = aux->next;
        }
    }
}

void listarCaminhos(VerticeList *v, int origem, int destino)
{
    int sequencia[numVertices(v)];
    listarCaminhosAux(v, origem, destino, sequencia, 0, 0);
}

void inicializararray(AuxCaminho a[], int max)
{

    for (int i = 0; i < max; i++)
    {
        a[i].anterior = -1;
        a[i].menor = INFINITO;
        a[i].vertice = -1;
        a[i].visitado = 0;
    }
}

int adicionaPesos(VerticeList *v, AuxCaminho *caminho, int vertice, int numvertices, float peso)
{
    Adjacente *aux;
    int count = 0;

    if (v == NULL)
        return 0;

    while (v != NULL && v->vertice != vertice)
        v = v->next;

    aux = v->adj;

    while (aux != NULL)
    {
        if ((peso + aux->peso) < caminho[aux->vertice].menor)
        {
            caminho[aux->vertice].menor = peso + aux->peso;
            caminho[aux->vertice].anterior = vertice;
            count++;
        }

        aux = aux->next;
    }

    if (count == 0)
        return 0;
    else
        return 1;
}

int escolherProximo(AuxCaminho *caminho, int numVertices, int atual, int *anterior, float *peso)
{
    int aux = INFINITO, vertice = -1, s = 0;

    for (int i = 0; i < numVertices; i++)
    {
        if ((caminho[i].menor < aux) && (caminho[i].visitado != 1))
        {
            aux = caminho[i].menor;
            vertice = i;
            s = 0;
        }
        else
        {
            if ((caminho[i].menor <= aux) && (atual != caminho[i].anterior) && (caminho[i].visitado != 1))
            {
                vertice = i;
                s = 1;
            }
        }
    }

    if (s == 0)
    {
        *anterior = caminho[vertice].anterior;
    }

    *peso = aux;
    return vertice;
}

int numVerticesViagem(AuxCaminho *caminho, int destino)
{
    int i = 0, atual = destino;

    while (caminho[atual].anterior != -2)
    {
        atual = caminho[atual].anterior;
        i++;
    }

    return i;
}

int *contruirCaminho(AuxCaminho *caminho, int destino)
{
    int numLocais, atual = destino, *aux, i = 0;

    numLocais = numVerticesViagem(caminho, destino);

    aux = malloc(sizeof(int) * numLocais);

    printf("Teste caminho mais curto fabio:\n");
    printf("caminho: ");
    while (caminho[atual].anterior != -2)
    {
        aux[i] = atual;
        printf("%d-", atual);
        atual = caminho[atual].anterior;
        i++;
    }
    printf("%d", atual);
    return aux;
}

float menorCaminho(VerticeList *v, int origem, int destino)
{
    AuxCaminho *a = NULL;
    int aux, i = 0, atual, anterior, ver = 1, *ret = NULL;
    float peso = 0;

    if (v == NULL)
        return -1;

    aux = numVertices(v);
    a = malloc(sizeof(AuxCaminho) * aux);

    for (int j = 0; j < aux; j++)
    {
        a[j].anterior = -1;
        a[j].menor = INFINITO;
        a[j].vertice = j;
        a[j].visitado = 0;
    }

    a[origem].visitado = 1;
    a[origem].anterior = -2;
    a[origem].menor = 0;

    atual = origem;
    while (atual != destino && atual != -1)
    {
        ver = adicionaPesos(v, a, atual, aux, peso);
        anterior = atual;
        atual = escolherProximo(a, aux, atual, &anterior, &peso);
        a[atual].anterior = anterior;
        a[atual].visitado = 1;
    }

    // ret = contruirCaminho(a,destino);

    free(a);

    return peso;
}


int auxVerificaCargas(AuxCaminho *Vertices, Meio *inicio, int capacidadeMaxima, int verticeAux, int *verifica){
    int ver = 0;

    if(Vertices[verticeAux].carga > capacidadeMaxima){
        for (int i = 0; i < Vertices[verticeAux].numMeios; i++){
            if( cargaPorId(inicio, Vertices[verticeAux].idmeios[i]) < capacidadeMaxima){
                ver = 1;
                *verifica = Vertices[verticeAux].idmeios[i];
            }    
        }
        return ver;
    }else
        *verifica = -1;
        return 1;

}



int encontrarProximoPickup(Meio *inicio,VerticeList *v, AuxCaminho *vertices, int pontoAtual, int capacidadeMaxima, int *verticesComMeios, int numVerticesComMeios, int *verifica) {
    int proximoPonto = -1;
    float menorDistancia = INFINITO;

    for (int i = 0; i < numVerticesComMeios; i++) {
        int verticeAux = verticesComMeios[i];

        if (vertices[verticeAux].visitado == 0 && auxVerificaCargas(vertices, inicio, capacidadeMaxima, verticeAux, verifica)) {
            float distancia = menorCaminho(v, pontoAtual, verticeAux);
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                proximoPonto = verticeAux;
            }
        }
    }

    return proximoPonto;
}


int startxCaminho(VerticeList *v, Meio *inicio,AuxCaminho caminho[]){
    VerticeList *aux = v;
    int count;

    while(aux != NULL){
        caminho[aux->vertice].vertice = aux->vertice;
        caminho[aux->vertice].anterior = -2;
        caminho[aux->vertice].carga = 0;
        caminho[aux->vertice].menor = INFINITO;
        caminho[aux->vertice].visitado = 0;
        caminho[aux->vertice].numMeios = 0;
        caminho[aux->vertice].idmeios = NULL;
        aux = aux->next;
    }
    count = 0;
    while(inicio != NULL){
        if(inicio->bateria < 50){
            int aux;
            verticePorGeocode(v,&aux,inicio->geocode);
            caminho[aux].carga += inicio->metrosQ;
            caminho[aux].numMeios++;
            if(caminho[aux].numMeios == 1)
                caminho[aux].idmeios = malloc(sizeof(int) * caminho[aux].numMeios);
            else
                caminho[aux].idmeios = realloc(caminho[aux].idmeios, sizeof(int) * caminho[aux].numMeios);
            caminho[aux].idmeios[caminho[aux].numMeios - 1] = inicio->codigo;
        }

        inicio = inicio->next;
    }

    return 1;
}



void rotaRecolha(VerticeList *v,Meio *inicio) {
    int capaciadeMaxima = 10, verifica, aux;
    int pontoAtual = 4, maxNumMeios = 0, numColunas = 1;
    int pontosRecolhidos = 0;
    float distanciaTotal = 0.0;
    int numVert = numVertices(v);
    int numVerticesComMeios = 0;
    int *reset, aux1 = 1, counter = 0;
    AuxCaminho *Vertices;
    int **caminho;

    Vertices = malloc(sizeof(AuxCaminho) * numVert);
    
    
    startxCaminho(v,inicio,Vertices);

    for(int i = 0; i < numVert; i++){
        if(Vertices[i].carga > 0)
            numVerticesComMeios++;
    }

    int *VerticesComMeios = malloc(sizeof(int) * numVerticesComMeios);

    for(int i = 0, j = 0; i < numVert; i++){
        if(Vertices[i].carga > 0){
            VerticesComMeios[j] = i;
            j++;
        }
    }

    for(int i = 0; i < numVert; i++){
        if(maxNumMeios < Vertices[i].numMeios)
            maxNumMeios = Vertices[i].numMeios;
    }

    caminho = (int **)malloc( sizeof(int *) * numColunas);
    for(int i = 0; i < numColunas; i++){
        caminho[i] = malloc(sizeof(int) * maxNumMeios + 1);
    }
    
    for(int i = 0; i < numColunas; i++){
        for(int j = 1; j < maxNumMeios + 1; j++){
            caminho[i][j] = -2;
        }
    }

    caminho[0][numColunas -1] = pontoAtual;

    while(pontosRecolhidos < numVerticesComMeios){
        if (pontosRecolhidos != 0){
            distanciaTotal += menorCaminho(v, pontoAtual, VERTICEDESCARGA);
            capaciadeMaxima = 10;
            pontoAtual = VERTICEDESCARGA;
            numColunas++;
            caminho = realloc(caminho, sizeof(int *) * numColunas);
            caminho[numColunas -1] = malloc(sizeof(int) * maxNumMeios + 1);
            caminho[numColunas-1][0] = pontoAtual;
            for(int i = 1; i < maxNumMeios + 1; i++){
                caminho[numColunas-1][i] = -2;
            }
        }
        while (pontosRecolhidos < numVerticesComMeios) {
        int proximoPonto = encontrarProximoPickup(inicio, v, Vertices, pontoAtual, capaciadeMaxima, VerticesComMeios, numVerticesComMeios, &verifica);

        if (proximoPonto == -1) {
            break;
        }

        if(verifica == -1){
            distanciaTotal += menorCaminho(v, pontoAtual, proximoPonto);
            capaciadeMaxima -= Vertices[proximoPonto].carga;
            Vertices[proximoPonto].visitado = 1;
            pontosRecolhidos++;
            numColunas++;
            caminho = realloc(caminho, sizeof(int *) * numColunas);
            caminho[numColunas -1] = malloc(sizeof(int) * maxNumMeios + 1);
            caminho[numColunas-1][0] = proximoPonto;
            aux = 0;
            for(int i = 1; i < maxNumMeios + 1; i++){ 
                if(aux < Vertices[proximoPonto].numMeios){
                    caminho[numColunas-1][i] = Vertices[proximoPonto].idmeios[aux];
                }else
                    caminho[numColunas-1][i] = -2;
                aux++;
            }
        }else{
            distanciaTotal += menorCaminho(v,pontoAtual,proximoPonto);
            capaciadeMaxima -= cargaPorId(inicio,verifica);
            Vertices[proximoPonto].carga -= cargaPorId(inicio,verifica);
            numColunas++;
            caminho = realloc(caminho, sizeof(int *) * numColunas);
            caminho[numColunas -1] = malloc(sizeof(int) * maxNumMeios + 1);
            caminho[numColunas-1][0] = proximoPonto;
            for(int i = 1; i < maxNumMeios + 1; i++){
                if( i == 0)
                    caminho[numColunas-1][i] = verifica;
                else
                    caminho[numColunas-1][i] = -2;
            }
        }

        pontoAtual = proximoPonto;
    }
    }
        numColunas++;
        caminho = realloc(caminho, sizeof(int *) * numColunas);
        caminho[numColunas -1] = malloc(sizeof(int) * maxNumMeios + 1);
        caminho[numColunas-1][0] = VERTICEDESCARGA;
        for(int i = 1; i < maxNumMeios + 1; i++){
            caminho[numColunas-1][i] = -2;
        }
        distanciaTotal += menorCaminho(v,pontoAtual,VERTICEDESCARGA);

    reset = malloc(sizeof(int) * aux1);

    printf("Caminho: \n");
    for(int i = 0; i < numColunas; i++){
        printf("Vertice visitado: %d - Meios carregados: ", caminho[i][0]);
        for(int j = 1; j < maxNumMeios + 1; j++){
            if(caminho[i][j] != -2){
                printf("%d ", caminho[i][j]);
                if(counter == 0){
                    reset[aux1 - 1] = caminho[i][j];
                    aux1++;
                    counter++;
                }else{
                    reset = realloc(reset, sizeof(int) * aux1);
                    reset[aux1 - 1] = caminho[i][j];
                    aux1++;
                    counter++;
                }
            }
        }
        printf("\n");
    }

    for(int i = 0; i < counter; i++){
        recarregarMeios(inicio, v, reset[i], VERTICEDESCARGA);
    }

    printf("\n\nDistancia total percorrida: %.2f\n", distanciaTotal);
    printf("Pontos de recolha visitados: %d\n", pontosRecolhidos);

    printf("\nMeios recolhidos com sucesso.\n");
}


void readGrafo(VerticeList **v)
{
    FILE *fp;
    char line[1024], geocode[MAX_GEOCODE], nome[MAX_NAME];
    int listVertice, i, aux, j, z;
    float aux1;
    char *campo1, *campo2;

    fp = fopen("grafos.txt", "r");

    if (fp != NULL)
    {
        j = 0;
        while (fgets(line, sizeof(line), fp))
        {

            campo1 = strtok(line, ";");
            if (j == 0)
            {
                z = 1;
                while (campo1 != NULL)
                {
                    if(z == 1){
                        listVertice = atoi(campo1);
                        z++;
                    }else{
                        if(z == 2){
                        strcpy(geocode, campo1);
                        z++;
                        }else{
                            if(z == 3){
                            strcpy(nome,campo1);
                            adicionarVertice(&(*v), listVertice, geocode, nome);
                            z = 1;
                            }
                        }
                    }
                    campo1 = strtok(NULL, ";");
                }
            }
            else
            {
                listVertice = atoi(campo1);
                campo2 = strtok(NULL, ";");
                i = 1;
                while (campo2 != NULL)
                {

                    if (i % 2 == 0)
                    {
                        aux1 = atof(campo2);
                        adicionarAresta(*v, listVertice, aux, aux1);
                    }
                    else
                    {
                        aux = atoi(campo2);
                    }
                    i++;
                    campo2 = strtok(NULL, ";");
                }
            }
            j++;
        }
        printf("Grafo carregado com sucesso\n");
        fclose(fp);
    }
    else
    {
        printf("Erro ao abrir o ficheiro do Grafo");
    }
}

void guardarGrafo(VerticeList *v)
{
    FILE *fp;
    VerticeList *head = v;
    Adjacente *aux;
    fp = fopen("grafos.txt", "w");

    if (fp != NULL)
    {

        while (v != NULL)
        {
            fprintf(fp, "%d;%s;%s", v->vertice, v->geocode, v->nome);
            v = v->next;
        }
        fprintf(fp, "\n");
        for (v = head; v != NULL; v = v->next)
        {
            fprintf(fp, "%d;", v->vertice);
            for (aux = v->adj; aux != NULL; aux = aux->next)
            {
                fprintf(fp, "%d;%.2f;", aux->vertice, aux->peso);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);

        printf("Grafo guardado com sucesso\n");
    }
    else
        printf("Erro ao abrir ficheiro Grafo\n");
}


/**
 * @brief Funçao para guardar os Meios e os seus dados num ficheiro binario
 * 
 * @param inicio Apontador para a variavel que guarda o apontador para a cabeça da lista ligada dos Meios
 */
void guardarGrafoBin(VerticeList *vertices){
    FILE *fp, *fp1;
    VerticeList *inicio = vertices;
    Adjacente *aux;

    fp = fopen("vertices.bin", "wb");
    fp1 = fopen("arestas.bin", "wb");

    if(fp != NULL){

        while (inicio != NULL) {
            fwrite(inicio, sizeof(VerticeList), 1, fp);
            inicio = inicio->next;
        }

        fclose(fp);

        printf("Vertices guardados em binario com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario dos vertices\n");
    }

    inicio = vertices;

    if(fp1 != NULL){

        while(inicio != NULL){
            aux = inicio->adj;
            while (aux != NULL){
                fwrite(aux,sizeof(Adjacente), 1, fp1);
                aux = aux->next;
            }
            inicio = inicio->next;
        }

        fclose(fp1);

        printf("Adjacentes guardados em binario com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario das arestas\n");
    }
}


/**
 * @brief Funçao para carregar os cliente e os seus dados do ficheiro binario
 * 
 * @param inicio Apontador para a variavel que guarda o apontador para a cabeça da lista ligada dos Meios
 */
void lerGrafoBin(VerticeList **inicio){
    FILE *fp, *fp1;
    VerticeList *new;
    Adjacente *new1;


    new = (VerticeList *) malloc(sizeof(VerticeList));
    new1 = (Adjacente *) malloc(sizeof(Adjacente));
    
    fp = fopen("vertices.bin", "rb");
    fp1 = fopen("arestas.bin", "rb");

    if(fp != NULL){

       while (fread(new, sizeof(VerticeList), 1, fp) == 1) {
            if(new != NULL){
                adicionarVertice(&(*inicio), new->vertice, new->geocode, new->nome);             
            }
        }
        free(new);
        fclose(fp);

        printf("Vertices binarios lidos com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario dos vertices\n");
    }

    if(fp1 != NULL){

       while (fread(new1, sizeof(Adjacente), 1, fp1) == 1) {
            if(new1 != NULL){
                adicionarAresta(*inicio, new1->origem, new1->vertice, new1->peso);     
            }
        }
        free(new1);
        fclose(fp1);

        printf("Arestas binarias lidos com sucesso\n");
    }else{
        printf("Erro ao abrir ficheiro binario das arestas\n");
    }
}