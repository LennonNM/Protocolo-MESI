#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct block{
  char State = 'I'; //MESI
  bool bValidBit = false;
  long lBlockTag = 0;
};

//dados los valores se calcula tamaño en bits del index y del offset necesarios en la función splitAddress
class cache{
  private:
    int iTagSize, iIndexSize;
    int iNumSets, iRand, iNumBlocks, iOffsetSize;
    long lAddress, lIndex, lTag, lIndexMask;
    std::vector<std::vector<block> > vBlocks;
  public:
    long long llBlockSize;
    int iAssociativity;
    long long llCacheSize;
    void splitAddress();
    bool readAddress(long);
    //Constructor con 3 parametros
    cache(int iAssoc,long long llCSize,long long llBSize){
      llBlockSize = llBSize;
      iAssociativity = iAssoc;
      llCacheSize = llCSize;
      iNumBlocks = llCSize/llBSize;
      iNumSets = iNumBlocks/iAssoc;
      iOffsetSize = int(log2(llBSize));
      iIndexSize = int(log2(iNumSets));
      iTagSize = 32 - iIndexSize - iOffsetSize;
      vBlocks.resize(iNumSets, std::vector<block>(iAssociativity) );
    }
    //Destructor >:O
    ~cache(){}
};

//para dividir la direccion en tag, index y offset
void cache::splitAddress(){
  lIndexMask = 0;
  for(int i = 0; i < iIndexSize; i++){
    lIndexMask <<= 1; //se crea una mascara para index
    lIndexMask |= 1; //indexMask or 1
  }
  lIndex = lAddress >> (iOffsetSize);
  lIndex = lIndex & lIndexMask;
  lTag = lAddress >> (iOffsetSize+iIndexSize);
}

//despues de saber en que set estoy de acuerdo al index
bool cache::readAddress(long lAddr){
  lAddress = lAddr;
  splitAddress();
  for(int i=0; i<iAssociativity; i++){ //para buscar la cantidad de bloques
    if (vBlocks[lIndex][i].bValidBit){
      if (vBlocks[lIndex][i].lBlockTag == lTag){//Si si esta el bloque
          return true
      }
    }
  }
  iRand = rand() % iAssociativity;//Si no esta el bloque
  vBlocks[lIndex][iRand].bValidBit = true;
  vBlocks[lIndex][iRand].lBlockTag = lTag;//Jala el bloque

  if (vBlocks[lIndex][i].State == 'I'){// Si en el read
      


  return false;
}


//se reciben los parametros del programa
int main(int arcg, char* argv[]){

  ifstream datos("aligned.trace");
	long lAddr;
        char lAct;
	bool bHit;

	long long llTotalCPU0 = 0;
	long long llHitsCPU0 = 0;
	long long llMissesCPU0 = 0;
	double dMissRateCPU0;
  string line;
  string line_copy;

  //cantidad de accesos totales, hits y misses
  cache CacheCPU0(1,8000,16);
  cache CacheCPU1(1,8000,16);
  cache CacheShared(1,64000,16);
  int CPU;
  CPU = 0;

  while ( getline (datos,line) ) {

    getline(datos,line);
    line_copy = line;
    line.erase(line.begin()+8,line.end());
    istringstream(line) >> std::hex >> lAddr; //Obtiene direccion

    lAct = line_copy.at(line_copy.size()-1); //Obtiene si es lectura (L) o escritura (S)

    if(CPU == 0 ) {//CPU0
        if(lAct == 'S'){//Escritura
  	    bHit = CacheCPU0.readAddress(lAddr);
        }else{
            //
        }
	CPU = 1;
    }else{//CPU1
	bHit = CacheCPU1.readAddress(lAddr);
        CPU = 0;
    }
  }
  //cout << "Hits CPU0: " << llHitsCPU0 << endl;
}