#include<stdio.h>
#include<stdlib.h>
#include<math.h>

unsigned int flag=0;
unsigned int CF=0;
unsigned int ZF=0;
unsigned int SF=0;
unsigned int OF=0;
char* err="";

unsigned int add_uint(unsigned int num1, unsigned int num2);
unsigned int negate(unsigned int num1);
unsigned int max(unsigned int num1,unsigned int num2);
void ADD(unsigned int num1,unsigned int num2,FILE* fp);
void ADDS(int num1,int num2,FILE* fp);
void SUB(unsigned int num1,unsigned int num2,FILE* fp);
void SUBS(unsigned int num1,unsigned int num2,FILE* fp);
void ITOF(int a,FILE* fp);
void FTOI(unsigned int n,FILE* fp);

int main(int argc,char** argv){
    if(argc!=2){
        printf("SYNTAX ERROR.");
        return -1;}
    else{
        FILE* stream=fopen(argv[1],"r");
        char *line = NULL;
        char* output="output";
        FILE* fp=fopen(output,"a");
        int count=0,i=0;
        size_t len = 0;
        ssize_t read;
        char fun[4];
        int num1,num2;

        if (stream == NULL){
            printf("Could not open the file.Exiting.");
            exit(EXIT_FAILURE);}
        while((read = getline(&line, &len, stream))!=-1){
            if(read>=2)count++;
        }
        free(line);
        rewind(stream);
        for(i=0;i<count;i++){
            fscanf(stream,"%s",fun);
            if(strcmp(fun,"ADD")==0){
            	fscanf(stream,"%x %x",&num1,&num2);
            	ADD((unsigned)num1,(unsigned)num2,fp);}
            else if(strcmp(fun,"ADDS")==0){
            	fscanf(stream,"%x %x",&num1,&num2);
            	ADDS(num1,num2,fp);}
            else if(strcmp(fun,"SUB")==0){
            	fscanf(stream,"%x %x",&num1,&num2);
            	SUB((unsigned)num1,(unsigned)num2,fp);}
            else if(strcmp(fun,"SUBS")==0){
            	fscanf(stream,"%x %x",&num1,&num2);
            	SUBS((unsigned)num1,(unsigned)num2,fp);}
            else if(strcmp(fun,"ITOF")==0){
            	fscanf(stream,"%x",&num1);
            	ITOF(num1,fp);}
            else if(strcmp(fun,"FTOI")==0){
            	fscanf(stream,"%x",&num1);
            	FTOI((unsigned)num1,fp);}
            fprintf(fp,"\n");
        }
        fclose(fp);
        fclose(stream);
        return 0;
    }
}

unsigned int max(unsigned int num1,unsigned int num2){
    if(num1>num2)return num1;
    else return num2;
}

unsigned int add_uint(unsigned int num1, unsigned int num2){
    unsigned int n=num1+num2;
    if((signed)n<0)SF=1;
    if(n==0)ZF=1;
    if(n<max(num1,num2))CF=1;
    if((signed)num1>0 && (signed)num2>0 && (signed)n<0)OF=1;
    else if((signed)num1<0 && (signed)num2<0 && (signed)n>0)OF=1;
    flag=OF<<11|SF<<7|ZF<<6|CF;
    return n;
}
unsigned int negate(unsigned int num1){
    unsigned int n=~(num1)+1;
    return n;
}

void ADD(unsigned int num1,unsigned int num2,FILE* fp){
    unsigned int n=add_uint(num1,num2);
    if(CF==1)err="ERR";
    fprintf(fp,"ADD  0x%08X 0x%08X 0x%08X 0x%08X %s\n",num1,num2,n,flag,err);
    CF=0;OF=0;ZF=0;SF=0;flag=0;err="";
}

void ADDS(int num1,int num2,FILE* fp){
    unsigned int n=add_uint((unsigned)num1,(unsigned)num2);
    if(OF==1)err="ERR";
    fprintf(fp,"ADDS 0x%08X 0x%08X 0x%08X 0x%08X %s\n",num1,num2,n,flag,err);
    CF=0;OF=0;ZF=0;SF=0;flag=0;err="";
}

void SUB(unsigned int num1,unsigned int num2,FILE* fp){
    unsigned int n2=negate(num2);
    unsigned int n=add_uint(num1,n2);
    if(OF==1)err="ERR";
    fprintf(fp,"SUB  0x%08X 0x%08X 0x%08X 0x%08X %s\n",num1,num2,n,flag,err);
    CF=0;OF=0;ZF=0;SF=0;flag=0;err="";
}

void SUBS(unsigned int num1,unsigned int num2,FILE* fp){
    unsigned int n2=negate(num2);
    unsigned int n=add_uint(num1,n2);
    if(OF==1)err="ERR";
    fprintf(fp,"SUBS 0x%08X 0x%08X 0x%08X 0x%08X %s\n",num1,num2,n,flag,err);
    CF=0;OF=0;ZF=0;SF=0;flag=0;err="";
}

void ITOF(int a,FILE* fp){
    int i=0,j=0,exp=0,res=0;
    unsigned int n=(unsigned)a;
	int arr1[32],arr2[32];
	for(i=0;i<32;i++){
    	arr1[i]=0;
    	arr2[i]=0;
	}
	if(a>0)arr2[0]=0;
	else if(a<0){
    	arr2[0]=1;
    	n=negate(n);    
	}
	for(i=31;i>=0;i--){
    	arr1[i]=n%2;
    	n=n/2;
	} 
	while(arr1[j]!=1){
    j++;  
	}
	exp=31-j+127;
	for(i=8;i>=1;i--){
    	arr2[i]=exp%2;
    	exp=exp/2;        
	}
	int k=9;
	if(j>=8){  
    	while(j<31){
    	    arr2[k]=arr1[j+1];
    		j++;k++;
    	}
	}
	else if(j<8 && arr1[j+24]==0){
    	for(i=0;i<23;i++){
    	    arr2[k]=arr1[j+1];
    		j++;k++;    
    	}
	}
	else{
    	int l=0;
    	l=j+25;
    	while(l<32){
    	    if(arr1[l]==1)break;
    	    l++;
    	}
    	if(l==32 && arr1[j+23]==0){
    	    while(j<31){
    	        arr2[k]=arr1[j+1];
		    	j++;k++;
			}
    	}
    	else{
    	    int mant=0,g=j+1;
    	    for(i=22;i>=0;i--,g++){
    	        mant=mant|(arr1[g]<<i);
    	    }
    	    if(mant==pow(2,24)-1){mant=0;exp++;}
    	    else{mant=mant+1;}
    	    for(i=31;i>8;i--){
    	       arr2[i]=mant%2;
    	       mant=mant/2;
    	    }
    	}  
	}
	for(i=0;i<32;i++){
    	res=res|(arr2[i]<<(31-i));
	}
	fprintf(fp,"ITOF 0x%08X 0x%08X 0x%08X\n",a,res,flag); 
}

void FTOI(unsigned int n,FILE* fp){
	int a=(signed)n,i=0,exp=0,res=0;
	int arr1[32],arr2[32];
    for(i=0;i<32;i++){
    	arr1[i]=0;
    	arr2[i]=0;
    }
    for(i=31;i>=0;i--){
    	arr1[i]=n%2;
    	n=n/2;
    }
    for(i=1;i<9;i++){
        exp=exp|(arr1[i]<<(8-i));
    }
    exp=exp-127;
    if(exp>31){OF=1;err="ERR";}
    else if(exp>=0 && exp<24){
    	for(i=0;i<exp;i++){
    	    arr2[31-i]=arr1[exp+8-i];
    	}
    	arr2[31-exp]=1;
    }
    else if(exp>23 && exp<32){
    	for(i=0;i<23;i++){
    	    arr2[31-(exp-23)-i]=arr1[31-i];
    	}
    	arr2[31-exp]=1;	
    }
    for(i=0;i<32;i++){
    	res=res|(arr2[i]<<(31-i));
    }
    if(a<0)res=~(res)+1;
    flag=OF<<11;
    if(OF==1 && a>0){fprintf(fp,"FTOI 0x%08X 0x7FFFFFFF 0x%08X %s\n",a,flag,err);}
    else if(OF==1 && a<0){fprintf(fp,"FTOI 0x%08X 0xFFFFFFFF 0x%08X %s\n",a,flag,err);}
    else{fprintf(fp,"FTOI 0x%08X 0x%08X 0x%08X %s\n",a,res,flag,err);}
    OF=0;flag=0;err="";	
}

