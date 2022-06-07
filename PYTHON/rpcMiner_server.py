from mimetypes import init
from xmlrpc.server import SimpleXMLRPCServer
import hashlib

class Row:
    status: int
    challenge: int
    seed: str 
    def __init__(self,status: int,challenge: int,seed: str):
        self.status=status
        self.challenge=challenge
        self.seed=seed 
        
class Elementos:
    transactionID: int
    challengeID: int
    clientID: int
    seed: str
    def __init__(self,transactionID: int, challengeID: int,clientID: int,seed: str):
        self.transactionID=transactionID
        self.challengeID=challengeID
        self.clientID=clientID
        self.seed=seed

listaDesafios = [Elementos(0,1,-1,"")]

def getTransactionID():
    
    if(listaDesafios[-1].clientID!=-1):
        challenge = (len(listaDesafios) % 128) + 1
        listaDesafios.append(Elementos(len(listaDesafios),challenge,-1,''))
        
    return len(listaDesafios)-1      
      
def getChallenge(tID):
    if(tID>=len(listaDesafios)):
        return -1
    return listaDesafios[tID].challengeID

def getTransactionStatus(tID):
    if(tID>=len(listaDesafios)):
        return -1
    
    if(listaDesafios[tID].clientID == -1):
        return 1
    else:
        return 0

def getWinner(tID):
    if(tID>=len(listaDesafios)):
        return -1
    w = listaDesafios[tID].clientID    
    if(w==-1):
        return 0
    else:
        return w

def getSeed(tID):
    if(tID>=len(listaDesafios)):
        return {"status":-1,"challenge":-1,"seed":-1}
    if( listaDesafios[tID].clientID == -1):
         return {"status":1,"challenge":listaDesafios[tID].challengeID,
         "seed":listaDesafios[tID].seed}
    else:
        return {"status":0,"challenge":listaDesafios[tID].challengeID,
         "seed":listaDesafios[tID].seed}
       
def submitChallenge(challengeTuple):
    print('submited')
    # print(listaDesafios)
    if(challengeTuple["transactionID"]>=len(listaDesafios)):
        return -1
       
    e=listaDesafios[challengeTuple["transactionID"]]
    
   
    if(e.clientID!=-1):
        return 2
    challenge=e.challengeID
    
    str = challengeTuple["seed"]
    # print(str)
    # encode the string
    encoded_str = str.encode()

    # create a sha1 hash object initialized with the encoded string
    hash_obj = hashlib.sha1(encoded_str)

    # convert the hash object to a hexadecimal value
    hash = hash_obj.hexdigest()
    # print("-----",str,hash)
    i=0 
    bin_value=[]
    count=0 
    valid=1 
    while(i<40 and count<challenge and valid==1):
        bin_value = bin(int(hash[i], base=16))[2:].zfill(4)
        # print(bin_value)
        for element in bin_value:
            if element =='0':
                count+=1
            else: 
                valid=0
                return 0
                
        # if(count>=challenge):
            # print("count",count)
            # print("valid",valid)
            # break
        # print("count",count)
        i+=1    
    
    #atualizar vencedor 
    listaDesafios[challengeTuple["transactionID"]].clientID=challengeTuple["clientID"]     
    listaDesafios[challengeTuple["transactionID"]].seed=challengeTuple["seed"]

    #gera novo desafio
    challenge = (len(listaDesafios) % 128) + 1
    listaDesafios.append(Elementos(len(listaDesafios),challenge,-1,''))
    # print(listaDesafios)

    return 1
   


# A simple server with simple arithmetic functions
server = SimpleXMLRPCServer(("localhost", 8000))
print("Listening on port 8000...")
#server.register_multicall_functions()
server.register_function(getTransactionID, 'getTransactionID')
server.register_function(getChallenge, 'getChallenge')
server.register_function(getTransactionStatus, 'getTransactionStatus')
server.register_function(getWinner, 'getWinner')
server.register_function(getSeed, 'getSeed')
server.register_function(submitChallenge, 'submitChallenge')

# server.register_function(add, 'add')
# server.register_function(subtract, 'subtract')
# server.register_function(multiply, 'multiply')
# server.register_function(divide, 'divide')
server.serve_forever()
