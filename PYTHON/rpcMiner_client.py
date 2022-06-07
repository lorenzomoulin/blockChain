import xmlrpc.client
import sys
import hashlib
import multiprocessing
import string
import random
from multiprocessing import Lock

class Row:
    status: int
    challenge: int
    seed: str 
    def __init__(self,status: int,challenge: int,seed: str):
        self.status=status
        self.challenge=challenge
        self.seed=seed 

def imprimeMenu():
    print("1 - GETTRANSACTIONID\n")
    print("2 - GETCHALLENGE\n")
    print("3 - GETTRANSACTIONSTATUS\n")
    print("4 - GETWINNER\n")
    print("5 - GETSEED\n")
    print("6 - MINERAR\n")
    print("Escolha uma opcao: ")
    output = int(input())
    return output

def testHash(challengeTuple):
    str = challengeTuple["seed"]
    id= challengeTuple["transactionID"]
    
    # encode the string
    encoded_str = str.encode()

    # create a sha1 hash object initialized with the encoded string
    hash_obj = hashlib.sha1(encoded_str)

    # convert the hash object to a hexadecimal value
    hash = hash_obj.hexdigest()

    i=0 
    bin_value=[]
    count=0 
    valid=1 
    challenge=proxy.getChallenge(id)
    

    while(i<40 and count<challenge and valid==1):
        bin_value = bin(int(hash[i], base=16))[2:].zfill(4)
        # print(bin_value)
        for element in bin_value:
            if element =='0':
                count+=1
            else: 
                valid=0
                return 0
        i+=1       
    return 1
 

def brute(tdata,n):
    finish=0
    resultado=0
    i=0
    while(finish==0):
        #status=1 ainda tem desafio
        # status=proxy.getTransactionStatus(tdata['tID'])

        res = ''.join(random.choices(string.ascii_lowercase +
                                string.digits, k = 7))
        ct={"transactionID":tdata['tID'],"clientID":777,
        "seed":str(res)}
        resultado=testHash(ct)
        if(resultado==1):
            break
        # n+=1  
        # print(n)  

    if(resultado==1):
        tdata['seed']=ct['seed']
        resp=proxy.submitChallenge({"transactionID":tdata['tID'],"clientID":777,
        "seed":tdata['seed']})
        if(resp==1):
            w= proxy.getWinner(tId)
            
            encoded_str = ct['seed'].encode()
            hash_obj = hashlib.sha1(encoded_str)
            hash = hash_obj.hexdigest() 
            print('seed:',ct['seed'],' hash:',hash)
            print("winner", w)
        
        



# Calcula total de argumentos
n = len(sys.argv)

# Verificação do uso correto/argumentos
if (n != 3):
    print("\nUso correto: rpcCalc_client server_address port_number.\n")
    exit(-1)

# print("\nArguments passed:", end = " ")
# for i in range(1, n):
#    print(sys.argv[i], end = " ")

rpcServerAddr = "http://" + sys.argv[1] + ":" + sys.argv[2] + "/"
proxy = xmlrpc.client.ServerProxy(rpcServerAddr)

# multicall = xmlrpc.client.MultiCall(proxy)
# multicall.add(7, 3)
# multicall.subtract(7, 3)
# multicall.multiply(7, 3)
# multicall.divide(7, 3)
# result = multicall()
while(1):
    opt = imprimeMenu()
    if(opt == 1):
        tId = proxy.getTransactionID()
        print("transacao corrente:", tId)
    elif(opt == 2):
        print("digite o ID da transacao: ")
        id = int(input())
        cId = proxy.getChallenge(id)
        print("DESAFIO =", cId)
    elif(opt == 3):
        print("digite o ID da transacao: ")
        id = int(input())
        cId = proxy.getTransactionStatus(id)
        print("STATUS =", cId)
    elif(opt == 4):
        print("digite o ID da transacao: ")
        id = int(input())
        cId = proxy.getWinner(id)
        print("VENCEDOR =", cId)
    elif(opt == 5):
        print("digite o ID da transacao: ")
        id = int(input()) 
        r = proxy.getSeed(id)
        print("desafio=", r["challenge"])
        print("status=", r["status"])
        print("seed", r["seed"])
    elif(opt==777):
        #teste
        cId= proxy.submitChallenge({"transactionID":0,"clientID":0,
         "seed":"azxcv342"})
        print(cId) 
    elif(opt==6):
        while(1):
            tId=proxy.getTransactionID()
            challenge=proxy.getChallenge(tId)
            print()
            print("transacao atual: ", tId)
            r = proxy.getSeed(tId)
            print("challenge: ", r["challenge"])
            
            
            #brute force
            processes = []
            n_threads=10
            n=0
            for i in range(n_threads):
                tdata={"tID":tId,"seed":''}
                n+=10000000
                p = multiprocessing.Process(target=brute, args=(tdata,n))
                processes.append(p)
                p.start()


            for process in processes:
                process.join()



    # soma = proxy.add(int(sys.argv[3]), int(sys.argv[4]))
    # print("%s+%s=%s" % (sys.argv[3], sys.argv[4], soma))

    # sub = proxy.subtract(int(sys.argv[3]), int(sys.argv[4]))
    # print("%s-%s=%d" % (sys.argv[3], sys.argv[4], sub))
