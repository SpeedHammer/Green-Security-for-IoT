/*
* RSA Algorithm - ENCRYPT
* Toy/textbook RSA (p=61, q=53) - nao e' tamanho de chave real, serve
* so' como referencia de custo no benchmark.
*
* @Code by Anvikshajain
*
* Dividido de RSA_algorithm.c (padrao do projeto). Entrada: 1 linha via
* stdin, ate' 10kB, sem '\n' obrigatorio. Primos fixos e hardcoded: o
* original lia 2 primos via scanf() ANTES da mensagem, e com entrada de
* 1 linha so' esses scanf() falhavam e o programa saia com "WRONG INPUT".
*
* Compilar: gcc -O2 RSA_algorithm_encrypt.c -o rsa_encrypt -lm
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define MSGCAP 10400
long int p,q,n,t,flag,e[100],d[100],temp[MSGCAP],j,m[MSGCAP],en[MSGCAP],i;
char msg[MSGCAP];
char msg_original[MSGCAP];
int prime(long int);
void ce();
long int cd(long int);
void encrypt();

int main(void) {
	p = 61;
	q = 53;
	flag = prime(p);
	if(flag==0) { printf("\nWRONG INPUT\n"); exit(1); }
	flag = prime(q);
	if(flag==0||p==q) { printf("\nWRONG INPUT\n"); exit(1); }

	if (!fgets(msg, sizeof(msg), stdin)) {
		fprintf(stderr, "Failed to read input.\n");
		return 1;
	}
	msg[strcspn(msg, "\n")] = '\0';
	strcpy(msg_original, msg);
	for (i=0;msg[i]!='\0';i++)
		m[i]=msg[i];
	n=p*q;
	t=(p-1)*(q-1);
	ce();
	
for(int i = 0; i < 10000; i++){
		encrypt();
	};
	
	return 0;
}
int prime(long int pr) {
	int i;
	j=sqrt(pr);
	for (i=2;i<=j;i++) {
		if(pr%i==0)
		    return 0;
	}
	return 1;
}
void ce() {
	int k;
	k=0;
	for (i=2;i<t;i++) {
		if(t%i==0)
		    continue;
		flag=prime(i);
		if(flag==1&&i!=p&&i!=q) {
			e[k]=i;
			flag=cd(e[k]);
			if(flag>0) {
				d[k]=flag;
				k++;
			}
			if(k==99)
			        break;
		}
	}
}
long int cd(long int x) {
	long int k=1;
	while(1) {
		k=k+t;
		if(k%x==0)
		    return(k/x);
	}
}
void encrypt() {
	long int pt,ct,key=e[0],k,len;
	i=0;
	len=strlen(msg);
	while(i!=len) {
		pt=m[i];
		pt=pt-96;
		k=1;
		for (j=0;j<key;j++) {
			k=k*pt;
			k=k%n;
		}
		temp[i]=k;
		ct=k+96;
		en[i]=ct;
		i++;
	}
	en[i]=-1;
	printf("Cipher text (hex): ");
	for (i=0;en[i]!=-1;i++)
	printf("%02lx ",en[i]);
	printf("\n");
}
