#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <ncurses.h>
#include <signal.h>

#define qtdMaxItens  1500

void sighandler(int);

//Variaveis globais
int esteira1 = 0, esteira2 = 0, esteira3 = 0, total = 0;
float pesos[1500], pesoTotal;
int pipefd[2], flag = 0;

//Mutex para fazer o lock nas sessões que vão utilizar a mesma variável global
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

//threads que farão a contagem das esteiras
void* contagem_esteira1(void* param){
    while(!flag){
        if(total < qtdMaxItens){
            pthread_mutex_lock(&mutex);
            pesos[total] = 5;
            total++;
            esteira1++;
            pthread_mutex_unlock(&mutex);
            
            usleep(1000000);
        }
    }
}

void* contagem_esteira2(void* param){
    while(!flag){ 
        if(total < qtdMaxItens){
            pthread_mutex_lock(&mutex);
            pesos[total] = 2;
            total++;
            esteira2++;
            pthread_mutex_unlock(&mutex);
            usleep(500000);
        }
    }
}

void* contagem_esteira3(void* param){
    while(!flag){
        if(total < qtdMaxItens){
            pthread_mutex_lock(&mutex);
            pesos[total] = 0.5;
            total++;
            esteira3++;
            pthread_mutex_unlock(&mutex);  
            usleep(100000);
        }
    }
}

void display(){
    //leitura do pipe
    char buffer[1024];
    read(pipefd[0], buffer, sizeof(buffer));
    int esteira1, esteira2, esteira3, total;
    double pesoTotal;
    sscanf(buffer, "%d %d %d %d %lf", &esteira1, &esteira2, &esteira3, &total,&pesoTotal);
 
    printf("Quantidade de produtos:\n");
    printf("Esteira 1: %d produto(s)\nEsteira 2: %d produto(s)\nEsteira 3: %d produto(s)\nTotal: %d produtos\n", esteira1, esteira2, esteira3, total);
    printf("\n");
    //Atualizar o peso total e imprimir
    if(total >= qtdMaxItens){
        printf("\n");
        printf("-------------------------------------------------------\n");
        printf("Iniciando pesagem total...\n");
        printf("Peso total: %.2f kg\n",pesoTotal);
        printf("Pesagem total concluída! Continuando a contagem...\n");
        printf("-------------------------------------------------------\n");
    }
}

void sighandler(int signum) {
    flag = 1;
}

int main(){
    pthread_t tid_esteira1, tid_esteira2, tid_esteira3, tid_display, tid_pararExecucao;  
    pid_t pid; //identificador do processo
  
    signal(SIGINT, sighandler);

    //Criar o pipe
    if(pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); //Criar o processo filho

    if(pid < 0){ //erro na criação do filho
        fprintf(stderr, "Erro na criação do processo!\n");
        return 1;
    }

    if(pid == 0){ //processo filho vai apresentar o display
        //fechar o pipe para escrita no filho pois ele só vai ler
        close(pipefd[1]);
    
        while(!flag){
            //usleep(2000000);
            display();
        }
    } else { //processo pai vai chamar as threads e fazer a contagem
        // Fecha o lado de leitura do pipe no pai
        close(pipefd[0]);
        

        //criar as threads
        pthread_create(&tid_esteira1,NULL,contagem_esteira1,NULL);
        pthread_create(&tid_esteira2,NULL,contagem_esteira2,NULL);
        pthread_create(&tid_esteira3,NULL,contagem_esteira3,NULL);
        sleep(0.2);
        int reset = 0;
        while(!flag){
            pthread_mutex_lock(&mutex);
            //Calcular o peso total
            if(total >= qtdMaxItens){
                //calculaPesoTotal();
                #pragma omp for reduction(+:pesoTotal)
                for(int i = 0; i < total; i++){
                    pesoTotal += pesos[i];
                }
                reset = 1;
            }
            //escrever no pipe
            char buffer[1024];
            sprintf(buffer, "%d %d %d %d %lf", esteira1, esteira2, esteira3, total,pesoTotal);
            write(pipefd[1], buffer, strlen(buffer) + 1); 
      
            if(reset){
                printf("Reiniciando contagem:");
                // Reiniciar contagem
                esteira1 = 0;
                esteira2 = 0;
                esteira3 = 0;
                total = 0;
                pesoTotal = 0;
                reset = 0;
              
            }
            pthread_mutex_unlock(&mutex);
            sleep(2);
        }

        pthread_join(tid_esteira1,NULL);
        pthread_join(tid_esteira2,NULL);
        pthread_join(tid_esteira3,NULL);
    }
  
    printf("\nBotão pressionado, finalizando contagem!\n");
}
