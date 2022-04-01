#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int row1;
int col1;
int row2;
int col2;
char globalfile[1000];
int mat1[1000][1000];
int mat2[1000][1000];
int solution[1000][1000];
char argg[1000];
struct element{
    int row;
    int column;
};


void upload_valuesfile(FILE *fptr, int mat[1000][1000], int row, int col )
{
    for(int i =0 ; i < row ; i++)
        for(int j = 0 ; j < col ; j++ )
            fscanf(fptr, "%d", &mat[i][j]);
}
void read( char path[1000],int num){
    int *row, *col, *mat;
    if(num==1){
     row=&row1; col=&col1; mat=&mat1;
    }
    else if(num==2){
        row=&row2; col=&col2; mat=&mat2;
    }
    FILE *file = fopen( path, "r" );
    if (file == NULL)
    {
        printf("Can't open the file.\n");
        exit(-1);
    }
    fscanf(file, "%[^\n]", argg);
    if(  sscanf(argg, "row=%d col=%d", row,col) != 2 )
    {
        printf("Invalid first line format");
        exit(1);
    }
    ///reading value from the file.
    upload_valuesfile(file,mat, *row, *col );
    fclose(file);
}

void method1(){
    for( int i = 0 ; i < row1 ; i++ )
    {
        for( int j = 0 ; j < col2 ; j++ )
        { for( int k = 0 ; k < col1 ; k++ )
            {
                solution[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }
    //pthread_exit(NULL);
}
//method2
void method2(void* row){
    for( int j = 0 ; j < col2 ; j++ )
    {
        solution[(int) row][j] = 0;
        for( int k = 0 ; k < col1 ; k++ )
        {
            solution[(int) row][j] += mat1[(int) row][k]*mat2[k][j];
        }
    }
    pthread_exit(NULL);
}
//thread method 2
void thread_method2(){
    int id=0;
    pthread_t meth[row1];
    for(int i=0;i<row1;i++){
        //creating thread for row
        id = pthread_create(&meth[i],NULL,method2,(void*)i);
        if(id !=0) {
            printf("ERROR!!!\n");
            exit(-1);
        }}
    for(int i=0;i<row1;i++){
        pthread_join(meth[i], NULL);
    }}

void method_3(void *thing){
    struct element *value = (struct element *) thing;
    int i=value->row;
    int j=value->column;
    solution[i][j]= 0;
    for(int l=0;l<col1;l++){
        solution[i][j]+=mat1[i][l]*mat2[l][j];
    }
    pthread_exit(NULL);
}
void thread_method3(){
    int id;
    pthread_t high[row1*col2];
    int k=0;
    for(int i=0;i<row1;i++){
        for(int j=0;j<col2;j++){
            struct element *temp = malloc(sizeof(struct element));
            temp->row=i;
            temp->column=j;
            id=pthread_create(&high[k++],NULL,method_3,(void*)temp);
            if(id !=0){
                printf("ERRROR!\n");
                exit(-1);
            }
        }
    }
    for(int t=0; t < row1*col2 ; t++)
    {
        pthread_join(high[t], NULL);
    }
}
//A function to print Matrix
void printing(int mat[1000][1000], int row, int col )
{
    for(int i =0 ; i < row ; i++)
    {
        for(int j = 0 ; j < col ; j++ )
            printf("%d ",mat[i][j]);
        printf("\n");
    }
}
void saving(char *path){
    FILE *file;
    file = fopen(path, "w");
    if(file == NULL) {
        printf("failure in creating file.\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<row1;i++){
        for(int j=0;j<col2;j++){
            fprintf(file,"%d ",solution[i][j]);
        }
        fprintf(file,"\n");
    }
    fclose(file);
}
void excution1(){
    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time
    //executing method
    method1();
    printf("Method 1\n");
    saving(strcat(globalfile,"_per_matrix.txt"));
    printing(solution,row1,col2);
    gettimeofday(&stop, NULL); //end checking time
    printf("Number of Threads used: 1\n");
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    printf("-------------------------------------\n");
}
void excution2(){
    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time
    //executing method
    thread_method2();
    printf("Method 2\n");
    saving(strcat(globalfile,"_per_row.txt"));
    printing(solution,row1,col2);
    gettimeofday(&stop, NULL); //end checking time
    printf("Number of Threads used: %d\n",row1);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    printf("-------------------------------------\n");
}
void excution3(){
    struct timeval stop, start;
    gettimeofday(&start, NULL); //start checking time
    //executing method
    thread_method3();
    printf("Method 3\n");
    saving(strcat(globalfile,"_per_element.txt"));
    printing(solution,row1,col2);
    gettimeofday(&stop, NULL); //end checking time
    printf("Number of Threads used: %d\n",row1*col2);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
    printf("-------------------------------------\n");
}
excution(){
    if( col1 != row2 )
    {
        printf("WARNING!!! not valid Dimensions\n");
        exit(3);
    }
    excution1();
    excution2();
    excution3();
}

setdef(int set,char *argv[]){
    if( set == 1 ){
        strcpy(globalfile , "");
        strcat(globalfile , "a.txt");
        read(globalfile,1);
        printing(mat1,row1,col1);
        strcpy(globalfile , "");
        strcat(globalfile , "b.txt");
        read(globalfile,2);
        strcpy(globalfile , "");
        strcat(globalfile , "c");
    }
    else if( set == 3 ){//a w b
        strcpy(globalfile , "");
        strcat(globalfile , argv[1]);
        strcat(globalfile,".txt");
        //
        read(globalfile,1);
        strcpy(globalfile , "");
        strcat(globalfile , argv[2]);
        strcat(globalfile,".txt");
        //
        printf("%s\n",globalfile);
        read(globalfile,2);
        strcpy(globalfile , "");
        strcat(globalfile , "c");
    }
    else if ( set == 4 ){ //3
        strcpy(globalfile , "");
        strcat(globalfile , argv[1]);
        strcat(globalfile,".txt");
        read(globalfile,1);
        //
        strcpy(globalfile , "");
        strcat(globalfile , argv[2]);
        strcat(globalfile,".txt");
        read(globalfile,2);
        //
        strcpy(globalfile , "");
        strcat(globalfile , argv[3]);
        //strcat(globalfile,".txt");
    }
    else{
        printf("Invalid Arguments");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    setdef(argc,argv);
    excution();
    return 0;
}
