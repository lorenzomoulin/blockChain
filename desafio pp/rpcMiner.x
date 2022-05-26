
struct row {
       int status;
       int challenge;
       char seed[40]; 
};

struct challengeTuple {
	int transactionId;
	int clientID;
	char seed[40];
};

program PROG { 
       version VERSAO { 
               int GETTRANSACTIONID() = 1; 
               int GETCHALLENGE(int) = 2;
               int GETTRANSACTIONSTATUS(int) = 3;
               int SUBMITCHALLENGE(challengeTuple) = 4;
               int GETWINNER(int) = 5;
               row GETSEED(int) = 6; 
       } = 100;
} = 666;

