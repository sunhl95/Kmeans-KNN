/*
 * main.c
 *
 *  Created on: 2017年9月13日
 *      Author: haolisun
 */

#include <stdio.h>
#include <stdlib.h>     // for rand, srand
#include <time.h>       // for time, clock
#include <string.h>     // for strncmp
#include <math.h>


#define K 6

double**      malloc_data   (int n1, int n2);
//void        free_matrix     (int n1, int n2, char **a);
void        init_data     (int n1, int n2, double **a, FILE* f);
//void        init_class     (int n1, int *a);
void        print_data    (int n1, int n2, double **a);
void        kmean_1         (int n1, int n2, double **a);
int         min_index      (int k, double *sum);
void        kmean         (int n1, int n2, double **a, double** cent);
void       get_new_cent   (int n1, int n2, double** a, double** b);
void        count          (int n1,int n2, double** a);
void 		save_to_file    (int n1, int n2, char *name, double **a);

int main()
{	if(K>0){
	 char input_file[30];
	 char output_file[30];
	 //printf("Enter number of cluster you want to apply:");
	 //scanf("%d",&K);
	 printf("Enter input file:");
	 scanf("%s",&input_file[0]);
	 printf("Enter output file:");
	 scanf("%s",&output_file[0]);

	 int n[2]={0};
	FILE *fpRead=fopen(input_file,"r");

	    if(fpRead==NULL)
	    {
	    		printf("reading error");
	        return 0;
	    }
	    for(int i=0;i<2;i++)
	    {

	        fscanf(fpRead,"%d ",&n[i]);
	    }
	 //int *class;
	 //class=(int*)malloc(sizeof(int)*n[0]);//mark each sample's class or cluster
	// init_class(n[0],class);//at the beginning classed are all 0
	 double** data=malloc_data(n[0],n[1]);
	 init_data(n[0],n[1], data,fpRead);
	 print_data(n[0],n[1],data);
	 printf("------------first cluster begin\n");
	 kmean_1(n[0],n[1],data);//cluster 1, use first k sample as centroid
	 count(n[0],n[1],data);
	 printf("------------first cluster end\n");
	 double** dup_cent=malloc_data(K,n[1]);//copy centroid to set threshold

	 for(int iter=2;iter<40;iter++){//do iteration, update center
		 int num=0;
		 printf("\n------------%d time begin\n",iter);
		 double** new_cent=malloc_data(K,n[1]);

		 get_new_cent(n[0],n[1],data,new_cent);//to update new center

		 if (iter==2){dup_cent=new_cent;}
		 else{

			 for (int i=0;i<K;i++){
				 for(int j=0;j<n[1];j++){
					if(new_cent[i][j]==dup_cent[i][j]){
						num=num+1;//count number of center that do not change anymore
					}
					dup_cent[i][j]=new_cent[i][j];
				 }
			 }
			 printf("%d\n",num);
		 }
		 if(num==K*n[1]){//THRESHOLD:check if centers still change
			 printf("centroid not change,stop\n");
			 break;
		  }
		 kmean(n[0],n[1],data,new_cent);//based on new center, do cluster
		 printf("------------%d cluster outcome\n",iter);
		 print_data(n[0],n[1],data);
		 //kmean(n[0],n[1],data,new_cent);// if you want to see the final distance between each point and center,please cancel commented-out
		 count(n[0],n[1],data);// count the number of each cluster

		 printf("------------%d cluster end\n\n",iter);

	 }
 //	printf("-------------print in main function\n");
    // print_data(n[0],n[1],data);
	 count(n[0],n[1],data);

    // float** new_cent=malloc_data(K,n[1]);
     save_to_file(n[0],n[1]+1,output_file,data);



     fclose(fpRead);
}
else{ printf("please set K > 0 if you want to use this function");}
	 return 0;
}


double** malloc_data(int n1, int n2) {

   double** mat = NULL;       // pointer to the matrix

    int i;

    if(n1<=0||n2<=0){
    	printf("%s\n","matrix size must be lager than 0*0");
    return 0;
    }
    mat=(double**)malloc(sizeof(double*)*n1);

    for (i=0;i<n1;i++){
    	mat[i]=(double*)malloc(sizeof(double)*(n2+1));
    }


    return mat;
}


void init_data(int n1, int n2, double **a,FILE* f) {


    if(n1<=0||n2<=0||a==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is empty");
    		return;
    }

    	    if(f==NULL)
    	    {
    	        return;
    	    }
    	    for(int i=0;i<n1;i++)
    	    {
    	    		for(int j=0;j<n2;j++)
    	    		{

    	        fscanf(f,"%lf ",&a[i][j]);
    	        //printf("%lf ",a[i][j]);
    	        //if(i==0&&j==1){continue;}
    	    		}
    	    		a[i][n2]=0;
    	    }

}

/*void init_class (int n1, int *a){
	for(int i=0; i<n1;i++)
	{
		a[i]=0;
	}
}*/


void print_data (int n1, int n2, double **a){
    if(n1<=0||n2<=0||a==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is null");
    		return;
    }
    for(int i=0;i<n1;i++)
    {
    		for(int j=0;j<n2+1;j++)
    		{
        printf("%lf ",a[i][j]);//a[i][n2] is group for sample i.
    		}
    		printf("\n");
    		//printf("%d \n",class[i]);// print sample's class at the end position
    }
    printf("\n");
}


void kmean_1 (int n1, int n2, double **a){
    if(n1<=0||n2<=0||a==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is null");
    		return;
    }
	for (int i=0;i<K; i++){
		a[i][n2]=i;
	}
	printf("-------------initial\n");
	print_data(n1,n2,a);
	int group;
	printf("-------------distance between data and centroid\n");
	for(int i=K; i<n1;i++){
		double sum[K]={0};//distance between sample and K centroid
		for(int k=0;k<K;k++){
			for(int j=0;j<n2;j++){
				sum[k]=pow(a[i][j]-a[k][j],2)+sum[k];//distance between sample and kth centroid
			}
			printf("%lf ",sum[k]);
		}
		printf("\n");
		group=min_index(K,sum);//if distance between sample and kth centroid is smallest, sample i shoule be group k-1
		a[i][n2]=group;
	}
	printf("-------------first cluster outcome\n");
	print_data(n1,n2,a);
}


int min_index(int k, double *sum){
    if(k<=0||sum==NULL)
    {
    		printf("%s\n","invalid input");
    		return 0;
    }

	float min=sum[0];
	int a=0;
	for(int i=0;i<k;i++){
		if(min>sum[i]){
			min=sum[i];
			a=i;
		}
	}

	return a;
}

void kmean (int n1, int n2, double **a, double** cent){
	//double** new_cent=malloc_data(K,n2);
	//get_new_cent(n1,n2,K,a,new_cent);//find new centroid by calculating average of each group
    if(n1<=0||n2<=0||a==NULL||cent==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is null");
    		return;
    }

	print_data(K,n2,cent);//print new controid
	int group;
	printf("-----------distance between data and centroid\n");
	for(int i=0; i<n1;i++){
		double sum[K]={0};//distance between sample and K centroid

		for(int k=0;k<K;k++){
			for(int j=0;j<n2;j++){
				sum[k]=pow(a[i][j]-cent[k][j],2)+sum[k];//distance between sample and kth centroid
			}
			printf("%lf ",sum[k]);
		}
		printf("\n");
		group=min_index(K,sum);//if distance between sample and kth centroid is smallest, sample i shoule be group k-1
		a[i][n2]=group;
	}

}


void get_new_cent (int n1, int n2, double **a, double** b){
    if(n1<=0||n2<=0||a==NULL||b==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is null");
    		return;
    }

	for(int k=0;k<K;k++){
		for(int j=0;j<K;j++){
			b[k][j]=0;
		}
	}

	for(int k=0;k<K;k++){
		int count=0;  //count number of each group in order to get the average
		for(int i=0; i<n1;i++){
			if(a[i][n2]==k){
				for(int j=0;j<n2; j++){
			        //if a[i][n2] belongs to group k
				b[k][j]=b[k][j]+a[i][j];

				}
				count=count+1;
			}

			}

		for(int j1=0; j1<n2;j1++){   //get average
				b[k][j1]=b[k][j1]/count;
		}
		b[k][n2]=k;
	}
	printf("------------new centroid\n");
	//print_data(n1,n2,b);
}


void count (int n1,int n2, double** a){

    if(n1<=0||n2<=0||a==NULL)
    {
    		printf("%s\n","matrix size must be lager than 0*0 or matrix is null");
    		return;
    }

    for(int k=0;k<K;k++){
			int count=0;  //count number of each group in order to get the average
			for(int i=0; i<n1;i++){
				if(a[i][n2]==k){

					count=count+1;

				}

				}
			printf("%d cluster contains %d elements\n",k,count);

		}
}


void save_to_file(int n1, int n2, char *name, double **a){

	if(n1<=0||n2<=0||a==NULL||name==NULL)
			{
				printf("input unvalid");
				return ;
		}


	FILE* doc=fopen(name,"w");
	fprintf(doc,"%d %d\n",n1,n2);
	for(int i=0; i<n1;i++){
		for(int j=0; j<n2; j++){
			fprintf(doc,"%lf",a[i][j]);
			fprintf(doc,"%c",' ');
		}
		fprintf(doc,"\n");
	}
	fclose(doc);

}
