/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
 

#include "rpcMiner.h"
#include <string.h>
#include <openssl/sha.h>

#define MAX_DESAFIOS 400

struct elements {
	int transactionID;
	int challenge;
	int clientID;
	char seed[40];
};
typedef struct elements elements;

int tamanhoLista = 0;
elements listaDesafios[MAX_DESAFIOS];

int *
gettransactionid_100_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;
	if (tamanhoLista == 0){
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		result = tamanhoLista;
		tamanhoLista++;
		return &result;
	}
	if (listaDesafios[tamanhoLista-1].clientID != -1){
		elements e;
		e.transactionID = tamanhoLista+1;
		e.clientID = -1;
		e.challenge = (tamanhoLista % 128) + 1;
		listaDesafios[tamanhoLista] = e;
		result = tamanhoLista;
		tamanhoLista++;
		return &result;
	}
	result = tamanhoLista-1;
	return &result;
}

int *
getchallenge_100_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	int transactionID = *argp;
	if (tamanhoLista == 0){
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		result = e.challenge;
		tamanhoLista++;
	}
	if (transactionID >= tamanhoLista){
		result = -1;
		return &result;
	}
	result = listaDesafios[transactionID].challenge;

	return &result;
}

int *
gettransactionstatus_100_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;

	int transactionID = *argp;
	if (tamanhoLista == 0){
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		tamanhoLista++;
	}
	if (transactionID >= tamanhoLista){
		result = -1;
		return &result;
	}
	result = listaDesafios[transactionID].clientID == -1 ? 1: 0;
	return &result;
}

int *
submitchallenge_100_svc(challengeTuple *argp, struct svc_req *rqstp)
{
	static int  result;
	challengeTuple ct = *argp;
	if (tamanhoLista == 0){ //nao existe desafio na lista
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		tamanhoLista++;
		result = -1;
		return &result;
	}
	if (ct.transactionId >= tamanhoLista){
		result = -1;
		return &result;
	}
	elements e = listaDesafios[ct.transactionId];
	if (e.clientID != -1) { //ja foi solucionado
		result = 2;
		return &result;
	}
	int challenge = e.challenge;
	// gerar a hash da semente
	char data[40];
	strcpy(data, ct.seed);
	size_t length = strlen(data);

	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(data, length, hash);
	// for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
	// 	printf("%02x", hash[i]);
	// printf("\n");

	//checar se a hash eh valida
	int valid = 1;
	int teste = 0;
	// printf("%02x\n", teste);
	// printf("%02x\n", (~0) << 4);
	for (int i = 0, qtd = 1; i < SHA_DIGEST_LENGTH && challenge > 0; i+=4,++qtd){ //de 4 em 4 bytes
		int left_shift = (32 - challenge < 0) ? 0 : 32 - challenge;
		// printf("left shift = %d\n", left_shift);
		// printf("shiftado = %02x\n", ((~0) << left_shift));
		int hash_int = 0;
		for (int j = 0; j < 4; ++j){
			hash_int = (hash_int | hash[i + j]);
			if (j < 3)
				hash_int <<= 8;
			// printf("%02x\n", hash_int);
		}
		// hash_int = (hash_int | hash[i+1])
		valid = valid && ((((~0) << left_shift) & hash_int) == 0);
		challenge -= 32;
		if (!valid){
			result = 0;
			return &result;
		}
	}
	//ATUALIZAR VENCEDOR
	listaDesafios[ct.transactionId].clientID = ct.clientID;
	strcpy(listaDesafios[ct.transactionId].seed, ct.seed);

	//gera novo desafio
	
	e.transactionID = tamanhoLista+1;
	e.clientID = -1;
	e.challenge = (tamanhoLista % 128) + 1;
	listaDesafios[tamanhoLista] = e;
	tamanhoLista++;
	printf("%s\n", ct.seed);
	printf("tamanholista = %d\n", tamanhoLista);
	result = 1;
	return &result;
}

int *
getwinner_100_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;

	int transactionID = *argp;
	if (tamanhoLista == 0){
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		tamanhoLista++;
	}
	if (transactionID >= tamanhoLista){
		result = -1;
		return &result;
	}
	int w = listaDesafios[transactionID].clientID;
	result = w == -1 ? 0: w;

	return &result;
}

row *
getseed_100_svc(int *argp, struct svc_req *rqstp)
{
	static row  result;

	int transactionID = *argp;
	if (tamanhoLista == 0){
		elements e;
		e.transactionID = 0;
		e.clientID = -1;
		e.challenge = 1;
		listaDesafios[tamanhoLista] = e;
		tamanhoLista++;
	}
	if (transactionID >= tamanhoLista){
		result.challenge = -1;
		return &result;
	}
	result.status = listaDesafios[transactionID].clientID == -1 ? 1: 0;
	result.challenge = listaDesafios[transactionID].challenge;
	strcpy(result.seed, listaDesafios[transactionID].seed);

	return &result;
}
