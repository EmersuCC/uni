#include <iostream>
#include <stdlib.h>
#include <chrono>

#define LEN 900000
int vec1[LEN], vec2[LEN], vec3[LEN], vec4[LEN];
int counter = 0;
void data_hazard(){
    int i, j = 0;
    vec2[0] = rand() % 30;
    vec3[1] = rand() % 50;
    for(i = 1; i<LEN; i++){
        vec1[i] = vec2[i-1] * 0.1;   //
        vec3[i] = vec1[i-2] * 0.3;  // Read After Write, write after read
        vec2[i] = vec4[i-1] * 0.2;   
        vec4[i] = rand() % 10 + 1;  /*Write after read*/
    }
}

void data_hazard_solution(){
    int i, j = 0;
    vec2[0] = rand() % 30;
    vec3[1] = rand() % 50;
    for(i = 1;i<LEN; i++){
        vec1[i] = vec2[i-1] * 0.1;   //Atribuição de valores aos vetores foram reordenados, evitando boa parte dos riscos de dados ou de hardware
        vec4[i] = rand() % 10 + 1;    // 
        vec3[i] = vec1[i-2] * 0.3;    
        vec2[i] = vec4[i-1] * 0.2;   
    }
}

void saltos(){
    int i, j, k;
    for(i = 0; i<LEN; i++){
        if(vec1[i] > vec2[i]){
            counter++;
            for(j = 0; j<LEN; j++)
                vec2[j] -= 1;
            
            if(vec3[j]>vec4[j]){  /*Dificuldade de prever valor da avaliação*/
                counter++;
                for(k = 0; k<LEN; k++)
                    vec4[k] -=1;
            }

        } 
    }
}

void salto_otimizado(){
    int i, j, k;
    bool maior;
    for(i = 0; i<LEN; i++){
        if(vec1[i] > vec2[i]){
            counter++;
            maior = vec3[i] > vec4[i];   /*Condicional avaliado antes de ser consultado*/
            for(j = 0; j<LEN; j++)
                vec2[j] -= 1;
            
            if(maior){
                counter++;
                for(k = 0; k<LEN; k++)
                    vec4[k] -=1;
            }

        } 
    }
}

int main(){
    srand(time(NULL));
    auto time = std::chrono::high_resolution_clock::now();
    //data_hazard();
    saltos();
    auto time2 = std::chrono::high_resolution_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time);
    std::cout << "Tempo total sem otimizacao: " << total_time.count() <<std::endl;

    auto time3 = std::chrono::high_resolution_clock::now();
    //data_hazard_solution();
    salto_otimizado();
    auto time4 = std::chrono::high_resolution_clock::now();
    auto total_time2 = std::chrono::duration_cast<std::chrono::milliseconds>(time4-time3);
    std::cout <<"Tempo total com otimizacao: " << total_time2.count() << std::endl;

    return 0;
}
