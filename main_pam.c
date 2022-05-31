#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_students(struct Student* Students, int length) {
    for (int i = 0; i < length; i++) {
        printf("- %s-%d-%d-%d-%d-%s\n",Students[i].nom,Students[i].courage,Students[i].loyaute,Students[i].sagesse,
        Students[i].malice,Students[i].maison);
    }
}

void print_clusters(struct Cluster* clusters, int length) {
    for (int i = 0; i < length; i++) {
        printf("\n\033[0;31m CLUSTER %d\n\033[0m", i);
        printf("\033[0;32mreprésentant: \033[0m");
        print_students(&clusters[i].obj_rep, 1);
        printf("\033[0;32mdata list: \033[0m ");
        printf("\033[0;36mLength: %d\033[0m\n",clusters[i].length);
        print_students(clusters[i].cluster_data, clusters[i].length);
    }
}


int is_equal(struct Student* student_data, struct Student student, int length){
    
    if (length == 0){
        return 0;
    }
    
    for (int i = 0; i < length; i++){
        if (!strcmp(student.nom, student_data[i].nom)){
            if (student.courage == student_data[i].courage){
                return 1;
            }
        }
    }
    return 0;
}

struct Student* csv_to_struct() {

    struct Student* Students = malloc(sizeof(struct Student)*50);
    char c='a';
    char String[200];
    
    FILE* file;
    file = fopen("choixpeauMagique.csv","r");

    //lecture de la première ligne
    fscanf(file, "%[^\n]", String);
    fscanf(file, "%c", &c); //pour lire le \n

    for (int i = 0; i < 50; i++) { 
        fscanf(file,"%c",&c);
        fscanf(file,"%[^;]",String);
        strncpy(Students[i].nom,String,strlen(String)-1);
        fscanf(file,"%c",&c);

        fscanf(file,"%d",&Students[i].courage);
        fscanf(file,"%c",&c);

        fscanf(file,"%d",&Students[i].loyaute);
        fscanf(file,"%c",&c);

        fscanf(file,"%d",&Students[i].sagesse);
        fscanf(file,"%c",&c);

        fscanf(file,"%d",&Students[i].malice);
        fscanf(file,"%c",&c);

        fscanf(file,"%c",&c); //lecture du ""
        fscanf(file,"%[^\n]",String); //lecture de la valeur 
        strncpy(Students[i].maison, String,strlen(String)-2);
        fscanf(file,"%c",&c);   
    }
    fclose(file);
    return Students;
}

struct Student* alea_students(struct Student* Students_data, int k) {

    struct Student* Students_alea = malloc(sizeof(struct Student)*k);
    int alea = 0;

    srand(time(NULL));

    for (int i = 0; i < k; i++) {
        alea = rand()%50;

        if (is_equal(Students_alea, Students_data[alea], i)){
            i--;
            continue;
        }
        else {
            Students_alea[i] = Students_data[alea];
        }
    }
    return Students_alea;
}



int distance(struct Student a, struct Student b) {
    int distance = abs(a.courage-b.courage)
    +abs(a.loyaute-b.loyaute)
    +abs(a.sagesse-b.sagesse)
    +abs(a.malice-b.malice);
    return distance;
}

void impl_cluster( struct Cluster* clusters , struct Student* Students_data, struct Student* Students_rep, int k) {

   
    int new_distance_value;
    int index = 0;

    for (int x = 0; x < k; x++){
        clusters[x].obj_rep = Students_rep[x];
        clusters[x].cluster_data[0] = Students_rep[x];
        clusters[x].length=1;
    }

    for (int i = 0; i < 50; i++){
        if (is_equal(Students_rep, Students_data[i], k)){
            continue;
        }
        int distance_value = 100;

        for (int j = 0; j < k; j++){
            new_distance_value = distance(Students_rep[j], Students_data[i]);
            if (distance_value > new_distance_value){
                distance_value = new_distance_value;
                index = j;
            }
        }
        clusters[index].cluster_data[clusters[index].length] = Students_data[i];
        clusters[index].length++;
    }
    
}

//retourne le cout de la partition (tout les clusters)
int cost_clusters(struct Cluster* clusters, int k) {
    int cout = 0;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j< clusters[i].length; j++) {
            cout = cout + distance(clusters[i].cluster_data[j], clusters[i].obj_rep);
        }
    }
    return cout;
}
 
 



//trouve les nouveaux centroides de tout les clusters 
int cost(struct Cluster* clusters, struct Student* Students_data,struct Student* Students_rep, int k){
    int Stop=0;
    struct Student old_rep;
    struct Student best_rep;
    int modify_rep;
    int old_cout = cost_clusters(clusters,k);
    int cout = 1000;
    int new_cout;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 50; j++){
            if (is_equal(Students_rep, Students_data[j], k)){
            continue;
        }
            old_rep=Students_rep[i];
            Students_rep[i]=Students_data[j];
            impl_cluster(clusters,Students_data,Students_rep,k);
            new_cout=cost_clusters(clusters, k);
            if (new_cout<cout) {
                best_rep=Students_data[j];
                modify_rep=i;
                cout=new_cout;
            }
            Students_rep[i]=old_rep;
                impl_cluster(clusters,Students_data,Students_rep,k);
                
        }
    }

    if (cout < old_cout) { 
        Students_rep[modify_rep]=best_rep;
        Stop=1;
    }

    
    printf("stop: %d\n",Stop);
    return Stop;
}



int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        exit (EXIT_FAILURE);
    }
    int k = atoi(argv[1]);
    int Stop=1;
    //Partie 1
    printf("\n\033[0;31m------STUDENT LIST STUDENTS------ \033[0m\n\n");
    struct Student* Students_data = csv_to_struct();
    print_students(Students_data, 50);

    printf("\n\033[0;31m------STUDENT ALEA LIST STUDENTS------ \033[0m\n\n");
    struct Student* Students_rep = alea_students(Students_data,k);
    print_students(Students_rep, k);
    //Répéter (partie 2)

    
    printf("\n\033[0;31m------CLUSTER LIST STUDENTS------ \033[0m\n");
    struct Cluster* Clusters_data = malloc(sizeof(struct Student)*k);
    impl_cluster(Clusters_data, Students_data, Students_rep, k);
    print_clusters(Clusters_data, k);
    Stop=cost(Clusters_data, Students_data, Students_rep, k);
    int t=0;
   while (Stop) {
      
        impl_cluster(Clusters_data, Students_data, Students_rep, k);
        Stop=cost(Clusters_data, Students_data, Students_rep, k);
        t++;
        
    }


    printf("\n\033[0;31m------FINAL CLUSTER LIST STUDENTS------ \033[0m\n");
    print_clusters(Clusters_data, k);
    printf("%d\n",t);
    printf("le cout de la partition est de: %d\n",cost_clusters(Clusters_data,k));
}