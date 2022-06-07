/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpcMiner.h"
#include <openssl/sha.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MENUGETTRANSACTIONID 1
#define MENUGETCHALLENGE 2
#define MENUGETTRANSACTIONSTATUS 3
#define MENUGETWINNER 4
#define MENUGETSEED 5
#define MENUMINERAR 6 
#define KILL_THREADS 1
#define RUN_THREADS 0
#define SUCCESS 1
#define EXHAUST 2
#define LATE 0


char *host;
int threads_flag = RUN_THREADS;
int n_threads;
int *last_first_idx;

typedef struct thread_data {
   int x_i, x_f, id, status, tID, challenge;
   int last_xi;
   char seed[40];
   CLIENT *clnt;
} thread_data;


int imprimeMenu(){
	int opcao;
	printf("1 - GETTRANSACTIONID\n");
	printf("2 - GETCHALLENGE\n");
	printf("3 - GETTRANSACTIONSTATUS\n");
	printf("4 - GETWINNER\n");
	printf("5 - GETSEED\n");
	printf("6 - MINERAR\n");
	printf("Escolha uma opcao: ");
	scanf("%d", &opcao);
	return opcao;
}


int getTransactionID(CLIENT *clnt){
	void *kk;
	assert(clnt != NULL);
	int *result = gettransactionid_100(kk, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

int getChallenge(CLIENT *clnt, int transactionID){
	assert(clnt != NULL);
	int *result = getchallenge_100(&transactionID, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

int getTransactionStatus(CLIENT *clnt, int transactionID){
	assert(clnt != NULL);
	int *result = gettransactionstatus_100(&transactionID, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

int submitChallenge(CLIENT *clnt, challengeTuple ct){
	assert(clnt != NULL);
	int *result = submitchallenge_100(&ct, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

int getWinner(CLIENT *clnt, int transactionID){
	assert(clnt != NULL);
	int *result = getwinner_100(&transactionID, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

row getSeed(CLIENT *clnt, int transactionID){
	assert(clnt != NULL);
	row *result = getseed_100(&transactionID, clnt);
	if (result == NULL){
		fprintf(stderr, "PROBLEMA NA CHAMADA RPC\n");
		exit(0);
	}
	return *(result);
}

int test_hash(char *seed, int challenge){
	// int challenge = e.challenge;
	// gerar a hash da semente
	// printf("INICIO\n");
	char data[40];
	strcpy(data, seed);
	size_t length = strlen(data);

	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(data, length, hash);
	// for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
	// 	printf("%02x", hash[i]);
	// printf("\n");
	int result = 1;
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
			break;
		}
	}
	// printf("FIM\n");
	return valid;
}

void *brute(void *arg){
	thread_data *tdata = (thread_data *) arg;
	challengeTuple ct;
	int xi = tdata->x_i, xf = tdata->x_f, id = tdata->id, challenge = tdata->challenge;
	// printf("COMECO DA FUNCAO THREAD ID = %d\n", id);
	ct.transactionId = tdata->tID;
	ct.clientID = 777;
	CLIENT *clnt = tdata->clnt;
	
	unsigned char hash[SHA_DIGEST_LENGTH];
	int finish = 0;
	int i;
	for (i = xi; i <= xf && !finish; ++i){
		// if (clnt == NULL){
		// 	clnt = clnt_create(host, PROG, VERSAO, "udp");
		// }
		// assert(clnt != NULL);
		if (threads_flag == KILL_THREADS || getTransactionStatus(clnt, ct.transactionId) == 0){
			threads_flag == KILL_THREADS;
			finish = 1;
			break;
		}
		sprintf(ct.seed, "%d", i);
		// printf("ANTES DA HASH\n");
		// fflush(stdout);
		
		size_t length = strlen(ct.seed);
		assert(length > 0);
		SHA1(ct.seed, length, hash);
		// for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
		// 	printf("%02x", hash[i]);
		// printf("\n");
		int result = 1;
		//checar se a hash eh valida
		int valida = 1;
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
			valida = valida && ((((~0) << left_shift) & hash_int) == 0);
			challenge -= 32;
			if (!valida){
				break;
			}
		}
		// int valida = test_hash(ct.seed, challenge);
		// printf("TESTOU HASH\n");
		// fflush(stdout);
		if (valida){
			// if (clnt == NULL){
			// 	clnt = clnt_create(host, PROG, VERSAO, "udp");
			// }
			// assert(clnt != NULL);
			int resultado = submitChallenge(clnt, ct);

			if (resultado == 1){
				// printf("resolvido transaction = %d!!!! ID = %d seed = %d\n\n\n", ct.transactionId, id, i);
				strcpy(tdata->seed, ct.seed);
				threads_flag = KILL_THREADS;
				tdata->status = SUCCESS;
				finish = 1;
				break;
			}
			else if (resultado == 2){
				// printf("alguem ja resolveu e nao fui eu transaction = %d!!!!\n", ct.transactionId);
				threads_flag = KILL_THREADS;
				finish = 1;
				break;
			}
		}
		
	}
	last_first_idx[id] = i;
	tdata->last_xi = i;
	// printf("FIM DA FUNCAO THREAD ID = %d\n", id);
	fflush(stdout);
	pthread_exit(NULL);
}


int
main (int argc, char *argv[])
{
	char *host;
	
	if (argc < 4) {
		printf ("usage: %s server_host N_max threads_number\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	strcpy(host, argv[1]);
	n_threads = atoi(argv[3]);
	last_first_idx = (int*)malloc(sizeof(last_first_idx)*n_threads);
	CLIENT *clnt;
	clnt = clnt_create(argv[1], PROG, VERSAO, "udp");

	while (1){
		int opt = imprimeMenu();
		printf("opcao = %d\n", opt);
		if (opt == MENUGETTRANSACTIONID){
			printf("transacao corrente: %d\n", getTransactionID(clnt));
		}
		else if (opt == MENUGETCHALLENGE){
			int id;
			printf("digite o ID da transacao: ");
			scanf("%d", &id);
			printf("DESAFIO = %d\n", getChallenge(clnt, id));
		}
		else if (opt == MENUGETTRANSACTIONSTATUS){
			int id;
			printf("digite o ID da transacao: ");
			scanf("%d", &id);
			printf("STATUS = %d\n", getTransactionStatus(clnt, id));
		}
		else if (opt == MENUGETWINNER){
			int id;
			printf("digite o ID da transacao: ");
			scanf("%d", &id);
			printf("VENCEDOR = %d\n", getWinner(clnt, id));
		}
		else if (opt == MENUGETSEED){
			int id;
			printf("digite o ID da transacao: ");
			scanf("%d", &id);
			row r = getSeed(clnt, id);
			printf("desafii=  %d\n", r.challenge);
			printf("status = %d\n", r.status);
			printf("%s\n", r.seed);
		}
		else if (opt == MENUMINERAR){
			int first_time = 1;
			MINERAR:
				1+1;
				assert(clnt != NULL);
				int tID = getTransactionID(clnt);
				int challenge = getChallenge(clnt, tID);
				int resultado = 0;
				challengeTuple ct;
				ct.transactionId = tID;
				// printf("transacao atual = %d\n", tID);
				row r = getSeed(clnt, tID);
				// printf("desafio=  %d\n", r.challenge);
				// printf("status = %d\n", r.status);
				// printf("%s\n", r.seed);
				ct.clientID = 777;
				while (resultado == 0){
					if (resultado == 2 || resultado == -1)
						goto MINERAR;
					
					int N = atoi(argv[2]);
					
					int remainder = N % n_threads;
					int block_size = N/n_threads;
					int acum = 0;
					thread_data tdata[n_threads];
    				pthread_t threads[n_threads];
					
					for (int i = 0; i < n_threads; ++i){
						int cur_block = block_size + (remainder > 0 ? 1: 0);
						tdata[i].id = i;
						if (!first_time){
							tdata[i].x_i = last_first_idx[i];
						}
						else
							tdata[i].x_i = acum;
						tdata[i].x_f = acum + cur_block - 1;
						
						// printf("segmento = [%d, %d]\n", tdata[i].x_i, tdata[i].x_f);
						tdata[i].tID = tID;
						tdata[i].challenge = challenge;
						tdata[i].status = -1;
						tdata[i].clnt = clnt_create(argv[1], PROG, VERSAO, "udp");
						remainder--;
						acum += cur_block;
						pthread_create(&(threads[i]), NULL, brute, &(tdata[i]));
					}

					for (int i = 0; i < n_threads; ++i){
						pthread_join(threads[i], NULL);
					}
					printf("transacao atual: %d\n", tID);
					printf("challenge: %d", challenge);
					for (int i = 0; i < n_threads; ++i){
						if (tdata[i].status == SUCCESS){
							size_t length = strlen(tdata[i].seed);
							unsigned char hash[SHA_DIGEST_LENGTH];
							printf("seed encontrada: %s -> ", tdata[i].seed);
							SHA1(tdata[i].seed, length, hash);
							for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
								printf("%02x", hash[i]);
							printf("\n");
							break;
						}
					}
					printf("vencedor: %d\n", getWinner(tID));
					threads_flag = RUN_THREADS;
					first_time = 0;
					goto MINERAR;
				}
		}
	}
	
exit (0);
}
